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
	const int HEIGHT = 35;

	sf::RenderWindow window(sf::VideoMode(WIDTH * PIXELSIZE, HEIGHT * PIXELSIZE + 50), "SFML works!");
	sf::CircleShape shape(100.f);
	shape.setFillColor(sf::Color::Green);

	Field field(WIDTH, HEIGHT, PIXELSIZE, &window);

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


		field.update();

		field.render();
	}

	return 0;
}