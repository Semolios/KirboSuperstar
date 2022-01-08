#include "Platformer_Dynamic.h"

cDynamic::cDynamic(std::string n)
{
	hitbox = new cHitbox();
}

cDynamic::~cDynamic()
{
	delete hitbox;
}

void cDynamic::SetPosition(float x, float y)
{
	px = x;
	py = y;
}

float cDynamic::GetPX()
{
	return px;
}

float cDynamic::GetPY()
{
	return py;
}

float cDynamic::GetVX()
{
	return vx;
}

float cDynamic::GetVY()
{
	return vy;
}

int cDynamic::GetDamage()
{
	return nDamage;
}

bool cDynamic::IsAttackable()
{
	return bIsAttackable;
}

bool cDynamic::IsFriendly()
{
	return bFriendly;
}

cHitbox* cDynamic::GetHitbox()
{
	return hitbox;
}
