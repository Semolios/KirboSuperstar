#ifndef DEF_MOVING_PLATFORM
#define DEF_MOVING_PLATFORM

#pragma once

#include "Platformer_Dynamic.h"

class OneLoneCoder_Platformer;

class cDynamicMovingPlatform : public cDynamic
{
public:
	cDynamicMovingPlatform(float ox, float oy, std::vector<olc::Decal*> map, std::wstring iced); // Spawn a normal platform
	cDynamicMovingPlatform(float ox, float oy, std::vector<olc::Decal*> map);					  // Spawn a Scenery
	virtual ~cDynamicMovingPlatform();

	virtual void DrawSwitch(float cameraX, float cameraY);

	static OneLoneCoder_Platformer* engine;

	void DrawSelf(float ox, float oy) override;
	void Update(float fElapsedTime, float playerX, float playerY) override;
	void UpdateHitbox(float cameraOffsetX, float cameraOffsetY) override;

	// Collisions
		// Top
	bool TopCollision(float leftCornerX, float rightCornerX, float bottomSpriteY);
	bool TopCollisionWithLag(float leftCornerX, float rightCornerX, float fPlayerPosY, float fNewPlayerPosY);
	bool TopCollisionOneCorner(float CornerX, float cornerY);
		// Left
	bool LeftCollision(float topCornerY, float botCornerY, float rightSpriteX);
	bool LeftCollisionWithLag(float topCornerY, float botCornerY, float fPlayerPosX, float fNewPlayerPosX);
	bool LeftCollisionOneCorner(float CornerX, float cornerY);
		// Right
	bool RightCollision(float topCornerY, float botCornerY, float leftSpriteX);
	bool RightCollisionWithLag(float topCornerY, float botCornerY, float fPlayerPosX, float fNewPlayerPosX);
	bool RightCollisionOneCorner(float CornerX, float cornerY);
		// Bot
	bool BotCollision(float leftCornerX, float rightCornerX, float topSpriteY);
	bool BotCollisionWithLag(float leftCornerX, float rightCornerX, float fPlayerPosY, float fNewPlayerPosY);
	bool BotCollisionOneCorner(float CornerX, float cornerY);

	static std::map<std::string, std::vector<olc::Sprite*>> LoadMovingPlatformsSprites();
	static std::map<std::string, std::vector<olc::Decal*>>  LoadMovingPlatformsDecals(std::map<std::string, std::vector<olc::Sprite*>> mapPlatforms);
	void TriggerMovement();
	void UntriggerMovement();

	float GetNormalizedWidth();
	float GetNormalizedHeight();

	bool IsIced();
	bool IsHarmfulblocTangible();

	olc::Sprite* GetCurrentSprite();

	std::string GetIdentifier();
	void SetIdentifier(std::string id);

	bool IsForeground();
	void SetForeground(bool foreground);

	virtual void Behaviour(float fElapsedTime, float playerX, float playerY);
	virtual void LinkPtfm(cDynamicMovingPlatform* ptfm);
	virtual bool IsLinkedToPreviousPtfm();

protected:
	std::vector<olc::Decal*> mapStates;

	std::string sIdentifier;

	// animation
	int nCurrentFrame = 0;
	float fTimeBetweenFrames = 0.033f;
	float fTimeCounter = 0.0f;

	float fHitboxThickness = 0.15f;
	bool bTriggerable;
	bool bTriggered = false;
	bool bIced = false;
	bool bForeground = false;

	// Sides Traversability
	bool bSolidTop = true;
	bool bSolidBot = false;
	bool bSolidLeft = false;
	bool bSolidRight = false;
	bool bTangible = true;

	float fPrevPosX;
	float fPrevPosY;

	cDynamicMovingPlatform* linkedPtfm;
	bool bLinkedToPreviousPtfm;
};

#endif // !DEF_MOVING_PLATFORM
