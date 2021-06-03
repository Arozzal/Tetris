#pragma once
#include<vector>

class BlockBody
{
public:
	int gridSizeX = -1;
	int gridSizeY = -1;
	std::vector<std::vector<unsigned char>> blockContainer;

	unsigned char get(int posx, int posy, int rotation);
};


