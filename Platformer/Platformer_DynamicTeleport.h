#ifndef DEF_TELEPORT
#define DEF_TELEPORT

#pragma once

#include "Platformer_Dynamic.h"

class OneLoneCoder_Platformer;

class cDynamicTeleport : public cDynamic
{
public:
	cDynamicTeleport(float ax, float ay, float bx, float by, std::vector<olc::Decal*> map);
	virtual ~cDynamicTeleport();

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

	olc::Sprite* GetCurrentSprite();

	static std::map<std::string, std::vector<olc::Sprite*>> LoadTeleportsSprites();
	static std::map<std::string, std::vector<olc::Decal*>>  LoadTeleportsDecals(std::map<std::string, std::vector<olc::Sprite*>> mapTeleports);

protected:
	std::vector<olc::Decal*> mapStates;

	// animation
	int nCurrentFrame = 0;
	float fTimeBetweenFrames = 0.033f;
	float fTimeCounter = 0.0f;

	float fDestX;
	float fDestY;

	cHitbox* destHitbox;
};

#endif // !DEF_TELEPORT
