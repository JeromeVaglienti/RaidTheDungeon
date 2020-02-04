#ifndef _GAMEPAD_H
#define _GAMEPAD_H

#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include "..\View\CustomXinput\CustomXinput.h"
#include <SFML\Graphics.h>
#include "..\Controller\MainSceneController\PlayerController\PlayerController.h"

typedef struct
{
	sfJoystickIdentification gamePadID;
	sfVector2f speedLeft;
	sfVector2f speedRight;
	sfBool isAlreadyConnected;
	short portID;
	unsigned short spellSelectedIndex;
	float leftTrigger;
	float rightTrigger;
} GamePad;

void OnAwakeGamePad(GamePad gamepads[]);
void ConnectionGamePads(GamePad data[]);
void ReassignedJoystick(GamePad data[], const int idPlayer, const short portID);
void UpdateGamePadsEvt(sfEvent event, GamePad data[], ActionListener toUpdate[2], const int numberOfPlayers);
void UpdateGamePads(GamePad data[], ActionListener toUpdate[2], const int numberOfPlayers);

//ActionListener UpdateGamePadsAction(GamePad data[], ActionListener *toUpdate);

#endif // !_GAMEPAD_H
