#ifndef DEF_PLAYER
#define	DEF_PLAYER

#pragma once
#include "Platformer_Animator.h"
#include "Platformer_Camera.h"
#include "Platformer_DynamicMovingPlatform.h"
#include "Platformer_Hitbox.h"
#include "Platformer_Item.h"
#include "Platformer_ItemCandy.h"
#include "Platformer_ItemDamage.h"
#include "Platformer_ItemDefense.h"
#include "Platformer_ItemMinorHeal.h"
#include "Platformer_ItemTomato.h"

class cCamera;

class cPlayer
{
public:
	cPlayer(cAnimator* animator);

	void InitialiseKirboHealth();
	bool IsDead();
	void HandleInput(float fElapsedTime, cCamera* camera, cLevel* lvl, OneLoneCoder_Platformer* engine, olc::Sprite* playerSprite, olc::Decal* playerDecal);
	void Jump(OneLoneCoder_Platformer* engine);
	bool IsEnteringDoor(cLevel* lvl, OneLoneCoder_Platformer* engine);
	void EnterDoor(cLevel* lvl, OneLoneCoder_Platformer* engine);
	void EnterTP(cLevel* level, OneLoneCoder_Platformer* engine);
	void Teleport(float px, float py, cLevel* level, OneLoneCoder_Platformer* engine);
	bool CanInteract(OneLoneCoder_Platformer* engine);
	void ApplyGravity(float fElapsedTime, OneLoneCoder_Platformer* engine);
	void Update(float fElapsedTime, OneLoneCoder_Platformer* engine, olc::Sprite* playerSprite, olc::Decal* playerDecal);
	float GetFaceDir();
	void OneCycleAnimations(float fElapsedTime, float& angle, float& offsetX, float& offsetY, std::map<std::string, std::vector<olc::Sprite*>> mapProjectiles, cLevel* lvl, OneLoneCoder_Platformer* engine, olc::Sprite* playerSprite, olc::Decal* playerDecal);
	void StopAnyAction();
	void ClampVelocities();
	void IncreaseVelocities(float dvx, float dvy);
	void DecreaseVelocities(float dvx, float dvy);
	void SetVelocities(float vx, float vy);
	void Collisions(float fElapsedTime, cLevel* lvl, OneLoneCoder_Platformer* engine, olc::Sprite* playerSprite, olc::Decal* playerDecal);
	void CheckHole(cLevel* lvl, OneLoneCoder_Platformer* engine, olc::Sprite* playerSprite, olc::Decal* playerDecal);
	void CheckPickUp(cLevel* lvl, float fNewPosX, float fNewPosY, OneLoneCoder_Platformer* engine);
	void CheckSolidFloor(cLevel* lvl, float fNewPosX, float& fNewPosY, OneLoneCoder_Platformer* engine, olc::Sprite* playerSprite, olc::Decal* playerDecal);
	void NormalDrag();
	void CheckDynamicFloor(float& fNewPosX, float& fNewPosY, float fElapsedTime, cLevel* lvl, OneLoneCoder_Platformer* engine, olc::Sprite* playerSprite, olc::Decal* playerDecal);
	void IcedDrag();
	bool CeilingFloorCrushed(cDynamicMovingPlatform*& ptfm);
	void CheckSolidCeiling(cLevel* lvl, float fNewPosX, float& fNewPosY, OneLoneCoder_Platformer* engine, olc::Sprite* playerSprite, olc::Decal* playerDecal);
	void CheckDynamicCeiling(float fNewPosX, float& fNewPosY, cLevel* lvl, OneLoneCoder_Platformer* engine, olc::Sprite* playerSprite, olc::Decal* playerDecal);
	bool FloorCeilingCrushed(cDynamicMovingPlatform*& ptfm);
	void Crushed(OneLoneCoder_Platformer* engine, olc::Sprite* playerSprite, olc::Decal* playerDecal);
	bool DynamicFloor(float fNewPosX, float fNewPosY, OneLoneCoder_Platformer* engine);
	bool DynamicCeiling(float fNewPosX, float fNewPosY, OneLoneCoder_Platformer* engine);
	bool SemiSolidFloor(cLevel* lvl, float fNewPosX, float fNewPosY, OneLoneCoder_Platformer* engine);
	bool SolidFloor(cLevel* lvl, float fNewPosX, float fNewPosY, OneLoneCoder_Platformer* engine);
	bool Ceiling(cLevel* lvl, float fNewPosX, float fNewPosY, OneLoneCoder_Platformer* engine);
	void CheckRightWall(cLevel* lvl, float& fNewPosX, OneLoneCoder_Platformer* engine, olc::Sprite* playerSprite, olc::Decal* playerDecal);
	bool LeftRightCrushed(cDynamicMovingPlatform*& ptfm);
	bool DynamicLeftWall(float fNewPosX, OneLoneCoder_Platformer* engine);
	void CheckLeftWall(cLevel* lvl, float& fNewPosX, OneLoneCoder_Platformer* engine, olc::Sprite* playerSprite, olc::Decal* playerDecal);
	void HarmfulBloc(cDynamicMovingPlatform*& ptfm, OneLoneCoder_Platformer* engine, olc::Sprite* playerSprite, olc::Decal* playerDecal);
	bool RightLeftCrushed(cDynamicMovingPlatform*& ptfm);
	bool DynamicRightWall(float fNewPosX, OneLoneCoder_Platformer* engine);
	float GetPosX();
	float GetPosY();
	void SetPosX(float px);
	void SetPosY(float py);
	bool IsVacuuming();
	void VacuumEnnemy(cDynamicCreature* object);
	bool IsAttackable();
	bool HasCandyPower();
	void SetAttackable(bool attackable);
	bool IsSwallowing();
	void UpdateInvulnerability(float fElapsedTime, OneLoneCoder_Platformer* engine);
	void DrawKirbo(float posx, float posy, float angle, float faceDir, OneLoneCoder_Platformer* engine, olc::Decal* playerDecal);
	float GetHealth();
	void Damage(cDynamic* object, OneLoneCoder_Platformer* engine, olc::Sprite* playerSprite, olc::Decal* playerDecal);
	void Kill(olc::Sprite* playerSprite, olc::Decal* playerDecal);
	void ResetVariables();
	void VacuumHitbox(float cameraOffsetX, float cameraOffsetY, OneLoneCoder_Platformer* engine);
	cHitbox* GetVacuumHitbox();
	void UpdateHitbox(float cameraOffsetX, float cameraOffsetY, OneLoneCoder_Platformer* engine);
	cHitbox* GetHitbox();
	void Attack(cDynamicCreature* victim, int damage);
	void Vacuum(cDynamicCreature* object, float cameraOffsetX, float cameraOffsetY, OneLoneCoder_Platformer* engine);
	void EnemyCollision(cDynamic* object, float cameraOffsetX, float cameraOffsetY, OneLoneCoder_Platformer* engine, olc::Sprite* playerSprite, olc::Decal* playerDecal);
	bool EnemyTouched(cDynamic* object, float cameraOffsetX, float cameraOffsetY);
	void PickupGrabbed(cDynamicProjectile* object, float cameraOffsetX, float cameraOffsetY, OneLoneCoder_Platformer* engine);
	void SetGrabbedByEnnemy(bool grabbed);
	void ChangeAnimation(std::string animation, olc::Sprite* playerSprite, olc::Decal* playerDecal);
	void SetVisible(bool visible);
	void ChangePosAfterTP();
	void DrawDoorCommand(cLevel* lvl, OneLoneCoder_Platformer* engine, float cameraOffsetX, float cameraOffsetY);
	void DoorCommandPosition(int posX, int posY, OneLoneCoder_Platformer* engine, float cameraOffsetX, float cameraOffsetY);
	void SetProjected(bool projected);

	// Item Pick-up functions
	bool IsCollectibleItem(wchar_t c);
	void SelectItem(wchar_t item, OneLoneCoder_Platformer* engine);
	void MaxHeal();
	void Heal(int hp);
	void SetInvincible(float time, bool blink = true);
	void SetCandyPower(bool candy);
	int GetCandyDmg();
	void SetDamageBooster(int boost);
	bool HasDamageBooster();
	void SetDefenseBooster(int boost);
	bool HasDefenseBooster();

private:
	const float cfMaxHealth = 10.0f;					// Max Kirbo health
	const float cfVelY = 5.0f;							// Up and Down velocities
	const float cfMinVelX = 1.0f;						// Minimum player horizontal velocity
	const float cfMaxVelX = 7.0f;						// Maximum player horizontal velocity
	const float cfMaxVelY = 25.0f;						// Maximum player vertical velocity
	const float cfMaxFlyingVelX = 5.0f;					// Maximum player horizontal velocity while flying
	const float cfMaxFlyingVelY = 5.0f;					// Maximum player vertical velocity while flying
	const float cfAccGrdX = 25.0f;						// Acceleration value when on ground
	const float cfAccAirX = 28.0f;						// Acceleration value when in the air
	const float cfAccIcedGrdX = 8.0f;					// Acceleration value when on iced ground
	const float cfAccIcedAirX = 10.0f;					// Acceleration value when in the air after walking on iced platform
	const float cfJumpAcceleration = -8.0f;				// Jump speed
	const float cfJumpDecelerationRatio = 1.5f;			// Jump Deceleration while holding jump
	const float cfSlapSpawnT = 2.0f;					// Number of frames to spawn slap attack
	const float cfSlapDuration = 0.1f;					// duration of the slap AOE
	const float cfJesusCrossSpawnT = 12.0f;				// Number of frames to spawn jesus cross
	const float cfJesusCrossVelX = 10.0f;				// X Velocity of the jesus cross
	const float cfJesusCrossVelY = -7.0f;				// Y Velocity of the jesus cross
	const float cfJesusCrossDuration = 10.0f;			// Duration of the jesus cross
	const float cfVacuumAnimT = 4.0f;					// Number of frames when vacuum animation change from begin vacuum to vacuum
	const float cfGrabbingCameraAnimT = 6.0f;
	const float cfDeadRotationAnimation = 15.0f;		// rotation speed of dead animation
	const float cfExplosionAnimation = 3.0f;			// Explosion after 3 seconds
	const float cfExplosionDuration = 0.528f;			// Duration of the explosion
	const float cfExplosionOffsetX = -0.6f;				// offset X position for the explosion
	const float cfExplosionOffsetY = -1.0f;				// offset Y position for the explosion
	const float cfDeadAnimation = 4.0f;					// Duration of the dead animation
	const float cfGoAwayRotationAnimation = 15.0f;		// rotation speed of Kirbo goes away animation
	const float cfGoAwayTranslationAnimation = 256.0f;	// speed of kirbo in go away animation
	const float cfInvulnerabilityTickingSpeed = 0.2f;	// Ticking Speed when invulnerable
	const float cfInvulnerabilityCandy = 30.0f;			// Invulnerability after taking a candy
	const float cfInvulnerabilityFrame = 2.0f;			// Invulnerability frame after a hit
	const float cfDamageEjectionVelX = 4.0f;			// horizontal velocity when hit by ennemy
	const float cfDamageEjectionVelY = 4.0f;			// vertical velocity when hit by ennemy
	const float cfJumpTime = 0.75f;						// The more you hold, the more kirbo jump high
	const float cfDblJumpTime = 0.5f;					// idem for double jump
	const float cfKnockBackDuration = 0.3f;				// Ennemies Knock back duration
	const float cfNormalDrag = -3.0f;					// Drag Value on normal ground
	const float cfIcedDrag = -0.5f;						// Drag Value on iced ground
	const float cfBrokenDoorFrame = 6.0f;				// Number of frames to break door
	const float cfFartDuration = 0.165f;				// Duration of the fart animation
	const float cfHalberdWindForce = 2.0f;				// Force of the wind effect on Halberd
	const float cfVacuumInvincibleFrame = 0.5f;			// a little invincibility frame when vacuuming
	const float cfWaitBeforeReenterDoor = 0.5f;			// a little time to prevent player from spamming doors

	const int cnSlapDmg = 3;							// damages of the slap attack
	const int cnJesusCrossDmg = 2;						// damages of the jesus cross
	const int cnCandyPowerDmg = 5;						// Damages when candy power activated

	float fPosX = 0.0f;
	float fPosY = 0.0f;
	float fTPX = 0.0f;
	float fTPY = 0.0f;
	float fVelX = 0.0f;
	float fVelY = 0.0f;
	float fAnimationTimer = 0.0f;
	float fInvulnerabilityTimer = 0.0f;
	float fFaceDir = 1.0f;
	float fHealth = 10.0f;
	float fInvulnerabilityTickingTimer = 0.0f;
	float fDeadAnimation = 0.0f;
	float fKirboGoesAwayTimer = 0.0f;
	float fWindDirection = 1.0f;
	float fWindPower = 25.0f;
	float fCollisionUpperLimit = 0.9f;
	float fCollisionLowerLimit = 0.1f;
	float fSwallowDistance = 0.25f;
	float fJumpTimer = 0.0f;
	float fCrushingObjVX = 0.0f;
	float fCrushingObjVY = 0.0f;
	float fDrag = -3.0f;
	float fAccGrdX = 25.0f;
	float fAccAirX = 28.0f;
	float fWaitBeforeReenterDoor = 0.0f;
	bool bOnGround = false;
	bool bOnIcedGround = false;
	bool bDoubleJump = false;
	bool bChargeJump = false;
	bool bChargeDoubleJump = false;
	bool bInteracting = false;
	bool bSlapping = false;
	bool bVacuuming = false;
	bool bHoldingCamera = false;
	bool bLaunchingJesusCross = false;
	bool bCanSpawnProjectile = false;
	bool bDamaged = false;
	bool bIsAttackable = true;
	bool bFlying = false;
	bool bShowKirbo = true;
	bool bDead = false;
	bool bSwallowing = false;
	bool bWind = false;
	bool bSwallowSound = false;
	bool bPoyo = false;
	bool bBreakDoor = false;
	bool bChangePos = false;
	bool bIsGrabbedByEnnemy = false;
	bool bForceInvincible = false;
	bool bForceInvisible = false;
	bool bHasCandyPower = false;
	bool bUsingFlyCmd = false;
	bool bRunningRight = false;
	bool bInvincibleBlink = true;
	bool bVacuumInvincible = false;
	bool bExplosion = false;
	bool bProjected = false;
	bool bFlySoundPlaying = false;
	int nDmgBoost = 1;
	int nDefBoost = 1;

	cAnimator* animPlayer;
	cHitbox* hitbox;
	cHitbox* vacuumHitbox;
};

#endif // !DEF_PLAYER
