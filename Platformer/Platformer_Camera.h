#pragma once
#include "olcPixelGameEngine.h"
#include "olcPGEX_Graphics2D.h"
#include "Platformer_Level.h"

class OneLoneCoder_Platformer;

class cCamera
{
public:
	cCamera();

private:
	const float cfCameraLowerPos = 1.0f / 4.0f;			// Lower position for the camera (when the player is not pushing down)
	const float cfCameraUpperPos = 2.0f / 3.0f;			// Upper position for the camera (when the player holds down when on ground)

	const float cfCameraMoveSpeed = 0.02f;
	const float cfShakeAttenuation = 1000.0f;
	const float cfShakeEffectChangeFrequency = 0.07f;
	float fOffsetX;
	float fOffsetY;
	float fCameraLookingDown = 2.0f / 3.0f;
	float fCameraPosX = 0.0f;
	float fCameraPosY = 0.0f;
	float fShakeTimerChange = 0.0f;
	float fShakeEffectX;
	float fShakeEffectY;
	int nShakeAmplitudeX;
	int nShakeAmplitudeY;
	bool bShake = false;

public:
	static OneLoneCoder_Platformer* engine;

public:
	void ClampCameraOffset();
	void SetPositions(float fPlayerPosX, float fPlayerPosY);
	void DrawLevel(cLevel* level, float fElapsedTime);
	float GetOffsetX();
	float GetOffsetY();
	void LowerCameraPosition();
	void RaiseCameraPosition();
	void SetShake(bool shake);
	void ActivateShakeEffect(bool activate, int shakeAmplitudeX = 50, int shakeAmplitudeY = 50);
};
