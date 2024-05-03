#pragma once

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
	int noChangesCount;
	int lifetime;
	int badStopwatch;
	bool paused;
	bool drawMode;
	sf::RenderTexture renderTexture;
	sf::Sprite drawingGrid;

	int countNeighbors(int x, int y);
	void generateDrawingGrid();

public:
	GameOfLife(sf::RenderWindow* window, int frameRate);
	~GameOfLife();
	void setArr();
	void next();
	void prev();
	void draw();
	void update();
	void pause();
	unsigned int getFrameRate();
	void setFrameRate(unsigned int frameRate);
	Loader getLoader();
	int getLifetime();
	void toggleDrawMode();
	bool isDrawMode();
	void setCell(int x, int y, bool value);
	void clear();
};
