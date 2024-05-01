#include "GameOfLife.h"
#include "Util.h"

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

	std::string txt = "LEFT prev, RIGHT next\nDOWN --fps, UP ++fps\nSPACE pause\nfps = ";
	sf::Text controls;
	controls.setFont(font);
	controls.setString(txt + std::to_string(frameRate));
	controls.setCharacterSize(20 * Util::getScale());
	controls.setFillColor(sf::Color::White);
	controls.setStyle(sf::Text::Bold);

	sf::Text note;
	note.setFont(font);
	note.setString("[There are " + std::to_string(gameOfLife.getLoader().getPresetsSize()) + " unique game presets chosen at random,\nwith a 5% chance of a completely random game]");
	note.setCharacterSize(20 * Util::getScale());
	note.setFillColor(sf::Color::White);
	note.setPosition(sf::Vector2f(
		0,
		sf::VideoMode::getDesktopMode().height - 50 * Util::getScale())
	);

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == sf::Event::KeyReleased) {
				switch (event.key.code) {
				case sf::Keyboard::Key::Space:
					gameOfLife.pause();
					break;
				case sf::Keyboard::Key::Right:
					gameOfLife.next();
					break;
				case sf::Keyboard::Key::Left:
					gameOfLife.prev();
					break;
				default:
					break;
				}
			}
			else if (event.type == sf::Event::KeyPressed) {
				switch (event.key.code) {
				case sf::Keyboard::Key::Up:
					if (frameRate < 60) {
						++frameRate;
						controls.setString(txt + std::to_string(frameRate));
						gameOfLife.setFrameRate(frameRate);
						window.setFramerateLimit(frameRate);
					}
					break;
				case sf::Keyboard::Key::Down:
					if (frameRate > 1) {
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

		window.clear();
		window.draw(note);
		gameOfLife.draw();
		window.draw(controls);
		window.display();

		gameOfLife.update();
	}

	return 0;
}
