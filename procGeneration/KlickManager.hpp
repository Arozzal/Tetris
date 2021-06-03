#pragma once
#include<SFML\System.hpp>
#include<SFML/Graphics.hpp>



class KlickManager
{
	bool wasKlickingLastTime = false;
public:
	sf::Vector2i checkClicking(sf::RenderWindow* win, int pixelSize);
	KlickManager();
	~KlickManager();
};

