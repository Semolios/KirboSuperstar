#include "Platformer_Engine.h"

std::wstring getSavedResolution()
{
	std::wifstream file("assets/settings/screenMode.txt");

	std::wstring resolution;

	if (file)
	{
		std::wstring line;
		while (std::getline(file, line))
		{
			resolution = line;
		}
	}

	return resolution;
}

int main()
{
	std::wstring resolution = getSavedResolution();

	bool fullScreen = (resolution == L"fullscreen");
	OneLoneCoder_Platformer game;
	if (game.Construct(800, 480, 1, 1, fullScreen, true, true))
		game.Start();
	return 0;
}
