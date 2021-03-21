#include "Platformer_DynamicProjectileBoomerang.h"

cDynamicProjectileBoomerang::cDynamicProjectileBoomerang(float ox, float oy, bool bFriend, float velx, float vely, float duration, std::vector<olc::Sprite*> map, int damage, bool solidVSMap, bool oneHit, int corner) : cDynamicProjectile(ox, oy, bFriend, velx, vely, duration, map, false, damage, solidVSMap, oneHit, corner)
{
	fInitPosX = ox;
	fInitPosY = oy;
	fInitVelX = velx;
	fInitVelY = vely;
	fTimer = 0.0f;
}

cDynamicProjectileBoomerang::~cDynamicProjectileBoomerang()
{
	delete hitbox;
}

void cDynamicProjectileBoomerang::UpdateTrajectory(float fElapsedTime)
{
	vx += (fInitPosX - px) * cfReturnSpeed * fElapsedTime;
	vy += (fInitPosY - py) * cfReturnSpeed * fElapsedTime;

	float fTargetX = fInitPosX - px;
	float fTargetY = fInitPosY - py;
	float fDistance = sqrtf(fTargetX * fTargetX + fTargetY * fTargetY);

	fTimer += fElapsedTime;

	if (fDistance <= cfMinReturnDistance && fTimer >= cfMinDurationTime) bRedundant = true;
}
