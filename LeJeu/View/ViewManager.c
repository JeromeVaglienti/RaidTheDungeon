#include "ViewManager.h"
#include "KeyboardManager.h"
#include "GamePad.h"
#include "../Controller/State/State.h"
#include "../Controller/MainSceneController/MainSceneController.h"
#include "../Controller/MainSceneController/PlayerController/PlayerController.h"
#include "..\Controller\Transition\Transition.h"
#include "CustomXinput\CustomXinput.h"
#include "..\DevTools\NodeDisplayer.h"

#define DELAY_INPUT_GAMEPAD 0.14f
int nbPics = 0;

typedef struct
{
	GamePad data[2];
	GamePadComposure actualInput[2];
	sfBool twoPlayer;
	sfBool sameHardware;
	char IdSend;
	sfBool CanSendInput;
}ViewManager;

ViewManager viewManager;
ActionListener toSend[2];
int ID = 0;

void OnAwakeViewManager()
{
	CONTROLLER_STATE* controllerState = GetControllerState();
	//OnAwakeGamePad(viewManager.data);
	if (!controllerState[0].bConnected && !controllerState[1].bConnected)
	{
		ID = 0;
		viewManager.actualInput[0] = keyBoard;
		viewManager.actualInput[1] = none;
		viewManager.sameHardware = sfFalse;
	}
	else if (controllerState[0].bConnected && controllerState[1].bConnected)
	{
		viewManager.actualInput[0] = gamePad;
		viewManager.actualInput[1] = gamePad;
		viewManager.sameHardware = sfTrue;
	}
	else if (controllerState[0].bConnected && !controllerState[1].bConnected)
	{
		ID = 1;
		viewManager.actualInput[0] = gamePad;
		viewManager.actualInput[1] = keyBoard;
		viewManager.sameHardware = sfFalse;
	}
	else
	{
		ID = 0;
		viewManager.actualInput[0] = keyBoard;
		viewManager.actualInput[1] = gamePad;
		viewManager.sameHardware = sfFalse;
	}

	ResetActionListener(&toSend[0]);
	ResetActionListener(&toSend[1]);

	viewManager.twoPlayer = sfFalse;
	viewManager.CanSendInput = sfTrue;
	toSend[0].IDPlayer = 0;
	toSend[1].IDPlayer = -1;
}

int GetTypeHardware(const int idOfPlayer)
{
	if (idOfPlayer == 1)
	{
		viewManager.twoPlayer = sfTrue;
		toSend[1].IDPlayer = 1;
	}
	return viewManager.actualInput[idOfPlayer];
}

int GetTypeOfHardware(const int idOfPlayer)
{
	//Set second PlayertoSend True;
	if (idOfPlayer == 1)
	{
		viewManager.twoPlayer = sfTrue;
		toSend[1].IDPlayer = 1;
	}
	return(GetControllerState()[idOfPlayer].bConnected == sfTrue);
}

int GetPortID(const int idOfPlayer)
{
	return viewManager.data[idOfPlayer].portID;
}

void ChangeIDAndHardware()
{
	if (viewManager.actualInput[0] == gamePad && viewManager.actualInput[1] == gamePad)
	{
		viewManager.sameHardware = sfTrue;
	}
	else if (viewManager.actualInput[0] == keyBoard && viewManager.actualInput[1] == gamePad)
	{
		ID = 0;
		viewManager.sameHardware = sfFalse;
	}
	else if (viewManager.actualInput[1] == keyBoard && viewManager.actualInput[0] == gamePad)
	{
		ID = 1;
		viewManager.sameHardware = sfFalse;
	}
	else if (viewManager.actualInput[0] == gamePad && viewManager.actualInput[1] == none)
	{
		viewManager.sameHardware = sfFalse;
	}
	else
	{
		ID = 0;
		viewManager.sameHardware = sfFalse;
	}
}

void SetTypeOfHardware(const int idOfPlayer, GamePadComposure typeHardware)
{
	/*if (idOfPlayer == 0 && typeHardware == 3)
	{
		data[i].isAlreadyConnected = sfTrue;
		data[i].portID = i;
		data[i].gamePadID = sfJoystick_getIdentification(i);
	}*/
	printf("input 1 : %d input 2 : %d\n", viewManager.actualInput[0], viewManager.actualInput[1]);
	if (idOfPlayer == 1 && typeHardware == 3)
	{
		printf("nseinfgs\n");
		viewManager.actualInput[1] = none;
		viewManager.data[1].portID = -1;
		viewManager.twoPlayer = sfFalse;
		ChangeIDAndHardware();
		return;
	}
	if ((typeHardware == 2 && viewManager.actualInput[0] == gamePad && viewManager.actualInput[1] == gamePad) ||
		(idOfPlayer == 0 && typeHardware == gamePad && viewManager.actualInput[1] == gamePad && viewManager.data[1].portID == 0 && viewManager.data[0].portID != -1) ||
		(idOfPlayer == 1 && typeHardware == gamePad && viewManager.actualInput[0] == gamePad && viewManager.data[0].portID == 0 && viewManager.data[1].portID != -1))
	{
		short portID = viewManager.data[0].portID;
		printf("azeofsf\n");
		ReassignedJoystick(viewManager.data, 0, viewManager.data[1].portID);
		ReassignedJoystick(viewManager.data, 1, portID);
		ChangeIDAndHardware();
		return;
	}
	else if (typeHardware == 2 && viewManager.actualInput[idOfPlayer] == gamePad)
	{
		printf("je suis ta mer\n");
		ReassignedJoystick(viewManager.data, idOfPlayer, 1);
		ChangeIDAndHardware();
		return;
	}
	else if (typeHardware == 2)
	{
		printf("aaaaaaah\n");
		typeHardware = gamePad;
	}

	if ((idOfPlayer == 0 && typeHardware == gamePad && viewManager.data[idOfPlayer].portID == 1 && viewManager.actualInput[1] == gamePad) ||
		(idOfPlayer == 1 && typeHardware == gamePad && viewManager.data[idOfPlayer].portID == 1 && viewManager.actualInput[0] == gamePad))
	{
		short portID = viewManager.data[0].portID;
		printf("switch gamepad\n");
		ReassignedJoystick(viewManager.data, 0, viewManager.data[1].portID);
		ReassignedJoystick(viewManager.data, 1, portID);
		ChangeIDAndHardware();
		return;
	}
	else if (typeHardware == gamePad && viewManager.data[idOfPlayer].portID == 1)
	{
		printf("passage de 1 à 0\n");
		ReassignedJoystick(viewManager.data, idOfPlayer, 0);
		ChangeIDAndHardware();
		return;
	}
	if (idOfPlayer == 0 && typeHardware == gamePad && viewManager.actualInput[1] == gamePad && viewManager.data[0].portID == 0 && viewManager.data[1].portID == 0)
	{
		printf("1111\n");
		viewManager.actualInput[idOfPlayer] = typeHardware;
		viewManager.actualInput[1] = keyBoard;
		ReassignedJoystick(viewManager.data, idOfPlayer, viewManager.data[1].portID);
	}
	else if (idOfPlayer == 0 && typeHardware == keyBoard && viewManager.actualInput[1] == keyBoard)
	{
		printf("2222\n");
		viewManager.actualInput[idOfPlayer] = typeHardware;
		viewManager.actualInput[1] = gamePad;
		ReassignedJoystick(viewManager.data, 1, viewManager.data[0].portID);
	}
	else if (idOfPlayer == 1 && typeHardware == gamePad && viewManager.actualInput[0] == gamePad && viewManager.data[0].portID == 0)
	{
		printf("3333\n");
		viewManager.actualInput[idOfPlayer] = typeHardware;
		viewManager.actualInput[0] = keyBoard;
		ReassignedJoystick(viewManager.data, idOfPlayer, viewManager.data[0].portID);
	}
	else if (idOfPlayer == 1 && typeHardware == keyBoard && viewManager.actualInput[0] == keyBoard)
	{
		printf("4444\n");
		viewManager.actualInput[idOfPlayer] = typeHardware;
		viewManager.actualInput[0] = gamePad;
		ReassignedJoystick(viewManager.data, 0, viewManager.data[1].portID);
	}
	else
	{
		printf("5555\n");
		viewManager.actualInput[idOfPlayer] = typeHardware;
	}
	ChangeIDAndHardware();
	/*
	if (viewManager.data[0].portID == -1 && viewManager.actualInput[1] != keyBoard)
	{
		viewManager.actualInput[0] = keyBoard;
	}
	if (viewManager.data[1].portID == -1 && viewManager.actualInput[0] != keyBoard)
	{
		viewManager.actualInput[1] = keyBoard;
	}
	*/
}

sfBool IsSameHardware()
{
	return viewManager.sameHardware;
}

char GetIdSend()
{
	return viewManager.IdSend;
}

void SetIdSend(const char id)
{
	viewManager.IdSend = id;
}

void SetCanSendInput(const sfBool canSend)
{
	viewManager.CanSendInput = canSend;
}

sfBool CanSendInput()
{
	return viewManager.CanSendInput;
}

void UpdateViewManager(sfRenderWindow* mainWindow, const float deltaTime, int(*callbackbUpdateMenu)(EventList*, int))
{
	static sfEvent event;
	static float delayCount = 0.0f;
	static sfBool delayIsOn = sfFalse;
	static int indexMenu = 0;
	ConnectionGamePads(viewManager.data);
	sfBool isPlayer1 = sfFalse;
	sfBool isPlayer2 = sfFalse;

	if (sfKeyboard_isKeyPressed(sfKeyF1))
	{
		TakeScreenShot(mainWindow, "ScreenShots");

	}

	sfEvent eventGarbage;

	switch (GetActualScene())
	{
	case(introduction):
		for (int i = 0; i < 2; i++)
		{
			if (JoystickIsButtonPressed(viewManager.data[i].portID, START))
			{
				SetScene(mainMenu);
				PlayMusic(mainMenuMusic);
			}
		}
	case(loading):
		UpdateTransition(deltaTime, GetActualScene());
		while (sfRenderWindow_pollEvent(mainWindow, &eventGarbage)) {}
		break;
	case(mainMenu):
		UpdateUI(deltaTime);

		//PutDelay for gamePad
		if (!delayIsOn)
		{
			delayIsOn = sfTrue;
		}
		else
		{
			delayCount += deltaTime;
			if (delayCount < DELAY_INPUT_GAMEPAD)
			{
				break;
			}
			else
			{
				delayCount = 0.0f;
			}
		}

		EventList *FirstEvent = NULL;
		sfEvent event;
		while (sfRenderWindow_pollEvent(mainWindow, &event))
		{
			AddElement(&FirstEvent, event);
		}

		//2players
		if (viewManager.actualInput[1] != none)
		{
			SetIdSend(0);
			callbackbUpdateMenu(FirstEvent, viewManager.actualInput[0]);
			SetIdSend(1);
			callbackbUpdateMenu(FirstEvent, viewManager.actualInput[1]);
		}
		else //one player
		{
			SetIdSend(0);
			callbackbUpdateMenu(FirstEvent, viewManager.actualInput[0]);
		}
		CleanEventList(&FirstEvent);
		break;
	case(mainScene):
		//PutDelay for gamePad
		if (GetState().isPaused)
		{
			if (!delayIsOn)
			{
				delayIsOn = sfTrue;
			}
			else
			{
				delayCount += deltaTime;
				if (delayCount < DELAY_INPUT_GAMEPAD)
				{
					break;
				}
				else
				{
					delayCount = 0.0f;
				}
			}

			EventList *FirstEvent = NULL;
			sfEvent event;
			while (sfRenderWindow_pollEvent(mainWindow, &event))
			{
				AddElement(&FirstEvent, event);
			}

			//2players
			if (viewManager.actualInput[1] != none)
			{
				SetIdSend(0);
				callbackbUpdateMenu(FirstEvent, viewManager.actualInput[0]);
				SetIdSend(1);
				callbackbUpdateMenu(FirstEvent, viewManager.actualInput[1]);
			}
			else //one player
			{
				SetIdSend(0);
				callbackbUpdateMenu(FirstEvent, viewManager.actualInput[0]);
			}
			CleanEventList(&FirstEvent);
		}
		else if ((!viewManager.twoPlayer || viewManager.sameHardware || viewManager.actualInput[1] == none) && viewManager.CanSendInput)
		{
			//Check input since they have the same hardware
			switch (viewManager.actualInput[0])
			{
			case(keyBoard):
				while (sfRenderWindow_pollEvent(mainWindow, &event))
				{
					UpdateKeyboardMouseEvt(event, toSend, 0, (viewManager.twoPlayer + 1));
				}
				UpdateKeyboardMouse(mainWindow, toSend, 0);
				break;
			case(gamePad):
				while (sfRenderWindow_pollEvent(mainWindow, &event))
				{
					UpdateGamePadsEvt(event, viewManager.data, toSend, (viewManager.twoPlayer + 1));
				}
				UpdateGamePads(viewManager.data, toSend, (viewManager.twoPlayer + 1));
				break;
			default:
				break;
			}
			break;
		}
		else if (viewManager.CanSendInput)
		{
			//Both player have different hardware we check each event with both event
			while (sfRenderWindow_pollEvent(mainWindow, &event))
			{
				UpdateKeyboardMouseEvt(event, toSend, ID, (viewManager.twoPlayer + 1));
				UpdateGamePadsEvt(event, viewManager.data, toSend, (viewManager.twoPlayer + 1));
			}
			UpdateKeyboardMouse(mainWindow, toSend, ID);
			UpdateGamePads(viewManager.data, toSend, (viewManager.twoPlayer + 1));
		}

		if (viewManager.twoPlayer)
		{
			ResetActionListener(&toSend[0]);
			ResetActionListener(&toSend[1]);
		}
		else
		{
			ResetActionListener(&toSend[0]);
		}
		break;
	case worldMap:
		if (!delayIsOn)
		{
			delayIsOn = sfTrue;
		}
		else
		{
			delayCount += deltaTime;
			if (delayCount < DELAY_INPUT_GAMEPAD)
			{
				break;
			}
			else
			{
				delayCount = 0.0f;
			}
		}

		/*EventList *FirstEvent2 = NULL;*/
		sfEvent event2;
		/*while (sfRenderWindow_pollEvent(mainWindow, &event2))
		{
			AddElement(&FirstEvent2, event2);
		}*/

		switch (viewManager.actualInput[0])
		{
		case(keyBoard):
			while (sfRenderWindow_pollEvent(mainWindow, &event2))
			{
				UpdateKeyboardMouseEvt(event2, toSend, 0, (viewManager.twoPlayer + 1));
				default:
					while (sfRenderWindow_pollEvent(mainWindow, &event2))
					{
						UpdateKeyboardMouseEvt(event2, toSend, 0, (viewManager.twoPlayer + 1));
					}
					break;
			}
			break;
		}
		//CleanEventList(&FirstEvent2);
		break;
	default:
		while (sfRenderWindow_pollEvent(mainWindow, &eventGarbage)) {}
		break;
	}
}