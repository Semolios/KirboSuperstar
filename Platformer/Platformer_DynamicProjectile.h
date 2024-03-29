#ifndef DEF_PROJECTILE
#define DEF_PROJECTILE

#pragma once

#include "Platformer_Dynamic.h"
#include "Platformer_Level.h"

class OneLoneCoder_Platformer;

class cDynamicProjectile : public cDynamic
{
public:
	cDynamicProjectile(float ox, float oy, bool bFriend, float velx, float vely, float duration, std::vector<olc::Decal*> map, bool affectedByGravity, int damage, bool solidVSMap, bool oneHit, std::string spriteName, int corner = 0, bool breackableAgainstTiles = true, float drag = -3.0f, bool bouncy = false, std::string bounceSound = "", bool scenery = false);
	virtual ~cDynamicProjectile();

	static OneLoneCoder_Platformer* engine;

	void DrawSelf(float ox, float oy) override;
	void Update(float fElapsedTime, float playerX, float playerY) override;
	void Collision(float fElapsedTime, cLevel* level);
	void SideCollision();
	void LeftCollision(cLevel* level, float fBorder);
	void RightCollision(cLevel* level, float fBorder);
	void TopCollision(cLevel* level, float fBorder);
	void BottomCollision(cLevel* level, float fBorder, float fElapsedTime);
	void BottomCollisionBehaviour(float fElapsedTime, float grdSpeed);
	void CheckBreakableBlocks(cLevel* level, float fBorder);
	void BreakTile(cLevel* level, float tileX, float tileY);
	void ChangeTile(cLevel* level, float tileX, float tileY, wchar_t t);
	void UpdateHitbox(float cameraOffsetX, float cameraOffsetY) override;
	void PlaySoundEffect();
	void SpawnEffect(float posX, float posY);
	static std::map<std::string, std::vector<olc::Sprite*>> LoadProjectilesSprites();
	static std::map<std::string, std::vector<olc::Decal*>>  LoadProjectilesDecals(std::map<std::string, std::vector<olc::Sprite*>> mapProjectiles);
	bool IsOneHit();
	void SetOneHit(bool oneHit);
	std::string GetSoundEffect();
	void SetSoundEffect(std::string sndEffect);
	void SetEffect(std::string eff, float duration);
	bool IsRedundant();
	void SetRedundant(bool redundant);
	bool BreaksAgainstTiles();
	void SetBreakableAgainstTiles(bool breakable);
	void SetDrag(float drag);
	bool IsScenery();
	float GetNormalizedW();
	float GetNormalizedH();
	void SetPickable(bool pickable);
	bool IsPickable();

	virtual void UpdateTrajectory(float fElapsedTime);

protected:
	const float cfMinGlideVX = 0.1;

	std::vector<olc::Decal*> mapStates;
	std::vector<olc::vf2d> crossedTiles;
	std::string projectileName;
	std::string soundEffect;
	std::string effect;
	std::string bounceSoundEffect;
	float fDuration;
	float fDrag;
	float fEffectDuration;
	float fNewObjectPosX;
	float fNewObjectPosY;
	bool bOneHit;
	bool bRedundant;
	bool bBreaksAgainstTiles;
	bool bBouncy;
	bool bScenery;
	bool bPickable;

	// animation
	int nCurrentFrame = 0;
	float fTimeBetweenFrames = 0.033f;
	float fTimeCounter = 0.0f;
};

#endif // !DEF_PROJECTILE
