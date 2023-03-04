#ifndef DEF_ITEM_MINOR_HEAL
#define DEF_ITEM_MINOR_HEAL

#pragma once

#include "Platformer_Item.h"
#include "Platformer_Engine.h"

class cItemMinorHeal : public cItem
{
public:
	cItemMinorHeal();
	virtual ~cItemMinorHeal();

	void PickItem(OneLoneCoder_Platformer* engine);
};

#endif // !DEF_ITEM_MINOR_HEAL
