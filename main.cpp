#include "bomberman.hpp"

int main(int argc, char *argv[])
{
	bool play = true;
	while (play) {
		Bomberman *bb = new Bomberman;
		if(bb-> start()) play = bb-> run();
		else play = false;
		delete bb;
	}
	

	return 0;

}
