#pragma once
#include<SFML\Graphics.hpp>
#include<vector>
#include<random>
#include"BlockBody.hpp"
#include "Rotator.hpp"

typedef std::mt19937 rng_type;

enum SHAPE {
	SH_NULL,
	SH_I,
	SH_L,
	SH_RL,
	SH_SQUARE,
	SH_FY,
	SH_T,
	SH_FYR,
	SHAPECOUNT
};


class Field
{
	unsigned char* fieldArray;
	int sizex;
	int sizey;
	int pixelSize;
	int fieldMiddle;

	int curX = 0;
	int curY = 0;
	SHAPE currentFigure = SH_I;
	float turnTime = 0.6f;

	sf::RenderWindow* win;
	rng_type rng;

public:

	sf::RectangleShape shapes[8];
	std::vector<std::pair<int, int>> activeBlocks;
	BlockBody* bodies;
	Rotator rotator;
	sf::Texture tex;

	


	Field(int sizex, int sizey, int pixelsize, sf::RenderWindow* win);
	int get(int posx, int posy);
	void set(int posx, int posy, int id);

	SHAPE getRandShape();

	char checkKeyboard();
	void update();
	void resetBody();
	void checkForLines();
	void moveLinesDown(int posy);
	bool updateBody(int posx, int posy, BlockBody& bodie, Rotator& rotator);
	bool isItColliding(int posx, int posy, BlockBody& bodie, Rotator& rotator);
	void render();


	int getSizeX() { return sizex; }
	int getSizeY() { return sizey; }

	~Field();
};

