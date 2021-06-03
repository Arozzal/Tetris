#include "field.hpp"
#include <memory>
#include <iostream>
#include <random>
#include <windows.h>
#include <stdio.h>

#include <SFML\System.hpp>

#include "initBodies.hpp"



Field::Field(int sizex, int sizey, int pixelSize, sf::RenderWindow* win)
{
	this->win = win;
	srand(time(NULL));
	fieldMiddle = sizex / 2 - 1;

	rng_type::result_type const seedval = time(NULL);
	rng.seed(seedval);

	fieldArray = new unsigned char[sizex * sizey];
	
	bodies = initBodies(SHAPECOUNT);


	memset(fieldArray, 0, sizex * sizey);


	this->sizex = sizex;
	this->sizey = sizey;
	this->pixelSize = pixelSize;


	shapes[0].setSize(sf::Vector2f( pixelSize, pixelSize ));
	shapes[0].setFillColor(sf::Color::Black);
	shapes[1].setSize(sf::Vector2f( pixelSize, pixelSize ));
	shapes[1].setFillColor(sf::Color::Cyan);
	shapes[2].setSize(sf::Vector2f( pixelSize, pixelSize ));
	shapes[2].setFillColor(sf::Color::Blue);
	shapes[3].setSize(sf::Vector2f( pixelSize, pixelSize ));
	shapes[3].setFillColor(sf::Color(255, 165, 0));
	shapes[4].setSize(sf::Vector2f( pixelSize, pixelSize ));
	shapes[4].setFillColor(sf::Color::Yellow);
	shapes[5].setSize(sf::Vector2f( pixelSize, pixelSize ));
	shapes[5].setFillColor(sf::Color::Green);
	shapes[6].setSize(sf::Vector2f(pixelSize, pixelSize));
	shapes[6].setFillColor(sf::Color::Magenta);
	shapes[7].setSize(sf::Vector2f(pixelSize, pixelSize));
	shapes[7].setFillColor(sf::Color::Red);
}

int Field::get(int posx, int posy)
{
	if (posx >= sizex || posy >= sizey || posx < 0 || posy < 0) {
		return 1;
	}

	return fieldArray[sizex * posy + posx];
}


void Field::set(int posx, int posy, int id)
{
	if (posx > sizex || posy > sizey || posx < 0 || posy < 0) {
		return;
	}

	fieldArray[sizex * posy + posx] = id;
}

SHAPE Field::getRandShape()
{
	static std::uniform_int_distribution<rng_type::result_type> udist(0, SHAPECOUNT - 2);
	rng_type::result_type random_number = udist(rng);


	return SHAPE(random_number + 1);
}

char Field::checkKeyboard()
{
	char keys = 0;

	static bool Lastpressed1 = false;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
		if (Lastpressed1 == false) {
			keys |= 0x1;
			Lastpressed1 = true;
		}
	}
	else {
		Lastpressed1 = false;
	}

	static bool Lastpressed2 = false;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
		if (Lastpressed2 == false) {
			keys |= 0x2;
			Lastpressed2 = true;
		}
	}
	else {
		Lastpressed2 = false;
	}

	static bool Lastpressed3 = false;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
		if (Lastpressed3 == false) {
			keys |= 0x4;
			Lastpressed3 = true;
		}
	}
	else {
		Lastpressed3 = false;
	}

	static bool Lastpressed4 = false;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
		if (Lastpressed4 == false) {
			keys |= 0x8;
			Lastpressed4 = true;
		}
	}
	else {
		Lastpressed4 = false;
	}
	

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {

			keys |= 0x10;

	}

	return keys;
}

void Field::update()
{
	static sf::Clock clock;
	clock.restart();

	

	while (true) {
		char keys = checkKeyboard();
		std::cout << int(keys) << std::endl;

		if ((keys & 0x1) != 0) {
			curX -= 1;
			if (updateBody(curX, curY, bodies[currentFigure], rotator)) {
				curX += 1;
			}
		}

		if ((keys & 0x2) != 0) {
			curX += 1;
			if (updateBody(curX, curY, bodies[currentFigure], rotator)) {
				curX -= 1;
			}
		}

		if ((keys & 0x4) != 0) {
			rotator.dec();
			if (updateBody(curX, curY, bodies[currentFigure], rotator)) {
				rotator.inc();
			}
		}

		if ((keys & 0x8) != 0) {
			rotator.inc();
			if (updateBody(curX, curY, bodies[currentFigure], rotator)) {
				rotator.dec();
			}
		}
		
		render();

		if ((keys & 0x10) != 0) {
			if (clock.getElapsedTime().asSeconds() > 0.02) {
				break;
			}
		}else{
			if (clock.getElapsedTime().asSeconds() > turnTime) {
				break;
			}
		}
	}


	curY += 1;

	if (updateBody(curX, curY, bodies[currentFigure], rotator)) {
		curY -= 1;
		updateBody(curX, curY, bodies[currentFigure], rotator);
		resetBody();
		checkForLines();
		if (isItColliding(curX, curY, bodies[currentFigure], rotator)) {
			exit(0);
		}
	}
}

void Field::resetBody()
{
	curY = 0;
	curX = fieldMiddle;
	activeBlocks.clear();
	rotator.set(0);
	currentFigure = getRandShape();
	std::cout << currentFigure << std::endl;
}

void Field::checkForLines()
{
	for (int y = sizey - 1; y > 0; y--) {
		bool wholeLine = true;
		for (int x = 0; x < sizex; x++) {
			if (get(x, y) == 0) {
				wholeLine = false;
			}
		}

		if (wholeLine) {
			moveLinesDown(y-1);
			y++;
			render();
			turnTime *= 0.95;
		}
	}
}

void Field::moveLinesDown(int posy)
{
	for (; posy >= 0; posy--) {
		for (int x = 0; x < sizex; x++) {
			set(x, posy + 1, get(x, posy));
		}
	}
}

bool Field::updateBody(int posx, int posy, BlockBody& bodie, Rotator& rotator)
{
	for (int i = 0; i < activeBlocks.size(); i++) {
		set(activeBlocks[i].first, activeBlocks[i].second, 0);
	}

	if (isItColliding(posx, posy, bodie, rotator)) {
		for (int i = 0; i < activeBlocks.size(); i++) {
			set(activeBlocks[i].first, activeBlocks[i].second, currentFigure);
		}

		return true;
	}


	activeBlocks.clear();

	for (int x = 0; x < bodie.gridSizeX; x++) {
		for (int y = 0; y < bodie.gridSizeY; y++) {
			if (bodie.get(x, y, rotator.get()) != 0) {
				set(posx + x, posy + y, currentFigure);
				activeBlocks.push_back({ posx + x, posy + y });
			}
		}
	}

	return false;
}

bool Field::isItColliding(int posx, int posy, BlockBody & bodie, Rotator & rotator)
{
	for (int x = 0; x < bodie.gridSizeX; x++) {
		for (int y = 0; y < bodie.gridSizeY; y++) {
			if (bodie.get(x, y, rotator.get()) == 1) {
				if (get(posx + x, posy + y) != 0) {
					return true;
				}
			}
		}
	}

	return false;
}




void Field::render()
{
	for (int y = 0; y < sizey; y++) {
		for (int x = 0; x < sizex; x++) {
			int tempId = get(x, y);
			shapes[tempId].setPosition(x * pixelSize, y * pixelSize + 50);
			win->draw(shapes[tempId]); 
		}
	}
	win->display();
}


Field::~Field()
{
}
