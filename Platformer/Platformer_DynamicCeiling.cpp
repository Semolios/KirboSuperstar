#include "Platformer_DynamicCeiling.h"

cDynamicCeiling::cDynamicCeiling(float ox, float oy, std::vector<olc::Sprite*> map, std::wstring topSolid, std::wstring linkToPreviousPlatform)
	: cDynamicMovingPlatform(ox, oy, map)
{
	bSolidBot = true;
	bSolidTop = (topSolid == L"1");
	bLinkedToPreviousPtfm = (linkToPreviousPlatform == L"1");
}

cDynamicCeiling::~cDynamicCeiling()
{
}

void cDynamicCeiling::Behaviour(float fElapsedTime, float playerX, float playerY)
{
	if (bLinkedToPreviousPtfm)
	{
		vx = linkedPtfm->GetVX();
		vy = linkedPtfm->GetVY();
	}
}
