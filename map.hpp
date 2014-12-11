#ifndef MAP_HPP
#define MAP_HPP

#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include "bomb.hpp"

#define FRAME_RATE 60

#define MAP_Height 10
#define MAP_Width 10
#define TILE_Size 70
#define OFFSET_x 250
#define OFFSET_y 50

using namespace std;

class Bomb;

class Map
{
public :
	
	Map(sf::RenderWindow *win, sf::Image *spriteset,list<Bomb*> *listBomb, string map);
	~Map();

	char getTile(int x, int y) const;
	void setTile(int x, int y, char type) ;
	char getRandTile() ;
	void display() const;


private :
	
	int cpt_bonus_;
	sf::RenderWindow *win_;

	
	sf::Image *spriteset_;
	vector<string>::iterator ite_File;
	char **map_;
	char *bonus_;
	sf::Sprite* Foin; 
	sf::Sprite* Pierre;
	sf::Sprite* Acceleration;
	sf::Sprite* Bombb;
	sf::Sprite* Colateral;
public :
	list<Bomb*> *listBomb_;
	
	
	
};

#endif //MAP_HPP
