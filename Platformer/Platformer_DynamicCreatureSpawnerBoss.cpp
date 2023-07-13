#include "Platformer_DynamicCreatureSpawnerBoss.h"
#include "Platformer_Engine.h"

OneLoneCoder_Platformer* cDynamicCreatureSpawnerBoss::engine = nullptr;

cDynamicCreatureSpawnerBoss::cDynamicCreatureSpawnerBoss(std::string n, olc::Sprite* sprite, int framesPerSecond) : cDynamicCreature(n, sprite, framesPerSecond)
{
    fSpawnTime = 0.0f;
}

cDynamicCreatureSpawnerBoss::~cDynamicCreatureSpawnerBoss()
{
}

void cDynamicCreatureSpawnerBoss::UpdateSpawn(float fElapsedTime)
{
    if (nHealth > 0)
    {
        fSpawnTime += fElapsedTime;

        if (fSpawnTime >= cfSpawnTimer)
        {
            fSpawnTime = 0.0f;
            engine->AddNewWaddleDee(17, 8, true);
        }
    }
}
