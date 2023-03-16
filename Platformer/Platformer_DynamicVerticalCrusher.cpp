#include "Platformer_DynamicVerticalCrusher.h"
#include "Platformer_Engine.h"

OneLoneCoder_Platformer* cDynamicVerticalCrusher::engine = nullptr;

cDynamicVerticalCrusher::cDynamicVerticalCrusher(float ox, float oy, std::vector<olc::Decal*> map, std::wstring side, float waitTime)
	: cDynamicBloc(ox, oy, map)
{
	bUp = (side == L"up");
	bDown = (side == L"down");
	fWaitTime = waitTime;
	fOriginY = oy;
}

cDynamicVerticalCrusher::~cDynamicVerticalCrusher()
{
}

void cDynamicVerticalCrusher::Behaviour(float fElapsedTime, float playerX, float playerY)
{
	switch (nState)
	{
		case WAITING:
		{
			fBehaviourTimer += fElapsedTime;
			vx = 0.0f;
			py = fOriginY;

			if (fBehaviourTimer >= fWaitTime)
			{
				fBehaviourTimer = 0.0f;
				nNextState = CRUSHING;
			}
		}
		break;
		case CRUSHING:
		{
			if (bUp)   vy = -cfCrushSpeed;
			if (bDown) vy = +cfCrushSpeed;

			if (fabs(py - fOriginY) >= (mapStates[nCurrentFrame]->sprite->height / engine->GetTileHeight()))
			{
				float fTargetX = playerX - px;
				float fTargetY = playerY - py;
				float fDistance = sqrtf(fTargetX * fTargetX + fTargetY * fTargetY);
				if (fDistance <= engine->ScreenWidth() / engine->GetTileWidth())
					engine->PlaySample("boom");

				nNextState = GOING_BACK;
			}
		}
		break;
		case GOING_BACK:
		{
			if (bUp) vy = cfResetPosSpeed;
			if (bDown) vy = -cfResetPosSpeed;

			if (fabs(py - fOriginY) <= cfInitPosGap)
			{
				nNextState = WAITING;
			}
		}
		break;
	}

	nState = nNextState;
}
