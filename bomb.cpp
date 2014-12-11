#include <SFML/Graphics.hpp>
#include "bomb.hpp"
#include "map.hpp"
#include "player.hpp"
#include <iostream>
using namespace std;

Bomb::Bomb (sf::RenderWindow *win, sf::Image *spriteset,  vector<Player*> tablePlayers ,Map *map,bool sound, int id) :
	win_(win), spriteset_(spriteset), tablePlayers_(tablePlayers), map_(map), id_(id)
{		
	tile_ = tablePlayers_[id_]->getTile();	
	isExplosed_ = false;
	currentsprite_ = new sf::Sprite;
	currentsprite_->SetImage(*spriteset_);
	for (int i = 7; i< 10; i++) {
		Bombsprite_.push_back(sf::IntRect(i*TILE_Size,0,TILE_Size*(i+1),TILE_Size));
	}
	
	ite_B = Bombsprite_.begin();
	for (int i = 2; i< 6; i++) {
		Explosionsprite_.push_back(sf::IntRect(i*TILE_Size,0,TILE_Size*(i+1),TILE_Size));
	}
	ite_E = Explosionsprite_.begin();	
	currentsprite_->SetSubRect(Bombsprite_[ite_B - Bombsprite_.begin()]);	
	
	// Son
	bombBuffer_.LoadFromFile("data/SonsProjet/Explosion2.wav");

	sound_bomb_ = new sf::Sound(bombBuffer_, false, 1 ,VOLUME);
	if (!sound) sound_bomb_->SetVolume(0);
	sound_bomb_-> Play();
	
	clock_.Reset();
}

Bomb::Bomb (sf::RenderWindow *win, sf::Image *spriteset,  vector<Player*> tablePlayers ,Map *map,bool sound, int x, int y) :
	win_(win), spriteset_(spriteset), tablePlayers_(tablePlayers), map_(map)
{		
	id_ = -1;
	tile_ = sf::Vector2<int> (x,y);	
	isExplosed_ = false;
	currentsprite_ = new sf::Sprite;
	currentsprite_->SetImage(*spriteset_);
	for (int i = 7; i< 10; i++) {
		Bombsprite_.push_back(sf::IntRect(i*TILE_Size,0,TILE_Size*(i+1),TILE_Size));
	}
	
	ite_B = Bombsprite_.begin();
	for (int i = 2; i< 6; i++) {
		Explosionsprite_.push_back(sf::IntRect(i*TILE_Size,0,TILE_Size*(i+1),TILE_Size));
	}
	ite_E = Explosionsprite_.begin();	
	currentsprite_->SetSubRect(Bombsprite_[ite_B - Bombsprite_.begin()]);	
	
	// Son
	bombBuffer_.LoadFromFile("data/SonsProjet/Explosion2.wav");

	sound_bomb_ = new sf::Sound(bombBuffer_, false, 1 ,VOLUME);
	if (!sound) sound_bomb_->SetVolume(0);
	sound_bomb_-> Play();
		
	
	clock_.Reset();
	
}

Bomb::~Bomb() {
	delete currentsprite_;
	delete sound_bomb_;
}

bool Bomb::update() {
	if (!isExplosed_) {
		if (clock_.GetElapsedTime() > 1) {
			ite_B ++;
			currentsprite_->SetSubRect(Bombsprite_[ite_B - Bombsprite_.begin()]);
			if (ite_B == Bombsprite_.end()) {
				isExplosed_ = true; 
				currentsprite_->SetSubRect(Explosionsprite_[ite_E - Explosionsprite_.begin()]);	
				explode();
				if (id_ !=-1)
				tablePlayers_[id_]->addBombtoCount();
				
			}			
			clock_.Reset();
				
		}
	}
	else {
		if (clock_.GetElapsedTime() > 0.2) {
			ite_E ++;
			if (ite_E == Explosionsprite_.end()){	
				return true; // Toute l'animation de la bombe s'est affichée.
			}else {
			currentsprite_->SetSubRect(Explosionsprite_[ite_E - Explosionsprite_.begin()]);
			}
		}
	}	
	return false; // Toute l'animation de la bombe ne s'est pas affichée. 

}

void Bomb::display() const {
	
	currentsprite_->SetPosition(tile_.x * TILE_Size + OFFSET_x , tile_.y * TILE_Size + OFFSET_y );
	win_->Draw(*currentsprite_);
	for(auto ite_C = Colateral_.begin(); ite_C != Colateral_.end(); ite_C ++){
		currentsprite_->SetPosition(Colateral_[ite_C-Colateral_.begin()].x * TILE_Size + OFFSET_x , 
					Colateral_[ite_C-Colateral_.begin()].y * TILE_Size + OFFSET_y );
		win_->Draw(*currentsprite_);
	}
	

}


void Bomb::explode() {
	sf::Vector2<int> currentTile;
	vector<Player *>::iterator ite_P;
	char type;
	int power;
	if (id_ != -1) {
	power = tablePlayers_[id_]->getPower();
	}
	else  power = 1;	
	//Explosion Tile
	currentTile = tile_; 
	type = map_->getTile(currentTile.x, currentTile.y);
	if (type == 'F') map_->setTile(currentTile.x, currentTile.y, map_->getRandTile());
	for(auto ite_P = tablePlayers_.begin(); ite_P != tablePlayers_.end(); ite_P ++){
		if(tablePlayers_[ite_P-tablePlayers_.begin()]->getTile()== currentTile) {
			tablePlayers_[ite_P-tablePlayers_.begin()]->kill();			
		}
	}	
	
	// Explosion UP
	currentTile = tile_; 
	currentTile.y--;
	while (currentTile.y >= 0 && tile_.y-currentTile.y <= power) {
		type = map_->getTile(currentTile.x, currentTile.y);
		if (type == 'P') break;
		Colateral_.push_back(sf::Vector2<int>(currentTile.x, currentTile.y));
		if (type == 'F') {
			map_->setTile(currentTile.x, currentTile.y, map_->getRandTile());
			break;
		}
		if (type == 'A' || type == 'B' || type == 'C') {
			map_->setTile(currentTile.x, currentTile.y, 'V');	
		}
		for(auto ite_P = tablePlayers_.begin(); ite_P != tablePlayers_.end(); ite_P ++){
			if(tablePlayers_[ite_P-tablePlayers_.begin()]->getTile()== currentTile) {
				tablePlayers_[ite_P-tablePlayers_.begin()]->kill();			
			}
		}
	currentTile.y--;
	}
	// Explosion DOWN
	currentTile = tile_; 
	currentTile.y++;
	while (currentTile.y <= (MAP_Height-1) && currentTile.y-tile_.y <= power) {
		type = map_->getTile(currentTile.x, currentTile.y);
		if (type == 'P') break;
		Colateral_.push_back(sf::Vector2<int>(currentTile.x, currentTile.y));
		if (type == 'F') {
			map_->setTile(currentTile.x, currentTile.y,map_->getRandTile());
			break;
		}
		if (type == 'A' || type == 'B' || type =='C'){
			map_->setTile(currentTile.x, currentTile.y, 'V');	
		}
		for(auto ite_P = tablePlayers_.begin(); ite_P != tablePlayers_.end(); ite_P ++){
			if(tablePlayers_[ite_P-tablePlayers_.begin()]->getTile()== currentTile) {
				tablePlayers_[ite_P-tablePlayers_.begin()]->kill();			
			}
		}
	currentTile.y++;
	}
	// Explosion RIGHT
	currentTile = tile_; 
	currentTile.x++;
	while (currentTile.x <= (MAP_Width-1) && currentTile.x-tile_.x <= power) {
		type = map_->getTile(currentTile.x, currentTile.y);
		if (type == 'P') break;
		Colateral_.push_back(sf::Vector2<int>(currentTile.x, currentTile.y));
		if (type == 'F') {
			map_->setTile(currentTile.x, currentTile.y, map_->getRandTile());
			break;
		}
		if (type == 'A' || type == 'B' || type == 'C'){
			map_->setTile(currentTile.x, currentTile.y, 'V');	
		}
		for(auto ite_P = tablePlayers_.begin(); ite_P != tablePlayers_.end(); ite_P ++){
			if(tablePlayers_[ite_P-tablePlayers_.begin()]->getTile()== currentTile) {
				tablePlayers_[ite_P-tablePlayers_.begin()]->kill();			
			}
		}
	currentTile.x++;
	}

	// Explosion LEFT
	currentTile = tile_; 
	currentTile.x--;
	while (currentTile.x >= 0 && tile_.x-currentTile.x <= power) {
		type = map_->getTile(currentTile.x, currentTile.y);
		if (type == 'P') break;
		Colateral_.push_back(sf::Vector2<int>(currentTile.x, currentTile.y));
		if (type == 'F') {
			map_->setTile(currentTile.x, currentTile.y, map_->getRandTile());
			break;
		}
		if (type == 'A' || type == 'B' || type == 'C'){
			map_->setTile(currentTile.x, currentTile.y, 'V');	
		}
		for(auto ite_P = tablePlayers_.begin(); ite_P != tablePlayers_.end(); ite_P ++){
			if(tablePlayers_[ite_P-tablePlayers_.begin()]->getTile()== currentTile) {
				tablePlayers_[ite_P-tablePlayers_.begin()]->kill();			
			}
		}
	currentTile.x--;
	}
}
		

sf::Vector2<int> Bomb::getTile() const {
	return tile_;
}


void Bomb::stopsound() {
	sound_bomb_->Pause();
}
void Bomb::playsound() {
	sound_bomb_->Play();
}

void Bomb::mute() {
	sound_bomb_->SetVolume(0);
}
void Bomb::demute() {
	sound_bomb_->SetVolume(60);
}





