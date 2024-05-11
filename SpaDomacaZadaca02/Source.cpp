#include <map>
#include <sstream>
#include "GameOfLife.h"
#include "FallingSand.h"
#include "LangtonsAnt.h"
#include "ElementaryCellularAutomata.h"
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

	Loader gameOfLifeLoader(true);
	GameOfLife* gameOfLife = new GameOfLife(window, gameOfLifeLoader, font, frameRate);
	MouseListener* gameOfLifeMouseListener = new MouseListener(gameOfLife, window);

	Loader fallingSandLoader(false);
	FallingSand* fallingSand = new FallingSand(window, fallingSandLoader, font, 60);
	MouseListener* fallingSandMouseListener = new MouseListener(fallingSand, window);

	Loader langtonsAntLoader(false);
	LangtonsAnt* langtonsAnt = new LangtonsAnt(window, langtonsAntLoader, font, 2);
	MouseListener* langtonsAntMouseListener = new MouseListener(langtonsAnt, window);

	Loader rule30Loader(false);
	ElementaryCellularAutomata* rule30CellularAutomata = new ElementaryCellularAutomata(window, rule30Loader, font, 30, 30);
	MouseListener* rule30MouseListener = new MouseListener(rule30CellularAutomata, window);

	Loader rule90Loader(false);
	ElementaryCellularAutomata* rule90CellularAutomata = new ElementaryCellularAutomata(window, rule90Loader, font, 30, 90);
	MouseListener* rule90MouseListener = new MouseListener(rule90CellularAutomata, window);

	Loader rule102Loader(false);
	ElementaryCellularAutomata* rule102CellularAutomata = new ElementaryCellularAutomata(window, rule102Loader, font, 30, 102);
	MouseListener* rule102MouseListener = new MouseListener(rule102CellularAutomata, window);

	Loader rule124Loader(false);
	ElementaryCellularAutomata* rule124CellularAutomata = new ElementaryCellularAutomata(window, rule124Loader, font, 30, 124);
	MouseListener* rule124MouseListener = new MouseListener(rule124CellularAutomata, window);

	Loader rule129Loader(false);
	ElementaryCellularAutomata* rule129CellularAutomata = new ElementaryCellularAutomata(window, rule129Loader, font, 30, 129);
	MouseListener* rule129MouseListener = new MouseListener(rule129CellularAutomata, window);

	Loader rule150Loader(false);
	ElementaryCellularAutomata* rule150CellularAutomata = new ElementaryCellularAutomata(window, rule150Loader, font, 30, 150);
	MouseListener* rule150MouseListener = new MouseListener(rule150CellularAutomata, window);

	std::vector<CellularAutomata*> cellularAutomata;
	std::vector<MouseListener*> mouseListeners;

	cellularAutomata.push_back(gameOfLife);
	mouseListeners.push_back(gameOfLifeMouseListener);

	cellularAutomata.push_back(fallingSand);
	mouseListeners.push_back(fallingSandMouseListener);

	cellularAutomata.push_back(langtonsAnt);
	mouseListeners.push_back(langtonsAntMouseListener);

	cellularAutomata.push_back(rule30CellularAutomata);
	mouseListeners.push_back(rule30MouseListener);

	cellularAutomata.push_back(rule90CellularAutomata);
	mouseListeners.push_back(rule90MouseListener);

	cellularAutomata.push_back(rule102CellularAutomata);
	mouseListeners.push_back(rule102MouseListener);

	cellularAutomata.push_back(rule124CellularAutomata);
	mouseListeners.push_back(rule124MouseListener);

	cellularAutomata.push_back(rule129CellularAutomata);
	mouseListeners.push_back(rule129MouseListener);

	cellularAutomata.push_back(rule150CellularAutomata);
	mouseListeners.push_back(rule150MouseListener);

	int cellularAutomataIndex = 0;
	std::map<sf::Keyboard::Key, int> cellularAutomataBindings;
	cellularAutomataBindings[sf::Keyboard::Key::Num1] = 0;
	cellularAutomataBindings[sf::Keyboard::Key::Num2] = 1;
	cellularAutomataBindings[sf::Keyboard::Key::Num3] = 2;
	cellularAutomataBindings[sf::Keyboard::Key::Num4] = 3;
	cellularAutomataBindings[sf::Keyboard::Key::Num5] = 4;
	cellularAutomataBindings[sf::Keyboard::Key::Num6] = 5;
	cellularAutomataBindings[sf::Keyboard::Key::Num7] = 6;
	cellularAutomataBindings[sf::Keyboard::Key::Num8] = 7;
	cellularAutomataBindings[sf::Keyboard::Key::Num9] = 8;

	sf::Text modes;
	std::stringstream modesSS;
	modesSS << "Keys to change rules: ";
	for (size_t i = 0; i < cellularAutomata.size(); i++) {
		modesSS << std::to_string(i + 1);
		if (i != cellularAutomata.size() - 1) modesSS << " ";
	}
	modes.setFont(font);
	modes.setString(modesSS.str());
	modes.setCharacterSize(20 * Util::getScale());
	modes.setFillColor(sf::Color::White);
	modes.setStyle(sf::Text::Bold);
	modes.setPosition(sf::Vector2f(sf::VideoMode::getDesktopMode().width - 400 * Util::getScale(), 0));

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();

			cellularAutomata[cellularAutomataIndex]->handleControls(event);

			if (event.type == sf::Event::KeyReleased) {
				for (const auto& pair : cellularAutomataBindings) {
					if (event.key.code == pair.first) {
						cellularAutomataIndex = pair.second;
						window.setFramerateLimit(cellularAutomata[cellularAutomataIndex]->getFrameRate());
					}
				}
			}
		}

		mouseListeners[cellularAutomataIndex]->handleDrawing();

		window.clear();
		cellularAutomata[cellularAutomataIndex]->draw();
		cellularAutomata[cellularAutomataIndex]->drawControls();
		window.draw(modes);
		window.display();

		cellularAutomata[cellularAutomataIndex]->update();
	}

	for (auto& ca : cellularAutomata) {
		delete ca;
	}
	for (auto& ml : mouseListeners) {
		delete ml;
	}

	return 0;
}
