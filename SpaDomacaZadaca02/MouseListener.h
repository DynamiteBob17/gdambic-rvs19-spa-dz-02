#pragma once

#include "CellularAutomata.h"

class MouseListener {
private:
	CellularAutomata& cellularAutomata;
	sf::RenderWindow& window;

	bool isMouseOverWindow() const;
	sf::Vector2i getCellAtMousePosition() const;

public:
	MouseListener(
		CellularAutomata& cellularAutomata,
		sf::RenderWindow& window
	) : cellularAutomata(cellularAutomata), window(window) {}
	void handleDrawing();
};
