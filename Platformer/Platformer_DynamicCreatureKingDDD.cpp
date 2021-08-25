#include "Platformer_DynamicCreatureKingDDD.h"
#include "Platformer_Level.h"
#include "Platformer_Engine.h"

OneLoneCoder_Platformer* cDynamicCreatureKingDDD::engine = nullptr;

cDynamicCreatureKingDDD::cDynamicCreatureKingDDD(cLevel* l) : cDynamicCreature("kingDDD", cAssets::get().GetSprite("kingDDD"), 30)
{
	fDynWidth = 128.0f;
	fDynHeight = 128.0f;
	fSpriteW = 384.0f;
	fSpriteH = 384.0f;
	fSpriteOffsetX = -128.0f;
	fSpriteOffsetY = -128.0f;
	bFriendly = false;
	nHealth = 100;
	nHealthMax = 100;
	level = l;
	bSolidVsMap = true;
	bAffectedByGravity = true;
	nDamage = 4;
	bCanBehaveWhileAttacked = true;
}

cDynamicCreatureKingDDD::~cDynamicCreatureKingDDD()
{
	delete hitbox;
	delete sSprite;
	delete level;
}

void cDynamicCreatureKingDDD::Behaviour(float fElapsedTime, float playerX, float playerY, olc::PixelGameEngine* gfx)
{
	// The spritesheet is very big, so the damaged animation is only drawn once
	if (IsOneFrameAnimation())
		nGraphicCounter = 0;

	switch (nAIState)
	{
		case AI_IDLE:
		{
			vx = 0.0f; vy = 0.0f;
			nGraphicCounter = 0;
		}
		break;
		case AI_MOVING:
		{

		}
		break;
		case AI_JUMPING:
		{

		}
		break;
		case AI_UPSMASH:
		{

		}
		break;
		case AI_SIDESMASH:
		{

		}
		break;
		case AI_DOWNSMASH:
		{

		}
		break;
		case AI_SIDEB:
		{

		}
		break;
		case AI_DOWNB:
		{

		}
		break;
		case AI_UPB:
		{

		}
		break;
		case AI_VACUUM:
		{

		}
		break;
		case AI_UPAIR:
		{

		}
		break;
		case AI_FORWARDAIR:
		{

		}
		break;
		case AI_BACKWARDAIR:
		{

		}
		break;
		case AI_DOWNAIR:
		{

		}
		break;
	}

	UpdateTimers();

	nAIState = nAINextState;
}

bool cDynamicCreatureKingDDD::IsOneFrameAnimation()
{
	return
		nGraphicState == DAMAGED ||
		nGraphicState == STANDING;
}

void cDynamicCreatureKingDDD::ChangeState(AI_STATE state, bool resetBehaviourTimer)
{
	nGraphicCounter = 0;
	fBehaviourTimer = 0.0f;
	nAINextState = state;
}

void cDynamicCreatureKingDDD::UpdateTimers()
{
	if (nHealth >= cnHiHP)
	{
		fWaitingTime = cfWaitingTimeMaxHp;
	}
	else if (nHealth < cnHiHP && nHealth >= cnMiHP)
	{
		fWaitingTime = cfWaitingTimeHiHp;
	}
	else if (nHealth < cnMiHP && nHealth >= cnLoHP)
	{
		fWaitingTime = cfWaitingTimeMiHp;
	}
	else
	{
		fWaitingTime = cfWaitingTimeLoHp;
	}
}
