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
	// Constructeurs 
	Bomb(sf::RenderWindow *win, sf::Image *spriteset, vector<Player*> tablePlayers ,Map *map,bool sound, int id);
	Bomb (sf::RenderWindow *win, sf::Image *spriteset,  vector<Player*> tablePlayers ,Map *map,bool sound, int x, int y);
	// Destrcuteur
	~Bomb();
	
	//Accesseurs 
	bool isExplosed()const;
	sf::Vector2<int> getTile() const;
		
	// Mutateurs 
	void stopsound();
	void playsound();
	void mute();
	void demute();
	bool update(bool sound);
	void explode(bool sound);
	
	//Affichage
	void display() ;
	
private :

	// Affichage
	sf::RenderWindow *win_;
	sf::Image *spriteset_;
	sf::Sprite currentsprite_;
	vector<sf::IntRect>  Bombsprite_;
	vector<sf::IntRect>  Explosionsprite_;
	vector<sf::IntRect>::iterator ite_B, ite_E;
	
	// Temps 
	sf::Clock clock_;
	
	// Son
	sf::Sound sound_bomb_;
	sf::SoundBuffer bombBuffer_;
	
	// Autres
	sf::Vector2<int> tile_;
	vector<Player *> tablePlayers_;
	Map *map_;
	vector<sf::Vector2<int>> Colateral_;
	vector<sf::Vector2<int>>::iterator ite_C;
	int id_;
	bool isExplosed_;
	
};

#endif //BOMB_HPP
