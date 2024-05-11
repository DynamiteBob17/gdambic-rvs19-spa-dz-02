#pragma once

#include <stack>
#include "CellularAutomata.h"

struct GameRecording {
	int** arr;
	int x;
	int y;
	GameRecording(int** arr, int x, int y) {
		this->arr = arr;
		this->x = x;
		this->y = y;
	}
};

class GameOfLife : public CellularAutomata {
private:
	int** activeArrOrg;
	std::stack<GameRecording> history;
	std::stack<GameRecording> future;
	int noChangesCount;
	int lifetime;
	int badStopwatch;

	void newGrid() override;
	int countNeighbors(int x, int y);

public:
	GameOfLife(sf::RenderWindow& window, Loader& loader, sf::Font& font, int frameRate);
	~GameOfLife();
	void next();
	void prev();
	void draw() override;
	void update() override;
	void drawControls() override;
	void handleControls(sf::Event& event) override;
};
