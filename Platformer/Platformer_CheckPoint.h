#ifndef DEF_CHECKPOINT
#define DEF_CHECKPOINT

#pragma once

#include "Platformer_Engine.h"

class cPlayer;

class cCheckPoint
{
public:
    cCheckPoint();
    ~cCheckPoint();

    void Use(cLevel* level, OneLoneCoder_Platformer* engine, std::vector<cDynamicMovingPlatform*> vecMecanisms);
    void Update(cLevel* level, cPlayer* player, std::vector<cDynamicMovingPlatform*> vecMecanisms);
    bool CheckCondition(cLevel* level, cPlayer* player, std::vector<cDynamicMovingPlatform*> vecMecanisms);
    void SaveDoorsStates(std::vector<cDynamicMovingPlatform*> vecMecanisms);
    void ManageOpennedDoors(std::vector<cDynamicMovingPlatform*> vecMecanisms);

private:
    // lvl 1-5 checkpoints position
    const float cfLvl1PX = 133.0f;
    const float cfLvl1PY = 8.0f;
    const float cfLvl2PX = 140.0f;
    const float cfLvl2PY = 8.0f;
    const float cfLvl3PX = 106.0f;
    const float cfLvl3PY = 14.0f;
    const float cfLvl4PX = 5.0f;
    const float cfLvl4PY = 144.0f;
    const float cfLvl5PX = 85.0f;
    const float cfLvl5PY = 6.0f;

    // lvl 6 switches positions
    const float cfDoor1PX = 0.0f; // TODO
    const float cfDoor1PY = 0.0f; // TODO
    const float cfDoor2PX = 0.0f; // TODO
    const float cfDoor2PY = 0.0f; // TODO
    const float cfDoor3PX = 0.0f; // TODO
    const float cfDoor3PY = 0.0f; // TODO
    const float cfDoor4PX = 0.0f; // TODO
    const float cfDoor4PY = 0.0f; // TODO
    const float cfDoor5PX = 0.0f; // TODO
    const float cfDoor5PY = 0.0f; // TODO

    const int cnNbCheckPoints = 6;
    const int cnNbLvl6Doors = 5;
    
    bool bCheckPoints[6];
    bool bOpenedDoors[5];
};

#endif // !DEF_CHECKPOINT
