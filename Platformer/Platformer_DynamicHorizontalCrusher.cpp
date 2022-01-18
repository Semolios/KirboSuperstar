#include "Platformer_DynamicHorizontalCrusher.h"
#include "Platformer_Engine.h"

OneLoneCoder_Platformer* cDynamicHorizontalCrusher::engine = nullptr;

cDynamicHorizontalCrusher::cDynamicHorizontalCrusher(float ox, float oy, std::vector<olc::Sprite*> map, std::wstring side, float waitTime)
	: cDynamicBloc(ox, oy, map)
{
	bLeft = (side == L"left");
	bRight = (side == L"right");
	fWaitTime = waitTime;
	fOriginX = ox;
}

cDynamicHorizontalCrusher::~cDynamicHorizontalCrusher()
{
}

void cDynamicHorizontalCrusher::Behaviour(float fElapsedTime, float playerX, float playerY)
{
	switch (nState)
	{
		case WAITING:
		{
			fBehaviourTimer += fElapsedTime;
			vx = 0.0f;
			px = fOriginX;

			if (fBehaviourTimer >= fWaitTime)
			{
				fBehaviourTimer = 0.0f;
				nNextState = CRUSHING;
			}
		}
		break;
		case CRUSHING:
		{
			if (bRight) vx = cfCrushSpeed;
			if (bLeft) vx = -cfCrushSpeed;

			if (fabs(px - fOriginX) >= (mapStates[nCurrentFrame]->width / 64.0f))
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
			if (bRight) vx = -cfResetPosSpeed;
			if (bLeft) vx = cfResetPosSpeed;

			if (fabs(px - fOriginX) <= cfInitPosGap)
			{
				nNextState = WAITING;
			}
		}
		break;
	}

	nState = nNextState;
}
