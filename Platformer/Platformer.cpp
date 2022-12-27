#include "Platformer_Engine.h"

std::wstring getSavedResolution()
{
	char username[UNLEN + 1];
	DWORD username_len = UNLEN + 1;
	GetUserNameA(username, &username_len);

	std::string un = username;

	std::wifstream file("C:/Users/" + un + "/AppData/Roaming/Kirbo Superstar/screenMode.txt");

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
