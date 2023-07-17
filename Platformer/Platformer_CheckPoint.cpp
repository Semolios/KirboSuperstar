#include "Platformer_CheckPoint.h"

cCheckPoint::cCheckPoint()
{
    for (int i = 0; i < cnNbCheckPoints; i++)
    {
        bCheckPoints[i] = false;
    }
    
    for (int i = 0; i < cnNbLvl6Doors; i++)
    {
        bOpenedDoors[i] = false;
    }
}

cCheckPoint::~cCheckPoint()
{
}

void cCheckPoint::Use(cLevel* level, OneLoneCoder_Platformer* engine, std::vector<cDynamicMovingPlatform*> vecMecanisms)
{
    if (bCheckPoints[level->GetCurrentLvl()])
    {
        switch (level->GetCurrentLvl())
        {
            case 0: engine->SetKirboPositions(fLvl1PX, fLvl1PY); break;
            case 1: engine->SetKirboPositions(fLvl2PX, fLvl2PY); break;
            case 2: engine->SetKirboPositions(fLvl3PX, fLvl3PY); break;
            case 3: engine->SetKirboPositions(fLvl4PX, fLvl4PY); break;
            case 4: engine->SetKirboPositions(fLvl5PX, fLvl5PY); break;
            case 5: ManageOpennedDoors(vecMecanisms);            break;
        }
    }
}

void cCheckPoint::Update(cLevel* level, cPlayer* player, std::vector<cDynamicMovingPlatform*> vecMecanisms)
{
    for (int i = 0; i < cnNbCheckPoints; i++)
    {
        bCheckPoints[i] = level->GetCurrentLvl() == i && CheckCondition(level, player, vecMecanisms);
    }

    // Reset lvl 6 doors if player goes to another level
    if (level->GetCurrentLvl() != 5)
    {
        for (int i = 0; i < 5; i++)
        {
            bOpenedDoors[i] = false;
        }
    }
}

bool cCheckPoint::CheckCondition(cLevel* level, cPlayer* player, std::vector<cDynamicMovingPlatform*> vecMecanisms)
{
    switch (level->GetCurrentLvl())
    {
        case 0: return player->GetPosX() >= fLvl1PX || bCheckPoints[0]; break;
        case 1: return player->GetPosX() >= fLvl2PX || bCheckPoints[1]; break;
        case 2: return player->GetPosX() >= fLvl3PX || bCheckPoints[2]; break;
        case 3: return player->GetPosY() <= fLvl4PY || bCheckPoints[3]; break;
        case 4: return player->GetPosX() >= fLvl5PX || bCheckPoints[4]; break;
        case 5: SaveDoorsStates(player); return true;                   break;
    }
}

void cCheckPoint::SaveDoorsStates(cPlayer* player)
{
    // TODO save the doors states
}

void cCheckPoint::ManageOpennedDoors(std::vector<cDynamicMovingPlatform*> vecMecanisms)
{
    // TODO open the already checked doors
}
