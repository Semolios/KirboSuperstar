#ifndef DEF_CAMERA
#define DEF_CAMERA

#pragma once
#include "olcPixelGameEngine.h"
#include "olcPGEX_Graphics2D.h"
#include "Platformer_Level.h"

class OneLoneCoder_Platformer;

constexpr int nMaxThreads = 32;

static std::atomic<int> nWorkerComplete;

class cCamera
{
public:
	cCamera();

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

	float cfHalberdCloudSpawnTimer;
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

	struct WorkerThread
	{
		int ox;
		int oy;
		float fBackgroundOffsetX;
		float fBackgroundOffsetY;
		int width;
		int height;
		std::condition_variable cvStart;
		bool alive = true;
		std::mutex mux;
		int screen_width = 0;
		cCamera* cam;

		std::thread thread;

		void Start(int x, int y, float fBckgrdoffX, float fBckgrdoffY, int w, int h, cCamera* camera)
		{
			ox = x;
			oy = y;
			fBackgroundOffsetX = fBckgrdoffX;
			fBackgroundOffsetY = fBckgrdoffY;
			width = w;
			height = h;
			cam = camera;

			std::unique_lock<std::mutex> lm(mux);
			cvStart.notify_one();
		}

		void DrawBackground()
		{
			while (alive)
			{
				std::unique_lock<std::mutex> lm(mux);
				cvStart.wait(lm);

				cam->DrawBackgroundThread(ox, oy, fBackgroundOffsetX, fBackgroundOffsetY, width, height);

				nWorkerComplete++;
			}
		}
	};

	WorkerThread workers[nMaxThreads];

public:
	static OneLoneCoder_Platformer* engine;

public:
	void ClampOffset();
	void SetPositions(float fPlayerPosX, float fPlayerPosY);
	void DrawLevel(cLevel* level, float fElapsedTime);
	void CalculateFOV(cLevel* level);
	void DrawBackground(cLevel* level);
	void DrawBackgroundThread(int x, int y, float fBckgrdoffX, float fBckgrdoffY, int w, int h);
	float GetOffsetX();
	float GetOffsetY();
	void LowerPosition();
	void RaisePosition();
	void SetShake(bool shake);
	void ActivateShakeEffect(bool activate, int shakeAmplitudeX = 50, int shakeAmplitudeY = 50);
	void InitialiseThreadPool();
	void SpawnSceneries(cLevel* level, float fElapsedTime);
};

#endif // !DEF_CAMERA
