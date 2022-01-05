#ifndef DEF_ITEM
#define DEF_ITEM

#pragma once
#include "olcPixelGameEngine.h"
#include "olcPGEX_Graphics2D.h"

class Platformer_Engine;

class cItem
{
public:
	cItem();
	virtual ~cItem();

	virtual void PickItem(){}
};

#endif // !DEF_ITEM
