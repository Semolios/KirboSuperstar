#include "Platformer_ScreenModeMenu.h"
#include "Platformer_Engine.h"

OneLoneCoder_Platformer* cScreenModeMenu::engine = nullptr;

cScreenModeMenu::cScreenModeMenu(olc::PixelGameEngine* gfx, olc::Sprite* screenModeMenu, olc::Sprite* rightArrow, olc::Sprite* leftArrow)
{
	sprScreenModeMenu = screenModeMenu;
	sprLeftArrow = leftArrow;
	sprRightArrow = rightArrow;

	char username[UNLEN + 1];
	DWORD username_len = UNLEN + 1;
	GetUserNameA(username, &username_len);

	std::string un = username;

	std::wifstream file("C:/Users/" + un + "/AppData/Roaming/Kirbo Superstar/screenMode.txt");

	if (file)
	{
		std::wstring line;
		while (std::getline(file, line))
		{
			if (line == L"windowed")
				bWindowed = true;
			else
				bWindowed = false;
		}
	}
}

bool cScreenModeMenu::Update(olc::PixelGameEngine* gfx, float fElapsedTime, ControllerManager* controller)
{
	gfx->SetPixelMode(olc::Pixel::ALPHA);

	gfx->DrawSprite(0, 0, sprScreenModeMenu);

	if (gfx->GetKey(olc::LEFT).bPressed  || controller->GetButton(LEFT).bPressed ||
		gfx->GetKey(olc::RIGHT).bPressed || controller->GetButton(RIGHT).bPressed)
	{
		bWindowed = !bWindowed;
		engine->PlaySample("menuBip");
		UpdateScreenMode();
	}

	if (!bWindowed) 
		engine->DrawKirboString(cnScreenModeTextX, cnScreenModeTextY, "Screen mode  Fullscreen", 2);
	else
		engine->DrawKirboString(cnScreenModeTextX, cnScreenModeTextY, "Screen mode  Windowed", 2);

	engine->DrawSprite(cnLeftArrowX, cnScreenModeTextY, sprLeftArrow);
	engine->DrawSprite(cnRightArrowX, cnScreenModeTextY, sprRightArrow);

	engine->DrawKirboString(cnTextX, cnTextY1, "You need to restart", 1, true);
	engine->DrawKirboString(cnTextX, cnTextY2, "the game to apply",   1, true);
	engine->DrawKirboString(cnTextX, cnTextY3, "the new resolution",  1, true);

	gfx->SetPixelMode(olc::Pixel::NORMAL);

	return true;
}

void cScreenModeMenu::UpdateScreenMode()
{
	std::ofstream mode;

	char username[UNLEN + 1];
	DWORD username_len = UNLEN + 1;
	GetUserNameA(username, &username_len);

	std::string un = username;

	mode.open("C:/Users/" + un + "/AppData/Roaming/Kirbo Superstar/screenMode.txt", std::ofstream::trunc);
	
	if (bWindowed)
		mode << "windowed";
	else
		mode << "fullscreen";
	
	mode.close();
}
