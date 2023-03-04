#include "Platformer_ItemTomato.h"

cItemTomato::cItemTomato()
{
}

cItemTomato::~cItemTomato()
{
}

void cItemTomato::PickItem(OneLoneCoder_Platformer* engine)
{
	engine->PlaySample("itemPicked");
	engine->MaxHealPlayer();
}
