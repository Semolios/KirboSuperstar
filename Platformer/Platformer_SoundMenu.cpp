#include "Platformer_SoundMenu.h"
#include "Platformer_Engine.h"

OneLoneCoder_Platformer* cSoundMenu::engine = nullptr;

cSoundMenu::cSoundMenu(olc::PixelGameEngine* gfx,
					   olc::Sprite* soundMenuNoSound,
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

	nSoundVolume = 9;
}

bool cSoundMenu::Update(olc::PixelGameEngine* gfx, float fElapsedTime, ControllerManager* controller)
{
	gfx->SetPixelMode(olc::Pixel::ALPHA);

	if (nSoundVolume == 0)
		gfx->DrawSprite(0, 0, sprSoundMenuNoSound);
	else if (nSoundVolume >= 1 && nSoundVolume <= 3)
		gfx->DrawSprite(0, 0, sprSoundMenuLow);
	else if (nSoundVolume >= 4 && nSoundVolume <= 6)
		gfx->DrawSprite(0, 0, sprSoundMenuMid);
	else if (nSoundVolume >= 7 && nSoundVolume <= 9)
		gfx->DrawSprite(0, 0, sprSoundMenuHigh);
	else if (nSoundVolume == 10)
		gfx->DrawSprite(0, 0, sprSoundMenuMax);

	if ((gfx->GetKey(olc::LEFT).bPressed || controller->GetButton(LEFT).bPressed) && nSoundVolume > 0)
	{
		nSoundVolume--;
		engine->UpdateVolume(nSoundVolume);
		engine->PlaySample("menuBip");
	}

	if ((gfx->GetKey(olc::RIGHT).bPressed || controller->GetButton(RIGHT).bPressed) && nSoundVolume < 10)
	{
		nSoundVolume++;
		engine->UpdateVolume(nSoundVolume);
		engine->PlaySample("menuBip");
	}

	engine->DrawKirboString(cnVolumeTextX, cnVolumeTextY, "Volume  " + std::to_string(nSoundVolume * 10) + "%", 2);

	engine->DrawSprite(cnLeftArrowX, cnVolumeTextY, sprSoundMenuLeftArrow);
	engine->DrawSprite(cnRightArrowX, cnVolumeTextY, sprSoundMenuRightArrow);

	gfx->SetPixelMode(olc::Pixel::NORMAL);

	return true;
}

int cSoundMenu::GetSoundVolume()
{
	return nSoundVolume;
}
