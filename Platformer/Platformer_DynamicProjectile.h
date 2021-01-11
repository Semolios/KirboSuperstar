#pragma once

#include "Platformer_Dynamic.h"

class OneLoneCoder_Platformer;

class cDynamicProjectile : public cDynamic
{
public:
	cDynamicProjectile(float ox, float oy, bool bFriend, float velx, float vely, float duration, std::vector<olc::Sprite*> map, bool affectedByGravity, int damage, bool solidVSMap, bool oneHit, int corner = 0);
	void DrawSelf(olc::PixelGameEngine* gfx, float ox, float oy) override;
	void Update(float fElapsedTime, float playerX, float playerY, olc::PixelGameEngine* gfx) override;
	void Collision(float fElapsedTime);
	cHitbox Hitbox(float cameraOffsetX, float cameraOffsetY) override;

	static std::map<std::string, std::vector<olc::Sprite*>> LoadProjectilesSprites();

public:
	std::vector<olc::Sprite*> mapStates;
	float fSpriteX;
	float fSpriteY;
	float fDuration;
	bool bOneHit;
	bool bAffectedByGravity;

	// animation
	int nCurrentFrame = 0;
	float fTimeBetweenFrames = 0.05f;
	float fTimeCounter = 0.0f;

	static OneLoneCoder_Platformer* engine;
};
