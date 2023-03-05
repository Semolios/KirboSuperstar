#ifndef DEF_CAMERA
#define DEF_CAMERA

#pragma once
#include "olcPixelGameEngine.h"
#include "Platformer_Level.h"
#include "Platformer_Engine.h"

class cCamera
{
public:
	cCamera();

	void ClampOffset();
	void SetPositions(float fPlayerPosX, float fPlayerPosY);
	void DrawLevel(cLevel* level, float fElapsedTime);
	void CalculateFOV(cLevel* level, OneLoneCoder_Platformer* engine);
	void DrawBackground(cLevel* level, OneLoneCoder_Platformer* engine);
	float GetOffsetX();
	float GetOffsetY();
	void LowerPosition();
	void RaisePosition();
	void SetShake(bool shake);
	void ActivateShakeEffect(bool activate, int shakeAmplitudeX = 50, int shakeAmplitudeY = 50);
	void SpawnSceneries(cLevel* level, float fElapsedTime, OneLoneCoder_Platformer* engine);

private:
	const float cfLowerPos = 1.0f / 4.0f;		  // Lower position for the camera (when the player is not pushing down)
	const float cfUpperPos = 2.0f / 3.0f;		  // Upper position for the camera (when the player holds down when on ground)
	const float cfMoveSpeed = 0.02f;			  // Move speed of the camera when lowering position
	const float cfShakeAttenuation = 1000.0f;	  // A constant value used in shake effect
	const float cfHalberdCloudVelX = -40.0f;	  // Speed of the clouds on halberd stage
	const float cfSnowVelX = 5.0f;				  // Speed of the clouds on halberd stage
	const float cfSnowVelY = 20.0f;				  // Speed of the clouds on halberd stage
	const float cfWahooVelX = -70.0f;			  // speed of speedrunner wahoo on halberd stage
	const float cfHalberdCloudSpawnTime = 0.125f; // spawn time of the clouds on halberd stage
	const int cnObjectPosXRange = 205;			  // a factor to spawn randomly object on the top of the screen
	const int cnObjectPosYRange = 75;			  // a factor to spawn randomly object on the right of the screen

	float fHalberdCloudSpawnTimer = 0.0f;
	float fOffsetX;
	float fOffsetY;
	float fLookingDownPos = 2.0f / 3.0f;
	float fPosX = 0.0f;
	float fPosY = 0.0f;
	float fShakeTimerChange = 0.0f;
	float fShakeEffectX;
	float fShakeEffectY;
	int nShakeAmplitudeX;
	int nShakeAmplitudeY;
	int nVisibleTilesX;
	int nVisibleTilesY;
	bool bShake = false;
};

#endif // !DEF_CAMERA
