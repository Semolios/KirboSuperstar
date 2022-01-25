#ifndef DEF_WIND
#define DEF_WIND

#pragma once

#include "Platformer_Dynamic.h"

class OneLoneCoder_Platformer;

class cDynamicWind : public cDynamic
{
public:
	cDynamicWind(float ox, float oy, std::vector<olc::Sprite*> map, std::wstring direction, float power);
	~cDynamicWind();

protected:
	std::vector<olc::Sprite*> mapStates;

	// animation
	int nCurrentFrame = 0;
	float fTimeBetweenFrames = 0.05f;
	float fTimeCounter = 0.0f;

	float fPower;
	std::string sDirection;

public:
	static OneLoneCoder_Platformer* engine;

	void DrawSelf(float ox, float oy) override;
	void Update(float fElapsedTime, float playerX, float playerY) override;
	void UpdateHitbox(float cameraOffsetX, float cameraOffsetY) override;

	std::string GetDirection();
	float GetPower();

	static std::map<std::string, std::vector<olc::Sprite*>> LoadWindSprites();
};

#endif // !DEF_WIND
