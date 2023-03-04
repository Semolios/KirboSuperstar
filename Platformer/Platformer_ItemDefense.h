#ifndef DEF_ITEM_DEFENSE
#define DEF_ITEM_DEFENSE

#pragma once

#include "Platformer_Item.h"
#include "Platformer_Engine.h"

class cItemDefense : public cItem
{
public:
	cItemDefense();
	virtual ~cItemDefense();

	void PickItem(OneLoneCoder_Platformer* engine);
};

#endif // !DEF_ITEM_DEFENSE
