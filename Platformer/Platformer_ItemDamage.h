#ifndef DEF_ITEM_DAMAGE
#define DEF_ITEM_DAMAGE

#pragma once
#include "Platformer_Item.h"

class OneLoneCoder_Platformer;

class cItemDamage : public cItem
{
public:
	cItemDamage();
	virtual ~cItemDamage();

	void PickItem();

	static OneLoneCoder_Platformer* engine;
};

#endif // !DEF_ITEM_DAMAGE
