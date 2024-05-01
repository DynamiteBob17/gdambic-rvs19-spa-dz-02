#pragma once

#include <SFML/Graphics.hpp>
#include <stack>
#include "Loader.h"

struct GameRecording {
	bool** arr;
	int x;
	int y;
	GameRecording(bool** arr, int x, int y) {
		this->arr = arr;
		this->x = x;
		this->y = y;
	}
};

class GameOfLife {
private:
	sf::RenderWindow* window;
	Loader loader;
	bool** activeArr;
	bool** activeArrOrg;
	std::stack<GameRecording> history;
	std::stack<GameRecording> future;
	int frameRate;
	int colorIdx;
	int noChangesCount;
	int lifetime;
	int badStopwatch;
	bool paused;

	int countNeighbors(int x, int y);

public:
	GameOfLife(sf::RenderWindow* window, int frameRate);
	void setArr();
	void next();
	void prev();
	void draw();
	void update();
	void pause();
	void setFrameRate(unsigned int frameRate);
	Loader getLoader();
};
