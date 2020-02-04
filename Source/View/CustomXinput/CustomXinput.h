#ifndef CUSTOMXINPUT_H
#define CUSTOMXINPUT_H
#include <windows.h>
#include "SFML\System\Vector2.h"
#include <stdbool.h>

#define WIN32_LEAN_AND_MEAN
#ifdef USE_DIRECTX_SDK
#include <C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\include\xinput.h>
#pragma comment(lib,"xinput.lib")
#elif (_WIN32_WINNT >= 0x0602 /*_WIN32_WINNT_WIN8*/)
#include <XInput.h>
#pragma comment(lib,"xinput.lib")
#else
#include <XInput.h>
#pragma comment(lib,"xinput9_1_0.lib")

#endif
// Enums
#define MAX_CONTROLLERS 2  // XInput handles up to 4 controllers 
#define INPUT_DEADZONE  ( 0.24f * 0x7FFF )  // Default to 24% of the +/- 32767 range.   This is a reasonable default value but can be altered if needed.

typedef struct CONTROLLER_STATE
{
	XINPUT_STATE state;
	bool bConnected;
}CONTROLLER_STATE;

typedef enum NBPLAYER{
	Count = 4       // Player 0-3
}Nbplayer;

typedef enum ControllerKey {
	DPAD_UP = 0x0001,
	DPAD_DOWN = 0x0002,
	DPAD_LEFT = 0x0004,
	DPAD_RIGHT = 0x0008,
	START = 0x0010,
	BACK = 0x0020,
	LEFT_THUMB = 0x0040,
	RIGHT_THUMB = 0x0080,
	LB = 0x0100,
	RB = 0x0200,
	A = 0x1000,
	B = 0x2000,
	X = 0x4000,
	Y = 0x8000,
}ControllerKey;

void UpdateControllerState();
//Set and get the left/right stick position between 100 and -100 
void GetSticksPosition(int joyNum, sfVector2f* left, sfVector2f* right);
//Set and get the left/right trigger between 0 and 100
void GetTriggers(int joyNum,  float* left,  float* right);
//Set vibration of left and right motor with the right id controller
void SetVibration(int joyNum, float leftMotor, float rightMotor);
bool JoystickIsButtonPressed(int joyNum, int buttonNum);
CONTROLLER_STATE* GetControllerState();
#endif // X360_CONTROLLER_HPP
