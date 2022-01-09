#ifndef DEF_MOVING_PLATFORM
#define DEF_MOVING_PLATFORM

#pragma once

#include "Platformer_Dynamic.h"

class OneLoneCoder_Platformer;

class cDynamicMovingPlatform : public cDynamic
{
public:
	cDynamicMovingPlatform(float ox, float oy, std::vector<olc::Sprite*> map);
	virtual ~cDynamicMovingPlatform();

protected:
	std::vector<olc::Sprite*> mapStates;

	// animation
	int nCurrentFrame = 0;
	float fTimeBetweenFrames = 0.05f;
	float fTimeCounter = 0.0f;
	float fHitboxThickness = 0.15f;
	bool bTriggered = false;

public:
	static OneLoneCoder_Platformer* engine;

public:
	void DrawSelf(float ox, float oy) override;
	void Update(float fElapsedTime, float playerX, float playerY) override;
	void UpdateHitbox(float cameraOffsetX, float cameraOffsetY) override;
	bool TopCollision(float leftCornerX, float rightCornerX, float bottomSpriteY);
	bool TopCollisionWithLag(float leftCornerX, float rightCornerX, float fPlayerPosY, float fNewPlayerPosY);
	bool TopCollisionOneCorner(float CornerX, float cornerY);
	static std::map<std::string, std::vector<olc::Sprite*>> LoadMovingPlatformsSprites();
	void TriggerMovement();

	virtual void UpdateTrajectory(float fElapsedTime);
};

#endif // !DEF_MOVING_PLATFORM
