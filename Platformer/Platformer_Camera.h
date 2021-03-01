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
	void ClampCameraOffset();
	void SetPositions(float fPlayerPosX, float fPlayerPosY);
	void DrawLevel(cLevel* level, float fElapsedTime);
	void DrawBackground(cLevel* level);
	void DrawBackgroundThread(int x, int y, float fBckgrdoffX, float fBckgrdoffY, int w, int h);
	float GetOffsetX();
	float GetOffsetY();
	void LowerCameraPosition();
	void RaiseCameraPosition();
	void SetShake(bool shake);
	void ActivateShakeEffect(bool activate, int shakeAmplitudeX = 50, int shakeAmplitudeY = 50);
	void InitialiseThreadPool();
};

#endif // !DEF_CAMERA