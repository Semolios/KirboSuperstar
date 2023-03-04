#ifndef DEF_ITEM_DAMAGE
#define DEF_ITEM_DAMAGE

#pragma once

#include "Platformer_Item.h"
#include "Platformer_Engine.h"

class cItemDamage : public cItem
{
public:
	cItemDamage();
	virtual ~cItemDamage();

	void PickItem(OneLoneCoder_Platformer* engine);
};

#endif // !DEF_ITEM_DAMAGE
