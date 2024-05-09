#pragma once

#include "CellularAutomata.h"

class LangtonsAnt : public CellularAutomata {
private:
	void newGrid() override;
	int antX;
	int antY;
	int antXDir;
	int antYDir;

	void setInitialAnt();
	void turnAnt90DegClockwise();
	void turnAnt90DegCounterClockwise();
	void moveAnt();

public:
	LangtonsAnt(sf::RenderWindow& window, Loader& loader, sf::Font& font, int frameRate);
	void draw() override;
	void update() override;
	void drawControls() override;
	void handleControls(sf::Event& event) override;
};
