#include "Platformer_ItemMinorHeal.h"

cItemMinorHeal::cItemMinorHeal()
{
}

cItemMinorHeal::~cItemMinorHeal()
{
}

void cItemMinorHeal::PickItem(OneLoneCoder_Platformer* engine)
{
	engine->PlaySample("itemPicked");
	engine->HealPlayer(2);
}

