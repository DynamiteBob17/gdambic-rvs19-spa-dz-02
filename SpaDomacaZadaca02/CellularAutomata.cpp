#include "CellularAutomata.h"

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
			activeArr[i][j] = false;
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
    activeArr = loader.initArr(0, 0);
    this->drawMode = drawMode;
    paused = false;
}

CellularAutomata::~CellularAutomata() {
    loader.deleteArr(activeArr, loader.getX());
}

Loader CellularAutomata::getLoader() {
	return loader;
}

bool CellularAutomata::isDrawMode() const {
	return drawMode;
}

void CellularAutomata::setCell(int x, int y, bool value) {
	activeArr[x][y] = value;
}
