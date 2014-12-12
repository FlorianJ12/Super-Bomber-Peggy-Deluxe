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
	// Constructeur
	Map(sf::RenderWindow *win, sf::Image *spriteset,list<Bomb*> *listBomb, string map);
	// Destructeur
	~Map();
	
	// Accesseurs
	char getTile(int x, int y) const;
	char getRandTile() ;
	
	// Mutateur
	void setTile(int x, int y, char type) ;
	
	//Affichage
	void display() const;


private :
	// Fenetre 
	sf::RenderWindow *win_;
	
	// Attribut d'affichage
	sf::Image *spriteset_;
	sf::Sprite* Foin; 
	sf::Sprite* Pierre;
	sf::Sprite* Acceleration;
	sf::Sprite* Bombb;
	sf::Sprite* Colateral;
	
	// Gestion de la map
	char **map_;
	char *bonus_;
	int cpt_bonus_;
	
	// Iterateur
	vector<string>::iterator ite_File;
	
public :
	// liste des bombes sur la map
	list<Bomb*> *listBomb_;
	
	
	
};

#endif //MAP_HPP
