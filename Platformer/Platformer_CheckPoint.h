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
    void SaveDoorsStates(cPlayer* player);
    void ManageOpennedDoors(std::vector<cDynamicMovingPlatform*> vecMecanisms);

private:
    const float fLvl1PX = 133.0f;
    const float fLvl1PY = 8.0f;
    const float fLvl2PX = 140.0f;
    const float fLvl2PY = 8.0f;
    const float fLvl3PX = 106.0f;
    const float fLvl3PY = 14.0f;
    const float fLvl4PX = 5.0f;
    const float fLvl4PY = 144.0f;
    const float fLvl5PX = 85.0f;
    const float fLvl5PY = 6.0f;
    const int cnNbCheckPoints = 6;
    const int cnNbLvl6Doors = 5;
    bool bCheckPoints[6];
    bool bOpenedDoors[5];
};

#endif // !DEF_CHECKPOINT
