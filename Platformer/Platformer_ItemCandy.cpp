#include "Platformer_ItemCandy.h"

cItemCandy::cItemCandy()
{
}

cItemCandy::~cItemCandy()
{
}

void cItemCandy::PickItem(OneLoneCoder_Platformer* engine)
{
	engine->StopLevelMusic();
	engine->PlaySample("itemPicked");
	engine->PlayerGetCandy(cfInvincibilityTimer);
}
