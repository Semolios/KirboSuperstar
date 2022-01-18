#ifndef DEF_VERTICALCRUSHER
#define DEF_VERTICALCRUSHER

#pragma once

#include "Platformer_DynamicBloc.h"

class cDynamicVerticalCrusher : public cDynamicBloc
{
public:
	cDynamicVerticalCrusher(float ox, float oy, std::vector<olc::Sprite*> map, std::wstring side, float waitTime);
	virtual ~cDynamicVerticalCrusher();

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
	float fOriginY;
	bool bUp;
	bool bDown;
};

#endif // !DEF_VERTICALCRUSHER
