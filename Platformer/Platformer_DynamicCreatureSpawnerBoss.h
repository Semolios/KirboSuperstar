#ifndef DEF_SPAWNER_BOSS
#define DEF_SPAWNER_BOSS

#pragma once

#include "Platformer_DynamicCreature.h"
#include "Platformer_Assets.h"

#include <cTime>

class OneLoneCoder_Platformer;

class cDynamicCreatureSpawnerBoss : public cDynamicCreature
{
public:
    cDynamicCreatureSpawnerBoss(std::string n, olc::Sprite* sprite, int framesPerSecond);
    virtual ~cDynamicCreatureSpawnerBoss();

    static OneLoneCoder_Platformer* engine;

    void UpdateSpawn(float fElapsedTime);

protected:
    const float cfWarningTimer = 18.0f;
    const float cfSpawnTimer = 20.0f;
    float fSpawnTime;
    bool bCanSpawnWarning;
};

#endif // !DEF_SPAWNER_BOSS
