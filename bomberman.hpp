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

#define GAME_TIME 90
#define HIROSHIMA 30
#define FRAME_RATE 60



using namespace std;

class Bomberman
{
public :

// Constructeur :
Bomberman();
// Destructeur : 
~Bomberman();

// Fonctions membres principales

bool start(); // Fonction permettant le paramétrage de la partie.
bool run(); // Moteur du jeu.


// Fonctions utilitaires

string keyCode (sf::Key::Code c) const; // Convertis un sf::Key::Code en string
void display_key (int x, int y, int id, char key_type); // Afficher la touche sélectionnée
void print_stat(string speed, string bomb, string power); // Affiche les stats des personnages lorsque survolé.
string inttoString(int n); // Convertis un int en string
string getStringTimer(); // Rend la valeur du timer sous une belle forme en string.
list<string> listMapFile(); // Rend la liste des fichiers maps.

// Accesseurs : 
bool isSound() const;




private :

// Fenêtre :
sf::RenderWindow *win_;

// Objets de jeu :
MapEditor *m_;
Timer timer_;
Map *map_;
list<Bomb*> *listBomb_;
vector<Player*> tablePlayers_;

// Attributs de musique et sons :
sf::Music music_menu_;
sf::Music music_game_;
sf::Sound end_music_;

// Attributs graphique
sf::Image *background_;
sf::Image *spriteset_;
sf::Image *spriteset_fast_;
sf::Image *spriteset_strong_;
sf::Image *background_menu_;
sf::Image *spriteset_menu_;
sf::Sprite victory_;
sf::Sprite headvictory_;
sf::Sprite *current_background_;
sf::Font groBold_;

// Divers
list<string> listMap_; // Liste des fichiers map
sf::Vector2<int>  *size_window_ ; // Tableau des tailles de fenêtre possible
char size_win_; // Taille de la fenêtre actuelle si non redimensionnée.
sf::Vector2<float>  *start_postion_ ; // Tableau des positions de spawn.
vector<vector<sf::Key::Code>> tableKeySet_; // Tableau des configurations de touches
bool music_, sound_,paused_; // Booléen d'état du jeu 

};


#endif // BOMBERMAN_HPP
