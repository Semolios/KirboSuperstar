#ifndef DEF_WIND
#define DEF_WIND

#pragma once

#include "Platformer_Dynamic.h"

class OneLoneCoder_Platformer;

class cDynamicWind : public cDynamic
{
public:
	cDynamicWind(float ox, float oy, std::vector<olc::Decal*> map, std::wstring direction, float power);
	~cDynamicWind();

	static OneLoneCoder_Platformer* engine;

	void DrawSelf(float ox, float oy) override;
	void Update(float fElapsedTime, float playerX, float playerY) override;
	void UpdateHitbox(float cameraOffsetX, float cameraOffsetY) override;
	std::string GetDirection();
	float GetPower();
	olc::Sprite* GetCurrentSprite();
	static std::map<std::string, std::vector<olc::Sprite*>> LoadWindSprites();
	static std::map<std::string, std::vector<olc::Decal*>>  LoadWindDecals(std::map<std::string, std::vector<olc::Sprite*>> mapWinds);

protected:
	std::vector<olc::Decal*> mapStates;

	// animation
	int nCurrentFrame = 0;
	float fTimeBetweenFrames = 0.033f;
	float fTimeCounter = 0.0f;

	float fPower;
	std::string sDirection;
};

#endif // !DEF_WIND
