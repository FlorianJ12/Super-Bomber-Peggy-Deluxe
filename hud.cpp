#ifndef HUD_HPP
#define HUD_HPP

#include <SFML/Graphics.hpp>


class HUD
{
public :
	
	Map(sf::RenderWindow *win, sf::Image *spriteset);
	~Map();

	char getTile(int x, int y) const;
	void setTile(int x, int y, char type) ;
	char getRandTile() ;
	void display() const;


private :
	
	int cpt_bonus_;
	sf::RenderWindow *win_;
	sf::Image *spriteset_;
	char **map_;
	char *bonus_;
	sf::Sprite* Foin; 
	sf::Sprite* Pierre;
	sf::Sprite* Acceleration;
	sf::Sprite* Bomb;
	sf::Sprite* Colateral;
	
	
	
};

#endif //HUD_HPP
