#include "ElementaryCellularAutomata.h"
#include "Util.h"

void ElementaryCellularAutomata::newGrid() {
    loader.deleteArr(activeArr, 1);
    activeArr = loader.emptyArray(4);
    activeArr[loader.getX() / 2][0] = 1;
}

ElementaryCellularAutomata::ElementaryCellularAutomata(
    sf::RenderWindow& window,
    Loader& loader,
    sf::Font& font,
    int frameRate,
	uint8_t rule
) : CellularAutomata(window, loader, frameRate, false), rule(rule) {
    paused = true;
	currentRow = 0;

	newGrid();

    controlsStr = "Cellular Automata Rule " + std::to_string(rule) + "\nSPACE start/pause\nC reset to initial state";
    controls.setFont(font);
    controls.setString(controlsStr);
    controls.setCharacterSize(20 * Util::getScale());
    controls.setFillColor(sf::Color::White);
    controls.setStyle(sf::Text::Bold);
}

void ElementaryCellularAutomata::draw() {
	int x = loader.getX();
	int y = loader.getY();
	float cs = loader.getCellSize();

	sf::RectangleShape cell(sf::Vector2f(cs, cs));
	int colorCounter = 0;
	for (int i = 0; i < x; ++i) {
		for (int j = 0; j < y; ++j) {
			++colorCounter;
			if (activeArr[i][j] == 1) {
				cell.setPosition(sf::Vector2f(i * cs, j * cs));
				cell.setFillColor(Util::HSBtoRGB((colorCounter * 1.f) / (1.f * x * y), 1.f, 1.f));
				window.draw(cell);
			}
		}
	}
}

void ElementaryCellularAutomata::update() {
	int x = loader.getX();
	int y = loader.getY();

	if (paused || currentRow + 1 >= y) return;

	for (int i = 0; i < x; ++i) {
		int leftIndex = (i - 1 + x) % x;
		int rightIndex = (i + 1) % x;

		bool bit2 = activeArr[leftIndex][currentRow] & 1;
		bool bit1 = activeArr[i][currentRow] & 1;
		bool bit0 = activeArr[rightIndex][currentRow] & 1;

		uint8_t bits = (bit2 << 2) | (bit1 << 1) | bit0;

		activeArr[i][currentRow + 1] = (rule >> bits) & 1;
	}

	if (currentRow < y) ++currentRow;
}

void ElementaryCellularAutomata::drawControls() {
    window.draw(controls);
}

void ElementaryCellularAutomata::handleControls(sf::Event& event) {
	if (event.type == sf::Event::KeyReleased) {
		switch (event.key.code) {
		case sf::Keyboard::Key::Space:
			pause();
			break;
		case sf::Keyboard::Key::C:
			clear();
            activeArr[loader.getX() / 2][0] = 1;
			currentRow = 0;
			break;
		default:
			break;
		}
	}
}

unsigned int ElementaryCellularAutomata::getFrameRate() const {
	return frameRate;
}
