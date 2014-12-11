#ifndef MAPEDITOR_HPP
#define MAPEDITOR_HPP

#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include "map.hpp"

using namespace std;

class MapEditor 
{

public : 
	MapEditor(int size_x, int size_y);
	~MapEditor();
	bool mapGenerator();
	void display() const;
	void write(char type, int x, int y);
	void save(string mapName);


private : 
	char **map_;
	sf::Font * groBold_;
	sf::RenderWindow* win_editor;
	sf::Image* background_editor_;
	sf::Image *spriteset_;
	sf::Image *spriteseteditor_;
	sf::Sprite* background_;
	sf::Sprite* FoinMobile_;
	sf::Sprite* PierreMobile_;
	sf::Sprite* Foin_; 
	sf::Sprite* Pierre_;
	sf::Sprite* NewFoin_; 
	sf::Sprite* NewPierre_;
	sf::Sprite* erase;
	sf::Sprite* eraseMobile;
	sf::Sprite * save_;
	sf::Sprite * enterstring_;
	sf::Sprite * spawn_;
	int size_x_, size_y_;

};




#endif //MAPEDITOR_HPP

