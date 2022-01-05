#include "Platformer_ItemCandy.h"
#include "Platformer_Engine.h"

OneLoneCoder_Platformer* cItemCandy::engine = nullptr;

cItemCandy::cItemCandy()
{
}

cItemCandy::~cItemCandy()
{
}

void cItemCandy::PickItem()
{
	engine->StopLevelMusic();
	engine->PlaySample("itemPicked");
	engine->PlayerGetCandy(cfInvincibilityTimer);
}
