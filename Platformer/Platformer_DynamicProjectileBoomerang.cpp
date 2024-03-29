#include "Platformer_DynamicProjectileBoomerang.h"

cDynamicProjectileBoomerang::cDynamicProjectileBoomerang(float ox, float oy, bool bFriend, float velx, float vely, float duration, std::vector<olc::Decal*> map, int damage, bool solidVSMap, bool oneHit, int corner, bool scenery, std::string spriteName) 
	: cDynamicProjectile(ox, oy, bFriend, velx, vely, duration, map, false, damage, solidVSMap, oneHit, spriteName, corner, true, 0.0f, false, "", scenery)
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
