#pragma once

#include "Loader.h"

class CellularAutomata {
protected:
	sf::RenderWindow& window;
	Loader loader;
	bool** activeArr;
	int frameRate;
	bool paused;
	bool drawMode;
	sf::RenderTexture renderTexture;
	sf::Sprite drawingGrid;
	std::string controlsStr;
	sf::Text controls;
	sf::Text note;

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
	bool isDrawMode() const;
	void setCell(int x, int y, bool value);
};
