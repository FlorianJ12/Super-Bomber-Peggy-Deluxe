#include <SFML/Graphics.hpp>
#include "mapEditor.hpp"
#include <fstream>
#include <string>
#include <iostream>

using namespace std;
MapEditor::MapEditor(int size_x, int size_y)  
{
	
	// Initialisation des sprites.
	spriteseteditor_ = new sf::Image;
	spriteseteditor_->LoadFromFile("data/mapeditorsprite.png");
	
	spriteset_ = new sf::Image;
	spriteset_ ->LoadFromFile("data/spritesheetb.png");
	
	win_editor = new sf::RenderWindow(sf::VideoMode(1024,768),"Super Bomber Peggy Deluxe : Map Editor");
	win_editor->SetSize (size_x, size_y);
	win_editor->SetFramerateLimit(FRAME_RATE);

	background_editor_ = new sf::Image;
	background_editor_ -> LoadFromFile("data/background.png");
	
	background_ = new sf::Sprite;
	background_ -> SetImage(*background_editor_);
	
	spawn_ = new sf::Sprite;
	spawn_ -> SetImage(*spriteseteditor_);
	spawn_ -> SetSubRect (sf::IntRect(TILE_Size, 2*TILE_Size, 2*TILE_Size, 3*TILE_Size));
	
	FoinMobile_ = new sf::Sprite;
	FoinMobile_->SetImage(*spriteset_);
	FoinMobile_->SetSubRect(sf::IntRect(0,0,TILE_Size,TILE_Size));
	PierreMobile_ = new sf::Sprite;
	PierreMobile_->SetImage(*spriteset_);
	PierreMobile_->SetSubRect(sf::IntRect(TILE_Size,0, 2*TILE_Size, TILE_Size));
	
	Foin_ = new sf::Sprite;
	Pierre_ = new sf::Sprite;
	Foin_->SetImage(*spriteset_);
	Foin_->SetSubRect(sf::IntRect(0,0,TILE_Size,TILE_Size));
	Pierre_->SetImage(*spriteset_);
	Pierre_->SetSubRect(sf::IntRect(TILE_Size,0, 2*TILE_Size, TILE_Size));	
	
	NewFoin_ = new sf::Sprite;
	NewPierre_ = new sf::Sprite;
	NewFoin_->SetImage(*spriteset_);
	NewFoin_->SetSubRect(sf::IntRect(0,0,TILE_Size,TILE_Size));
	NewPierre_->SetImage(*spriteset_);
	NewPierre_->SetSubRect(sf::IntRect(TILE_Size,0, 2*TILE_Size, TILE_Size));
	
	map_ = new char * [MAP_Width];
	for(int i = 0; i<MAP_Width; i++) map_[i]= new char[MAP_Height];
	for(int j = 0; j<MAP_Height; j++) {
		for (int i=0; i<MAP_Width; i++){
			map_[i][j]='V';
		}				
	}
	erase = new sf::Sprite;
	erase->SetImage(*spriteset_);
	erase->SetSubRect(sf::IntRect(0,5*TILE_Size,TILE_Size, 6*TILE_Size));
	
	eraseMobile = new sf::Sprite;
	eraseMobile->SetImage(*spriteset_);
	eraseMobile->SetSubRect(sf::IntRect(0,5*TILE_Size,TILE_Size, 6*TILE_Size));
	
	save_ = new sf::Sprite;
	save_-> SetImage(*spriteseteditor_);
	save_->SetSubRect(sf::IntRect(0, 2*TILE_Size, TILE_Size, 3*TILE_Size));
	
	enterstring_ = new sf::Sprite;
	enterstring_-> SetImage(*spriteseteditor_);
	enterstring_->SetSubRect(sf::IntRect(0, 0, 300, 2*TILE_Size));
	
	groBold_ = new sf::Font;
	groBold_ -> LoadFromFile("data/Font/GROBOLD.ttf");
	
	
}

MapEditor::~MapEditor() {
	delete win_editor;
	delete background_editor_;
	delete spriteset_;
	delete spriteseteditor_;
	delete background_;
	delete FoinMobile_;
	delete PierreMobile_;
	delete Foin_; 
	delete Pierre_;
	delete NewFoin_;
	delete NewPierre_;
	delete groBold_;
	for (int i = 0; i<MAP_Width; i++) delete [] map_[i];
	delete [] map_;
	delete erase;
	delete eraseMobile;
	delete save_;
	delete enterstring_;
	delete spawn_;
}


bool MapEditor::mapGenerator(){
	
	// Initialisation du mask efficace
	sf::Shape mask = sf::Shape::Rectangle (0,0, 1024,768, sf::Color(0,0,0,200));
	
	// Création de l'extention du fichier. 
	sf::String str(" .txt");
	str.SetSize(26);
	str.SetFont(*groBold_);
	
	//Nom de la map
	string mapName;
	
	// Gestion d'évènement.
	sf::Event event;
	const sf::Input& Input = win_editor->GetInput();
	
	// Positionnement des objets 
	Pierre_->SetPosition(35,50);
	Foin_->SetPosition(35,150);
	erase-> SetPosition(35,250);
	eraseMobile-> SetPosition(35,250);
	save_ -> SetPosition(35,500);
	enterstring_ -> SetPosition(400, 300);
	str.SetPosition(430,350);

	// Créations des zones cliquables
	sf::IntRect buttonPierre (35,50,35+TILE_Size,50+TILE_Size);		// Bouton Pierre
	sf::IntRect buttonFoin (35,150,35+TILE_Size,150+TILE_Size);		// Bouton Foin
	sf::IntRect buttonErase(35, 250, 35+TILE_Size, 250+TILE_Size);		// Bouton Erase
	sf::IntRect buttonSave(35, 500, 35+TILE_Size, 500+TILE_Size);		// Bouton Save
	
		
	bool addFoin = false;
	bool addPierre = false;
	bool tileErase = false;
	bool saved = false;
	
	float x,y;
	sf::Vector2<float> pointinview;
	
	while (win_editor -> IsOpened()) {
		while(win_editor -> GetEvent(event)) {
			switch (event.Type) {
				case sf::Event::Closed : 
					win_editor-> Close();
					return false;
				break;
				case sf::Event::MouseButtonPressed : // Récupération des évènements de clic
					pointinview =  win_editor->ConvertCoords(event.MouseButton.X, event.MouseButton.Y);
					x = pointinview.x;
					y = pointinview.y;

					if (buttonFoin.Contains(x,y)) { // Clic sur foin
						addFoin = true;
						addPierre = false;
						tileErase = false;
					}
					else if (buttonPierre.Contains(x,y)) {// Clic sur pierre
						addPierre = true;
						addFoin = false;
						tileErase = false;
					}
					else if (buttonErase.Contains(x,y)) {// Clic sur suppression
						addPierre = false;
						addFoin = false;
						tileErase = true;
					}
					else if (buttonSave.Contains(x,y)) {// Clic sur sauvegarder
						saved = true;
					}
				break; 	
				case sf::Event::MouseButtonReleased : // Placement d'un nouvel objet
					pointinview =  win_editor->ConvertCoords(event.MouseButton.X, event.MouseButton.Y);
					x = pointinview.x;
					y = pointinview.y;
					if ((x > OFFSET_x) && (x < (10*TILE_Size+OFFSET_x)) && (y > OFFSET_y) && (y < (10*TILE_Size+OFFSET_y))) { 
						if (addPierre) {
							write('P', (int)((x - OFFSET_x)/TILE_Size), (int)(y -OFFSET_y)/TILE_Size);
						}
						else if (addFoin) {
							write('F', (int)(x - OFFSET_x)/TILE_Size, (int)(y -OFFSET_y)/TILE_Size); 
						}
						else if (tileErase) {
							write('V', (int)(x - OFFSET_x)/TILE_Size, (int)(y -OFFSET_y)/TILE_Size); 
						}
					}
					addFoin = false;
					addPierre = false;
					tileErase = false;
				break;
				case sf::Event::TextEntered: // Choix du nom de map
					if (saved) {
						
						if (event.Text.Unicode == 8) { // Unicode back = 8
							if (!mapName.empty()) {
								mapName.pop_back();
								str.SetText( mapName + ".txt");
							}
						}
						else if (event.Text.Unicode == 13) { // Unicode return = 13
							save(mapName);
							win_editor -> Close();
						}
						else {
							if (mapName.size() < 15) {
								mapName += event.Text.Unicode;
								str.SetText( mapName + ".txt");
							}
						}
					
					}
				break;	
				case sf::Event::KeyPressed: 
					if (saved) {
						if (event.Key.Code == sf::Key::Escape) {
							saved = !saved;	
							mapName = "";
							str.SetText( mapName + ".txt");
						}
					}
					else {
						win_editor -> Close();
						return true;
					}
				break;	
				default : break;
				
			}
		}
	win_editor->Clear();
	win_editor -> Draw(*background_);
	win_editor -> Draw(*Foin_);
	win_editor -> Draw(*Pierre_);
	win_editor -> Draw(*erase);
	win_editor -> Draw(*save_);
	display();
	pointinview =  win_editor->ConvertCoords(Input.GetMouseX(), Input.GetMouseY());
	x = pointinview.x;
	y = pointinview.y;
	if (addPierre) {
		PierreMobile_ -> SetPosition(x-TILE_Size/2,y-TILE_Size/2);
		win_editor -> Draw(*PierreMobile_);		
	}
	else if (addFoin) {
		FoinMobile_ -> SetPosition(x-TILE_Size/2,y-TILE_Size/2);
		win_editor -> Draw(*FoinMobile_);
	}
	else if (tileErase) {
		eraseMobile -> SetPosition(x-TILE_Size/2,y-TILE_Size/2);
		win_editor -> Draw(*eraseMobile);
	}
	else if (saved) {
		win_editor -> Draw(mask);
		win_editor -> Draw(*enterstring_);
		win_editor -> Draw(str);
	}
	
	win_editor -> Display();
	
	}
	
	return true;	
}


void MapEditor::write(char type, int x, int y) { // Ecriture dans le tableau à deux dimensions
	if ((x == 0 && y== 0) ||  (x == 0 && y== 9) || (x == 9 && y== 0) || (x == 9 && y== 9)) { // On écrit pas sur un spawn
		cout << "Spawn position : you can't place it here ! "<< endl;
	}
	else {
		map_[x][y] = type;
	}
}


void MapEditor::display() const  // Affichage courant
{
	for(int j = 0; j<MAP_Height; j++) {	
		for (int i=0; i<MAP_Width; i++){
			switch (map_[i][j]) { 
				case 'P' :
					NewPierre_->SetPosition(i*TILE_Size + OFFSET_x , j*TILE_Size + OFFSET_y);
					win_editor->Draw(*NewPierre_);					
				break; 
				case 'F' :
					NewFoin_->SetPosition(i*TILE_Size + OFFSET_x , j*TILE_Size + OFFSET_y);
					win_editor->Draw(*NewFoin_);					
				break; 
				default : break;
			}
		}
	}
	spawn_-> SetPosition (0 + OFFSET_x, 0+OFFSET_y);
	win_editor-> Draw (*spawn_);
	spawn_-> SetPosition (0 + OFFSET_x, 9*TILE_Size+OFFSET_y);
	win_editor-> Draw (*spawn_);
	spawn_-> SetPosition (9*TILE_Size + OFFSET_x, 9*TILE_Size+OFFSET_y);
	win_editor-> Draw (*spawn_);
	spawn_-> SetPosition (9*TILE_Size + OFFSET_x, 0+OFFSET_y);
	win_editor-> Draw (*spawn_);
}


void MapEditor::save(string mapName) { // Sauvegarde et codage du tableau[][] dans un fichier.

	string new_map = "data/maps/" + mapName + ".txt";
	ofstream new_file(new_map); 
	 
	if (!new_file) {
		cerr << "Erreur création fichier" << endl;
	}
	else {
		for(int j = 0; j <= 9 ; j++)  {
		 	for(int i = 0; i <= 9 ; i++) {
		    		new_file << map_[i][j];
		    	}
		new_file << "\n";
		}
		cout << "Map saved" << endl;
	}
}
		

	
	
	

