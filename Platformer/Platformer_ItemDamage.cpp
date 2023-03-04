#include "Platformer_ItemDamage.h"

cItemDamage::cItemDamage()
{
}

cItemDamage::~cItemDamage()
{
}

void cItemDamage::PickItem(OneLoneCoder_Platformer* engine)
{
	engine->PlaySample("itemPicked");
	engine->BuffPlayerDamage();
}
