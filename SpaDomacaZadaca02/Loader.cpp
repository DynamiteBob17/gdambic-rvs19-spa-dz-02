#include <fstream>
#include <sstream>
#include "Loader.h"
#include "Util.h"

static std::vector<std::string> split(std::string input, std::string delimiter) {
	std::vector<std::string> result;
	size_t pos = 0;
	while ((pos = input.find(delimiter)) != std::string::npos) {
		result.push_back(input.substr(0, pos));
		input.erase(0, pos + 1);
	}
	result.push_back(input);
	return result;
}

static int extractCoordinate(std::string str) {
	return std::stoi(str.substr(str.find("=") + 1));
}

static void parseRLE(int** arr, std::string rleCoords) {
	std::string number = "";
	int xTracker = 0;
	int yTracker = 0;

	for (char& currChar : rleCoords) {
		if (isdigit(currChar)) {
			number += currChar;
			continue;
		}

		int count = number.empty() ? 1 : std::stoi(number);

		switch (currChar) {
		case 'b':
		{
			for (int i = 0; i < count; ++i) {
				++xTracker;
			}
		} break;
		case 'o':
		{
			for (int i = 0; i < count; ++i) {
				arr[xTracker][yTracker] = 1;
				++xTracker;
			}
		} break;
		case '$':
		{
			for (int i = 0; i < count; ++i) {
				++yTracker;
			}
			xTracker = 0;
		} break;
		default: break;
		}

		number = "";
	}
}

static std::string concatenateVectorOfStrings(std::vector<std::string>& strings, int startIdx) {
	std::string res;
	for (int i = startIdx; i < strings.size(); ++i) {
		res += strings[i];
	}
	return res;
}

void Loader::fillPresets() {
	std::ifstream file("presets.txt");

	if (!file.is_open()) {
		throw std::runtime_error("Error: unable to open presets.txt file!");
	}

	std::stringstream ss;
	std::string line;

	while (std::getline(file, line)) {
		if (line == "NEXT") {
			presets.push_back(ss.str());
			ss.str("");
		}
		else {
			ss << line << "\n";
		}
	}

	if (!ss.str().empty()) {
		presets.push_back(ss.str());
	}

	file.close();
}

int** Loader::createPresetArray(std::string rle) {
	std::vector<std::string> newLineSplit = split(rle, "\n");
	std::vector<std::string> commaSplit = split(newLineSplit[0], ",");
	int x = extractCoordinate(commaSplit[0]);
	int y = extractCoordinate(commaSplit[1]);

	auto arr = initArr(x, y);
	parseRLE(arr, concatenateVectorOfStrings(newLineSplit, 1));

	return insertArrayInMiddle(arr, x, y);
}

int** Loader::insertArrayInMiddle(int** smaller, int smallerX, int smallerY) {
	determineCellSize(smallerX, smallerY);

	auto larger = initArr(x, y);

	int startX = (x - smallerX) / 2;
	int startY = (y - smallerY) / 2;

	int is = 0;
	int js = 0;
	for (int i = startX; i < (startX + smallerX); ++i) {
		for (int j = startY; j < (startY + smallerY); ++j) {
			larger[i][j] = smaller[is][js];
			++js;
		}
		js = 0;
		++is;
	}

	deleteArr(smaller, smallerX);

	return larger;
}

void Loader::setX(int x) {
	this->x = x;
}

void Loader::setY(int y) {
	this->y = y;
}

void Loader::setCellSize(float cellSize) {
	this->cellSize = cellSize;
}

Loader::Loader(bool isGameOfLife) {
	CELL_SIZES = {
	10 * Util::getScale(),
	8 * Util::getScale(),
	6 * Util::getScale(),
	5 * Util::getScale(),
	4 * Util::getScale(),
	3 * Util::getScale(),
	2 * Util::getScale(),
	1 * Util::getScale()
	};
	if (isGameOfLife) fillPresets();
	else determineCellSize(1, 1);
}

int** Loader::randomArray() {
	setCellSize(6 * Util::getScale());
	setX((int)(sf::VideoMode::getDesktopMode().width / cellSize));
	setY((int)(sf::VideoMode::getDesktopMode().height / cellSize));

	auto arr = initArr(x, y);

	std::random_device rd;
	std::default_random_engine gen(rd());
	std::uniform_int_distribution<int> dis(1, 4); // 25% chance for an alive cell

	for (int i = 0; i < x; ++i) {
		for (int j = 0; j < y; ++j) {
			arr[i][j] = dis(gen) == 1 ? 1 : 0;
		}
	}

	return arr;
}

int** Loader::randomPreset() {
	std::random_device rd;
	std::default_random_engine gen(rd());
	std::uniform_int_distribution<int> dis(0, presets.size() - 1);
	return createPresetArray(presets[dis(gen)]);
}

int** Loader::emptyArray(int rawCellSize) {
	setCellSize(rawCellSize * Util::getScale());
	setX((int)(sf::VideoMode::getDesktopMode().width / cellSize));
	setY((int)(sf::VideoMode::getDesktopMode().height / cellSize));

	auto arr = initArr(x, y);

	return arr;
}

int** Loader::initArr(int x, int y) {
	int** arr = new int* [x];
	for (int i = 0; i < x; ++i) {
		arr[i] = new int[y];
		for (int j = 0; j < y; ++j) {
			arr[i][j] = 0;
		}
	}
	return arr;
}

void Loader::deleteArr(int** arr, int x) {
	for (int i = 0; i < x; ++i) {
		delete[] arr[i];
	}
	delete[] arr;
}

int** Loader::copyArr(int** source, int x, int y) {
	int** copy = initArr(x, y);
	for (int i = 0; i < x; ++i) {
		for (int j = 0; j < y; ++j) {
			copy[i][j] = source[i][j];
		}
	}
	return copy;
}

void Loader::determineCellSize(int minimalX, int minimalY) {
	for (auto& cellSize : CELL_SIZES) {
		int x = (int)(sf::VideoMode::getDesktopMode().width / cellSize);
		int y = (int)(sf::VideoMode::getDesktopMode().height / cellSize);

		if (x >= minimalX && y >= minimalY) {
			setX(x);
			setY(y);
			setCellSize(cellSize);
			return;
		}
	}
}

int Loader::getX() const {
	return x;
}

int Loader::getY() const {
	return y;
}

float Loader::getCellSize() const {
	return cellSize;
}

int Loader::getPresetsSize() {
	return presets.size();
}
