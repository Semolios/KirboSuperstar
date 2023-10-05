#include "Platformer_SoundMenu.h"

cSoundMenu::cSoundMenu(olc::Sprite* soundMenuNoSound,
					   olc::Sprite* soundMenuLow,
					   olc::Sprite* soundMenuMid,
					   olc::Sprite* soundMenuHigh,
					   olc::Sprite* soundMenuMax,
					   olc::Sprite* soundMenuRightArrow,
					   olc::Sprite* soundMenuLeftArrow)
{
	sprSoundMenuNoSound = soundMenuNoSound;
	sprSoundMenuLow = soundMenuLow;
	sprSoundMenuMid = soundMenuMid;
	sprSoundMenuHigh = soundMenuHigh;
	sprSoundMenuMax = soundMenuMax;
	sprSoundMenuRightArrow = soundMenuRightArrow;
	sprSoundMenuLeftArrow = soundMenuLeftArrow;

	std::wifstream file("assets/settings/volume.txt");

	if (file)
	{
		std::wstring line;
		while (std::getline(file, line))
		{
			if (std::stoi(line) < 0)
				nSoundVolume = 0;
			else if (std::stoi(line) > 10)
				nSoundVolume = 10;
			else
				nSoundVolume = std::stoi(line);
		}
	}
}

bool cSoundMenu::Update(OneLoneCoder_Platformer* engine, float fElapsedTime, ControllerManager* controller)
{
	engine->SetPixelMode(olc::Pixel::ALPHA);

	if (nSoundVolume == 0)
		engine->DrawSprite(0, 0, sprSoundMenuNoSound);
	else if (nSoundVolume >= 1 && nSoundVolume <= 3)
		engine->DrawSprite(0, 0, sprSoundMenuLow);
	else if (nSoundVolume >= 4 && nSoundVolume <= 6)
		engine->DrawSprite(0, 0, sprSoundMenuMid);
	else if (nSoundVolume >= 7 && nSoundVolume <= 9)
		engine->DrawSprite(0, 0, sprSoundMenuHigh);
	else if (nSoundVolume == 10)
		engine->DrawSprite(0, 0, sprSoundMenuMax);

	if ((engine->GetKey(olc::LEFT).bPressed || controller->GetButton(LEFT).bPressed) && nSoundVolume > 0)
	{
		nSoundVolume--;
		engine->UpdateVolume(nSoundVolume);
		engine->PlaySample("menuBip");
		UpdateVolume();
	}

	if ((engine->GetKey(olc::RIGHT).bPressed || controller->GetButton(RIGHT).bPressed) && nSoundVolume < 10)
	{
		nSoundVolume++;
		engine->UpdateVolume(nSoundVolume);
		engine->PlaySample("menuBip");
		UpdateVolume();
	}

	engine->DrawKirboString(cnVolumeTextX, cnVolumeTextY, "Volume  " + std::to_string(nSoundVolume * 10) + "%", 2);

	engine->DrawSprite(cnLeftArrowX, cnVolumeTextY, sprSoundMenuLeftArrow);
	engine->DrawSprite(cnRightArrowX, cnVolumeTextY, sprSoundMenuRightArrow);

	engine->SetPixelMode(olc::Pixel::NORMAL);

	return true;
}

int cSoundMenu::GetSoundVolume()
{
	return nSoundVolume;
}

void cSoundMenu::UpdateVolume()
{
	std::ofstream volumeFile;

	volumeFile.open("assets/settings/volume.txt", std::ofstream::trunc);
	volumeFile << std::to_string(nSoundVolume);
	volumeFile.close();
}
