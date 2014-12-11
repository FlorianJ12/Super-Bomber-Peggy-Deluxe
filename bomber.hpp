#ifndef BOMBER_HPP
#define BOMBER_HPP

class Bomber : public Player {
public :
	Bomber(sf::RenderWindow *win, sf::Vector2<float> pos, sf::Image *spriteset, Map *map, vector<sf::Key::Code> keySet, string name);
	~Bomber();
};



#endif // BOMBER_HPP
