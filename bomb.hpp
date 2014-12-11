#ifndef BOMB_HPP
#define BOMB_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "map.hpp"

#define VOLUME 60


using namespace std;
class Player;
class Map;
class Bomb
{
public :
	
	Bomb(sf::RenderWindow *win, sf::Image *spriteset, vector<Player*> tablePlayers ,Map *map,bool sound, int id);
	Bomb (sf::RenderWindow *win, sf::Image *spriteset,  vector<Player*> tablePlayers ,Map *map,bool sound, int x, int y);
	~Bomb();
	bool update();
	void display() const;
	void explode();
	sf::Vector2<int> getTile() const;
	void stopsound();
	void playsound();
	void mute();
	void demute();

private :
	sf::RenderWindow *win_;
	sf::Image *spriteset_;
	sf::Vector2<int> tile_;
	vector<Player *> tablePlayers_;
	Map *map_;
	int id_;
	sf::Sprite *currentsprite_;
	vector<sf::IntRect>  Bombsprite_;
	vector<sf::IntRect>  Explosionsprite_;
	vector<sf::IntRect>::iterator ite_B, ite_E;
	vector<sf::Vector2<int>> Colateral_;
	vector<sf::Vector2<int>>::iterator ite_C;
	sf::Clock clock_;
	sf::Sound * sound_bomb_;
	sf::SoundBuffer bombBuffer_;
	
	bool isExplosed_;
	
};

#endif //BOMB_HPP
