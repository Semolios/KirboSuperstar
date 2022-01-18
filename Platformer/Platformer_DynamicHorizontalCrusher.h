#ifndef DEF_HORIZONTALCRUSHER
#define DEF_HORIZONTALCRUSHER

#pragma once

#include "Platformer_DynamicBloc.h"

class cDynamicHorizontalCrusher : public cDynamicBloc
{
public:
	cDynamicHorizontalCrusher(float ox, float oy, std::vector<olc::Sprite*> map, std::wstring side, float waitTime);
	virtual ~cDynamicHorizontalCrusher();

	void Behaviour(float fElapsedTime, float playerX, float playerY);

	enum STATE
	{
		WAITING,
		CRUSHING,
		GOING_BACK,
	} nState, nNextState;

	static OneLoneCoder_Platformer* engine;

private:
	const float cfCrushSpeed = 20.0f;
	const float cfResetPosSpeed = 2.0f;
	const float cfInitPosGap = 0.2f;
	float fBehaviourTimer;
	float fWaitTime;
	float fOriginX;
	bool bLeft;
	bool bRight;
};

#endif // !DEF_HORIZONTALCRUSHER
