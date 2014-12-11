#ifndef BOMBERMAN_HPP 
#define BOMBERMAN_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "map.hpp"
#include "player.hpp"
#include <list>
#include "timer.hpp"
#include <sstream>
#include "mapEditor.hpp"
#include <dirent.h>

#define GAME_TIME 60
#define HIROSHIMA 20
#define FRAME_RATE 60



using namespace std;

class Bomberman
{
public :

// Constructeur :
Bomberman();
~Bomberman();

// Méthodes
bool run();
bool start();
string keyCode (sf::Key::Code c) const;
void display_key (int x, int y, int id, char key_type);
void print_stat(string speed, string bomb, string power);
string inttoString(int n);
bool isSound() const;
string getStringTimer();
list<string> listMapFile();




private :
sf::Font * groBold_;
MapEditor *m_;
sf::Clock bombtime_;
Timer *timer_;
sf::Music *music_menu_;
sf::Music *music_game_;
sf::RenderWindow *win_;
sf::Image *background_;
sf::Image *spriteset_;
sf::Image *spriteset_fast_;
sf::Image *spriteset_strong_;
sf::Image *background_menu_;
sf::Image *spriteset_menu_;
sf::Sprite *victory_;
sf::Sprite headvictory_;
Map *map_;
vector<Player*> tablePlayers_;
list<string> listMap_;
sf::Vector2<int>  *size_window_ ;
sf::Vector2<float>  *start_postion_ ;
list<Bomb*> *listBomb_;
vector<vector<sf::Key::Code>> tableKeySet_;
sf::Sprite *current_background_;
bool music_, sound_,paused_;
char size_win_;

};


#endif // BOMBERMAN_HPP
