#include "KlickManager.hpp"



sf::Vector2i KlickManager::checkClicking(sf::RenderWindow* win, int pixelSize)
{
	sf::Vector2i pos = sf::Mouse::getPosition(*win);
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) == false && wasKlickingLastTime == true) {
		wasKlickingLastTime = false;
		return pos / pixelSize;
	}

	wasKlickingLastTime = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
	return sf::Vector2i(-1, -1);
}

KlickManager::KlickManager()
{
}


KlickManager::~KlickManager()
{
}
