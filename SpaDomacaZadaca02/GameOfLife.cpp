#include "GameOfLife.h"
#include "Util.h"

void GameOfLife::newGrid() {
	loader.deleteArr(activeArr, 1);

	std::random_device rd;
	std::default_random_engine gen(rd());
	std::uniform_int_distribution<int> dis(1, 20); // 5% chance for random array
	int** tempArr = dis(gen) == 1 ? loader.randomArray() : loader.randomPreset();

	int x = loader.getX();
	int y = loader.getY();

	activeArr = new int* [x];
	for (int i = 0; i < x; ++i) {
		activeArr[i] = new int[y];
		for (int j = 0; j < y; ++j) {
			activeArr[i][j] = tempArr[i][j];
		}
		delete[] tempArr[i];
	}
	delete[] tempArr;

	activeArrOrg = loader.copyArr(activeArr, x, y);

	badStopwatch = 0;
}

int GameOfLife::countNeighbors(int x, int y) {
	int count = 0;
	int _x = loader.getX();
	int _y = loader.getY();

	for (int i = -1; i <= 1; ++i) {
		for (int j = -1; j <= 1; ++j) {
			if (!(i == 0 && j == 0)) {
				int i2 = (i + x + _x) % _x;
				int j2 = (j + y + _y) % _y;
				if (activeArr[i2][j2] > 0) {
					++count;
				}
			}
		}
	}

	return count;
}

GameOfLife::GameOfLife(
	sf::RenderWindow& window,
	Loader& loader,
	sf::Font& font,
	int frameRate
) : CellularAutomata(window, loader, frameRate, false) {
	noChangesCount = 0;
	lifetime = 120;
	paused = true;

	newGrid();

	controlsStr = "SPACE start/pause\nD toggle draw mode\n(L_CLICK alive, R_CLICK dead)\nC kill alive cells\nLEFT prev, RIGHT next\nDOWN --fps, UP ++fps\nfps = ";
	controls.setFont(font);
	controls.setString(controlsStr + std::to_string(frameRate));
	controls.setCharacterSize(20 * Util::getScale());
	controls.setFillColor(sf::Color::White);
	controls.setStyle(sf::Text::Bold);

	note.setFont(font);
	note.setString("[There are " + std::to_string(loader.getPresetsSize())
		+ " unique game presets chosen at random\nwith a 5% chance of a completely random game;\ngames change automatically after ~"
		+ std::to_string(lifetime) + " s]"
	);
	note.setCharacterSize(20 * Util::getScale());
	note.setFillColor(sf::Color::White);
	note.setPosition(sf::Vector2f(0, sf::VideoMode::getDesktopMode().height - 75 * Util::getScale()));
}

GameOfLife::~GameOfLife() {
	loader.deleteArr(activeArrOrg, loader.getX());
}

void GameOfLife::next() {
	history.push(GameRecording(activeArrOrg, loader.getX(), loader.getY()));

	if (!future.empty()) {
		activeArrOrg = future.top().arr;
		activeArr = loader.copyArr(activeArrOrg, future.top().x, future.top().y);
		loader.determineCellSize(future.top().x, future.top().y);
		future.pop();

		badStopwatch = 0;
	} else {
		newGrid();
	}
}

void GameOfLife::prev() {
	if (history.empty()) return;

	future.push(GameRecording(activeArrOrg, loader.getX(), loader.getY()));
	activeArrOrg = history.top().arr;
	activeArr = loader.copyArr(activeArrOrg, history.top().x, history.top().y);
	loader.determineCellSize(history.top().x, history.top().y);
	history.pop();

	badStopwatch = 0;
}

void GameOfLife::draw() {
	int x = loader.getX();
	int y = loader.getY();
	float cs = loader.getCellSize();

	if (drawMode) {
		window.draw(drawingGrid);
	}

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

void GameOfLife::update() {
	if (paused || drawMode) return;

	++badStopwatch;
	if (badStopwatch >= lifetime * frameRate) {
		next();
	}

	int x = loader.getX();
	int y = loader.getY();
	bool changed = false;
	int** nextArr = loader.initArr(x, y);

	for (int i = 0; i < x; ++i) {
		for (int j = 0; j < y; ++j) {
			int neighbors = countNeighbors(i, j);

			if (activeArr[i][j] > 0 && (neighbors == 2 || neighbors == 3)) {
				nextArr[i][j] = true;
			} else if (neighbors == 3) {
				nextArr[i][j] = true;
			}

			if (!changed && nextArr[i][j] != activeArr[i][j]) {
				changed = true;
			}
		}
	}

	loader.deleteArr(activeArr, x);
	activeArr = nextArr;

	if (!changed) {
		++noChangesCount;
		if (noChangesCount >= frameRate * 5) {
			badStopwatch = lifetime * frameRate;
			noChangesCount = 0;
		}
	}
}

void GameOfLife::drawControls() {
	window.draw(controls);
	window.draw(note);
}

void GameOfLife::handleControls(sf::Event& event) {
	if (event.type == sf::Event::KeyReleased) {
		switch (event.key.code) {
		case sf::Keyboard::Key::Space:
			if (!drawMode) {
				pause();
			}
			break;
		case sf::Keyboard::Key::Right:
			if (!drawMode) {
				next();
			}
			break;
		case sf::Keyboard::Key::Left:
			if (!drawMode) {
				prev();
			}
			break;
		case sf::Keyboard::Key::D:
			window.setFramerateLimit(
				!drawMode ? 60 : frameRate
			);
			controls.setString(
				controlsStr + std::to_string(frameRate)
				+ (!drawMode ? " (locked)" : "")
				+ (!drawMode ? "\n(prev/next disabled)" : "")
			);
			toggleDrawMode();
			break;
		case sf::Keyboard::Key::C:
			clear();
			break;
		default:
			break;
		}
	} else if (event.type == sf::Event::KeyPressed) {
		switch (event.key.code) {
		case sf::Keyboard::Key::Up:
			if (frameRate < 60 && !drawMode) {
				++frameRate;
				controls.setString(controlsStr + std::to_string(frameRate));
				setFrameRate(frameRate);
				window.setFramerateLimit(frameRate);
			}
			break;
		case sf::Keyboard::Key::Down:
			if (frameRate > 1 && !drawMode) {
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
