#ifndef DEF_BOOMERANG
#define DEF_BOOMERANG

#pragma once

#include "Platformer_DynamicProjectile.h"

class cDynamicProjectileBoomerang : public cDynamicProjectile
{
public:
	cDynamicProjectileBoomerang(float ox, float oy, bool bFriend, float velx, float vely, float duration, std::vector<olc::Sprite*> map, int damage, bool solidVSMap, bool oneHit, int corner, bool scenery);
	~cDynamicProjectileBoomerang();

private:
	const float cfMinReturnDistance = 0.1f;
	const float cfMinDurationTime = 0.1f;
	const float cfReturnSpeed = 10.0f;
	float fInitPosX;
	float fInitPosY;
	float fInitVelX;
	float fInitVelY;
	float fTimer;

public:
	void UpdateTrajectory(float fElapsedTime) override;
};

#endif // !DEF_BOOMERANG
