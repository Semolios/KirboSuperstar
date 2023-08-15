#include "Platformer_Dynamic.h"

cDynamic::cDynamic(std::string n)
{
	sName = n;
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

void cDynamic::SetVX(float velX)
{
	vx = velX;
}

void cDynamic::SetVY(float velY)
{
	vy = velY;
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

void cDynamic::SetFriendly(bool friendly)
{
	bFriendly = friendly;
}

bool cDynamic::IsReflectible()
{
	return bReflectible;
}

void cDynamic::SetReflectible(bool reflect)
{
	bReflectible = reflect;
}

cHitbox* cDynamic::GetHitbox()
{
	return hitbox;
}
