

#include "game.h"
#include "globals.h"

int main()
{

	Game game;

	game.initGame();

	while (1)
	{
		game.updateGame();
		frames++;
	}

	return 0;
}
