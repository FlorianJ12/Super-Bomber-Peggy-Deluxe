#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/Graphics.hpp>
#include <list>
#include "map.hpp"



#define HEIGHT_Hitbox 20
#define WIDTH_Hitbox 26
#define OFFSET_X_Hitbox 22
#define OFFSET_Y_Hitbox 50
#define SPEEDMAX 250
#define SPEEDMIN 100
#define SIZE_X_Hud 150
#define SIZE_Y_Hud 120

class Map;

using namespace std;

class Player
{
	public :
	typedef enum {Up, Down, Right, Left, Default} State;
 	// Constructeur
	Player(sf::RenderWindow *win, sf::Vector2<float> pos, sf::Image *spriteset, Map *map, vector<sf::Key::Code> keySet,string name);
	// Destructeur
	~Player();
	
	// Accesseurs
	sf::Vector2<float> getPosition() const;
	int getPower() const;	
	int getLife() const;
	int getBombCount() const;
	sf::Sprite getHead()const;
	string getName() const;
	sf::Key::Code getKeyLeft() const;
	sf::Key::Code getKeyRight() const;
	sf::Key::Code getKeyUp() const;
	sf::Key::Code getKeyDown() const;
	sf::Key::Code getKeyBomb() const;
	sf::Vector2<int> getTile();	
	float getBombTime();
	
	// Mutateurs
	void setName(string name);
	void updateKeySet(char key_type, sf::Key::Code c);
	void setKeyBomb(sf::Key::Code c);
	void setKeyUp(sf::Key::Code c);
	void setKeyDown(sf::Key::Code c);
	void setKeyLeft(sf::Key::Code c);
	void setKeyRight(sf::Key::Code c);
	void increaseSpeed(); 
	void decreaseSpeed();
	void increasePower(); 
	void decreasePower();
	void kill(bool sound);
	void addBombtoCount();
	void subBombtoCount();
	void resetBombTime();
	
	// Mouvement
	bool isOutOfBound (sf::IntRect hitBox) const;
	bool canMoveR(sf::IntRect hitBox) const;
	bool canMoveL(sf::IntRect hitBox) const;
	bool canMoveU(sf::IntRect hitBox) const;
	bool canMoveD(sf::IntRect hitBox) const;
	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();
	void setDefault();
	void getBonus();
	
	// Affichage	
	void display() ;
	void display_HUD() ;

	
	
	private :
	
	// Attribut affichage
	sf::RenderWindow *win_;
	sf::Font groBold_;
	sf::Image *spriteset_;
	vector<sf::IntRect>  Rsprite_;
	vector<sf::IntRect>  Lsprite_;
	vector<sf::IntRect>  Usprite_;
	vector<sf::IntRect>  Dsprite_;
	vector<sf::IntRect>::iterator ite_R, ite_L, ite_U, ite_D;
	sf::IntRect front_;
	sf::Sprite currentsprite_;
	sf::Sprite heart_;
	sf::Sprite empty_heart_;
	sf::Sprite HUD_;
	sf::Sprite head_;
	
	// Attribut mouvement
	sf::IntRect hitBox_;
	sf::Vector2<float> position_;
	State current_,previous_;
	vector<sf::Key::Code> keySet_;
	
	// Temps
	sf::Clock bomb_timer_;
	sf::Clock move_clock_;
	
	// Sons
	sf::Sound sound_pig_;
	sf::SoundBuffer pigBuffer_;
	
	// Caracteristiques
	int life_;
	string name_; 
	sf::String player_name_;
	Map *map_;
	int id_;
	protected :
	int bomb_count_;
	int power_;
	float deplacement_;
	
	
	
	
};

sf::Vector2<float> getCenter(sf::IntRect hitBox) ;
#endif //PLAYER_HPP
