#include "Platformer_DynamicProjectileOrbital.h"

cDynamicProjectileOrbital::cDynamicProjectileOrbital(float ox, float oy, bool bFriend, float duration, std::vector<olc::Sprite*> map, int damage, bool solidVSMap, bool oneHit, int corner, float cx, float cy, float angrot) : cDynamicProjectile(ox, oy, bFriend, 0.0f, 0.0f, duration, map, false, damage, solidVSMap, oneHit, corner)
{
	fCenterX = cx;
	fCenterY = cy;
	fAngularRotation = angrot;

	fInitAngle = acosf((ox - cx) / sqrtf((ox - cx) * (ox - cx) + (oy - cy) * (oy - cy)));

	if (asinf((oy - cy) / sqrtf((ox - cx) * (ox - cx) + (oy - cy) * (oy - cy))) < 0)
		fInitAngle = -fInitAngle;

	fBehaviourTimer = 0.0f;
}

cDynamicProjectileOrbital::~cDynamicProjectileOrbital()
{
	delete hitbox;
}

void cDynamicProjectileOrbital::UpdateTrajectory(float fElapsedTime)
{
	fBehaviourTimer += fElapsedTime;

	float r = sqrtf((px - fCenterX) * (px - fCenterX) + (py - fCenterY) * (py - fCenterY));
	vx = -r * fAngularRotation * sinf(fAngularRotation * fBehaviourTimer + fInitAngle);
	vy = +r * fAngularRotation * cosf(fAngularRotation * fBehaviourTimer + fInitAngle);
}
