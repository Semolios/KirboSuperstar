#ifndef DEF_ITEM_TOMATO
#define DEF_ITEM_TOMATO

#pragma once
#include "Platformer_Item.h"

class OneLoneCoder_Platformer;

class cItemTomato : public cItem
{
public:
	cItemTomato();
	virtual ~cItemTomato();

	void PickItem();

	static OneLoneCoder_Platformer* engine;
};

#endif // !DEF_ITEM_TOMATO
