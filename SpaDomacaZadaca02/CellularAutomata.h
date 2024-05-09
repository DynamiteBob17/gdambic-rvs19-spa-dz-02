#pragma once

#include "Loader.h"

class CellularAutomata {
protected:
	sf::RenderWindow& window;
	Loader loader;
	int** activeArr;
	unsigned int frameRate;
	bool paused;
	bool drawMode;
	sf::RenderTexture renderTexture;
	sf::Sprite drawingGrid;
	std::string controlsStr;
	sf::Text controls;
	sf::Text note;
	std::random_device rd;
	std::default_random_engine gen;
	int hue;

	static int const HUE_MAX = 1080;

	virtual void newGrid() = 0;
	void generateDrawingGrid();
	void pause();
	void clear();
	void toggleDrawMode();
	void setFrameRate(unsigned int frameRate);

public:
	CellularAutomata(sf::RenderWindow& window, Loader loader, int frameRate, bool drawMode);
	~CellularAutomata();
	virtual void draw() = 0;
	virtual void update() = 0;
	virtual void drawControls() = 0;
	virtual void handleControls(sf::Event& event) = 0;
	Loader getLoader();
	unsigned int getFrameRate() const;
	bool isDrawMode() const;
	void setCell(int x, int y, int value, bool multiple);
};
