#ifndef DEF_PROJECTILE
#define DEF_PROJECTILE

#pragma once

#include "Platformer_Dynamic.h"
#include "Platformer_Level.h"

class OneLoneCoder_Platformer;

class cDynamicProjectile : public cDynamic
{
public:
	cDynamicProjectile(float ox, float oy, bool bFriend, float velx, float vely, float duration, std::vector<olc::Sprite*> map, bool affectedByGravity, int damage, bool solidVSMap, bool oneHit, int corner = 0, bool breackableAgainstTiles = true, float drag = -3.0f);
	virtual ~cDynamicProjectile();

protected:
	const float cfMinGlideVX = 0.1;

	std::vector<olc::Sprite*> mapStates;
	std::string soundEffect;
	float fSpriteX;
	float fSpriteY;
	float fDuration;
	float fDrag;
	bool bOneHit;
	bool bRedundant;
	bool bBreaksAgainstTiles;

	// animation
	int nCurrentFrame = 0;
	float fTimeBetweenFrames = 0.05f;
	float fTimeCounter = 0.0f;

public:
	static OneLoneCoder_Platformer* engine;

public:
	void DrawSelf(float ox, float oy) override;
	void Update(float fElapsedTime, float playerX, float playerY) override;
	void Collision(float fElapsedTime, cLevel* level);
	void UpdateHitbox(float cameraOffsetX, float cameraOffsetY) override;
	void PlaySoundEffect();
	static std::map<std::string, std::vector<olc::Sprite*>> LoadProjectilesSprites();
	bool IsOneHit();
	void SetOneHit(bool oneHit);
	std::string GetSoundEffect();
	void SetSoundEffect(std::string sndEffect);
	bool IsRedundant();
	void SetRedundant(bool redundant);
	bool BreaksAgainstTiles();
	void SetBreakableAgainstTiles(bool breakable);
	void SetDrag(float drag);

	virtual void UpdateTrajectory(float fElapsedTime);
};

#endif // !DEF_PROJECTILE