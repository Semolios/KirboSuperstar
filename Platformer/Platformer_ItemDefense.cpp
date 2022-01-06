#include "Platformer_ItemDefense.h"
#include "Platformer_Engine.h"

OneLoneCoder_Platformer* cItemDefense::engine = nullptr;

cItemDefense::cItemDefense()
{
}

cItemDefense::~cItemDefense()
{
}

void cItemDefense::PickItem()
{
	engine->PlaySample("itemPicked");
	engine->BuffPlayerDefense();
}
