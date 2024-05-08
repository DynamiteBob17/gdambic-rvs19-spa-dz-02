#include "GameOfLife.h"
#include "MouseListener.h"
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

	Loader gameOfLifeLoader;
	GameOfLife gameOfLife(window, gameOfLifeLoader, font, frameRate);
	MouseListener gameOfLifeMouseListener(gameOfLife, window);

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();

			gameOfLife.handleControls(event);
		}

		gameOfLifeMouseListener.handleDrawing();

		window.clear();
		gameOfLife.draw();
		gameOfLife.drawControls();
		window.display();

		gameOfLife.update();
	}

	return 0;
}
