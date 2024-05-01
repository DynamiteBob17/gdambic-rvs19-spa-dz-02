#include "GameOfLife.h"

int GameOfLife::countNeighbors(int x, int y) {
	int count = 0;
	int _x = loader.getX();
	int _y = loader.getY();

	for (int i = -1; i <= 1; ++i) {
		for (int j = -1; j <= 1; ++j) {
			if (!(i == 0 && j == 0)) {
				int i2 = (i + x + _x) % _x;
				int j2 = (j + y + _y) % _y;
				if (activeArr[i2][j2]) {
					++count;
				}
			}
		}
	}

	return count;
}

GameOfLife::GameOfLife(sf::RenderWindow* window, int frameRate) {
	this->window = window;
	this->frameRate = frameRate;
	colorIdx = 0;
	noChangesCount = 0;
	lifetime = 120;
	setArr();
}

void GameOfLife::setArr() {
	std::random_device rd;
	std::default_random_engine gen(rd());
	std::uniform_int_distribution<int> dis(1, 20); // 5% chance for random array
	bool** tempArr = dis(gen) == 1 ? loader.randomArray() : loader.randomPreset();

	int x = loader.getX();
	int y = loader.getY();

	activeArr = new bool* [x];
	for (int i = 0; i < x; ++i) {
		activeArr[i] = new bool[y];
		for (int j = 0; j < y; ++j) {
			activeArr[i][j] = tempArr[i][j];
		}
		delete[] tempArr[i];
	}
	delete[] tempArr;

	activeArrOrg = loader.copyArr(activeArr, x, y);

	badStopwatch = 0;
}

void GameOfLife::next() {
	history.push(GameRecording(activeArrOrg, loader.getX(), loader.getY()));

	if (!future.empty()) {
		activeArrOrg = future.top().arr;
		activeArr = loader.copyArr(activeArrOrg, future.top().x, future.top().y);
		loader.determineCellSize(future.top().x, future.top().y);
		future.pop();

		badStopwatch = 0;
	}
	else {
		setArr();
	}
}

void GameOfLife::prev() {
	if (history.empty()) return;

	future.push(GameRecording(activeArrOrg, loader.getX(), loader.getY()));
	activeArrOrg = history.top().arr;
	activeArr = loader.copyArr(activeArrOrg, history.top().x, history.top().y);
	loader.determineCellSize(history.top().x, history.top().y);
	history.pop();

	badStopwatch = 0;
}

static sf::Color HSBToRGB(float hue, float saturation, float brightness) {
	float r, g, b;

	int i = floor(hue * 6);
	float f = hue * 6 - i;
	float p = brightness * (1 - saturation);
	float q = brightness * (1 - f * saturation);
	float t = brightness * (1 - (1 - f) * saturation);

	switch (i % 6) {
	case 0: r = brightness; g = t, b = p; break;
	case 1: r = q; g = brightness; b = p; break;
	case 2: r = p; g = brightness; b = t; break;
	case 3: r = p; g = q; b = brightness; break;
	case 4: r = t; g = p; b = brightness; break;
	case 5: r = brightness; g = p; b = q; break;
	default: r = brightness; g = t, b = p; break;
	}

	return sf::Color(r * 255, g * 255, b * 255);
}

void GameOfLife::draw() {
	int x = loader.getX();
	int y = loader.getY();
	float cs = loader.getCellSize();

	sf::RectangleShape cell(sf::Vector2f(cs, cs));
	int colorCounter = 0;
	for (int i = 0; i < x; ++i) {
		for (int j = 0; j < y; ++j) {
			++colorCounter;
			if (activeArr[i][j]) {
				cell.setPosition(sf::Vector2f(i * cs, j * cs));
				cell.setFillColor(HSBToRGB((colorCounter * 1.f) / (1.f * x * y), 1.f, 1.f));
				window->draw(cell);
			}
		}
	}
}

void GameOfLife::update() {
	if (paused) {
		return;
	}

	++colorIdx;
	++badStopwatch;
	if (badStopwatch >= lifetime * frameRate) {
		next();
	}

	int x = loader.getX();
	int y = loader.getY();
	bool changed = false;
	bool** nextArr = loader.initArr(x, y);

	for (int i = 0; i < x; ++i) {
		for (int j = 0; j < y; ++j) {
			int neighbors = countNeighbors(i, j);
			if (activeArr[i][j]) {
				if (neighbors == 2 || neighbors == 3) {
					nextArr[i][j] = true;
				}
			}
			else if (neighbors == 3) {
				nextArr[i][j] = true;
			}

			if (!changed && nextArr[i][j] != activeArr[i][j]) {
				changed = true;
			}
		}
	}

	loader.deleteArr(activeArr, x);
	activeArr = nextArr;

	if (!changed) {
		++noChangesCount;
		if (noChangesCount >= frameRate * 5) {
			badStopwatch = lifetime * frameRate;
			noChangesCount = 0;
		}
	}
}

void GameOfLife::pause() {
	this->paused = !paused;
}

void GameOfLife::setFrameRate(unsigned int frameRate) {
	this->frameRate = frameRate;
}

Loader GameOfLife::getLoader() {
	return loader;
}
