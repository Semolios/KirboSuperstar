#ifndef DEF_ITEM
#define DEF_ITEM

#pragma once

class OneLoneCoder_Platformer;

class cItem
{
public:
	cItem();
	virtual ~cItem();

	virtual void PickItem(OneLoneCoder_Platformer* engine){}
};

#endif // !DEF_ITEM
