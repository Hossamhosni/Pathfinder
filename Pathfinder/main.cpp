#include <iostream>
#include <vector>
#include <map>
#include <cmath>
#include <algorithm>
#include <string>
#include <iomanip>

#include <SFML/Graphics.hpp>

#include "AStarSolver.h"
using namespace std;

int main() {
	
	vector<vector<Cell*>> grid(SIZE);
	AStarSolver solver(grid);
	sf::RenderWindow window(sf::VideoMode(600, 600), "Maze");
	solver.setup();
	sf::Clock clock;
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {

				sf::Vector2i localPosition = sf::Mouse::getPosition(window);
				int x = localPosition.x * SIZE / window.getSize().x;
				int y = localPosition.y * SIZE / window.getSize().y;
				if (x >= 0 && y >= 0 && x < SIZE && y < SIZE)
					solver.make_cell_wall(y, x);
			}
			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Enter) {
					solver.solve();
				}
				else if (event.key.code == sf::Keyboard::Escape) {
					solver.clear_grid();
				}
			}
			clock.restart();
		}

		window.clear();
		solver.draw_grid(&window);
		window.display();
	}
	return 0;
}