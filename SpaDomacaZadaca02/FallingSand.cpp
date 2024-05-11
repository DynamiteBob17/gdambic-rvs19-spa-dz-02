#include "FallingSand.h"
#include "Util.h"

void FallingSand::newGrid() {
	loader.deleteArr(activeArr, 1);
	activeArr = loader.emptyArray(6);
}

FallingSand::FallingSand(
	sf::RenderWindow& window,
	Loader& loader,
	sf::Font& font,
	int frameRate
) : CellularAutomata(window, loader, frameRate, true) {
	newGrid();
	gen = std::default_random_engine(rd());

	controls.setFont(font);
	controls.setString("Draw (falling) sand\n(L_CLICK put sand, R_CLICK remove sand)\nC clean up the sand\nSPACE pause");
	controls.setCharacterSize(20 * Util::getScale());
	controls.setFillColor(sf::Color::White);
	controls.setStyle(sf::Text::Bold);
}

void FallingSand::draw() {
	int x = loader.getX();
	int y = loader.getY();
	float cs = loader.getCellSize();

	sf::RectangleShape cell(sf::Vector2f(cs, cs));
	for (int i = 0; i < x; ++i) {
		for (int j = 0; j < y; ++j) {
			if (activeArr[i][j] > 0) {
				cell.setPosition(sf::Vector2f(i * cs, j * cs));
				cell.setFillColor(Util::HSBtoRGB(Util::map(activeArr[i][j], 1, HUE_MAX, 0.f, 1.f), 1.f, 1.f));
				window.draw(cell);
			}
		}
	}
}

void FallingSand::update() {
	if (paused) return;

	int x = loader.getX();
	int y = loader.getY();
	int** nextArr = loader.initArr(x, y);

	for (int i = 0; i < x; ++i) {
		for (int j = 0; j < y; ++j) {
			if (activeArr[i][j] == 0) continue;

			if (j + 1 < y && activeArr[i][j + 1] == 0) {
				nextArr[i][j + 1] = activeArr[i][j];
			} else if (j + 1 < y) {
				bool leftEmpty = i - 1 >= 0 && !activeArr[i - 1][j + 1];
				bool rightEmpty = i + 1 < x && !activeArr[i + 1][j + 1];

				if (leftEmpty && rightEmpty) {
					int dir = dis(gen) ? -1 : 1;
					nextArr[i + dir][j + 1] = activeArr[i][j];
				} else if (leftEmpty) {
					nextArr[i - 1][j + 1] = activeArr[i][j];
				} else if (rightEmpty) {
					nextArr[i + 1][j + 1] = activeArr[i][j];
				} else {
					nextArr[i][j] = activeArr[i][j];
				}
			} else {
				nextArr[i][j] = activeArr[i][j];
			}
		}
	}

	loader.deleteArr(activeArr, x);
	activeArr = nextArr;
}

void FallingSand::drawControls() {
	window.draw(controls);
}

void FallingSand::handleControls(sf::Event& event) {
	if (event.type == sf::Event::KeyReleased) {
		switch (event.key.code) {
		case sf::Keyboard::Key::Space:
			pause();
			break;
		case sf::Keyboard::Key::C:
			clear();
			break;
		default:
			break;
		}
	}
}

unsigned int FallingSand::getFrameRate() const {
	return frameRate;
}
