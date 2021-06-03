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

	blockTexture.loadFromFile("block.png");

	font.loadFromFile("small_pixel.ttf");
	scoreLabel.setFont(font);
	scoreNum.setFont(font);

	linesLabel.setFont(font);
	linesNum.setFont(font);

	levelLabel.setFont(font);
	levelNum.setFont(font);

	gameOverLabel1.setFont(font);
	gameOverLabel1.setCharacterSize(36);
	gameOverLabel1.setString("Game Over");
	gameOverLabel1.setPosition(300, 200);
	gameOverLabel2.setFont(font);
	gameOverLabel2.setCharacterSize(36);
	gameOverLabel2.setString("Press SPACE to restart");
	gameOverLabel2.setPosition(200, 300);

	scoreLabel.setString("SCORE");
	incScore(0);

	linesLabel.setString("LINES");
	incLines(0);

	levelLabel.setString("LEVEL");
	incLevel(0);


	int xpos = win->getSize().x / 2 + (sizex * pixelSize) / 2 + 100;
	int ypos = 120;

	scoreLabel.setPosition(xpos, ypos);
	scoreNum.setPosition(xpos, ypos + 60 - 20);
	scoreNum.setFillColor(sf::Color::Blue);

	linesLabel.setPosition(xpos, ypos + 120);
	linesNum.setPosition(xpos, ypos + 180 - 20);
	linesNum.setFillColor(sf::Color::Green);

	levelLabel.setPosition(xpos, ypos + 240);
	levelNum.setPosition(xpos, ypos + 300 - 20);
	levelNum.setFillColor(sf::Color::Yellow);

	for (sf::Sprite& spr : shapes) {
		spr.setTexture(blockTexture);
		spr.setScale(2.0f, 2.0f);
	}

	memset(fieldArray, 0, sizex * sizey);


	this->sizex = sizex;
	this->sizey = sizey;
	this->pixelSize = pixelSize;

	shapes[0].setColor(sf::Color(25, 25, 25));
	shapes[1].setColor(sf::Color::Cyan);
	shapes[2].setColor(sf::Color::Blue);
	shapes[3].setColor(sf::Color(255, 165, 0));
	shapes[4].setColor(sf::Color::Yellow);
	shapes[5].setColor(sf::Color::Green);
	shapes[6].setColor(sf::Color::Magenta);
	shapes[7].setColor(sf::Color::Red);
	shapes[8].setColor(sf::Color(128, 128, 128));
}

int Field::get(int posx, int posy)
{
	if (posx >= sizex || posy >= sizey || posx < 0 || posy < 0) {
		return 8;
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

int Field::getRandNum()
{
	static std::uniform_int_distribution<rng_type::result_type> udist(0, SHAPECOUNT - 2);
	rng_type::result_type random_number = udist(rng);
	return random_number;
}

SHAPE Field::getRandShape(int index, bool update)
{
	if (update) {
		if (shapeIds[0] == -1) {
			for (int i = 0; i < 10; i++) {
				shapeIds[i] = getRandNum() + 1;
			}
		}
		else {
			for (int i = 0; i < 9; i++) {
				shapeIds[i] = shapeIds[i + 1];
			}

			shapeIds[9] = getRandNum() + 1;
		}
	}

	return SHAPE(shapeIds[index]);

	std::cerr << "incorrect index number " << index << std::endl;
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

	if (isGameOver()) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
			setRestard();
		}

		return;
	}
		

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

		float elapsedTime = clock.getElapsedTime().asSeconds();

		if ((keys & 0x10) != 0) {
			if (elapsedTime > 0.02) {
				break;
			}
		}else{
			if (elapsedTime > turnTime) {
				break;
			}
		}



		
		if (updateCycleCounter >= 500) {
			turnTime *= 0.95;
			incLevel(1);
			updateCycleCounter = 0;
		}

		
	}


	curY += 1;

	if (updateBody(curX, curY, bodies[currentFigure], rotator)) {
		curY -= 1;
		updateBody(curX, curY, bodies[currentFigure], rotator);
		resetBody();
		checkForLines();
		if (isItColliding(curX, curY, bodies[currentFigure], rotator)) {
			setGameOver();
		}
	}
}

void Field::resetBody()
{
	curY = 0;
	curX = fieldMiddle;
	activeBlocks.clear();
	rotator.set(0);
	currentFigure = getRandShape(0, true);

	for (int i = 1; i < 9; i++) {
		shapeNext[i - 1] = getRandShape(i, false);
	}

	std::cout << currentFigure << std::endl;
}

void Field::checkForLines()
{
	int lineCount = 0;
	for (int y = sizey - 1; y > 0; y--) {
		bool wholeLine = true;
		for (int x = 0; x < sizex; x++) {
			if (get(x, y) == 0) {
				wholeLine = false;
			}
		}

		if (wholeLine) {
			moveLinesDown(y-1);
			lineCount++;
			y++;
			render();
		}
	}

	if (lineCount > 0) {
		incScore(level * 40 * (lineCount + lineCount / 1.5f));
		incLines(lineCount);
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
	updateCycleCounter++;
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

void Field::incScore(int score)
{
	this->score += score;
	scoreNum.setString(std::to_string(this->score));
}

void Field::incLines(int lines)
{
	this->lines += lines;
	linesNum.setString(std::to_string(this->lines));
}

void Field::incLevel(int level)
{
	this->level += level;
	levelNum.setString(std::to_string(this->level));
}




void Field::render()
{
	win->clear();
	for (int y = 0; y < sizey + 2; y++) {
		for (int x = 0; x < sizex + 2; x++) {
			int tempId = get(x - 1, y - 1);
			shapes[tempId].setPosition((x + 8) * pixelSize, y * pixelSize + 50);
			win->draw(shapes[tempId]); 
		}
	}
	
	
	win->draw(scoreLabel);
	win->draw(scoreNum);
	win->draw(linesLabel);
	win->draw(linesNum);
	win->draw(levelLabel);
	win->draw(levelNum);

	for (int j = 0; j < 9; j++) {
		for (int x = 0; x < bodies[shapeNext[j]].gridSizeX; x++) {
			for (int y = 0; y < bodies[shapeNext[j]].gridSizeY; y++) {

				if (bodies[shapeNext[j]].get(x, y, 0) == true) {
					shapes[shapeNext[j]].setPosition(x * pixelSize + 40, y * pixelSize + 40 + 130 * j);
					win->draw(shapes[shapeNext[j]]);
				}

			}
		}
	}
	
	if (isGameOver()) {
		win->draw(gameOverLabel1);
		win->draw(gameOverLabel2);
	}

	win->display();
}


Field::~Field()
{
}
