#ifndef ZIX_PGE_CONTROLLER
#define ZIX_PGE_CONTROLLER

#pragma once

#ifdef _WIN32
#include <windows.h>
#include <xinput.h>

typedef DWORD(WINAPI XInputGetState_t)(DWORD dwUserIndex, XINPUT_STATE* pState);
static XInputGetState_t* XInputStateGet;

typedef DWORD(WINAPI XInputSetState_t)(DWORD dwUserIndex, XINPUT_VIBRATION* pVibration);
static XInputSetState_t* XInputStateSet;
#endif

#define C_BUTTON_COUNT 14
enum CButton
{
	UP,
	DOWN,
	LEFT,
	RIGHT,
	START,
	BACK,
	A,
	B,
	X,
	Y,
	LEFT_SHOULDER,
	RIGHT_SHOULDER,
	LEFT_THUMB,
	RIGHT_THUMB
};

struct CBState
{
	bool bPressed = false;
	bool bReleased = false;
	bool bHeld = false;
};

class ControllerManager
{
private:
	bool buttonState[C_BUTTON_COUNT];
	bool lastButtonState[C_BUTTON_COUNT];

	// Trigger values are in the range of 0 to 1, where 0 is fully
	// released and 1 is fully pressed.
	float triggerLeft = 0;
	float triggerRight = 0;

	// Stick values are in the range of -1 to 1. For X values, -1 is
	// all the way to the left while +1 is all the way to the right.
	float leftStickX = 0;
	float leftStickY = 0;
	float rightStickX = 0;
	float rightStickY = 0;

	// Whether or not the controller is plugged in.
	bool pluggedIn = true;

	bool vibrating = false;
	float vibrateTime = 0;
	float vibrateCounter = 0;

public:
	bool Initialize();
	void Update(float dt);

	void Vibrate(short amt, int timeMs);

	CBState GetButton(CButton button);

	float GetLeftTrigger();
	float GetRightTrigger();

	float GetLeftStickX();
	float GetLeftStickY();

	float GetRightStickX();
	float GetRightStickY();

	bool IsVibrating();
	bool IsPluggedIn();

private:
	float NormalizeStickValue(short value);
};

#endif // !ZIX_PGE_CONTROLLER
