#include <stdexcept>
#include "LangtonsAnt.h"
#include "Util.h"

void LangtonsAnt::newGrid() {
	activeArr = loader.emptyArray(8);
	setInitialAnt();
}

void LangtonsAnt::setInitialAnt() {
	antX = loader.getX() / 2;
	antY = loader.getY() / 2;
	antXDir = -1;
	antYDir = 0;
	hue = HUE_MAX * 6;
}

void LangtonsAnt::turnAnt90DegClockwise() {
	int temp = antXDir;
	antXDir = -antYDir;
	antYDir = temp;
}

void LangtonsAnt::turnAnt90DegCounterClockwise() {
	int temp = antXDir;
	antXDir = antYDir;
	antYDir = -temp;
}

void LangtonsAnt::moveAnt() {
	antX += antXDir;
	antY += antYDir;
}

LangtonsAnt::LangtonsAnt(
	sf::RenderWindow& window,
	Loader& loader,
	sf::Font& font,
	int frameRate
) : CellularAutomata(window, loader, frameRate, false) {
	newGrid();
	paused = true;

	controlsStr = "Langton's Ant\nSPACE start/pause\nC reset to initial state\nDOWN --fps, UP ++fps\nfps = ";
	controls.setFont(font);
	controls.setString(controlsStr + std::to_string(frameRate));
	controls.setCharacterSize(20 * Util::getScale());
	controls.setFillColor(sf::Color::White);
	controls.setStyle(sf::Text::Bold);
}

void LangtonsAnt::draw() {
	int x = loader.getX();
	int y = loader.getY();
	float cs = loader.getCellSize();

	sf::RectangleShape cell(sf::Vector2f(cs, cs));
	for (int i = 0; i < x; ++i) {
		for (int j = 0; j < y; ++j) {
			if (activeArr[i][j] > 0) {
				cell.setPosition(sf::Vector2f(i * cs, j * cs));
				cell.setFillColor(Util::HSBtoRGB(Util::map(activeArr[i][j], 1, HUE_MAX * 12, 0.f, 1.f), 1.f, 1.f));
				window.draw(cell);
			}

			if (i == antX && j == antY) {
				cell.setPosition(sf::Vector2f(i * cs, j * cs));
				cell.setFillColor(sf::Color::Red);
				window.draw(cell);
			}
		}
	}
}

void LangtonsAnt::update() {
	if (paused || !Util::isWithinBounds(antX, antY, loader.getX(), loader.getY())) {
		return;
	}

	if (activeArr[antX][antY] == 0) {
		turnAnt90DegClockwise();
		activeArr[antX][antY] = hue;
		moveAnt();
	} else {
		turnAnt90DegCounterClockwise();
		activeArr[antX][antY] = 0;
		moveAnt();
	}

	++hue;
}

void LangtonsAnt::drawControls() {
	window.draw(controls);
}

void LangtonsAnt::handleControls(sf::Event& event) {
	if (event.type == sf::Event::KeyReleased) {
		switch (event.key.code) {
		case sf::Keyboard::Key::Space:
			pause();
			break;
		case sf::Keyboard::Key::C:
			clear();
			setInitialAnt();
			break;
		default:
			break;
		}
	} else if (event.type == sf::Event::KeyPressed) {
		switch (event.key.code) {
		case sf::Keyboard::Key::Up:
			if (frameRate < 480) {
				++frameRate;
				controls.setString(controlsStr + std::to_string(frameRate));
				setFrameRate(frameRate);
				window.setFramerateLimit(frameRate);
			}
			break;
		case sf::Keyboard::Key::Down:
			if (frameRate > 1) {
				--frameRate;
				controls.setString(controlsStr + std::to_string(frameRate));
				setFrameRate(frameRate);
				window.setFramerateLimit(frameRate);
			}
			break;
		default:
			break;
		}
	}
}
