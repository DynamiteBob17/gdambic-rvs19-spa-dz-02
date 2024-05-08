#include "MouseListener.h"

bool MouseListener::isMouseOverWindow() const {
	sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
	return mousePosition.x >= 0 && mousePosition.y >= 0
		&& mousePosition.x < window.getSize().x && mousePosition.y < window.getSize().y;
}

sf::Vector2i MouseListener::getCellAtMousePosition() const {
	sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
	float cellSize = cellularAutomata.getLoader().getCellSize();
	int cellX = mousePosition.x / cellSize;
	int cellY = mousePosition.y / cellSize;
	return sf::Vector2i(cellX, cellY);
}

void MouseListener::handleDrawing() {
	if (!(isMouseOverWindow() && cellularAutomata.isDrawMode())) {
		return;
	}

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		sf::Vector2i cellPos = getCellAtMousePosition();
		cellularAutomata.setCell(cellPos.x, cellPos.y, true);
	} else if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
		sf::Vector2i cellPos = getCellAtMousePosition();
		cellularAutomata.setCell(cellPos.x, cellPos.y, false);
	}
}
