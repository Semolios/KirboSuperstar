#include "Platformer_Engine.h"

int main()
{
	OneLoneCoder_Platformer game;
	if (game.Construct(800, 480, 1, 1, true))
		game.Start();
	return 0;
}