#ifndef DEF_ORBITAL
#define DEF_ORBITAL

#pragma once

#include "Platformer_DynamicProjectile.h"

class cDynamicProjectileOrbital : public cDynamicProjectile
{
public:
	cDynamicProjectileOrbital(float ox, float oy, bool bFriend, float duration, std::vector<olc::Sprite*> map, int damage, bool solidVSMap, bool oneHit, int corner, float cx, float cy, float angrot, bool scenery, std::string spriteName);
	~cDynamicProjectileOrbital();

	void UpdateTrajectory(float fElapsedTime) override;

private:
	float fCenterX = 0.0f;
	float fCenterY = 0.0f;
	float fAngularRotation = 0.0f;
	float fInitAngle = 0.0f;
	float fBehaviourTimer = 0.0f;
};

#endif // !DEF_ORBITAL
