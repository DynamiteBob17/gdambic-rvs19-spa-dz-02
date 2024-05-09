#pragma once

#include "CellularAutomata.h"

class FallingSand : public CellularAutomata {
private:
	void newGrid() override;
	std::random_device rd;
	std::default_random_engine gen;
	std::bernoulli_distribution dis;
	float** velocityArr;

public:
	FallingSand(sf::RenderWindow& window, Loader& loader, sf::Font& font, int frameRate);
	void draw() override;
	void update() override;
	void drawControls() override;
	void handleControls(sf::Event& event) override;
};
