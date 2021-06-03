#include "BlockBody.hpp"

unsigned char BlockBody::get(int posx, int posy, int rotation)
{
	return blockContainer[rotation][posy * gridSizeX + posx];
}
