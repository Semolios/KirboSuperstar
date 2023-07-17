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
            case 0: engine->SetKirboPositions(cfLvl1PX, cfLvl1PY); break;
            case 1: engine->SetKirboPositions(cfLvl2PX, cfLvl2PY); break;
            case 2: engine->SetKirboPositions(cfLvl3PX, cfLvl3PY); break;
            case 3: engine->SetKirboPositions(cfLvl4PX, cfLvl4PY); break;
            case 4: engine->SetKirboPositions(cfLvl5PX, cfLvl5PY); break;
            case 5: ManageOpennedDoors(vecMecanisms);              break;
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
        case 0: return player->GetPosX() >= cfLvl1PX || bCheckPoints[0]; break;
        case 1: return player->GetPosX() >= cfLvl2PX || bCheckPoints[1]; break;
        case 2: return player->GetPosX() >= cfLvl3PX || bCheckPoints[2]; break;
        case 3: return player->GetPosY() <= cfLvl4PY || bCheckPoints[3]; break;
        case 4: return player->GetPosX() >= cfLvl5PX || bCheckPoints[4]; break;
        case 5: SaveDoorsStates(vecMecanisms); return true;              break;
    }
}

void cCheckPoint::SaveDoorsStates(std::vector<cDynamicMovingPlatform*> vecMecanisms)
{
    for (auto& mec : vecMecanisms)
    {
        if (mec->GetIdentifier() == "door1") bOpenedDoors[0] = ((cDynamicWall*)mec)->IsTriggered();
        if (mec->GetIdentifier() == "door2") bOpenedDoors[1] = ((cDynamicWall*)mec)->IsTriggered();
        if (mec->GetIdentifier() == "door3") bOpenedDoors[2] = ((cDynamicWall*)mec)->IsTriggered();
        if (mec->GetIdentifier() == "door4") bOpenedDoors[3] = ((cDynamicWall*)mec)->IsTriggered();
        if (mec->GetIdentifier() == "door5") bOpenedDoors[4] = ((cDynamicWall*)mec)->IsTriggered();
    }
}

void cCheckPoint::ManageOpennedDoors(std::vector<cDynamicMovingPlatform*> vecMecanisms)
{
    for (auto& mec : vecMecanisms)
    {
        if (mec->GetIdentifier() == "door1") ((cDynamicWall*)mec)->SetTriggered(bOpenedDoors[0]);
        if (mec->GetIdentifier() == "door2") ((cDynamicWall*)mec)->SetTriggered(bOpenedDoors[1]);
        if (mec->GetIdentifier() == "door3") ((cDynamicWall*)mec)->SetTriggered(bOpenedDoors[2]);
        if (mec->GetIdentifier() == "door4") ((cDynamicWall*)mec)->SetTriggered(bOpenedDoors[3]);
        if (mec->GetIdentifier() == "door5") ((cDynamicWall*)mec)->SetTriggered(bOpenedDoors[4]);
    }
}
