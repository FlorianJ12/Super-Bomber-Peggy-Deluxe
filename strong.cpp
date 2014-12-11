#include <SFML/Graphics.hpp>
#include "player.hpp"
#include "map.hpp"
#include "strong.hpp"


using namespace std;

Strong:: Strong(sf::RenderWindow *win, sf::Vector2<float> pos, sf::Image *spriteset, Map *map,vector<sf::Key::Code> keySet, string name) : 
	Player(win,pos,spriteset,map,keySet,name)
{
	power_ = 2;
}

Strong::~Strong () 
{
}
