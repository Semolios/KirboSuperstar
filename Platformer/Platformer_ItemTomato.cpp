#include "Platformer_ItemTomato.h"
#include "Platformer_Engine.h"

OneLoneCoder_Platformer* cItemTomato::engine = nullptr;

cItemTomato::cItemTomato()
{
}

cItemTomato::~cItemTomato()
{
}

void cItemTomato::PickItem()
{
	engine->PlaySample("itemPicked");
	engine->MaxHealPlayer();
}
