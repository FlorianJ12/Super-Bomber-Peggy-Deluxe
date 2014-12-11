#include <SFML/Graphics.hpp>
#include "map.hpp"
#include <fstream>
#include <string>
#include <iostream>

using namespace std;

Map::Map (sf::RenderWindow *win, sf::Image *spriteset, list<Bomb*> *listBomb, string map)
	: win_(win), spriteset_(spriteset), listBomb_(listBomb)

{
	char randTab[3] = {'A','B','C'}; // Accélération, Bombe, Collatéral
	int indice, nb_foin;
	
	// Initialisation des spriteset
	
	Foin = new sf::Sprite;
	Pierre = new sf::Sprite;
	Acceleration = new sf::Sprite;
	Bombb = new sf::Sprite;
	Colateral = new sf::Sprite;
	
	Foin->SetImage(*spriteset_);
	Foin->SetSubRect(sf::IntRect(0,0,TILE_Size,TILE_Size));
	Pierre->SetImage(*spriteset_);
	Pierre->SetSubRect(sf::IntRect(TILE_Size,0, 2*TILE_Size, TILE_Size));
	Acceleration->SetImage(*spriteset_);
	Acceleration->SetSubRect(sf::IntRect(TILE_Size*8,TILE_Size,TILE_Size*9,2*TILE_Size));
	Bombb->SetImage(*spriteset_);
	Bombb->SetSubRect(sf::IntRect(TILE_Size*7,TILE_Size,TILE_Size*8,2*TILE_Size));
	Colateral->SetImage(*spriteset_);
	Colateral->SetSubRect(sf::IntRect(TILE_Size*6,TILE_Size,TILE_Size*7,2*TILE_Size));
	
	// Allocation d'une map [MAP_Width][MAP_Height]	
	map_ = new char * [MAP_Width];
	for(int i = 0; i<MAP_Width; i++) map_[i]= new char[MAP_Height];
	
	
	//Initialisation avec le fichier map.txt
	string line;
	ifstream fichiermap ("data/maps/"+map, ios::in);
	if (fichiermap) {
		for(int j = 0; j<MAP_Height; j++) {
			getline(fichiermap, line);	
			for (int i=0; i<MAP_Width; i++){
				map_[i][j]=line[i];
				if (line[i] == 'F') nb_foin++; 	
			}				
		}
		fichiermap.close();	
	} else {
		cerr <<"Impossible d'ouvrir le fichier map !" << endl;	
	}
	
	// Allocation du tableau de bonus et compteur de bonus_
	cpt_bonus_ = -1;
	bonus_ = new char [nb_foin];

	// Initialisation du tableau de bonus.
	for (int i = 0; i < nb_foin; i++ ) {
		indice = sf::Randomizer::Random (0,9); //* De O à 9.
		if (indice > 4) {
			indice = sf::Randomizer::Random (0,2);
			bonus_[i] = randTab[indice];
		}
		else bonus_[i] = 'V';
	}

}

Map::~Map()
{	
	delete Foin;
	delete Pierre;
	delete Colateral;
	delete Acceleration;
	delete Bombb;
	for (int i = 0; i<MAP_Width; i++) delete [] map_[i];
	delete [] map_;
	delete []bonus_;
	delete listBomb_;
}

char Map::getTile(int x, int y) const
{
	return map_[x][y];
}




void Map::setTile(int x, int y, char type) {
	map_[x][y] = type;
}

char Map::getRandTile() {
	cpt_bonus_++;
	return bonus_[cpt_bonus_];
}

void Map::display() const
{

	
	for(int j = 0; j<MAP_Height; j++) {	
		for (int i=0; i<MAP_Width; i++){
			switch (map_[i][j]) { 
				case 'P' :
					Pierre->SetPosition(i*TILE_Size + OFFSET_x , j*TILE_Size + OFFSET_y);
					win_->Draw(*Pierre);					
				break; 
				case 'F' :
					Foin->SetPosition(i*TILE_Size + OFFSET_x , j*TILE_Size + OFFSET_y);
					win_->Draw(*Foin);					
				break; 
				case 'A' :
					Acceleration->SetPosition(i*TILE_Size + OFFSET_x , j*TILE_Size + OFFSET_y);
					win_->Draw(*Acceleration);					
				break; 
				case 'B' :
					Bombb->SetPosition(i*TILE_Size + OFFSET_x , j*TILE_Size + OFFSET_y);
					win_->Draw(*Bombb);					
				break; 
				case 'C' :
					Colateral->SetPosition(i*TILE_Size + OFFSET_x , j*TILE_Size + OFFSET_y);
					win_->Draw(*Colateral);					
				break; 
				default : break;
				}
			}	
	}			
}








