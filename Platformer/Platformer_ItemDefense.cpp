#include "Platformer_ItemDefense.h"

cItemDefense::cItemDefense()
{
}

cItemDefense::~cItemDefense()
{
}

void cItemDefense::PickItem(OneLoneCoder_Platformer* engine)
{
	engine->PlaySample("itemPicked");
	engine->BuffPlayerDefense();
}
