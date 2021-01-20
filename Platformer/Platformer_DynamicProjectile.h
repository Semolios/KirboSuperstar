#pragma once

#include "Platformer_Dynamic.h"
#include "Platformer_Level.h"

class OneLoneCoder_Platformer;

class cDynamicProjectile : public cDynamic
{
public:
	cDynamicProjectile(float ox, float oy, bool bFriend, float velx, float vely, float duration, std::vector<olc::Sprite*> map, bool affectedByGravity, int damage, bool solidVSMap, bool oneHit, int corner = 0);
	void DrawSelf(float ox, float oy) override;
	void Update(float fElapsedTime, float playerX, float playerY) override;
	void Collision(float fElapsedTime, cLevel* level);
	cHitbox Hitbox(float cameraOffsetX, float cameraOffsetY) override;
	void SoundEffect();

	static std::map<std::string, std::vector<olc::Sprite*>> LoadProjectilesSprites();

public:
	std::vector<olc::Sprite*> mapStates;
	std::string soundEffect;
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
