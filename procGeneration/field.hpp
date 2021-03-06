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

	int score = 0;
	int lines = 0;
	int level = 1;

	int curX = 0;
	int curY = 0;
	SHAPE currentFigure = SH_I;
	float turnTime = 0.6f;
	int updateCycleCounter = 0;

	int shapeIds[10] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, };

	bool gameOver = false;
	bool restard = false;

	sf::Texture blockTexture;
	
	sf::Font font;

	sf::Text scoreLabel;
	sf::Text scoreNum;

	sf::Text linesLabel;
	sf::Text linesNum;

	sf::Text levelLabel;
	sf::Text levelNum;

	sf::Text gameOverLabel1;
	sf::Text gameOverLabel2;

	sf::RenderWindow* win;
	rng_type rng;

	SHAPE shapeNext[9];

public:

	sf::Sprite shapes[9];
	std::vector<std::pair<int, int>> activeBlocks;
	BlockBody* bodies;
	Rotator rotator;
	sf::Texture tex;

	bool isGameOver() { return gameOver; }
	void setGameOver() { gameOver = true; }
	bool isRestard() { return restard; }
	void setRestard() { restard = true; }


	Field(int sizex, int sizey, int pixelsize, sf::RenderWindow* win);
	int get(int posx, int posy);
	void set(int posx, int posy, int id);

	int getRandNum();
	SHAPE getRandShape(int index, bool update = false);

	char checkKeyboard();
	void update();
	void resetBody();
	void checkForLines();
	void moveLinesDown(int posy);
	bool updateBody(int posx, int posy, BlockBody& bodie, Rotator& rotator);
	bool isItColliding(int posx, int posy, BlockBody& bodie, Rotator& rotator);
	void incScore(int inc);
	void incLines(int lines);
	void incLevel(int level);
	void render();


	int getSizeX() { return sizex; }
	int getSizeY() { return sizey; }

	~Field();
};

