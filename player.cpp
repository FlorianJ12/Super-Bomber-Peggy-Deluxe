#include <SFML/Graphics.hpp>
#include <list>
#include "player.hpp"
#include <iostream>

using namespace std;

Player :: Player(sf::RenderWindow *win, sf::Vector2<float> pos, sf::Image *spriteset, Map *map,vector<sf::Key::Code> keySet, string name) : 
win_(win), spriteset_(spriteset), position_(pos), current_(Default),previous_(Default), map_(map), keySet_(keySet) , name_(name)
{
	// deplacement_ max = 250		
	// deplacement_ min = 100	
	deplacement_ = 100;
	power_ = 1;
	bomb_count_ = 1;
	life_=3;
	groBold_ = new sf::Font;
	groBold_ -> LoadFromFile("data/Font/GROBOLD.ttf");
	player_name_.SetText(name_);
	player_name_.SetFont(*groBold_);


	// Initialisation des sprites
	
	//Sprite de l'HUD
	heart_ = new sf::Sprite;
	heart_->SetImage(*spriteset_);
	heart_-> SetSubRect (sf::IntRect (0, 4*TILE_Size, TILE_Size, 5*TILE_Size));
	
	empty_heart_ = new sf::Sprite;
	empty_heart_->SetImage(*spriteset_);
	empty_heart_-> SetSubRect (sf::IntRect (TILE_Size, TILE_Size*4, 2*TILE_Size, 5*TILE_Size));
	
	HUD_ = new sf::Sprite;
	HUD_->SetImage(*spriteset_);
	HUD_-> SetSubRect (sf::IntRect (2*TILE_Size, 4*TILE_Size, 5*TILE_Size, 6*TILE_Size));
	
	head_ = new sf::Sprite;
	head_->SetImage(*spriteset_);
	head_-> SetSubRect (sf::IntRect (9*TILE_Size, TILE_Size, 10*TILE_Size,2*TILE_Size));
	

	// Sprite sans mouvement 	
	currentsprite_ = new sf::Sprite;
	currentsprite_->SetImage(*spriteset_);
	front_= sf::IntRect(0,TILE_Size,TILE_Size,TILE_Size*2);	
	currentsprite_->SetSubRect(front_);	
	currentsprite_->SetPosition(position_.x,position_.y);
	hitBox_= sf::IntRect(position_.x + OFFSET_X_Hitbox ,
				position_.y + OFFSET_Y_Hitbox, 
				position_.x + OFFSET_X_Hitbox + WIDTH_Hitbox,

				position_.y + OFFSET_Y_Hitbox + HEIGHT_Hitbox);


	// Sprite mouvememnt droite

	for (int i = 0; i< 6; i++) {
		Rsprite_.push_back(sf::IntRect(i*TILE_Size,TILE_Size,TILE_Size*(i+1),2*TILE_Size));
	}
	ite_R = Rsprite_.begin();
	// Sprite mouvement gauche

	for (int i = 0; i< 6; i++) {
		Lsprite_.push_back(sf::IntRect(i*TILE_Size,2*TILE_Size,TILE_Size*(i+1),3*TILE_Size));
	}
	ite_L = Lsprite_.begin();
	
	// Sprite mouvement bas 

	for (int i = 0; i< 5; i++) {
		Dsprite_.push_back(sf::IntRect(i*TILE_Size,3*TILE_Size,TILE_Size*(i+1),4*TILE_Size));
	}
	ite_D = Dsprite_.begin();
	// Sprite mouvement haut

	for (int i = 0; i< 5; i++) {
		Usprite_.push_back(sf::IntRect((i+5)*TILE_Size,3*TILE_Size,TILE_Size*(i+6),4*TILE_Size));
	}
	ite_U = Usprite_.begin();
	
	// Timer de bomber
	bomb_timer_.Reset();
}

Player :: ~Player(){
	delete currentsprite_;
	delete heart_;
	delete empty_heart_;
	delete HUD_;
	delete head_;
	delete groBold_;
}

sf::Vector2<float> Player::getPosition() const {

	return position_;

}



sf::Vector2<int> Player::getTile() {
	
	sf::Vector2<float> center = getCenter(hitBox_);
	return sf::Vector2<int> (((int)(center.x - OFFSET_x)/TILE_Size),((int)(center.y -OFFSET_y)/TILE_Size));
}


void Player::display() const {

	currentsprite_->SetPosition(position_.x , position_.y);
	win_->Draw(*currentsprite_);

}

void Player::display_HUD(){
	int cpt = 0;
	int id = (int)(name_[7] - '0');
	int cornerX = 0;
	int cornerY = (id-1) * SIZE_Y_Hud;
	
	// Affichage planche :
	HUD_->SetPosition(cornerX, cornerY);
	win_->Draw(*HUD_);
	
	// Affichage personnage
	head_->SetPosition(cornerX, cornerY);
	win_->Draw(*head_);
	
	// Affichage du nom
	player_name_.SetPosition ((cornerX+TILE_Size-3),cornerY+25);
	player_name_.SetSize (20);
	player_name_.SetColor(sf::Color(255,255,255,200));
	win_->Draw(player_name_);
	
	//Affichage des vies
	for (cpt = 0; cpt < 3; cpt++) {
		if ((cpt+1) <= life_) {
			heart_->SetPosition(cornerX + cpt* 40, cornerY + TILE_Size-18);
			win_->Draw(*heart_);
		}
		else {
			empty_heart_->SetPosition(cornerX + cpt* 40, cornerY + TILE_Size-18);
			win_->Draw(*empty_heart_);
		}
	}
}



bool Player::isOutOfBound (sf::IntRect hitBox) const {
	
	if (hitBox.Top < OFFSET_y || 
		hitBox.Left < OFFSET_x ||
		hitBox.Right > (OFFSET_x + MAP_Width*TILE_Size) ||
		hitBox.Bottom > (OFFSET_y + MAP_Height*TILE_Size)
							
	)return true;
		
	

	else return false;
}







bool Player::canMoveR(sf::IntRect hitBox) const {	
	
	int tile1x, tile1y, tile2x, tile2y;
	bool t1; 
	bool t2;
	sf::Vector2<float> center = getCenter(hitBox);
	sf::Vector2<int> tileBomb;
	list<Bomb*>::iterator ite_Bomb;
	tile1x = min(max(((int)(center.x - OFFSET_x)/TILE_Size)+1,0),9);
	tile2x = tile1x;
	tile1y = min(max(((int)(center.y -(OFFSET_y+(HEIGHT_Hitbox/2))) / TILE_Size),0),9);
	tile2y = min(max(((int)(center.y -(OFFSET_y-(HEIGHT_Hitbox/2))) / TILE_Size),0),9);
	// Traitement case
	if (tile1x < 10){
		if (tile2x==10) tile2x--;
		//cout << tile1y <<',' <<tile1x <<','<< tile2y<<','<< tile2x <<endl;
		char type1 = map_->getTile(tile1x, tile1y);
		char type2 = map_->getTile(tile2x, tile2y);
		// Verif Bombe
		for (ite_Bomb = map_->listBomb_->begin(); ite_Bomb != map_->listBomb_->end(); ++ite_Bomb) {		
				tileBomb = (*ite_Bomb)->getTile();
				if ((tileBomb.x == tile1x) && (tileBomb.y== tile1y)) type1 = 'Z';
				if ((tileBomb.x == tile2x )&& (tileBomb.y== tile2y)) type2 = 'Z';
		}
		
		if (((type1 == 'V')||(type1 == 'A')||(type1 == 'B')||(type1 == 'C')) && 
			((type2 == 'V')||(type2 == 'A')||(type2 == 'B')||(type2 == 'C'))) {												
			return true;
			
		}
		else {
			t1 = hitBox.Intersects(sf::IntRect(OFFSET_x+TILE_Size*tile1x,
							OFFSET_y+TILE_Size*tile1y,
							OFFSET_x+TILE_Size*((tile1x)+1),
							OFFSET_y+TILE_Size*((tile1y)+1)));
			t2 = hitBox.Intersects(sf::IntRect(OFFSET_x+TILE_Size*tile2x,
							OFFSET_y+TILE_Size*tile2y,
							OFFSET_x+TILE_Size*((tile2x)+1),
							OFFSET_y+TILE_Size*((tile2y)+1)));
		return !( t1 || t2);
				
		}
		
	}
	return true;
	
}


bool Player::canMoveL(sf::IntRect hitBox) const {	
	
	int tile1x, tile1y, tile2x, tile2y;
	bool t1; 
	bool t2;
	sf::Vector2<float> center = getCenter(hitBox);
	sf::Vector2<int> tileBomb;
	list<Bomb*>::iterator ite_Bomb;
	tile1x = min(max(((int)(center.x - OFFSET_x)/TILE_Size)-1,0),9);
	tile2x = tile1x;
	tile1y = min(max(((int)(center.y -(OFFSET_y+(HEIGHT_Hitbox/2))) / TILE_Size),0),9);
	tile2y = min(max(((int)(center.y -(OFFSET_y-(HEIGHT_Hitbox/2))) / TILE_Size),0),9);

	if (tile1x > -1){
			if (tile2x==-1) tile2x++;
			//cout << tile1y <<',' <<tile1x <<','<< tile2y<<','<< tile2x <<endl;
		char type1 = map_->getTile(tile1x, tile1y);
		char type2 = map_->getTile(tile2x, tile2y);
		// Verif Bombe
		for (ite_Bomb = map_->listBomb_->begin(); ite_Bomb != map_->listBomb_->end(); ++ite_Bomb) {		
				tileBomb = (*ite_Bomb)->getTile();
				if ((tileBomb.x == tile1x) && (tileBomb.y== tile1y)) type1 = 'Z';
				if ((tileBomb.x == tile2x )&& (tileBomb.y== tile2y)) type2 = 'Z';
		}
		if (((type1 == 'V')||(type1 == 'A')||(type1 == 'B')||(type1 == 'C')) && 
			((type2 == 'V')||(type2 == 'A')||(type2 == 'B')||(type2 == 'C'))) {								
			return true;
			
		}
		else {
			t1 = hitBox.Intersects(sf::IntRect(OFFSET_x+TILE_Size*tile1x,
							OFFSET_y+TILE_Size*tile1y,
							OFFSET_x+TILE_Size*((tile1x)+1),
							OFFSET_y+TILE_Size*((tile1y)+1)));
			t2 = hitBox.Intersects(sf::IntRect(OFFSET_x+TILE_Size*tile2x,
							OFFSET_y+TILE_Size*tile2y,
							OFFSET_x+TILE_Size*((tile2x)+1),
							OFFSET_y+TILE_Size*((tile2y)+1)));
		return !( t1 || t2);
				
		}
		
	}
	return true;
	
}

bool Player::canMoveU(sf::IntRect hitBox) const {	
	
	int tile1x, tile1y, tile2x, tile2y;
	bool t1; 
	bool t2;
	sf::Vector2<float> center = getCenter(hitBox);
	sf::Vector2<int> tileBomb;
	list<Bomb*>::iterator ite_Bomb;
	tile1y = min(max(((int)(center.y - OFFSET_y)/TILE_Size)-1,0),9);
	tile2y = tile1y;
	tile1x = min(max(((int)(center.x -(OFFSET_x+(WIDTH_Hitbox/2))) / TILE_Size),0),9);
	tile2x = min(max(((int)(center.x -(OFFSET_x-(WIDTH_Hitbox/2))) / TILE_Size),0),9);

	if (tile1y > -1){
		//cout << tile1y <<',' <<tile1x <<','<< tile2y<<','<< tile2x <<endl;
		char type1 = map_->getTile(tile1x, tile1y);
		char type2 = map_->getTile(tile2x, tile2y);
		// Verif Bombe
		for (ite_Bomb = map_->listBomb_->begin(); ite_Bomb != map_->listBomb_->end(); ++ite_Bomb) {		
				tileBomb = (*ite_Bomb)->getTile();
				if ((tileBomb.x == tile1x) && (tileBomb.y== tile1y)) type1 = 'Z';
				if ((tileBomb.x == tile2x )&& (tileBomb.y== tile2y)) type2 = 'Z';
		}
		if (((type1 == 'V')||(type1 == 'A')||(type1 == 'B')||(type1 == 'C')) && 
			((type2 == 'V')||(type2 == 'A')||(type2 == 'B')||(type2 == 'C'))) {												
			return true;
			
		}
		else {
			t1 = hitBox.Intersects(sf::IntRect(OFFSET_x+TILE_Size*tile1x,
							OFFSET_y+TILE_Size*tile1y,
							OFFSET_x+TILE_Size*((tile1x)+1),
							OFFSET_y+TILE_Size*((tile1y)+1)));
			t2 = hitBox.Intersects(sf::IntRect(OFFSET_x+TILE_Size*tile2x,
							OFFSET_y+TILE_Size*tile2y,
							OFFSET_x+TILE_Size*((tile2x)+1),
							OFFSET_y+TILE_Size*((tile2y)+1)));
		return !( t1 || t2);
				
		}
		
	}
	return true;
	
}

bool Player::canMoveD(sf::IntRect hitBox) const {	
	
	int tile1x, tile1y, tile2x, tile2y;
	bool t1; 
	bool t2;
	sf::Vector2<float> center = getCenter(hitBox);
	sf::Vector2<int> tileBomb;
	list<Bomb*>::iterator ite_Bomb;
	tile1y = min(max(((int)(center.y - OFFSET_y)/TILE_Size)+1,0),9);
	tile2y = tile1y;
	tile1x = min(max(((int)(center.x -(OFFSET_x+(WIDTH_Hitbox/2))) / TILE_Size),0),9);
	tile2x = min(max(((int)(center.x -(OFFSET_x-(WIDTH_Hitbox/2))) / TILE_Size),0),9);
	if (tile1y < 10){
		if (tile2y==10) tile2y--;
		//cout << tile1y <<',' <<tile1x <<','<< tile2y<<','<< tile2x <<endl;
		char type1 = map_->getTile(tile1x, tile1y);
		char type2 = map_->getTile(tile2x, tile2y);
		// Verif Bombe
		for (ite_Bomb = map_->listBomb_->begin(); ite_Bomb != map_->listBomb_->end(); ++ite_Bomb) {		
				tileBomb = (*ite_Bomb)->getTile();
				if ((tileBomb.x == tile1x) && (tileBomb.y== tile1y)) type1 = 'Z';
				if ((tileBomb.x == tile2x )&& (tileBomb.y== tile2y)) type2 = 'Z';
		}
		if (((type1 == 'V')||(type1 == 'A')||(type1 == 'B')||(type1 == 'C')) && 
			((type2 == 'V')||(type2 == 'A')||(type2 == 'B')||(type2 == 'C'))) {												
			return true;
			
		}
		else {
		//cout <<OFFSET_x+TILE_Size*tile1x<<','<< OFFSET_y+TILE_Size*tile1y<<','<<OFFSET_x+TILE_Size*((tile1x)+1)<<','<<
							//OFFSET_y+TILE_Size*((tile1y)+1)<< endl;
		//cout <<hitBox.Left<<','<<hitBox.Top<<','<<hitBox.Right<<','<<hitBox.Bottom<<','<< endl;

			t1 = hitBox.Intersects(sf::IntRect(OFFSET_x+TILE_Size*tile1x,
							OFFSET_y+TILE_Size*tile1y,
							OFFSET_x+TILE_Size*((tile1x)+1),
							OFFSET_y+TILE_Size*((tile1y)+1)));
			t2 = hitBox.Intersects(sf::IntRect(OFFSET_x+TILE_Size*tile2x,
							OFFSET_y+TILE_Size*tile2y,
							OFFSET_x+TILE_Size*((tile2x)+1),
							OFFSET_y+TILE_Size*((tile2y)+1)));
		return !( t1 || t2);
				
		}
		
	}
	return true;
	
}

	

void Player::moveRight() {
	int deplacement = min(deplacement_*win_->GetFrameTime(),(WIDTH_Hitbox/2.0f)-1);
	sf::IntRect hitBox = hitBox_;
	hitBox.Offset(deplacement,0);
	if (!isOutOfBound(hitBox)) {
		if (canMoveR(hitBox)) {
			position_.x = position_.x + deplacement;
			hitBox_.Offset(deplacement,0);
			current_ = Right;
			if (previous_ == Right) {
			// On incrémente l'itérateur de sprite droite
				if (ite_R != Rsprite_.end()) {
					currentsprite_->SetSubRect(Rsprite_[(ite_R)-(Rsprite_.begin())]);
					ite_R++;
				}
				else {
					ite_R = Rsprite_.begin();
					currentsprite_->SetSubRect(Rsprite_[(ite_R)-(Rsprite_.begin())]);
				}
		
			}
			else {
				currentsprite_->SetSubRect(Rsprite_[(ite_R)-(Rsprite_.begin())]);
				switch (previous_) {
		
					case Left :
						ite_L = Lsprite_.begin();
					break;
					case Up :
						ite_U = Usprite_.begin();
					break;
					case Down : 
						ite_D = Dsprite_.begin();
					break;
					default : break;
				}
				previous_ = Right;
			}
		}
		else {
			previous_ = Default;
			currentsprite_->SetSubRect(front_);
		}
	}
	else {
		previous_ = Default;
		currentsprite_->SetSubRect(front_);
	}
}
void Player::moveLeft() {
	int deplacement = min(deplacement_*win_->GetFrameTime(),(WIDTH_Hitbox/2.0f)-1);
	sf::IntRect hitBox = hitBox_;
	hitBox.Offset(-deplacement,0);

	if (!isOutOfBound(hitBox)) {	
		if (canMoveL(hitBox)) {
			current_ = Left;
			position_.x = position_.x - deplacement;
			hitBox_.Offset(-deplacement,0);
			if (previous_ == Left) {
			// On incrémente l'itérateur de sprite droite
				if (ite_L != Lsprite_.end()) {
					currentsprite_->SetSubRect(Lsprite_[(ite_L)-(Lsprite_.begin())]);
					ite_L++;
				}
				else {
					ite_L = Lsprite_.begin();
					currentsprite_->SetSubRect(Lsprite_[(ite_L)-(Lsprite_.begin())]);
				}
		
			}
			else {
				currentsprite_->SetSubRect(Lsprite_[(ite_L)-(Lsprite_.begin())]);
				switch (previous_) {
		
					case Right :
						ite_R = Rsprite_.begin();
					break;
					case Up :
						ite_U = Usprite_.begin();
					break;
					case Down : 
						ite_D = Dsprite_.begin();
					break;
					default : break;
				}
				previous_ = Left;
			}
		}
		else {
			previous_ = Default;
			currentsprite_->SetSubRect(front_);
		}
	}
	else {
		previous_ = Default;
		currentsprite_->SetSubRect(front_);
	}
}

void Player::moveUp() {
	int deplacement = min(deplacement_*win_->GetFrameTime(),(HEIGHT_Hitbox/2.0f)-1);
	sf::IntRect hitBox = hitBox_;
	hitBox.Offset(0,-deplacement);

	if (!isOutOfBound(hitBox)) {	
		if (canMoveU(hitBox)) {
			current_ = Up;
			position_.y = position_.y - deplacement;
			hitBox_.Offset(0,-deplacement);
			if (previous_ == Up) {
			// On incrémente l'itérateur de sprite droite
				if (ite_U != Usprite_.end()) {
					currentsprite_->SetSubRect(Usprite_[(ite_U)-(Usprite_.begin())]);
					ite_U++;
				}
				else {
					ite_U = Usprite_.begin();
					currentsprite_->SetSubRect(Usprite_[(ite_U)-(Usprite_.begin())]);
				}
		
			}
			else {
				currentsprite_->SetSubRect(Usprite_[(ite_U)-(Usprite_.begin())]);
				switch (previous_) {
		
					case Right :
						ite_R = Rsprite_.begin();
					break;
					case Left :
						ite_L = Lsprite_.begin();
					break;
					case Down : 
						ite_D = Dsprite_.begin();
					break;
					default : break;
				}
				previous_ = Up;
			}
		}
		else {
			previous_ = Default;
			currentsprite_->SetSubRect(front_);
		}
	}
	else {
		previous_ = Default;
		currentsprite_->SetSubRect(front_);
	}
}

void Player::moveDown() {
	int deplacement = min(deplacement_*win_->GetFrameTime(),(HEIGHT_Hitbox/2.0f)-1);
	sf::IntRect hitBox = hitBox_;
	hitBox.Offset(0,deplacement);

	if (!isOutOfBound(hitBox)) {	
		if (canMoveD(hitBox)) {
		current_ = Down;
		position_.y = position_.y + deplacement;
		hitBox_.Offset(0,deplacement);
		if (previous_ == Down) {
		// On incrémente l'itérateur de sprite droite
			if (ite_D != Dsprite_.end()) {
				currentsprite_->SetSubRect(Dsprite_[(ite_D)-(Dsprite_.begin())]);
				ite_D++;
			}
			else {
				ite_D = Dsprite_.begin();
				currentsprite_->SetSubRect(Dsprite_[(ite_D)-(Dsprite_.begin())]);
			}
		
		}
		else {
			currentsprite_->SetSubRect(Dsprite_[(ite_D)-(Dsprite_.begin())]);
			switch (previous_) {
		
				case Right :
					ite_R = Rsprite_.begin();
				break;
				case Left :
					ite_L = Lsprite_.begin();
				break;
				case Up : 
					ite_U = Usprite_.begin();
				break;
				default : break;
			}
			previous_ = Down;
		}
		}
		else {
			previous_ = Default;
			currentsprite_->SetSubRect(front_);
		}
	}
	else {
		previous_ = Default;
		currentsprite_->SetSubRect(front_);
	}
}

void Player::setDefault() {
	switch (previous_) {
		
			case Right :
				ite_R = Rsprite_.begin();
			break;
			case Left :
				ite_L = Lsprite_.begin();
			break;
			case Up : 
				ite_U = Usprite_.begin();
			break;
			case Down : 
				ite_D = Dsprite_.begin();
			break;
			default : break;
		}
	previous_ = Default;
	currentsprite_->SetSubRect(front_);
	
}

void Player::increasePower() { 
		power_++; 
}

void Player::decreasePower() { 
		power_--; 
}

void Player::increaseSpeed() {
	if (deplacement_ < SPEEDMAX) 
		deplacement_ += 50; 
}

void Player::decreaseSpeed() {
	if (deplacement_ > SPEEDMIN) 
		deplacement_ -= 50; 
}


sf::Vector2<float> getCenter(sf::IntRect hitBox) {
	return sf::Vector2<float> ((hitBox.Right - hitBox.Left)/2.0 + hitBox.Left,
				(hitBox.Bottom - hitBox.Top)/2.0 + hitBox.Top);
}	
	
int Player::getPower() const {	
	return power_;
}
int Player::getLife() const {	
	return life_;
}
int Player::getBombCount() const {	
	return bomb_count_;
}
void Player::kill() {
	life_--;
}

void Player::addBombtoCount() {
	bomb_count_++;
}

void Player::subBombtoCount() {
	bomb_count_--;
}
sf::Key::Code Player::getKeyLeft() const {
	return keySet_[0];
}
sf::Key::Code Player::getKeyRight() const {
	return keySet_[1];
}
sf::Key::Code Player::getKeyUp() const {
	return keySet_[2];
}
sf::Key::Code Player::getKeyDown() const{
	return keySet_[3];
}
sf::Key::Code Player::getKeyBomb() const{
	return keySet_[4];
}


void Player::setKeyUp(sf::Key::Code c) {
	keySet_[2] = c;
}

void Player::setKeyDown(sf::Key::Code c) {
	keySet_[3] = c;
}

void Player::setKeyLeft(sf::Key::Code c) {
	keySet_[0] = c;
}

void Player::setKeyRight(sf::Key::Code c) {
	keySet_[1] = c;
}

void Player::setKeyBomb(sf::Key::Code c) {
	keySet_[4] = c;
}

void Player::updateKeySet(char key_type, sf::Key::Code c) {
	switch (key_type) {
		case 'u' :
			setKeyUp(c);
		break;
		case 'd' : 
			setKeyDown(c);
		break;
		case 'l' : 
			setKeyLeft(c);
		break;
		case 'r' : 
			setKeyRight(c);
		break;
		case 'b' :
			setKeyBomb(c);
		break;
		default : break;
	}
}

void Player::getBonus() {
	sf::Vector2<int> tile = getTile();
	switch (map_->getTile(tile.x,tile.y)) {
		case 'A' :
			increaseSpeed();
			map_->setTile(tile.x, tile.y, 'V');
		break;
		case 'B' :
			addBombtoCount();
			map_->setTile(tile.x, tile.y, 'V');
		break;
		case 'C' : 
			increasePower();
			map_->setTile(tile.x, tile.y, 'V');
		break;
		default : break;
	}
}
float Player::getBombTime() {
	return bomb_timer_.GetElapsedTime();
}
void Player::resetBombTime() {
	bomb_timer_.Reset();
}

sf::Sprite Player::getHead() const {
	return *head_;
}

string Player::getName() const {
	return name_;
}
