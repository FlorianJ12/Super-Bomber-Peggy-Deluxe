#ifndef FAST_HPP
#define FAST_HPP

class Fast : public Player {
public :
	Fast(sf::RenderWindow *win, sf::Vector2<float> pos, sf::Image *spriteset, Map *map, vector<sf::Key::Code> keySet, string name);
	~Fast();
};



#endif // STRONG_HPP
