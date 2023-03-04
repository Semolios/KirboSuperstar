#ifndef DEF_ITEM_TOMATO
#define DEF_ITEM_TOMATO

#pragma once

#include "Platformer_Item.h"
#include "Platformer_Engine.h"

class cItemTomato : public cItem
{
public:
	cItemTomato();
	virtual ~cItemTomato();

	void PickItem(OneLoneCoder_Platformer* engine);
};

#endif // !DEF_ITEM_TOMATO
