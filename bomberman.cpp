#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "bomberman.hpp"
#include "map.hpp"
#include "player.hpp"
#include "bomber.hpp"
#include "strong.hpp"
#include "fast.hpp"
#include "bomb.hpp"
#include "mapEditor.hpp"




Bomberman::Bomberman ()
{

	// Taille de fenêtre
	size_win_ = 3;
	size_window_ = new sf::Vector2<int>[4];
	size_window_[0] = sf::Vector2<int>(400,300);	
	size_window_[1] = sf::Vector2<int>(800,600);
	size_window_[2] = sf::Vector2<int>(1024,768);
	size_window_[3] = sf::Vector2<int>(500,385);
	
	// Position de spawn
	start_postion_ = new sf::Vector2<float>[4];
	start_postion_[0] = sf::Vector2<float>(250,50);	
	start_postion_[1] = sf::Vector2<float>(880,680);
	start_postion_[2] = sf::Vector2<float>(880,50);
	start_postion_[3] = sf::Vector2<float>(250,680);

	// Récupération des images
	background_ = new sf::Image ;
	spriteset_ = new sf::Image;
	spriteset_fast_ = new sf::Image;
	spriteset_strong_ = new sf::Image;
	background_menu_ = new sf::Image ;
	spriteset_menu_ = new sf::Image;
	current_background_ = new sf::Sprite;
	music_menu_ = new sf::Music;	
	music_game_ = new sf::Music;	
	background_->LoadFromFile("data/background2.png");
	spriteset_ ->LoadFromFile("data/spritesheetb.png");
	spriteset_fast_ ->LoadFromFile("data/spritesheetf.png");
	spriteset_strong_ ->LoadFromFile("data/spritesheets.png");
	background_menu_->LoadFromFile("data/background_settings.png");
	spriteset_menu_ ->LoadFromFile("data/spritesetting.png");
	victory_ = new sf::Sprite;
	
	map_ = NULL;
	groBold_ = new sf::Font;
	groBold_ -> LoadFromFile("data/Font/GROBOLD.ttf");
	
	music_menu_->OpenFromFile("data/SonsProjet/superstition.ogg");
	music_menu_->SetLoop(true);
	music_menu_->SetPitch(1.1);
	music_menu_->SetVolume(75);
	music_game_->OpenFromFile("data/SonsProjet/SweetHomeAlabama.ogg");
	music_game_->SetLoop(true);
	music_game_->SetPitch(1.15);
	music_game_->SetVolume(25);


	
	bombtime_.Reset();
	
	listBomb_ = new list<Bomb*>;
	// Création fenêtre
	win_ = new sf::RenderWindow(sf::VideoMode(size_window_[(int)size_win_].x, size_window_[(int)size_win_].y),
					"Super BomberPeggy Deluxe", sf::Style::Titlebar | sf::Style::Close);
	win_->SetFramerateLimit(FRAME_RATE);

	// Initialisation 

	timer_ = new Timer();
	music_ = true;
	sound_ = true;
	size_win_ = 1;
	
	vector <sf::Key::Code> setKey1;
	setKey1.push_back(sf::Key::Left);
	setKey1.push_back(sf::Key::Right);
	setKey1.push_back(sf::Key::Up);
	setKey1.push_back(sf::Key::Down);
	setKey1.push_back(sf::Key::Space);	
	tableKeySet_.push_back(setKey1);
	vector<sf::Key::Code> setKey2;
	setKey2.push_back(sf::Key::Q);
	setKey2.push_back(sf::Key::D);
	setKey2.push_back(sf::Key::Z);
	setKey2.push_back(sf::Key::S);
	setKey2.push_back(sf::Key::LShift);	
	tableKeySet_.push_back(setKey2);
	vector<sf::Key::Code> setKey3;
	setKey3.push_back(sf::Key::J);
	setKey3.push_back(sf::Key::L);
	setKey3.push_back(sf::Key::I);
	setKey3.push_back(sf::Key::K);
	setKey3.push_back(sf::Key::B);	
	tableKeySet_.push_back(setKey3);
	vector<sf::Key::Code> setKey4;
	setKey4.push_back(sf::Key::J);
	setKey4.push_back(sf::Key::L);
	setKey4.push_back(sf::Key::I);
	setKey4.push_back(sf::Key::K);
	setKey4.push_back(sf::Key::B);	
	tableKeySet_.push_back(setKey4);
	
}

Bomberman::~Bomberman ()
{
	delete win_;
	delete background_;
	delete background_menu_;
	if (map_ != NULL) delete map_;
	delete size_window_;
	delete spriteset_;
	delete spriteset_menu_;
	delete current_background_;
	delete spriteset_fast_;
	delete spriteset_strong_;
	delete music_menu_;
	delete music_game_;
	delete timer_;
	delete groBold_;
	delete victory_;
}


bool Bomberman::start() {
	// Chargement du fond
	current_background_->SetImage(*background_menu_);
	// Création des sprites

	sf::Sprite on, off, small, medium, large, board, 
		number,peggy,key, keyUp, keyDown, keyLeft, 
		keyRight, keyBomb, next, choixmap;
	sf::Shape mask;
	sf::String map1, map2, map3;
	
	map1.SetSize(30);
	map2.SetSize(30);
	map3.SetSize(30);
	
	map1.SetFont(*groBold_);
	map2.SetFont(*groBold_);
	map3.SetFont(*groBold_);
		
	map1.SetPosition(100, 100);
	map2.SetPosition(100, 185);
	map3.SetPosition(100, 270);

	on.SetImage(*spriteset_menu_);
	on.SetSubRect(sf::IntRect(0,380,120,430));

	off.SetImage(*spriteset_menu_);
	off.SetSubRect(sf::IntRect(120,380,240,430));

	small.SetImage(*spriteset_menu_);
	small.SetSubRect(sf::IntRect(11,300,139,370));

	medium.SetImage(*spriteset_menu_);
	medium.SetSubRect(sf::IntRect(148,300,276,370));

	large.SetImage(*spriteset_menu_);
	large.SetSubRect(sf::IntRect(285,300,413,370));
	
	board.SetImage(*spriteset_menu_);
	board.SetSubRect(sf::IntRect(0,0,400,300));
	board.SetPosition(50,42);

	number.SetImage(*spriteset_menu_);
	number.SetSubRect(sf::IntRect(0,940,420, 1250));
	number.SetPosition(50,42);

	peggy.SetImage(*spriteset_menu_);
	peggy.SetSubRect(sf::IntRect(0,435,420, 730));
	peggy.SetPosition(50,42);
	
	choixmap.SetImage(*spriteset_menu_);
	choixmap.SetSubRect (sf::IntRect (0, 1260, 422, 1600));
	choixmap.SetPosition (50,42);

	key.SetImage(*spriteset_menu_);
	key.SetSubRect(sf::IntRect(0,735,420, 940));
	key.SetPosition(57,90);
	
	keyUp.SetImage(*spriteset_menu_);
	keyUp.SetSubRect(sf::IntRect(120,800,172, 852));
	keyUp.SetPosition(177,155);
	
	keyDown.SetImage(*spriteset_menu_);
	keyDown.SetSubRect(sf::IntRect(121,860,173, 912));
	keyDown.SetPosition(177,215);

	keyLeft.SetImage(*spriteset_menu_);
	keyLeft.SetSubRect(sf::IntRect(50,860,102, 912));
	keyLeft.SetPosition(107, 215);

	keyRight.SetImage(*spriteset_menu_);
	keyRight.SetSubRect(sf::IntRect(196,860,248, 912));
	keyRight.SetPosition(253, 215);
	
	keyBomb.SetImage(*spriteset_menu_);
	keyBomb.SetSubRect(sf::IntRect(277,860,329, 912));
	keyBomb.SetPosition(333, 215);
	
	next.SetImage(*spriteset_menu_);
	next.SetSubRect(sf::IntRect(292, 747, 391, 779));
	next.SetPosition(350, 102);


	mask = sf::Shape::Rectangle (0,0, size_window_[3].x,size_window_[3].y, sf::Color(0,0,0,200));
	
	// Utilitaire d'évènement.
	sf::Event event;
	const sf::Input& Input = win_->GetInput();
	
	// MAP
	list<string>::iterator ite_m;
	
	
	// Création des zones cliquables
	sf::IntRect a_play (80,217,185,300);
	sf::IntRect a_setting (330, 292, 453, 337);
	sf::IntRect a_quit (340,342, 450, 373);
	sf::IntRect a_music (285,87,405,132);
	sf::IntRect a_sound (285,167,405,212);
	sf::IntRect a_win (290,242,420,287);
	sf::IntRect a_back (170,312, 435,342);
	sf::IntRect a_2p (90,87,437,163);
	sf::IntRect a_3p (90,164, 437,243);
	sf::IntRect a_4p (90,244, 437,320);
	sf::IntRect a_1m (80, 95, 438, 156);
	sf::IntRect a_2m (80, 167, 438, 244);
	sf::IntRect a_3m (80, 256, 438, 330);
	sf::IntRect a_flash (100,130, 200,255);
	sf::IntRect a_bomber (208,130, 308,255);
	sf::IntRect a_strong (318,130, 418,255);
	sf::IntRect a_next (327,90,427,130);
	sf::IntRect a_up (177,155,227,205);
	sf::IntRect a_down (177,215,227,265);
	sf::IntRect a_left (107, 215, 157, 265);
	sf::IntRect a_right (253, 215, 303, 265);
	sf::IntRect a_bomb (333, 215, 383, 265);
	sf::IntRect a_MapEditor(0, 326, 160 , 367);
	sf::IntRect a_downm(170,334, 256, 369);
	sf::IntRect a_upm(258,334, 344, 369);
	
	

	int nbr_player = 0;
	int nbr_player_set = 0;
	
	char changeKey = 0;
	bool setting = true; // On a pas commencé à joué.
	bool modifier = false; // Les settings ne sont pas ouvert.
	bool choosenumber = false;
	bool chooseplayer = false; // On choisit le joueur
	bool choosekey = false ; // On choisit les touches
	bool already_stop = false;
	bool choosemap = false;
	bool mapedit = false;
	int x,y, choosemap_pos = 0, choicemap = 0;
	string name;
	sf::String player_Name;
	player_Name.SetPosition (310,15);
	player_Name.SetSize (40);
	player_Name.SetColor(sf::Color(255,255,255,200));
	music_menu_->Play();
	while (win_->IsOpened() && setting) { // Tant que la fenêtre de jeu est ouverte

		while (win_-> GetEvent(event)) {
			switch (event.Type) {

				case sf::Event::Closed : 
					win_-> Close();
					return false;
				break;
				case sf::Event::MouseButtonPressed : // L'utilisateur a cliqué
					x = event.MouseButton.X;
					y = event.MouseButton.Y;
					if (!modifier && !choosenumber && !chooseplayer && !choosekey && !choosemap) {	// On a pas cliqué sur settings	
						if (a_play.Contains(x,y)) {	// On passe au choix de personnage.
							choosemap = true;
							listMap_ = listMapFile();
							ite_m = listMap_.begin();
							if (ite_m != listMap_.end()) map1.SetText (*ite_m++);
							if (ite_m != listMap_.end()) map2.SetText (*(ite_m++));
							if (ite_m != listMap_.end()) map3.SetText (*(ite_m++));
												
						}
						else if (a_setting.Contains(x,y)) {	// On passe à la page de settings
								modifier = true;						
						}
						else if (a_quit.Contains (x,y)){ // On quitte
							win_ -> Close();
							return false;
						}
						else if (a_MapEditor.Contains(x,y)){
							mapedit = true;
						}					
					}
					else {
						if (modifier) {
							if (a_music.Contains(x,y)) {
								music_ = !music_;						
							}
							else if (a_sound.Contains(x,y)){
								sound_ = !sound_;							
							}						
							else if (a_win.Contains(x,y)){
								size_win_++;
								if (size_win_ > 2) {
									size_win_ = 0;
								}							
							}
							else if (a_back.Contains(x,y)) {
								modifier = false ;			
							}			
						}
						else {
							if (choosemap) {
								ite_m = listMap_.begin();
								for (int i = 0; i<choosemap_pos; i++) ite_m++;
								if (a_upm.Contains(x,y) && ite_m != listMap_.begin()) {
									choosemap_pos--;
									ite_m--;
									if (ite_m != listMap_.end()) map1.SetText (*ite_m++);
									if (ite_m != listMap_.end()) map2.SetText (*(ite_m++));
									if (ite_m != listMap_.end()) map3.SetText (*(ite_m++));
								}
								else {
									for (int j = 0; j < 3; j++) if (ite_m != listMap_.end()) ite_m++;
									if (a_downm.Contains (x,y) && ite_m != listMap_.end()) {
										choosemap_pos++;
										ite_m = listMap_.begin();
										for (int i = 0; i<choosemap_pos; i++) ite_m++;
										if (ite_m != listMap_.end()) map1.SetText (*ite_m++);
										if (ite_m != listMap_.end()) map2.SetText (*(ite_m++));
										if (ite_m != listMap_.end()) map3.SetText (*(ite_m++));
									}
								}
								if (a_1m.Contains (x,y)) choicemap = 1;
								else if (a_2m.Contains (x,y)) choicemap = 2;
								else if (a_3m.Contains (x,y)) choicemap = 3;
								if (choicemap != 0) {
									ite_m = listMap_.begin();
									for (int i = 0; i<choosemap_pos+choicemap-1; i++) ite_m++;
									map_ = new Map(win_, spriteset_,listBomb_, *ite_m);
									choosenumber = true;
									choosemap=false;
									
								}
								
							}
							else if (choosenumber) {
								name = "Player "+inttoString(nbr_player_set+1);
								player_Name.SetText(name);
								if (a_2p.Contains(x,y)){
									nbr_player = 2; 
									choosenumber = false;
									chooseplayer = true;
									}
								else {
									if (a_3p.Contains(x,y)){
									nbr_player = 3;
									choosenumber = false;
									chooseplayer = true;
									}
									else {
										if (a_4p.Contains(x,y)){
										nbr_player = 4;
										choosenumber = false;
										chooseplayer = true;
										}
									}	
								}						
							}
							else if (chooseplayer && (nbr_player_set<nbr_player)) {
								if (!choosekey) {
									if (a_flash.Contains(x,y)){
										
										tablePlayers_.push_back(new Fast
										(win_,start_postion_[nbr_player_set], 
										spriteset_fast_, map_,
										tableKeySet_[nbr_player_set], name));

										choosekey = true;
										}
									else if (a_bomber.Contains(x,y)){
										tablePlayers_.push_back(new Bomber
										(win_,start_postion_[nbr_player_set], 
										spriteset_, map_,tableKeySet_[nbr_player_set],
										name));	
										choosekey = true;
										}
										else if (a_strong.Contains(x,y)){
											tablePlayers_.push_back(new Strong
											(win_,start_postion_[nbr_player_set],
											spriteset_strong_, map_,
											tableKeySet_[nbr_player_set],
											name));
											choosekey = true;
										}
										
										
								}else if (a_next.Contains(x,y)){// Modification des touches
									nbr_player_set++;
									choosekey=false;
									changeKey = 0;
									name = "Player "+inttoString(nbr_player_set+1);
									player_Name.SetText(name);
									if (nbr_player_set == nbr_player) {
										chooseplayer = false;
										return true;
									}
								}
								else if (a_up.Contains(x,y)) {
									changeKey = 'u';
								}
								else if (a_down.Contains(x,y)) {
									changeKey = 'd';
								}
								else if (a_left.Contains(x,y)) {
									changeKey = 'l';
								}
								else if (a_right.Contains(x,y)) {
									changeKey = 'r';
								}
								else if(a_bomb.Contains(x,y)){
									changeKey = 'b';
								}			 
							}						
						}						
					}
				break;
				case sf::Event::KeyPressed : 
					if (event.Key.Code == sf::Key::Escape) {
						setting = true;modifier = false ;chooseplayer = false;
						choosekey = false;choosemap=false;mapedit = false; choosenumber = false;
						for (int i = 0; i< (int)tablePlayers_.size(); i++) {
							delete tablePlayers_[i];
						}
						tablePlayers_.clear();
						nbr_player = 0;
						nbr_player_set = 0;
						
					}
					switch (changeKey ) {
						case 'u' : 
							tableKeySet_[nbr_player_set][2] = event.Key.Code;
							tablePlayers_[nbr_player_set]-> updateKeySet(changeKey, event.Key.Code);
							changeKey = 0;
							win_ -> Draw(key);
						break;
						case 'd' : 
							tableKeySet_[nbr_player_set][3] = event.Key.Code;
							tablePlayers_[nbr_player_set]-> updateKeySet(changeKey, event.Key.Code);
							changeKey = 0;
							win_ -> Draw(key);
						break;
						case 'l' : 
							tableKeySet_[nbr_player_set][0] = event.Key.Code;
							tablePlayers_[nbr_player_set]-> updateKeySet(changeKey, event.Key.Code);
							changeKey = 0;
							win_ -> Draw(key);
						break;
						case 'r' : 
							tableKeySet_[nbr_player_set][1] = event.Key.Code;
							tablePlayers_[nbr_player_set]-> updateKeySet(changeKey, event.Key.Code);
							changeKey = 0;
							win_ -> Draw(key);
						break;
						case 'b': 
							tableKeySet_[nbr_player_set][4] = event.Key.Code;
							tablePlayers_[nbr_player_set]-> updateKeySet(changeKey, event.Key.Code);
							changeKey = 0;
							win_ -> Draw(key);
						break;
						default : break;
					
					}
				break;
				default : break;
			}				
		}
		
		// Affichage
		win_->Draw(*current_background_);
		
		if (modifier) {
			win_->Draw (mask);
			win_->Draw(board);
			if (sound_) {

				on.SetPosition (a_sound.Left, a_sound.Top);	
				win_->Draw (on);
			}
			else {

				off.SetPosition (a_sound.Left, a_sound.Top);
				win_->Draw (off);			
			}
			if (music_) {
				if(already_stop) {
					music_menu_ -> Play();
					already_stop = false;
				}
				on.SetPosition (a_music.Left, a_music.Top);	
				win_->Draw (on);
			}
			else {
				music_menu_ -> Stop();
				already_stop = true;	
				off.SetPosition (a_music.Left, a_music.Top);
				win_->Draw (off);			
			}
			switch (size_win_) {
				case 0 :
					small.SetPosition (a_win.Left, a_win.Top);
					win_->Draw (small);
				break;
				case 1 :
					medium.SetPosition (a_win.Left, a_win.Top);
					win_->Draw (medium);
				break;
				case 2 :
					large.SetPosition (a_win.Left, a_win.Top);
					win_->Draw (large);
				break;
				default : break;
			}
		}
		if (choosenumber) {
			win_->Draw (mask);
			win_->Draw(number);
			
		}
		else {
			if (chooseplayer ) {
				win_->Draw (mask);
				win_->Draw(peggy);
				win_->Draw(player_Name);
				if (!choosekey) {
					x = Input.GetMouseX();
					y = Input.GetMouseY();
					if (a_flash.Contains(x,y)) {
						print_stat ("2","1","1");
					}
					else if (a_strong.Contains(x,y)) {
						print_stat ("1","1","2");
					}
					else if (a_bomber.Contains(x,y)) {
						print_stat ("1","2","1");
					}
				}
			}
			if (mapedit) {
				m_ = new MapEditor(size_window_[1].x, size_window_[1].y);
				if(m_-> mapGenerator()) {
				}
				mapedit = false;
			}
			if (choosekey) {

				win_->Draw (mask);
				win_->Draw(key);
				display_key(185,175, nbr_player_set, 'u');
				display_key(185,235, nbr_player_set, 'd');
				display_key(115,235, nbr_player_set, 'l');
				display_key(261,235, nbr_player_set, 'r');
				display_key(341,235, nbr_player_set, 'b');
				switch (changeKey) {
					case 'u' : 
						win_->Draw (mask);
						win_->Draw(keyUp);
						display_key(185,175, nbr_player_set, changeKey);
						win_->Draw(next);
					break;
					case 'd' : 
						win_->Draw (mask);
						win_->Draw(keyDown);
						display_key(185,235, nbr_player_set, changeKey);
						win_->Draw(next);
					break;	
					case 'l' : 
						win_->Draw (mask);
						win_->Draw(keyLeft);
						display_key(115,235, nbr_player_set, changeKey);
						win_->Draw(next);
					break;	
					case 'r' : 
						win_->Draw (mask);
						win_->Draw(keyRight);
						display_key(261,235, nbr_player_set, changeKey);
						win_->Draw(next);
					break;	
					case 'b' : 
						win_->Draw (mask);
						win_->Draw(keyBomb);
						display_key(341,235, nbr_player_set, changeKey);
						win_->Draw(next);
					break;	
					default : break;
				}
			}
			if (choosemap) {
				win_->Draw(mask);
				win_->Draw(choixmap);
				win_->Draw(map1);
				win_->Draw(map2);
				win_->Draw(map3);
			
			}											
			
		}
		

		win_->Display();
	}
return true;
}


bool Bomberman::run() {

	music_menu_ -> Stop();
	
	if (music_) {
		music_game_ -> Play();
	}
	// Dimensionnement de la fenêtre
	win_->Create(sf::VideoMode(size_window_[2].x, size_window_[2].y),"Super BomberPeggy Deluxe");
	win_->SetSize(size_window_[(int)size_win_].x, size_window_[(int)size_win_].y);

	sf::Shape mask = sf::Shape::Rectangle (0,0, size_window_[2].x,size_window_[2].y, sf::Color(0,0,0,200));
	sf::Shape mask_timer = sf::Shape::Rectangle (23,483, 123,519, sf::Color(0,0,0,150),2,sf::Color(0,0,0,220) );
	vector<Player*>::iterator ite_P;
	int id,x,y;
	bool bombt = false;
	bool hiroshima = false;
	bool play = true;
	paused_ = false;

	list<Bomb*>::iterator ite_Bomb; 
	current_background_->SetImage(*background_);
	current_background_->SetSubRect(sf::IntRect(0,0,size_window_[2].x, size_window_[2].y));
	
	sf::String str_time, str_victory, str_continue;
	str_time.SetPosition (30,480);
	str_time.SetSize (34);
	str_time.SetColor(sf::Color(255,255,255,200));
	str_time.SetFont(*groBold_);
	str_victory.SetPosition (200,550);
	str_victory.SetSize (34);
	str_victory.SetColor(sf::Color(255,255,255,200));
	str_victory.SetFont(*groBold_);
	str_continue.SetPosition(500,650);
	str_continue.SetSize (26);
	str_continue.SetColor(sf::Color(255,255,255,200));
	str_continue.SetFont(*groBold_);
	str_continue.SetText (" Click anywhere to continue ...");
	
	victory_->SetImage(*spriteset_menu_);
	victory_-> SetSubRect (sf::IntRect (0, 23*TILE_Size, 6*TILE_Size, 28*TILE_Size));
	victory_-> SetPosition (350, 150);
		
	//Gestion d'évènements
	const sf::Input& Input = win_->GetInput();
	sf::Event event;
	
	// Initialisation zone cliquables
	sf::IntRect a_music, a_sound, a_quit, a_pause;
	float a = 1;
	a_music = sf::IntRect (a*4,a*528,a*54,a*574);
	a_sound = sf::IntRect (a*58,a*528,a*108,a*574);
	a_quit = sf::IntRect (a*113,a*528,a*163,a*574);
	a_pause = sf::IntRect (a*67,a*615, a*115,a*660);

	sf::Sprite Cross, Play;
	Cross.SetImage(*spriteset_);
	Cross.SetSubRect(sf::IntRect(0, TILE_Size*5, TILE_Size, TILE_Size*6));
	Play.SetImage(*spriteset_);
	Play.SetSubRect(sf::IntRect(TILE_Size, TILE_Size*5, TILE_Size*2, TILE_Size*6));
	Play.SetPosition(57,603);
	sf::Vector2<float> pointinview;

	timer_->start();
	while (win_->IsOpened()) { // Tant que la fenêtre de jeu est ouverte
		
			if (timer_->getElapsedTime() > GAME_TIME - HIROSHIMA) {
				hiroshima = true;
				str_time.SetColor(sf::Color(255,10,10,200));
			}
			ite_P = tablePlayers_.begin();	
			while (win_ -> GetEvent(event)) {	// On prend les évênements
				switch (event.Type) {

					case sf::Event::Closed : // Fermeture de la fenêtre.
						win_->Close();
					break;
					case sf::Event::MouseButtonPressed : // L'utilisateur a cliqué
						pointinview =  win_->ConvertCoords(event.MouseButton.X, event.MouseButton.Y);
						x = pointinview.x;
						y = pointinview.y;
						if (!paused_) {
							if (a_sound.Contains(x,y)) {
								sound_ = !sound_;
								if (sound_) {
									if (!listBomb_->empty()) {
										for (ite_Bomb = listBomb_->begin(); ite_Bomb != listBomb_->end(); ++ite_Bomb) {
											(*ite_Bomb)->demute();
										}
									}
								}
								else {
									if (!listBomb_->empty()) {
									for (ite_Bomb = listBomb_->begin(); ite_Bomb != listBomb_->end(); ++ite_Bomb) {
										(*ite_Bomb)->mute();
									}
									}
								}
							}
							else if (a_music.Contains(x,y)) {
								music_ = !music_;
								if (music_){
									music_game_->Play();
								}
								else {
									music_game_->Pause();
								}
							}
							else if (a_quit.Contains(x,y)) {
								return true;
							}
							else if (!play) {
								return true;	
							}
						}
						if (a_pause.Contains(x,y)) {
							paused_ = !paused_;
							if (paused_) {
							// On met les sons de bombes en pause, la musique ainsi que les animations
								timer_->stop();
								if (!listBomb_->empty()) {
									for (ite_Bomb = listBomb_->begin(); ite_Bomb != listBomb_->end(); ++ite_Bomb) {
									(*ite_Bomb)->stopsound();
									}
								}
								music_game_->Pause();
							}
							else {
								timer_->start();
								if (!listBomb_->empty()) {
									for (ite_Bomb = listBomb_->begin(); ite_Bomb != listBomb_->end(); ++ite_Bomb) {
									(*ite_Bomb)->playsound();
									}
								}
								music_game_->Play();
							}	
						}
					break;
				
					default : break;
				}
			}
		win_->Clear();
		win_->Draw(*current_background_);
		map_->display();
		if (!paused_ && play) {
			if (!tablePlayers_.empty()) {
				for(auto ite_P = tablePlayers_.begin(); ite_P != tablePlayers_.end(); ite_P ++){
				
					id =ite_P-tablePlayers_.begin();
					tablePlayers_[id]->getBonus();
					// Posons une bombe ?
					if (Input.IsKeyDown(tablePlayers_[id]->getKeyBomb())) {
						if (tablePlayers_[id]->getBombCount()>0) {
							if (tablePlayers_[id]->getBombTime() > 0.3) {					
								listBomb_->push_front(new Bomb(win_, spriteset_, tablePlayers_, map_,sound_, id));
								tablePlayers_[id]->subBombtoCount();
								tablePlayers_[id]->resetBombTime();
							}
						}				
					}
					// Allons à droite ?
					if (Input.IsKeyDown(tablePlayers_[id]->getKeyRight())) {
						tablePlayers_[id]->moveRight();
					}
					else {	
						// Allons à gauche ?
						if (Input.IsKeyDown(tablePlayers_[id]->getKeyLeft())) {
							tablePlayers_[id]->moveLeft();
						}
						else {
							//Allons en haut ?
							if (Input.IsKeyDown(tablePlayers_[id]->getKeyUp())) {
								tablePlayers_[id]->moveUp();
							}
							else {
								// Allons en bas ?
								if (Input.IsKeyDown(tablePlayers_[id]->getKeyDown())) {
									tablePlayers_[id]->moveDown();
								}
								// Ok, on bouge pas.
								else tablePlayers_[id]->setDefault();
							}
						}
					}
					if (tablePlayers_[id]->getLife()>0) {
						tablePlayers_[id]->display();
						tablePlayers_[id]->display_HUD();
					} else {
					tablePlayers_.erase (ite_P);
					break;	
					}
				}
			} 

			if (!listBomb_->empty()) {
				for (ite_Bomb = listBomb_->begin(); ite_Bomb != listBomb_->end(); ++ite_Bomb) {		
						bombt = bombt || (*ite_Bomb)->update();
						(*ite_Bomb)->display();
				}

				if (bombt) {
				ite_Bomb--;
				delete (*ite_Bomb);
				listBomb_->pop_back();
				bombt = false;		
				}		
			}	
			if (hiroshima) {
				int proba = sf::Randomizer::Random (0,(int)1/win_->GetFrameTime() / 1.5);
				if (proba == 0) {
					int probx = sf::Randomizer::Random (0,9);
					int proby = sf::Randomizer::Random (0,9);
					if (map_-> getTile(probx,proby) == 'V') {
						listBomb_->push_front(new Bomb(win_, spriteset_, tablePlayers_, map_,sound_, probx , proby));
					}		
				}
			}
		}else {
			if (!listBomb_->empty()) {
				for (ite_Bomb = listBomb_->begin(); ite_Bomb != listBomb_->end(); ++ite_Bomb) {		
						(*ite_Bomb)->display();
				}
			}
			if (!tablePlayers_.empty()) {
				for(auto ite_P = tablePlayers_.begin(); ite_P != tablePlayers_.end(); ite_P ++){
					id =ite_P-tablePlayers_.begin();
					tablePlayers_[id]->display();
					tablePlayers_[id]->display_HUD();
				}
			}
		}
		if ((timer_->getElapsedTime() >= GAME_TIME) || (tablePlayers_.size() < 2)) {
			play = false;
			timer_->stop();
		}
	 
	
	// Affichage Param
	str_time.SetText(getStringTimer());
	win_->Draw(mask_timer);
	win_->Draw(str_time);
	
	if (!play) {	// La partie est terminée.
		win_->Draw(mask);
		win_->Draw(*victory_);
		if (tablePlayers_.size() == 1) { // Nous avons un vainqueur.
			headvictory_ = tablePlayers_[0]->getHead();
			headvictory_.SetPosition (485,140);
			win_->Draw(headvictory_);
			str_victory.SetText(tablePlayers_[0]->getName()+ ", you have the PEGGY in you !");
			win_->Draw(str_victory);
			win_->Draw(str_continue);
		}
		else {
			str_victory.SetText("It's a tie, YOU all SUCK ! ");
			str_victory.SetPosition(330,550);
			str_victory.SetColor(sf::Color(255,10,10,200));
			win_->Draw(str_victory);
			win_->Draw(str_continue);
		}
		
	
	}
	
	if (!music_) {
		Cross.SetPosition(4,528);
		win_->Draw(Cross);
	}	
	if (!sound_) {
		Cross.SetPosition(58,528);
		win_->Draw(Cross);
	}	
	if (paused_) {
		win_->Draw(mask);
		win_->Draw(Play);
		
	}
	win_->Display();
	}
	if (!win_->IsOpened()) return false;
	else return true;
}

bool Bomberman::isSound() const {
	return sound_;
}

string Bomberman::getStringTimer() {
	int time = GAME_TIME - timer_->getElapsedTime() ;
	int minute = time / 60;
	int second = time % 60;
	if (second>9)
	return (inttoString(minute)+" : "+inttoString(second));
	else return (inttoString(minute)+" : 0"+inttoString(second));
}

string Bomberman::keyCode (sf::Key::Code c) const {
	switch(c) {
		case sf::Key::A :
			return "A";
		break;
		case sf::Key::B :
			return "B";
		break;
		case sf::Key::C :
			return "C";
		break;
		case sf::Key::D :
			return "D";
		break;
		case sf::Key::E :
			return "E";
		break;
		case sf::Key::F :
			return "F";
		break;
		case sf::Key::G :
			return "G";
		break;
		case sf::Key::H :
			return "H";
		break;
		case sf::Key::I :
			return "I";
		break;
		case sf::Key::J :
			return "J";
		break;
		case sf::Key::K :
			return "K";
		break;
		case sf::Key::L :
			return "L";
		break;
		case sf::Key::M :
			return "M";
		break;
		case sf::Key::N :
			return "N";
		break;
		case sf::Key::O :
			return "O";
		break;
		case sf::Key::P :
			return "P";
		break;
		case sf::Key::Q :
			return "Q";
		break;
		case sf::Key::R :
			return "R";
		break;
		case sf::Key::S :
			return "S";
		break;
		case sf::Key::T :
			return "T";
		break;
		case sf::Key::U :
			return "U";
		break;
		case sf::Key::V :
			return "V";
		break;
		case sf::Key::W :
			return "W";
		break;
		case sf::Key::X :
			return "X";
		break;
		case sf::Key::Y :
			return "Y";
		break;
		case sf::Key::Z :
			return "Z";
		break;
		case sf::Key::Num0 :
			return "0";
		break;
		case sf::Key::Num1 :
			return "1";
		break;
		case sf::Key::Num2 :
			return "2";
		break;
		case sf::Key::Num3 :
			return "3";
		break;
		case sf::Key::Num4 :
			return "4";
		break;
		case sf::Key::Num5 :
			return "5";
		break;
		case sf::Key::Num6 :
			return "6";
		break;
		case sf::Key::Num7 :
			return "7";
		break;
		case sf::Key::Num8 :
			return "8";
		break;
		case sf::Key::Num9 :
			return "9";
		break;
		case sf::Key::Escape :
			return "Escape";
		break;
		case sf::Key::LControl :
			return "LCtrl";
		break;
		case sf::Key::LShift :
			return "LShift";
		break;
		case sf::Key::LAlt :
			return "LAlt";
		break;
		case sf::Key::LSystem :
			return "LSystem";
		break;
		case sf::Key::RControl :
			return "RCtrl";
		break;
		case sf::Key::RShift :
			return "RShift";
		break;
		case sf::Key::RAlt :
			return "RAlt";
		break;
		case sf::Key::RSystem :
			return "RSystem";
		break;
		case sf::Key::Menu :
			return "Menu";
		break;
		case sf::Key::LBracket :
			return "[";
		break;
		case sf::Key::RBracket :
			return "]";
		break;
		case sf::Key::SemiColon :
			return ";";
		break;
		case sf::Key::Comma :
			return ",";
		break;
		case sf::Key::Period :
			return "Period";
		break;
		case sf::Key::Quote :
			return "\'";
		break;
		case sf::Key::Slash :
			return "/";
		break;
		case sf::Key::BackSlash :
			return "\\";
		break;
		case sf::Key::Tilde:
			return "~";
		break;
		case sf::Key::Equal :
			return "=";
		break;
		case sf::Key::Dash :
			return "-";
		break;
		case sf::Key::Space :
			return "Space";
		break;
		case sf::Key::Return :
			return "Return";
		break;
		case sf::Key::Back :
			return "Back";
		break;
		case sf::Key::Tab :
			return "Tab";
		break;
		case sf::Key::PageUp :
			return "PageUp";
		break;
		case sf::Key::PageDown :
			return "PageDown";
		break;
		case sf::Key::End :
			return "End";
		break;
		case sf::Key::Home :
			return "Home";
		break;
		case sf::Key::Insert :
			return "Insert";
		break;
		case sf::Key::Delete :
			return "Delete";
		break;
		case sf::Key::Add :
			return "+";
		break;
		case sf::Key::Subtract :
			return "-";
		break;
		case sf::Key::Multiply :
			return "*";
		break;
		case sf::Key::Divide :
			return "/";
		break;
		case sf::Key::Left :
			return "LArrow";
		break;
		case sf::Key::Right :
			return "RArrow";
		break;
		case sf::Key::Up :
			return "UArrow";
		break;
		case sf::Key::Down :
			return "DArrow";
		break;
		case sf::Key::Numpad0 :
			return "0";
		break;
		case sf::Key::Numpad1 :
			return "1";
		break;
		case sf::Key::Numpad2 :
			return "2";
		break;
		case sf::Key::Numpad3 :
			return "3";
		break;
		case sf::Key::Numpad4 :
			return "4";
		break;
		case sf::Key::Numpad5 :
			return "5";
		break;
		case sf::Key::Numpad6 :
			return "6";
		break;
		case sf::Key::Numpad7 :
			return "7";
		break;
		case sf::Key::Numpad8 :
			return "8";
		break;
		case sf::Key::Numpad9 :
			return "9";
		break;
		case sf::Key::F1 :
			return "F1";
		break;
		case sf::Key::F2 :
			return "F2";
		break;
		case sf::Key::F3 :
			return "F3";
		break;
		case sf::Key::F4 :
			return "F4";
		break;
		case sf::Key::F5 :
			return "F5";
		break;
		case sf::Key::F6 :
			return "F6";
		break;
		case sf::Key::F7 :
			return "F7";
		break;
		case sf::Key::F8 :
			return "F8";
		break;
		case sf::Key::F9 :
			return "F9";
		break;
		case sf::Key::F10 :
			return "F10";
		break;
		case sf::Key::F11 :
			return "F11";
		break;
		case sf::Key::F12 :
			return "F12";
		break;
		case sf::Key::F13 :
			return "F13";
		break;
		case sf::Key::F14 :
			return "F14";
		break;
		case sf::Key::F15 :
			return "F15";
		break;
		default : break;
	}
	return "Touche inconnue";
}


void Bomberman::display_key (int x, int y, int id, char key_type) {
	sf::Color c (0,0,0,255);
	sf::String *s;
	switch (key_type) {
		case 'u' :
			s = new sf::String (keyCode(tablePlayers_[id] -> getKeyUp()));
		break;
		case 'd' :
			s = new sf::String (keyCode(tablePlayers_[id] -> getKeyDown()));
		break;
		case 'l' :
			s = new sf::String (keyCode(tablePlayers_[id] -> getKeyLeft()));
		break;
		case 'r' :
			s = new sf::String (keyCode(tablePlayers_[id] -> getKeyRight()));
		break;
		case 'b' :
			s = new sf::String (keyCode(tablePlayers_[id] -> getKeyBomb()));
		break;
		default : break;
	}
	s->SetPosition(x,y);
	s->SetSize(12);
	s->SetColor(c);
	win_->Draw(*s);
	delete s;
}		

void Bomberman::print_stat(string speed, string bomb, string power) {
	sf::Color c (255,255,255,200);
	sf::String s(speed); 
	sf::String b(bomb); 
	sf::String p(power);
	s.SetPosition (183,280);
	s.SetSize (24);
	s.SetColor(c);
	b.SetPosition (290,280);
	b.SetSize (24);
	b.SetColor(c);
	p.SetPosition (405,280);
	p.SetSize (24);
	p.SetColor(c);
	win_->Draw(s);
	win_->Draw(b);
	win_->Draw(p);
}


string Bomberman::inttoString(int n) {
	std::ostringstream oss;
	oss << n;
	return oss.str();
}

list<string> Bomberman::listMapFile() {

	
	list<string> listmap;
	string current;
	DIR * rep = opendir("./data/maps");
	if (rep == NULL) {
		cerr <<"Impossible d'ouvrir le repertoire map !" << endl;	
	}
	else {
		struct dirent * ent;
		while ((ent = readdir(rep)) != NULL) {
			current = ent->d_name;
			if (current.find(".txt") != string::npos) {
			listmap.push_back(current); 
			}
		}	
		closedir(rep);
	}
	return listmap;
}

