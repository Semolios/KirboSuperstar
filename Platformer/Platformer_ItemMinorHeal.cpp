#include "Platformer_ItemMinorHeal.h"
#include "Platformer_Engine.h"

OneLoneCoder_Platformer* cItemMinorHeal::engine = nullptr;

cItemMinorHeal::cItemMinorHeal()
{
}

cItemMinorHeal::~cItemMinorHeal()
{
}

void cItemMinorHeal::PickItem()
{
	engine->PlaySample("itemPicked");
	engine->HealPlayer(2);
}

