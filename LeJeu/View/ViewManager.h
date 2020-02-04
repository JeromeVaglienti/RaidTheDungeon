#ifndef _VIEWMANAGER_H
#define _VIEWMANAGER_H

#include <stdio.h>
#include <stdlib.h>
#include <SFML\Graphics.h>
#include "EventList\EventList.h"
typedef enum
{
	keyBoard,
	gamePad,
	none
}GamePadComposure;
//Called when the game is launched checking and assigning hardware connected 
void OnAwakeViewManager();
//Get the hardware's type for each player
int GetTypeHardware(const int idOfPlayer);
int GetTypeOfHardware(const int idOfPlayer);
int GetPortID(const int idOfPlayer);
void ChangeIDAndHardware();
void SetTypeOfHardware(const int idOfPlayer, GamePadComposure typeHardware);
sfBool IsSameHardware();
int GetPortID(const int idOfPlayer);
char GetIdSend();
void SetIdSend(const char id);
void SetCanSendInput(const sfBool canSend);
sfBool CanSendInput();
//Update ViewManager each frame to send to corresponding hardware API to handle user inputs
void UpdateViewManager(sfRenderWindow* mainWindow, const float deltaTime, int(*callbackbUpdateMenu)(EventList*, int));

#endif _VIEWMANAGER_H