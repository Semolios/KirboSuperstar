#pragma once
#include "Platformer_Animator.h"
#include "Platformer_Camera.h"
#include "Platformer_Hitbox.h"
#include "Platformer_Level.h"

class OneLoneCoder_Platformer;

class cPlayer
{
public:
	cPlayer(cAnimator* animator);

private:
	const float cfMaxHealth = 10.0f;					// Max Kirbo health
	const float cfPlayerVelY = 5.0f;					// Up and Down velocities
	const float cfMinPlayerVelX = 1.0f;					// Minimum player horizontal velocity
	const float cfMaxPlayerVelX = 7.0f;					// Maximum player horizontal velocity
	const float cfMaxPlayerVelY = 100.0f;				// Maximum player vertical velocity
	const float cfMaxPlayerFlyingVelX = 5.0f;			// Maximum player horizontal velocity while flying
	const float cfMaxPlayerFlyingVelY = 5.0f;			// Maximum player vertical velocity while flying
	const float cfPlayerAccGrdX = 25.0f;				// Acceleration value when on ground
	const float cfPlayerAccAirX = 15.0f;				// Acceleration value when in the air
	const float cfJumpAcceleration = -8.0f;				// Jump speed
	const float cfJumpDecelerationRatio = 1.5f;			// Jump Deceleration while holding jump
	const float cfSlapSpawnT = 1.0f;					// Number of frames to spawn slap attack
	const float cfSlapDuration = 0.1f;					// duration of the slap AOE
	const float cfJesusCrossSpawnT = 7.0f;				// Number of frames to spawn jesus cross
	const float cfJesusCrossVelX = 10.0f;				// X Velocity of the jesus cross
	const float cfJesusCrossVelY = -10.0f;				// Y Velocity of the jesus cross
	const float cfJesusCrossDuration = 10.0f;			// Duration of the jesus cross
	const float cfVacuumAnimT = 2.0f;					// Number of frames when vacuum animation change to begin vacuum to vacuum
	const float cfDeadRotationAnimation = 15.0f;		// rotation speed of dead animation
	const float cfDeadAnimation = 4.0f;					// Duration of the dead animation
	const float cfGoAwayRotationAnimation = 15.0f;		// rotation speed of Kirbo goes away animation
	const float cfGoAwayTranslationAnimation = 256.0f;	// speed of kirbo in go away animation
	const float cfInvulnerabilityTickingSpeed = 0.2f;	// Ticking Speed when invulnerable
	const float cfInvulnerabilityFrame = 4.0f;			// Invulnerability frame after a hit
	const float cfDamageEjectionVelX = 4.0f;			// horizontal velocity when hit by ennemy
	const float cfDamageEjectionVelY = 4.0f;			// vertical velocity when hit by ennemy
	const float cfJumpTime = 0.75f;						// The more you hold, the more kirbo jump high
	const float cfDblJumpTime = 0.5f;					// idem for double jump
	const float cfKnockBackDuration = 0.3f;				// Ennemies Knock back duration

	const int cnSlapDmg = 3;							// damages of the slap attack
	const int cnJesusCrossDmg = 5;						// damages of the jesus cross

	float fPlayerPosX = 0.0f;
	float fPlayerPosY = 0.0f;
	float fPlayerVelX = 0.0f;
	float fPlayerVelY = 0.0f;
	float fAnimationTimer = 0.0f;
	float fInvulnerabilityTimer = 0.0f;
	float fFaceDir = 1.0f;
	float fHealth = 10.0f;
	float fInvulnerabilityTickingTimer = 0.0f;
	float fDeadAnimation = 0.0f;
	float fKirboGoesAwayTimer = 0.0f;
	float fWindDirection = 1.0f;
	float fWindPower = 25.0f;
	float fPlayerCollisionUpperLimit = 0.9f;
	float fPlayerCollisionLowerLimit = 0.1f;
	float fSwallowDistance = 0.1f;
	float fJumpTimer = 0.0f;
	bool bPlayerOnGround = false;
	bool bDoubleJump = false;
	bool bChargeJump = false;
	bool bChargeDoubleJump = false;
	bool bAttacking = false;
	bool bSlapping = false;
	bool bVacuuming = false;
	bool bLaunchingJesusCross = false;
	bool bCanSpawnProjectile = false;
	bool bPlayerDamaged = false;
	bool bIsPlayerAttackable = true;
	bool bFlying = false;
	bool bShowKirbo = true;
	bool bDead = false;
	bool bSwallowing = false;
	bool bWind = false;

	cAnimator* animPlayer;

public:
	static OneLoneCoder_Platformer* engine;

public:
	void InitialiseKirboHealth();
	bool IsDead();
	void HandleInput(olc::PixelGameEngine* gfx, float fElapsedTime, cCamera* camera);
	bool CanInteract();
	void ApplyGravity(float fElapsedTime);
	void Update(float fElapsedTime);
	float GetFaceDir();
	void OneCycleAnimations(float fElapsedTime, olc::GFX2D::Transform2D* t, std::map<std::string, std::vector<olc::Sprite*>> mapProjectiles);
	void StopAnyAttack();
	void ClampVelocities();
	void IncreaseVelocities(float dvx, float dvy);
	void DecreaseVelocities(float dvx, float dvy);
	void Collisions(float fElapsedTime, cLevel* lvl);
	float GetPlayerPosX();
	float GetPlayerPosY();
	void SetPlayerPosX(float px);
	void SetPlayerPosY(float py);
	bool IsVacuuming();
	void VacuumEnnemy(cDynamicCreature* object);
	bool IsAttackable();
	bool IsSwallowing();
	void UpdateInvulnerability(float fElapsedTime);
	void DrawKirbo(olc::PixelGameEngine* gfx, olc::GFX2D::Transform2D t);
	float GetHealth();
	void Damage(cDynamic* object);
	void ResetVariables();
	cHitbox VacuumHitbox(float cameraOffsetX, float cameraOffsetY);
	cHitbox Hitbox(float cameraOffsetX, float cameraOffsetY);
	void Attack(cDynamicCreature* victim, int damage);
	void Vacuum(cDynamicCreature* object, float cameraOffsetX, float cameraOffsetY);
	void CheckIfDamaged(cDynamic* object, float angle, float fOffsetX, float fOffsetY);
};
