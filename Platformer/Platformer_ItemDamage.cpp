#include "Platformer_ItemDamage.h"
#include "Platformer_Engine.h"

OneLoneCoder_Platformer* cItemDamage::engine = nullptr;

cItemDamage::cItemDamage()
{
}

cItemDamage::~cItemDamage()
{
}

void cItemDamage::PickItem()
{
	engine->PlaySample("itemPicked");
	engine->BuffPlayerDamage();
}
