#include "Platformer_Engine.h"

int main()
{
	OneLoneCoder_Platformer game;
	if (game.Construct(800, 480, 1, 1, false, true, true))
		game.Start();
	return 0;
}
