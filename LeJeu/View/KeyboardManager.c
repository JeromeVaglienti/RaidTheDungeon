#include "KeyboardManager.h"
#include "../Model/Math/MathUtilities.h"
#include "..\Controller\State\State.h"
#include "..\Controller\HUD\MiniMap\MiniMap.h"
#include "MapKey\MapKey.h"
#include "..\Controller\Serialization\Serialization.h"


//Event gestion
void UpdateKeyboardMouseEvt(sfEvent event, ActionListener toUpdate[2], const int ID, const int numberOfPlayers)
{
	if (event.type == sfEvtKeyPressed)
	{
		if (event.key.code == GetMapKey(SelectFireBall))
		{
			toUpdate[ID].ChangingWeapon = 0;
		}

		if (event.key.code == GetMapKey(SelectConeOfFire))
		{
			toUpdate[ID].ChangingWeapon = 1;
		}

		if (event.key.code == GetMapKey(SelectFireTrap))
		{
			toUpdate[ID].ChangingWeapon = 2;
		}

		if (event.key.code == sfKeyEscape)
		{
			if (GetState().isPaused)
			{
				for (int i = 0; i < numberOfPlayers; i++)
				{
					SetStatePause(sfFalse);
				}
			}
			else
			{
				for (int i = 0; i < numberOfPlayers; i++)
				{
					SetStatePause(sfTrue);
				}
			}
		}

		if (event.key.code == sfKeyK)
		{
			if (IsNmEnabled())
			{
				SetNmEnabled(sfFalse);
			}
			else
			{
				SetNmEnabled(sfTrue);
			}
		}

		if (event.key.code == GetMapKey(ShowMiniMap))
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
		if (event.key.code == sfKeyF5)
		{
			SetSave(sfTrue);
			SetTypeOfSave(quickSave);
		}
	}
	TakeInputSkillTree(event, ID);
}

//Real time Keyboard event gestion
void UpdateKeyboardMouse(sfRenderWindow *mainWindow, ActionListener toUpdate[2], const int ID)
{
	sfVector2f dir = { 0.0f, 0.0f };
	if (sfKeyboard_isKeyPressed(GetMapKey(MoveUp)))
	{
		dir.y = -1;
	}
	if (sfKeyboard_isKeyPressed(GetMapKey(MoveDown)))
	{
		dir.y = 1;
	}
	if (sfKeyboard_isKeyPressed(GetMapKey(MoveRight)))
	{
		dir.x = 1;
	}
	if (sfKeyboard_isKeyPressed(GetMapKey(MoveLeft)))
	{
		dir.x = -1;
	}
	if (sfKeyboard_isKeyPressed(GetMapKey(UseDash)))
	{
		toUpdate[ID].isDashing = sfTrue;
	}
	else
	{
		toUpdate[ID].isDashing = sfFalse;
	}
	TakeInputMovement(dir, ID);
	dir.x = (float)sfMouse_getPositionRenderWindow(mainWindow).x;
	dir.y = (float)sfMouse_getPositionRenderWindow(mainWindow).y;
	sfBool StateIsPaused = GetState().isPaused;
	if (sfMouse_isButtonPressed(GetMapKey(BasicAttack)) && !StateIsPaused)
	{
		toUpdate[ID].isFiring = sfTrue;
	}
	else
	{
		toUpdate[ID].isFiring = sfFalse;
	}

	if (sfMouse_isButtonPressed(GetMapKey(UseSpell)) && !StateIsPaused)
	{

		toUpdate[ID].isCastSpell = sfTrue;
	}
	else
	{
		toUpdate[ID].isCastSpell = sfFalse;
	}
	
	TakeInputAction(dir, toUpdate[ID]);
}