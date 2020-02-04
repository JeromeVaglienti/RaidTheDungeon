#include "GamePad.h"
#include "..\Controller\State\State.h"
#include "ViewManager.h"
#include "..\Controller\Sounds\Sounds.h"

/*typedef enum
{
A,
B,
X,
Y,
LB,
RB,
SELECT,
START,
L3,
R3
}buttonJoystick;*/

//Call this fonction one time in the game
//Assign Different GamePad That are already Connected
void OnAwakeGamePad(GamePad gamepads[])
{
	//gamepads[0].isConnected = gamepads[1].isConnected = sfFalse;
	//UpdateControllerState();
	//sfJoystick_update();
	int cpt = 0;
	for (int i = 0; i < 2; i++)
	{
		if (!gamepads[i].isAlreadyConnected)
		{
			gamepads[i].gamePadID = sfJoystick_getIdentification(i);
			gamepads[i].speedLeft.x = gamepads[i].speedLeft.y = 0.0f;
			gamepads[i].speedRight.x = gamepads[i].speedRight.y = 0.0f;
			gamepads[i].isAlreadyConnected = sfFalse;
			gamepads[i].spellSelectedIndex = 0;
			gamepads[i].leftTrigger = 0;
			gamepads[i].rightTrigger = 0;
			gamepads[i].portID = -1;

		}
	}
}

void ConnectionGamePads(GamePad data[])
{
	OnAwakeGamePad(data);
	CONTROLLER_STATE* controllerState = GetControllerState();
	Scene scene = GetActualScene();
	if (controllerState[0].bConnected)
	{
		if ((!data[0].isAlreadyConnected && !data[1].isAlreadyConnected) ||
			(!data[0].isAlreadyConnected && data[1].isAlreadyConnected && data[1].portID != 0))
		{
			printf("connect 0 for p1\n");
			data[0].isAlreadyConnected = sfTrue;
			data[0].portID = 0;
			data[0].gamePadID = sfJoystick_getIdentification(0);
			OnAwakeViewManager();
		}
		if (data[0].isAlreadyConnected && !data[1].isAlreadyConnected && data[0].portID != 0)
		{
			printf("connect 0 for p2\n");
			data[1].isAlreadyConnected = sfTrue;
			data[1].portID = 0;
			data[1].gamePadID = sfJoystick_getIdentification(0);
			OnAwakeViewManager();
		}
	}

	if (controllerState[1].bConnected)
	{
		if ((!data[0].isAlreadyConnected && !data[1].isAlreadyConnected) ||
			(!data[0].isAlreadyConnected && data[1].isAlreadyConnected && data[1].portID != 1))
		{
			printf("connect 1 for p1\n");
			data[0].isAlreadyConnected = sfTrue;
			data[0].portID = 1;
			data[0].gamePadID = sfJoystick_getIdentification(1);
			OnAwakeViewManager();
		}
		if (data[0].isAlreadyConnected && !data[1].isAlreadyConnected && data[0].portID != 1)
		{
			printf("connect 1 for p2\n");
			data[1].isAlreadyConnected = sfTrue;
			data[1].portID = 1;
			data[1].gamePadID = sfJoystick_getIdentification(1);
			OnAwakeViewManager();
		}
	}

	if (!controllerState[0].bConnected)
	{
		if (data[0].isAlreadyConnected && data[0].portID == 0)
		{
			printf("disconnect 0 for p1\n");
			data[0].isAlreadyConnected = sfFalse;
			data[0].portID = -1;
			if (scene == mainScene)
			{
				SetStatePause(sfTrue);
			}
			OnAwakeViewManager();
		}
		else if (data[1].isAlreadyConnected && data[1].portID == 0)
		{
			printf("disconnect 0 for p2\n");
			data[1].isAlreadyConnected = sfFalse;
			data[1].portID = -1;
			if (scene == mainScene)
			{
				SetStatePause(sfTrue);
			}
			OnAwakeViewManager();
		}
	}
	if (!controllerState[1].bConnected)
	{
		if (data[0].isAlreadyConnected && data[0].portID == 1)
		{
			printf("disconnect 1 for p1\n");
			data[0].isAlreadyConnected = sfFalse;
			data[0].portID = -1;
			if (scene == mainScene)
			{
				SetStatePause(sfTrue);
			}
			OnAwakeViewManager();
		}
		else if (data[1].isAlreadyConnected && data[1].portID == 1)
		{
			data[1].isAlreadyConnected = sfFalse;
			data[1].portID = -1;
			printf("disconnect 1 for p2\n");
			if (scene == mainScene)
			{
				SetStatePause(sfTrue);
			}
			OnAwakeViewManager();
		}
	}

	/*for (int i = 0; i < 2; i++)
	{
		if (controllerState[i].bConnected && !data[i].isAlreadyConnected)
		{
			printf("connect port %d\n", i);
			data[i].isAlreadyConnected = sfTrue;
			data[i].portID = i;
			data[i].gamePadID = sfJoystick_getIdentification(i);
			OnAwakeViewManager();
		}
		if (!controllerState[data[i].portID].bConnected && data[i].isAlreadyConnected)
		{
			printf("port %d\n", data[i].portID);
			printf("disconnect port %d\n", i);
			data[i].isAlreadyConnected = sfFalse;
			data[i].portID = -1;
			if (GetActualScene() == mainScene)
			{
				SetStatePause(sfTrue);
			}
			OnAwakeViewManager();
		}
	}*/
	UpdateControllerState();
}

void ReassignedJoystick(GamePad data[], const int idPlayer, const short portID)
{
	sfJoystickIdentification joystickId = sfJoystick_getIdentification(portID);
	data[idPlayer].isAlreadyConnected = sfTrue;
	data[idPlayer].portID = portID;
	data[idPlayer].gamePadID = joystickId;
	if (idPlayer == 0 && data[1].portID == portID)
	{
		data[1].isAlreadyConnected = sfTrue;
		data[1].portID = -1;
		data[1].gamePadID = joystickId;
	}
	if (idPlayer == 1 && data[0].portID == portID)
	{
		data[0].isAlreadyConnected = sfTrue;
		data[0].portID = -1;
		data[0].gamePadID = joystickId;
	}
}

//Event Gestion
void UpdateGamePadsEvt(sfEvent event, GamePad data[], ActionListener toUpdate[2], const int numberOfPlayers)
{
	/*	int id;
		if (IsSameHardware())
		{
			id = event.joystickConnect.joystickId;
		}
		else
		{
			id = GetTypeHardware(0) == gamePad ? 0 : 1;
		}*/
	for (int i = 0; i < numberOfPlayers; i++)
	{
		//if (GetControllerState()[i].bConnected)
		if (GetTypeHardware(i) == gamePad && (event.type == sfEvtJoystickButtonPressed))
		{
			printf("%d\n", data[i].portID);
			if (JoystickIsButtonPressed(data[i].portID, RB))
			{
				if (data[i].spellSelectedIndex < 2)
				{
					data[i].spellSelectedIndex++;
				}
				else
				{
					data[i].spellSelectedIndex = 0;
				}
				toUpdate[i].ChangingWeapon = data[i].spellSelectedIndex;
			}

			if (JoystickIsButtonPressed(data[i].portID, LB))
			{
				if (data[i].spellSelectedIndex > 0)
				{
					data[i].spellSelectedIndex--;
				}
				else
				{
					data[i].spellSelectedIndex = 2;
				}
				toUpdate[i].ChangingWeapon = data[i].spellSelectedIndex;
			}

			if (JoystickIsButtonPressed(data[i].portID, START))
			{
				if (GetState().isPaused)
				{
					SetStatePause(sfFalse);
				}
				else
				{
					SetStatePause(sfTrue);
				}
			}

			if (JoystickIsButtonPressed(data[i].portID, Y))
			{
				if (IsEnableMap())
				{
					SetEnableMap(sfFalse);
				}
				else
				{
					SetEnableMap(sfTrue);
				}
				SetAlreadyEnableMap(sfFalse);
			}
		}
	}
}

//Real time Event Gestion
void UpdateGamePads(GamePad data[], ActionListener toUpdate[2], const int numberOfPlayers)
{
	for (int i = 0; i < numberOfPlayers; i++)
	{
		if (GetTypeHardware(i) == gamePad)
		{
			GetSticksPosition(data[i].portID, &data[i].speedLeft, &data[i].speedRight);
			GetTriggers(data[i].portID, &data[i].leftTrigger, &data[i].rightTrigger);
			if (data[i].speedRight.x > 50 || data[i].speedRight.x < -50 || data[i].speedRight.y < -50 || data[i].speedRight.y > -50)
			{
				toUpdate[i].isFiring = sfTrue;
			}
			else
			{
				toUpdate[i].isFiring = sfFalse;
			}

			if (data[i].leftTrigger)
			{
				toUpdate[i].isCastSpell = sfTrue;
			}
			else
			{
				toUpdate[i].isCastSpell = sfFalse;
			}
			if (data[i].rightTrigger)
			{
				toUpdate[i].isDashing = sfTrue;
			}
			else
			{
				toUpdate[i].isDashing = sfFalse;
			}
			TakeInputMovement(data[i].speedLeft, toUpdate[i].IDPlayer);
			TakeInputViewMovement(data[i].speedRight, toUpdate[i].IDPlayer);
			TakeInputAction(data[i].speedRight, toUpdate[i]);
		}
	}
}

