#include <SFML/Graphics.hpp>
#include "player.hpp"
#include "map.hpp"
#include "fast.hpp"


using namespace std;

Fast::Fast(sf::RenderWindow *win, sf::Vector2<float> pos, sf::Image *spriteset, Map *map,vector<sf::Key::Code> keySet, string name) : 
	Player(win,pos,spriteset,map,keySet,name)
{
deplacement_ = 150;
}

Fast::~Fast () 
{
}
