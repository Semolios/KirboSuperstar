#ifndef DEF_ITEM_CANDY
#define DEF_ITEM_CANDY

#pragma once
#include "Platformer_Item.h"

class OneLoneCoder_Platformer;

class cItemCandy : public cItem
{
public:
	cItemCandy();
	virtual ~cItemCandy();

	void PickItem();

	static OneLoneCoder_Platformer* engine;

private:
	float cfInvincibilityTimer = 30.0f;
};

#endif // !DEF_ITEM_CANDY
