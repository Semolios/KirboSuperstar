#ifndef DEF_ITEM_DEFENSE
#define DEF_ITEM_DEFENSE

#pragma once
#include "Platformer_Item.h"

class OneLoneCoder_Platformer;

class cItemDefense : public cItem
{
public:
	cItemDefense();
	virtual ~cItemDefense();

	void PickItem();

	static OneLoneCoder_Platformer* engine;
};

#endif // !DEF_ITEM_DEFENSE
