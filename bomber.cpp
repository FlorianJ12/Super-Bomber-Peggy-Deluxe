#include <SFML/Graphics.hpp>
#include "player.hpp"
#include "map.hpp"
#include "bomber.hpp"


using namespace std;

Bomber:: Bomber(sf::RenderWindow *win, sf::Vector2<float> pos, sf::Image *spriteset, Map *map,vector<sf::Key::Code> keySet, string name) : 
	Player(win,pos,spriteset,map,keySet, name)
{
bomb_count_ = 2;
}

Bomber::~Bomber () 
{
}
