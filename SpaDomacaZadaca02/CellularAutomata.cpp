#include "CellularAutomata.h"
#include "Util.h"

void CellularAutomata::generateDrawingGrid() {
	int x = loader.getX();
	int y = loader.getY();
	float cs = loader.getCellSize();

	renderTexture.create(
		loader.getX() * cs,
		loader.getY() * cs
	);
	renderTexture.clear();

	sf::RectangleShape cell(sf::Vector2f(cs, cs));
	cell.setOutlineThickness(1.f);
	cell.setOutlineColor(sf::Color(128, 128, 128));
	cell.setFillColor(sf::Color::Black);

	for (int i = 0; i < x; ++i) {
		for (int j = 0; j < y; ++j) {
			cell.setPosition(sf::Vector2f(i * cs, j * cs));
			renderTexture.draw(cell);
		}
	}

	renderTexture.display();
	drawingGrid.setTexture(renderTexture.getTexture());
}

void CellularAutomata::pause() {
	this->paused = !paused;
}

void CellularAutomata::clear() {
	for (int i = 0; i < loader.getX(); ++i) {
		for (int j = 0; j < loader.getY(); ++j) {
			activeArr[i][j] = 0;
		}
	}
}

void CellularAutomata::toggleDrawMode() {
	if (!drawMode) {
		generateDrawingGrid();
	}
	drawMode = !drawMode;
}

void CellularAutomata::setFrameRate(unsigned int frameRate) {
	this->frameRate = frameRate;
}

CellularAutomata::CellularAutomata(
    sf::RenderWindow& window,
    Loader loader,
    int frameRate,
    bool drawMode
) : window(window),
    loader(loader),
    frameRate(frameRate) {
	activeArr = loader.initArr(1, 1);
    this->drawMode = drawMode;
    paused = false;
	hue = 1;
}

CellularAutomata::~CellularAutomata() {
    loader.deleteArr(activeArr, loader.getX());
}

Loader CellularAutomata::getLoader() {
	return loader;
}

unsigned int CellularAutomata::getFrameRate() const {
	return frameRate;
}

bool CellularAutomata::isDrawMode() const {
	return drawMode;
}

void CellularAutomata::setCell(int x, int y, int value, bool multiple) {
	if (multiple && value != 0) {
		std::uniform_int_distribution<int> dis(1, 4);
		int _x = loader.getX();
		int _y = loader.getY();
		int boxSize = 8;
		int diff = boxSize / 2;

		for (int i = x - diff; i <= x + diff; ++i) {
			for (int j = y - diff; j <= y + diff; ++j) {
				if (Util::isWithinBounds(i, j, _x, _y) && dis(gen) == 1) {
					activeArr[i][j] = hue;
				}
			}
		}

		++hue;
		if (hue > HUE_MAX) {
			hue = 1;
		}
	} else {
		activeArr[x][y] = value;
	}
}
