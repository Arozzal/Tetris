#include <SFML/Graphics.hpp>
#include <iostream>

#include <chrono>
#include <thread>

#include "field.hpp"
#include "BlockBody.hpp"

int main()
{
	const int PIXELSIZE = 32;
	const int WIDTH = 10;
	const int HEIGHT = 25;
	const int BORDER = 8;

	sf::RenderWindow window(sf::VideoMode((WIDTH + 2 + 2 * BORDER) * PIXELSIZE, (HEIGHT + 2) * PIXELSIZE + 50), "Tetris!");
	sf::CircleShape shape(100.f);
	shape.setFillColor(sf::Color::Green);

	Field* field = new Field(WIDTH, HEIGHT, PIXELSIZE, &window);

	srand(time(NULL));

	
	int y = 0;


	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}


		field->update();

		field->render();

		if (field->isRestard()) {
			delete field;
			field = new Field(WIDTH, HEIGHT, PIXELSIZE, &window);
		}

	}

	return 0;
}