#ifndef STRONG_HPP
#define STRONG_HPP

class Strong : public Player {
public :
	Strong(sf::RenderWindow *win, sf::Vector2<float> pos, sf::Image *spriteset, Map *map, vector<sf::Key::Code> keySet, string name);
	~Strong();
};



#endif // STRONG_HPP
