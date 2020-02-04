#include "CustomXinput.h"
#include <commdlg.h>
#include <basetsd.h>
#include <objbase.h>
#include <stdio.h>





CONTROLLER_STATE g_Controllers[MAX_CONTROLLERS];

void GetSticksPosition(int joyNum, sfVector2f* left, sfVector2f* right)
{
	XINPUT_STATE state;
	ZeroMemory(&state, sizeof(XINPUT_STATE));

	XInputGetState(joyNum, &state);
	// Check for the "DEAD ZONE"
	// Left Stick
	if ((state.Gamepad.sThumbLX < INPUT_DEADZONE &&
		state.Gamepad.sThumbLX > -INPUT_DEADZONE) &&
		(state.Gamepad.sThumbLY < INPUT_DEADZONE &&
			state.Gamepad.sThumbLY > -INPUT_DEADZONE)) 
	{

		state.Gamepad.sThumbLX = 0;
		state.Gamepad.sThumbLY = 0;

	}

	// Right Stick
	if ((state.Gamepad.sThumbRX < INPUT_DEADZONE &&
		state.Gamepad.sThumbRX > -INPUT_DEADZONE) &&
		(state.Gamepad.sThumbRY < INPUT_DEADZONE &&
			state.Gamepad.sThumbRY > -INPUT_DEADZONE)) 
	{

		state.Gamepad.sThumbRX = 0;
		state.Gamepad.sThumbRY = 0;

	}

	// Convert values to SFML style (-100..100)
	left->x = (float)(state.Gamepad.sThumbLX / 327);
	left->y = -(float)(state.Gamepad.sThumbLY / 327);
	right->x = (float)(state.Gamepad.sThumbRX / 327);
	right->y = -(float)(state.Gamepad.sThumbRY / 327);

}

// This function returns nothing
// It fills the variables left and right with the current state of the triggers (LT and RT)
// The values will always be in the range 0..1
// TODO: TAKE CARE OF THE DEAD ZONE ??????????????????????????????????
void GetTriggers(int joyNum, float* left, float* right)
{

	XINPUT_STATE state;
	ZeroMemory(&state, sizeof(XINPUT_STATE));

	XInputGetState(joyNum, &state);

	// Normalize and take care of the Dead Zone
	*left = (float)(state.Gamepad.bLeftTrigger) / 255;
	*right = (float)(state.Gamepad.bRightTrigger) / 255;

}

bool JoystickIsButtonPressed(int joyNum, int buttonNum)
{

	XINPUT_STATE state;
	ZeroMemory(&state, sizeof(XINPUT_STATE));

	XInputGetState(joyNum, &state);

	return  (state.Gamepad.wButtons & buttonNum);

}

// Set vibration (0.0 to 1.0)
// 0 stops the vibration
void SetVibration(int joyNum, float leftMotor, float rightMotor)
{

	XINPUT_VIBRATION vib;
	ZeroMemory(&vib, sizeof(XINPUT_VIBRATION));

	vib.wLeftMotorSpeed = (WORD)(leftMotor  * 65535.0f);
	vib.wRightMotorSpeed = (WORD)(rightMotor * 65535.0f);

	XInputSetState(joyNum, &vib);

}

void UpdateControllerState()
{
	DWORD dwResult;
	for (DWORD i = 0; i < MAX_CONTROLLERS; i++)
	{
		// Simply get the state of the controller from XInput.
		dwResult = XInputGetState(i, &g_Controllers[i].state);

		if (dwResult == ERROR_SUCCESS)
			g_Controllers[i].bConnected = true;
		else
			g_Controllers[i].bConnected = false;
	}
}

CONTROLLER_STATE* GetControllerState()
{
	return g_Controllers;
}