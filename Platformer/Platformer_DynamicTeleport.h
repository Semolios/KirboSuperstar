#ifndef DEF_TELEPORT
#define DEF_TELEPORT

#pragma once

#include "Platformer_Dynamic.h"

class OneLoneCoder_Platformer;

class cDynamicTeleport : public cDynamic
{
public:
	cDynamicTeleport(float ax, float ay, float bx, float by, std::vector<olc::Sprite*> map);
	virtual ~cDynamicTeleport();

protected:
	std::vector<olc::Sprite*> mapStates;

	// animation
	int nCurrentFrame = 0;
	float fTimeBetweenFrames = 0.05f;
	float fTimeCounter = 0.0f;

	float fDestX;
	float fDestY;

	cHitbox* destHitbox;

public:
	static OneLoneCoder_Platformer* engine;

	void DrawSelf(float ox, float oy) override;
	void DrawDest(float ox, float oy);
	void Update(float fElapsedTime, float playerX, float playerY) override;
	void UpdateHitbox(float cameraOffsetX, float cameraOffsetY) override;
	void UpdateDestHitbox(float cameraOffsetX, float cameraOffsetY);

	float GetNormalizedWidth();
	float GetNormalizedHeight();

	float GetDestX();
	float GetDestY();

	cHitbox* GetDestHitbox();

	static std::map<std::string, std::vector<olc::Sprite*>> LoadTeleportsSprites();
};

#endif // !DEF_TELEPORT
