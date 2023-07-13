#include "Platformer_DynamicCreatureSpawnerBoss.h"
#include "Platformer_Engine.h"

OneLoneCoder_Platformer* cDynamicCreatureSpawnerBoss::engine = nullptr;

cDynamicCreatureSpawnerBoss::cDynamicCreatureSpawnerBoss(std::string n, olc::Sprite* sprite, int framesPerSecond) : cDynamicCreature(n, sprite, framesPerSecond)
{
    fSpawnTime = 0.0f;
    bCanSpawnWarning = true;
}

cDynamicCreatureSpawnerBoss::~cDynamicCreatureSpawnerBoss()
{
}

void cDynamicCreatureSpawnerBoss::UpdateSpawn(float fElapsedTime)
{
    if (nHealth > 0)
    {
        fSpawnTime += fElapsedTime;
        if (fSpawnTime >= cfWarningTimer && bCanSpawnWarning)
        {
            engine->AddProjectile(16.5f, 8.0f, true, 0.0f, 0.0f, 2.0f, "spawnWarning", false, 0, false, true, 0, false, 0.0f, "", false, "", true);
            bCanSpawnWarning = false;
        }

        if (fSpawnTime >= cfSpawnTimer)
        {
            fSpawnTime = 0.0f;
            engine->AddNewWaddleDee(17, 8, true);
            bCanSpawnWarning = true;
        }
    }
}
