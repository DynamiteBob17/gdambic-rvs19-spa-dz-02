#include "GameOfLife.h"
#include "Util.h"

static bool isMouseOverWindow(const sf::RenderWindow& window) {
	sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
	return mousePosition.x >= 0 && mousePosition.y >= 0
		&& mousePosition.x < window.getSize().x && mousePosition.y < window.getSize().y;
}

static sf::Vector2i getCellAtMousePosition(GameOfLife& gameOfLife, const sf::RenderWindow& window) {
	sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
	float cellSize = gameOfLife.getLoader().getCellSize();
	int cellX = mousePosition.x / cellSize;
	int cellY = mousePosition.y / cellSize;
	return sf::Vector2i(cellX, cellY);
}

static void handleDrawing(GameOfLife& gameOfLife, const sf::RenderWindow& window) {
	if (!(isMouseOverWindow(window) && gameOfLife.isDrawMode())) {
		return;
	}

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		sf::Vector2i cellPos = getCellAtMousePosition(gameOfLife, window);
		gameOfLife.setCell(cellPos.x, cellPos.y, true);
	} else if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
		sf::Vector2i cellPos = getCellAtMousePosition(gameOfLife, window);
		gameOfLife.setCell(cellPos.x, cellPos.y, false);
	}
}

int main() {
	sf::Font font;
	if (!font.loadFromFile("Minecraft.ttf")) {
		return EXIT_FAILURE;
	}

	sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Hello, SFML world!", sf::Style::Fullscreen);
	unsigned int frameRate = 20;
	window.setFramerateLimit(frameRate);
	window.display();

	GameOfLife gameOfLife(&window, frameRate);

	std::string txt = "D toggle draw mode\n(L_CLICK alive, R_CLICK dead)\nLEFT prev, RIGHT next\nDOWN --fps, UP ++fps\nSPACE pause\nfps = ";
	sf::Text controls;
	controls.setFont(font);
	controls.setString(txt + std::to_string(frameRate));
	controls.setCharacterSize(20 * Util::getScale());
	controls.setFillColor(sf::Color::White);
	controls.setStyle(sf::Text::Bold);

	sf::Text note;
	note.setFont(font);
	note.setString("[There are " + std::to_string(gameOfLife.getLoader().getPresetsSize())
		+ " unique game presets chosen at random\nwith a 5% chance of a completely random game;\ngames change automatically after ~"
		+ std::to_string(gameOfLife.getLifetime()) + " s]"
	);
	note.setCharacterSize(20 * Util::getScale());
	note.setFillColor(sf::Color::White);
	note.setPosition(sf::Vector2f(0, sf::VideoMode::getDesktopMode().height - 75 * Util::getScale()));

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == sf::Event::KeyReleased) {
				switch (event.key.code) {
				case sf::Keyboard::Key::Space:
					if (!gameOfLife.isDrawMode()) {
						gameOfLife.pause();
					}
					break;
				case sf::Keyboard::Key::Right:
					gameOfLife.next();
					break;
				case sf::Keyboard::Key::Left:
					gameOfLife.prev();
					break;
				case sf::Keyboard::Key::D:
					window.setFramerateLimit(
						!gameOfLife.isDrawMode() ? 60 : gameOfLife.getFrameRate()
					);
					controls.setString(
						txt + std::to_string(frameRate)
						+ (!gameOfLife.isDrawMode() ? " (locked)" : "")
					);
					gameOfLife.toggleDrawMode();
					break;
				default:
					break;
				}
			} else if (event.type == sf::Event::KeyPressed) {
				switch (event.key.code) {
				case sf::Keyboard::Key::Up:
					if (frameRate < 60 && !gameOfLife.isDrawMode()) {
						++frameRate;
						controls.setString(txt + std::to_string(frameRate));
						gameOfLife.setFrameRate(frameRate);
						window.setFramerateLimit(frameRate);
					}
					break;
				case sf::Keyboard::Key::Down:
					if (frameRate > 1 && !gameOfLife.isDrawMode()) {
						--frameRate;
						controls.setString(txt + std::to_string(frameRate));
						gameOfLife.setFrameRate(frameRate);
						window.setFramerateLimit(frameRate);
					}
					break;
				default:
					break;
				}
			}
		}

		handleDrawing(gameOfLife, window);

		window.clear();
		window.draw(note);
		gameOfLife.draw();
		window.draw(controls);
		window.display();

		gameOfLife.update();
	}

	return 0;
}
