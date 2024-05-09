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
	int** createPresetArray(std::string rle);
	int** insertArrayInMiddle(
		int** smaller,
		int smallerX,
		int smallerY
	);
	void setX(int x);
	void setY(int y);
	void setCellSize(float cellSize);

public:
	Loader(bool isGameOfLife);
	int** randomArray();
	int** randomPreset();
	int** emptyArray(int rawCellSize);
	int** initArr(int x, int y);
	void deleteArr(int** arr, int x);
	int** copyArr(int** source, int x, int y);
	void determineCellSize(int sx, int sy);
	int getX() const;
	int getY() const;
	float getCellSize() const;
	int getPresetsSize();
};
