#pragma once

#include "CellularAutomata.h"

class ElementaryCellularAutomata : public CellularAutomata {
private:
	uint8_t rule;
	int currentRow;

	void newGrid() override;

public:
	ElementaryCellularAutomata(sf::RenderWindow& window, Loader& loader, sf::Font& font, int frameRate, uint8_t rule);
	void draw() override;
	void update() override;
	void drawControls() override;
	void handleControls(sf::Event& event) override;
};
