#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <random>

class Loader {
private:
	std::vector<float> CELL_SIZES;
	std::vector<std::string> presets;
	int x;
	int y;
	float cellSize;

	void fillPresets();
	bool** createPresetArray(std::string rle);
	bool** insertArrayInMiddle(
		bool** smaller,
		int smallerX,
		int smallerY
	);

public:
	Loader();
	bool** randomArray();
	bool** randomPreset();
	bool** initArr(int x, int y);
	void deleteArr(bool** arr, int x);
	bool** copyArr(bool** source, int x, int y);
	float getCellSize() const;
	int getX() const;
	int getY() const;
	void determineCellSize(int sx, int sy);
	void setCellSize(float cellSize);
	void setX(int x);
	void setY(int y);
	int getPresetsSize();
};
