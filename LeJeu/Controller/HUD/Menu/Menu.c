#include "Menu.h"
#include "..\..\LeJeu\View\CustomXinput\CustomXinput.h"
#include "..\..\LeJeu\View\MapKey\MapKey.h"
#include "..\..\Sounds\Sounds.h"
#include "..\..\Serialization\Serialization.h"
#include "..\..\..\View\ViewManager.h"

#include <string.h>

Menu CreateGameMenu(const sfVector2f menuPosition, const Button* buttons, const int buttonNumber, const int imagesNumber, const bool isCentered)
{
	Menu menu;

	menu.isActive = true;
	menu.imagesNumber = imagesNumber;
	menu.buttonNumber = buttonNumber;
	menu.background = NULL;
	menu.title = NULL;
	menu.TextToDisplayLoad = NULL;

	menu.images = (sfSprite**)malloc(sizeof(sfSprite*) * imagesNumber);

	menu.button = (Button*)malloc(sizeof(Button) * buttonNumber);

	for (int i = 0; i < buttonNumber; i++)
	{
		menu.button[i] = buttons[i];
	}

	menu.Position = menuPosition;

	return menu;
}

void AddBackgroundToMenu(Menu* menu, const char* fileDirectory, const sfVector2f position, const bool isCentered)
{
	sfTexture* texture = sfTexture_createFromFile(fileDirectory, NULL);
	menu->background = sfSprite_create();
	sfTexture_setSmooth(texture, sfTrue);
	sfSprite_setTexture(menu->background, texture, true);

	if (isCentered)
	{
		sfVector2u imageSize = sfTexture_getSize(texture);
		sfVector2f center = { imageSize.x / 2.0f, imageSize.y / 2.0f };
		sfSprite_setOrigin(menu->background, center);
	}

	sfSprite_setPosition(menu->background, position);
}

void AddTitleToMenu(Menu* menu, char* fileDirectory, sfVector2f position, const bool isCentered)
{
	sfTexture* texture = sfTexture_createFromFile(fileDirectory, NULL);
	menu->title = sfSprite_create();
	sfTexture_setSmooth(texture, sfTrue);
	sfSprite_setTexture(menu->title, texture, true);

	if (isCentered)
	{
		sfVector2u imageSize = sfTexture_getSize(texture);
		sfVector2f center = { imageSize.x / 2.0f, imageSize.y / 2.0f };
		sfSprite_setOrigin(menu->title, center);
	}

	sfSprite_setPosition(menu->title, position);
}

void AddImageToMenu(Menu* menu, char* fileDirectory, int arrayIndex, sfVector2f position, const bool isCentered)
{
	menu->images[arrayIndex] = LoadSprite(fileDirectory, isCentered);
	sfTexture_setSmooth((sfTexture*)sfSprite_getTexture(menu->images[arrayIndex]), sfTrue);
	sfSprite_setPosition(menu->images[arrayIndex], position);
}

void ChangeSelectorPosition(Selector* toChange, sfVector2f newPos)
{
	sfSprite_setPosition(toChange->selectorBackground, newPos); // reset the button's position
	toChange->rect = sfSprite_getGlobalBounds(toChange->selectorBackground); // reset the button's hitbox
}

void AddImagesToMenu(Menu* menu, char* fileDirectories[], sfVector2f positions[], const bool isCentered)
{
	for (int i = 0; i < menu->imagesNumber; i++)
	{
		menu->images[i] = LoadSprite(fileDirectories[i], isCentered);
		sfSprite_setPosition(menu->images[i], positions[i]);
	}
}

void AlignMenuButtons(Menu* menu, int firstButtonIndex, sfVector2f initPosition, float offset, int offsetDir)
{
	sfVector2f newposition;
	ChangeButtonPosition(&menu->button[firstButtonIndex], initPosition);
	for (int i = firstButtonIndex + 1; i < menu->buttonNumber; i++)
	{
		if (offsetDir == 1)
		{
			newposition.x = sfSprite_getPosition(menu->button[i - 1].sprite).x + menu->button[i - 1].rect.width / 2 + menu->button[i].rect.width / 2 + offset;
			newposition.y = initPosition.y;
		}
		else if (offsetDir == 2)
		{
			newposition.x = initPosition.x;
			newposition.y = sfSprite_getPosition(menu->button[i - 1].sprite).y + menu->button[i - 1].rect.height / 2.0f + menu->button[i].rect.height / 2.0f + offset;
		}
		else
		{
			newposition.x = sfSprite_getPosition(menu->button[i - 1].sprite).x + menu->button[i - 1].rect.width / 2 + menu->button[i].rect.width / 2 + offset;
			newposition.y = sfSprite_getPosition(menu->button[i - 1].sprite).y + menu->button[i - 1].rect.height / 2 + menu->button[i].rect.height / 2 + offset;
		}

		ChangeButtonPosition(&menu->button[i], newposition);
	}
}

sfVector2f CenterInMiddleElements(Menu menu, sfVector2f actualCenter, int firstButtonIndex, int lastButtonIndex, int selectorIndex, float offset, int offsetDir)
{
	float length = 0.0f;
	for (int i = firstButtonIndex; i < lastButtonIndex + 1; i++)
	{
		if (offsetDir == 1)
		{
			length += menu.button[i].rect.width;
		}
		else if (offsetDir == 2)
		{
			length += menu.button[i].rect.height;
		}
	}

	length += offset;

	if (offsetDir == 1)
	{
		actualCenter.x -= (length / 2.0f);
	}
	else if (offsetDir == 2)
	{
		actualCenter.y -= (length / 2.0f);
	}

	return(actualCenter);

}

void AlignButtonsAndSelector(Menu* menu, sfVector2f initPosition, int firstButtonIndex, int lastButtonIndex, int selectorIndex, float offset, int offsetDir)
{
	sfVector2f newposition;
	ChangeButtonPosition(&menu->button[firstButtonIndex], initPosition);
	for (int i = firstButtonIndex + 1; i < lastButtonIndex + 1; i++)
	{
		if (offsetDir == 1)
		{
			newposition.x = sfSprite_getPosition(menu->button[i - 1].sprite).x + menu->button[i - 1].rect.width / 2 + menu->button[i].rect.width / 2 + offset;
			newposition.y = initPosition.y;
		}
		else if (offsetDir == 2)
		{
			newposition.x = initPosition.x;
			newposition.y = sfSprite_getPosition(menu->button[i - 1].sprite).y + menu->button[i - 1].rect.height / 2 + menu->button[i].rect.height / 2 + offset;
		}
		else
		{
			newposition.x = sfSprite_getPosition(menu->button[i - 1].sprite).x + menu->button[i - 1].rect.width / 2 + menu->button[i].rect.width / 2 + offset;
			newposition.y = sfSprite_getPosition(menu->button[i - 1].sprite).y + menu->button[i - 1].rect.height / 2 + menu->button[i].rect.height / 2 + offset;
		}

		ChangeButtonPosition(&menu->button[i], newposition);
	}

	if (offsetDir == 1)
	{
		newposition.x = sfSprite_getPosition(menu->button[lastButtonIndex].sprite).x + menu->button[lastButtonIndex].rect.width / 2 + menu->selectors[selectorIndex].rect.width / 2 + offset;
		newposition.y = initPosition.y;
	}
	else if (offsetDir == 2)
	{
		newposition.x = initPosition.x;
		newposition.y = sfSprite_getPosition(menu->button[lastButtonIndex].sprite).y + menu->button[lastButtonIndex].rect.height / 2 + menu->selectors[selectorIndex].rect.height / 2 + offset;
	}
	else
	{
		newposition.x = sfSprite_getPosition(menu->button[lastButtonIndex].sprite).x + menu->button[lastButtonIndex].rect.width / 2 + menu->selectors[selectorIndex].rect.width / 2 + offset;
		newposition.y = sfSprite_getPosition(menu->button[lastButtonIndex].sprite).y + menu->button[lastButtonIndex].rect.height / 2 + menu->selectors[selectorIndex].rect.height / 2 + offset;
	}

	ChangeSelectorPosition(&menu->selectors[selectorIndex], newposition);
}

void InitSelector(Selector* toInit, sfVector2f pos, sfVector2f size, char* spritePath, const bool isCentered)
{
	sfVector2f scale; // the button's scale
	sfTexture* texture; // the button's texture

	if (spritePath != NULL)
	{
		texture = sfTexture_createFromFile(spritePath, NULL); // load the button's texture
		sfTexture_setSmooth(texture, true);
	}
	else
	{
		toInit->selectorBackground = NULL;

		toInit->rect.left = pos.x;
		toInit->rect.top = pos.y;
		toInit->rect.width = size.x;
		toInit->rect.height = size.y;
		return;
	}

	toInit->selectorBackground = sfSprite_create();
	sfTexture_setSmooth(texture, sfTrue);
	sfSprite_setTexture(toInit->selectorBackground, texture, 0);
	// the scale's ratio (to rescale the texture according to the button's size)
	sfVector2u tempSize = sfTexture_getSize(texture);
	scale.x = size.x / (float)tempSize.x;
	scale.y = size.y / (float)tempSize.y;

	sfSprite_setScale(toInit->selectorBackground, scale);

	if (isCentered)
	{
		sfVector2f center = { (float)tempSize.x / 2.0f, (float)tempSize.y / 2.0f }; // get the texture's center
		sfSprite_setOrigin(toInit->selectorBackground, center);
	}

	sfSprite_setPosition(toInit->selectorBackground, pos);

	toInit->rect = sfSprite_getGlobalBounds(toInit->selectorBackground);

}

void ResponsiveSelector(Selector* toUpdate, sfVector2f newPos, sfVector2f newSize, const bool isCentered)
{
	if (toUpdate->selectorBackground == NULL)
	{
		toUpdate->rect.left = newPos.x;
		toUpdate->rect.top = newPos.y;
		toUpdate->rect.width = newSize.x;
		toUpdate->rect.height = newSize.y;
	}
	else
	{
		sfVector2f scale;

		sfTexture *temp = (sfTexture *)sfSprite_getTexture(toUpdate->selectorBackground);
		sfVector2u tempSize = sfTexture_getSize(temp);
		scale.x = newSize.x / (float)tempSize.x;
		scale.y = newSize.y / (float)tempSize.y;

		sfSprite_setScale(toUpdate->selectorBackground, scale);

		if (isCentered)
		{
			sfVector2f center = { (float)tempSize.x / 2, (float)tempSize.y / 2 }; // get the texture's center
			sfSprite_setOrigin(toUpdate->selectorBackground, center);
		}

		sfSprite_setPosition(toUpdate->selectorBackground, newPos);

		toUpdate->rect = sfSprite_getGlobalBounds(toUpdate->selectorBackground);
	}
}

void UpdateMenuPosition(Menu* menu, sfVector2f Position)
{
	for (int i = 0; i < menu->buttonNumber; i++)
	{
		ChangeButtonPosition(&menu->button[i], Position);
	}

	sfSprite_setPosition(menu->background, Position);
}

void InitArrow(DirectionalArrow *Arrow, sfVector2f PosCenter, float ratioX, float ratioY, float size, sfBool isCentered, char** Path, float Spacing)
{
	sfTexture* texture_1, *texture_2, *texture_3, *texture_4;
	//0 is left 1 is right 2 is leftH 3 is rightH

	////////
	texture_1 = sfTexture_createFromFile(Path[0], NULL);
	Arrow[0].Sprites[0] = sfSprite_create();

	sfTexture_setSmooth(texture_1, sfTrue);
	sfSprite_setTexture(Arrow[0].Sprites[0], texture_1, 0);

	///////
	texture_2 = sfTexture_createFromFile(Path[2], NULL);
	Arrow[0].Sprites[1] = sfSprite_create();

	sfTexture_setSmooth(texture_2, sfTrue);
	sfSprite_setTexture(Arrow[0].Sprites[1], texture_2, 0);
	
	//////
	texture_3 = sfTexture_createFromFile(Path[1], NULL);
	Arrow[1].Sprites[0] = sfSprite_create();

	sfTexture_setSmooth(texture_3, sfTrue);
	sfSprite_setTexture(Arrow[1].Sprites[0], texture_3, 0);

	//////
	texture_4 = sfTexture_createFromFile(Path[3], NULL);
	Arrow[1].Sprites[1] = sfSprite_create();

	sfTexture_setSmooth(texture_4, sfTrue);
	sfSprite_setTexture(Arrow[1].Sprites[1], texture_4, 0);	

	if (isCentered)
	{
		sfVector2f center; // get the texture's center
		sfVector2u tempSizeTexture_1 = sfTexture_getSize(texture_1);
		sfVector2u tempSizeTexture_2 = sfTexture_getSize(texture_2);
		sfVector2u tempSizeTexture_3 = sfTexture_getSize(texture_3);
		sfVector2u tempSizeTexture_4 = sfTexture_getSize(texture_4);
	
		//////////
		center.x = (float)tempSizeTexture_1.x / 2.0f;
		center.y = (float)tempSizeTexture_1.y / 2.0f;

		sfSprite_setOrigin(Arrow[0].Sprites[0], center);

		////////
		center.x = (float)tempSizeTexture_2.x / 2.0f;
		center.y = (float)tempSizeTexture_2.y / 2.0f;

		sfSprite_setOrigin(Arrow[0].Sprites[1], center);

		/////////
		center.x = (float)tempSizeTexture_3.x / 2.0f;
		center.y = (float)tempSizeTexture_3.y / 2.0f;

		sfSprite_setOrigin(Arrow[1].Sprites[0], center);

		////////
		center.x = (float)tempSizeTexture_4.x / 2.0f;
		center.y = (float)tempSizeTexture_4.y / 2.0f;

		sfSprite_setOrigin(Arrow[1].Sprites[1], center);
	}
	
	sfVector2f pos = PosCenter;
	sfVector2f scaleArrow = { ratioX, ratioY };

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			sfSprite_setScale(Arrow[i].Sprites[j], scaleArrow);
		}
	}

	pos.x -= size / 2.0f + (Spacing/2.0f) * ratioX;
	sfSprite_setPosition(Arrow[0].Sprites[0], pos);
	sfSprite_setPosition(Arrow[0].Sprites[1], pos);

	pos.x += size + Spacing * ratioX;
	sfSprite_setPosition(Arrow[1].Sprites[0], pos);
	sfSprite_setPosition(Arrow[1].Sprites[1], pos);

	Arrow[0].isActivated = Arrow[1].isActivated = sfFalse;
}

KeyMaping UpdateMenu(Menu *menuToUpdate, EventList* eventList, const int typeOfGamePad)
{
	KeyMaping keyMaping;
	keyMaping.indexSelector = -1;
	keyMaping.Key = sfKeyUnknown;

	if (menuToUpdate->selectors != NULL)
	{
		for (int i = 0; i < menuToUpdate->selectorsNumber; i++)
		{
			if (menuToUpdate->selectors[i].isOn)
			{
				keyMaping.indexSelector = i;
				i = menuToUpdate->selectorsNumber;
			}
		}
	}
	sfEvent event;
	sfVector2f left;
	sfVector2f right;
	if (eventList != NULL)
	{
		event = eventList->Event;
		GetSticksPosition(GetPortID(GetIdSend()), &left, &right);
	}

	while (eventList != NULL)
	{
		event = eventList->Event;
		if (!typeOfGamePad)
		{
			if (event.type == sfEvtMouseButtonPressed && menuToUpdate->selectorsNumber == 11)
			{
				if ((menuToUpdate->selectionCursor.indexPos == 9 || menuToUpdate->selectionCursor.indexPos == 10) && menuToUpdate->selectors[menuToUpdate->selectionCursor.indexPos].isOn)
				{
					keyMaping.indexSelector = menuToUpdate->selectionCursor.indexPos;
					keyMaping.Key = event.mouseButton.button;
					return keyMaping;
				}
			}

			if (event.type == sfEvtKeyPressed || event.type == sfEvtTextEntered)
			{
				if (menuToUpdate->Index == 13)
				{
					keyMaping.indexSelector = 0;
					StopMusic(1);
					return keyMaping;
				}

				if (event.key.code == sfKeyEscape)
				{
					PlayUISounds(1);
					keyMaping.indexSelector = menuToUpdate->buttonNumber - 1;
					keyMaping.Key = event.key.code;
					return keyMaping;
				}

				if (event.key.code == sfKeyReturn)//active the button
				{
					PlayUISounds(1);
					keyMaping.indexSelector = menuToUpdate->selectionCursor.indexPos;
					keyMaping.Key = event.key.code;
					return keyMaping;
				}

				if (menuToUpdate->selectorsNumber == 11 && menuToUpdate->selectors[menuToUpdate->selectionCursor.indexPos].isOn && menuToUpdate->selectionCursor.indexPos != 11 && event.type != sfEvtTextEntered)
				{
					if (menuToUpdate->selectionCursor.indexPos != 9 && menuToUpdate->selectionCursor.indexPos != 10 || event.key.code == sfKeyEscape)
					{
						keyMaping.indexSelector = menuToUpdate->selectionCursor.indexPos;
						keyMaping.Key = event.key.code;
						return keyMaping;
					}
				}

				if (menuToUpdate->classDisplayer.isOn)
				{
					if (event.key.code == GetMapKey(MoveLeft)) //change the class of the player
					{
						int temp = menuToUpdate->classDisplayer.ActualClassToDisplay - 1;
						menuToUpdate->classDisplayer.ActualClassToDisplay = temp >= 0 ? temp : menuToUpdate->classDisplayer.NumberOfClass - 1;
					}
					if (event.key.code == GetMapKey(MoveRight)) //change the class of the player
					{
						menuToUpdate->classDisplayer.ActualClassToDisplay = (menuToUpdate->classDisplayer.ActualClassToDisplay + 1) % menuToUpdate->classDisplayer.NumberOfClass;
					}

					if (event.key.code == GetMapKey(MoveDown))
					{
						menuToUpdate->selectionCursor.indexPos = (menuToUpdate->selectionCursor.indexPos + 1) % menuToUpdate->buttonNumber;
						sfVector2f newPos = sfSprite_getPosition(menuToUpdate->button[menuToUpdate->selectionCursor.indexPos].sprite);
						sfSprite_setPosition(menuToUpdate->selectionCursor.indexDisplay, newPos);
					}

					if (event.key.code == GetMapKey(MoveUp))
					{
						menuToUpdate->selectionCursor.indexPos = (menuToUpdate->selectionCursor.indexPos - 1) >= 0 ? menuToUpdate->selectionCursor.indexPos - 1 : menuToUpdate->buttonNumber - 1;
						sfVector2f newPos = sfSprite_getPosition(menuToUpdate->button[menuToUpdate->selectionCursor.indexPos].sprite);
						sfSprite_setPosition(menuToUpdate->selectionCursor.indexDisplay, newPos);
					}
				}
				else if (keyMaping.indexSelector != -1)
				{
					if (event.key.code == GetMapKey(MoveLeft)) //change the window's style/resolution
					{
						int temp = menuToUpdate->selectors[keyMaping.indexSelector].textDisplay.actualTextToDisplay - 1;
						menuToUpdate->selectors[keyMaping.indexSelector].textDisplay.actualTextToDisplay = temp >= 0 ? temp : menuToUpdate->selectors[keyMaping.indexSelector].textDisplay.numberOfText - 1;
					}

					if (event.key.code == GetMapKey(MoveRight)) //change the window's style/resolution
					{
						menuToUpdate->selectors[keyMaping.indexSelector].textDisplay.actualTextToDisplay = (menuToUpdate->selectors[keyMaping.indexSelector].textDisplay.actualTextToDisplay + 1) % menuToUpdate->selectors[keyMaping.indexSelector].textDisplay.numberOfText;
					}

					if (event.key.code == sfKeyBack)//cancel the window's style/resolution
					{
						menuToUpdate->selectors[keyMaping.indexSelector].isOn = sfFalse;
						if (menuToUpdate->Index == 6)
						{
							if (keyMaping.indexSelector == 0)
							{
								menuToUpdate->selectors[keyMaping.indexSelector].textDisplay.actualTextToDisplay = GetIndexActualRes();
							}
							else if (keyMaping.indexSelector == 1)
							{
								menuToUpdate->selectors[keyMaping.indexSelector].textDisplay.actualTextToDisplay = GetIndexActualWinMode();
							}
							else
							{
								menuToUpdate->selectors[keyMaping.indexSelector].textDisplay.actualTextToDisplay = GetConfigDynamicLight();
							}
						}
						else if (menuToUpdate->Index == 4)
						{
							menuToUpdate->selectors[keyMaping.indexSelector].textDisplay.actualTextToDisplay = GetActualLanguageSelected();
						}
						else if (menuToUpdate->Index == 5)
						{
							int TypeHardware = GetTypeHardware(keyMaping.indexSelector);
							if (TypeHardware == 2 && keyMaping.indexSelector == 1)
							{
								menuToUpdate->selectors[keyMaping.indexSelector].textDisplay.actualTextToDisplay = 3;
							}
							else if (TypeHardware == 1 && GetPortID(keyMaping.indexSelector) == 1)
							{
								if (keyMaping.indexSelector == 0)
								{
									menuToUpdate->selectors[keyMaping.indexSelector].textDisplay.actualTextToDisplay = 2;
								}
								else
								{
									menuToUpdate->selectors[keyMaping.indexSelector].textDisplay.actualTextToDisplay = 3;
								}
							}
							else
							{
								menuToUpdate->selectors[keyMaping.indexSelector].textDisplay.actualTextToDisplay = TypeHardware;
							}
						}
						else if (menuToUpdate->Index == 1)
						{
							if (keyMaping.indexSelector == 2)
							{
								menuToUpdate->selectors[keyMaping.indexSelector].textDisplay.actualTextToDisplay = GetActualDungeonId();
							}
						}
						sfVector2f newPos = sfSprite_getPosition(menuToUpdate->button[menuToUpdate->selectionCursor.indexPos].sprite);
						sfSprite_setPosition(menuToUpdate->selectionCursor.indexDisplay, newPos);
						keyMaping.Key = sfKeyUnknown;
						keyMaping.indexSelector = -1;
						return keyMaping;
					}
				}
				else if (menuToUpdate->SliderSelectors != NULL)
				{
					int tempID = -1;
					for (int i = 0; i < menuToUpdate->SliderSelectorsNumber; i++)
					{
						if (menuToUpdate->SliderSelectors[i].isSelected)
						{
							tempID = i;
							i = menuToUpdate->SliderSelectorsNumber;
						}
					}
					if (tempID == -1)
					{
						if (event.key.code == GetMapKey(MoveDown))
						{
							if (menuToUpdate->selectionCursor.indexPos == menuToUpdate->SliderSelectorsNumber - 1)
							{
								menuToUpdate->selectionCursor.indexPos = (menuToUpdate->selectionCursor.indexPos + 1) % (menuToUpdate->buttonNumber + menuToUpdate->SliderSelectorsNumber);
								sfVector2f newPos = sfSprite_getPosition(menuToUpdate->button[menuToUpdate->selectionCursor.indexPos - menuToUpdate->SliderSelectorsNumber].sprite);
								sfSprite_setPosition(menuToUpdate->selectionCursor.indexDisplay, newPos);
								sfSprite_setPosition(menuToUpdate->SliderCursor, newPos);
							}
							else
							{
								menuToUpdate->selectionCursor.indexPos = (menuToUpdate->selectionCursor.indexPos + 1) % (menuToUpdate->buttonNumber + menuToUpdate->SliderSelectorsNumber);
								sfVector2f newPos = sfSprite_getPosition(menuToUpdate->SliderSelectors[menuToUpdate->selectionCursor.indexPos].Background);
								sfSprite_setPosition(menuToUpdate->selectionCursor.indexDisplay, newPos);
								sfSprite_setPosition(menuToUpdate->SliderCursor, newPos);
							}
							PlayUISounds(scratch);
						}

						if (event.key.code == GetMapKey(MoveUp))
						{
							menuToUpdate->selectionCursor.indexPos = (menuToUpdate->selectionCursor.indexPos - 1) >= 0 ? menuToUpdate->selectionCursor.indexPos - 1 : menuToUpdate->buttonNumber - 1 + menuToUpdate->SliderSelectorsNumber;
							if (menuToUpdate->selectionCursor.indexPos < menuToUpdate->SliderSelectorsNumber)
							{
								sfVector2f newPos = sfSprite_getPosition(menuToUpdate->SliderSelectors[menuToUpdate->selectionCursor.indexPos].Background);
								sfSprite_setPosition(menuToUpdate->selectionCursor.indexDisplay, newPos);
								sfSprite_setPosition(menuToUpdate->SliderCursor, newPos);
							}
							else
							{
								sfVector2f newPos = sfSprite_getPosition(menuToUpdate->button[menuToUpdate->selectionCursor.indexPos - menuToUpdate->SliderSelectorsNumber].sprite);
								sfSprite_setPosition(menuToUpdate->selectionCursor.indexDisplay, newPos);
								sfSprite_setPosition(menuToUpdate->SliderCursor, newPos);
							}
							PlayUISounds(scratch);
						}
					}
				}
				else
				{
					if (event.key.code == GetMapKey(MoveDown))
					{
						menuToUpdate->selectionCursor.indexPos = (menuToUpdate->selectionCursor.indexPos + 1) % menuToUpdate->buttonNumber;
						sfVector2f newPos = sfSprite_getPosition(menuToUpdate->button[menuToUpdate->selectionCursor.indexPos].sprite);
						sfSprite_setPosition(menuToUpdate->selectionCursor.indexDisplay, newPos);
						PlayUISounds(scratch);
					}
					else if (event.key.code == GetMapKey(MoveUp))
					{
						menuToUpdate->selectionCursor.indexPos = (menuToUpdate->selectionCursor.indexPos - 1) >= 0 ? menuToUpdate->selectionCursor.indexPos - 1 : menuToUpdate->buttonNumber - 1;
						sfVector2f newPos = sfSprite_getPosition(menuToUpdate->button[menuToUpdate->selectionCursor.indexPos].sprite);
						sfSprite_setPosition(menuToUpdate->selectionCursor.indexDisplay, newPos);
						PlayUISounds(scratch);
					}
				}
			}
		}
		else
		{
			if (JoystickIsButtonPressed(GetPortID(GetIdSend()), A))
			{
				PlayUISounds(1);
				keyMaping.indexSelector = menuToUpdate->selectionCursor.indexPos;
				return keyMaping;
			}

			if (JoystickIsButtonPressed(GetPortID(GetIdSend()), B) && (menuToUpdate->selectors == NULL || (menuToUpdate->selectors != NULL && menuToUpdate->selectors[keyMaping.indexSelector].isOn != sfTrue)))
			{
				PlayUISounds(1);
				keyMaping.indexSelector = menuToUpdate->buttonNumber - 1;
				return keyMaping;
			}

			if (menuToUpdate->classDisplayer.isOn)
			{
				if (left.x <= -50) //change the class of the player
				{
					int temp = menuToUpdate->classDisplayer.ActualClassToDisplay - 1;
					menuToUpdate->classDisplayer.ActualClassToDisplay = temp >= 0 ? temp : menuToUpdate->classDisplayer.NumberOfClass - 1;
					left.x = 0;
				}
				else if (left.x >= 50)//change the class of the player
				{
					menuToUpdate->classDisplayer.ActualClassToDisplay = (menuToUpdate->classDisplayer.ActualClassToDisplay + 1) % menuToUpdate->classDisplayer.NumberOfClass;
					left.x = 0;
				}

				if (left.y <= -50)
				{
					menuToUpdate->selectionCursor.indexPos = (menuToUpdate->selectionCursor.indexPos + 1) % menuToUpdate->buttonNumber;
					sfVector2f newPos = sfSprite_getPosition(menuToUpdate->button[menuToUpdate->selectionCursor.indexPos].sprite);
					sfSprite_setPosition(menuToUpdate->selectionCursor.indexDisplay, newPos);
					left.y = 0;
				}
				else if (left.y >= 50)
				{
					menuToUpdate->selectionCursor.indexPos = (menuToUpdate->selectionCursor.indexPos - 1) >= 0 ? menuToUpdate->selectionCursor.indexPos - 1 : menuToUpdate->buttonNumber - 1;
					sfVector2f newPos = sfSprite_getPosition(menuToUpdate->button[menuToUpdate->selectionCursor.indexPos].sprite);
					sfSprite_setPosition(menuToUpdate->selectionCursor.indexDisplay, newPos);
					left.y = 0;
				}
			}
			else if (keyMaping.indexSelector != -1)
			{
				if (left.x <= -50)//change the window's style/resolution
				{
					int temp = menuToUpdate->selectors[keyMaping.indexSelector].textDisplay.actualTextToDisplay - 1;
					menuToUpdate->selectors[keyMaping.indexSelector].textDisplay.actualTextToDisplay = temp >= 0 ? temp : menuToUpdate->selectors[keyMaping.indexSelector].textDisplay.numberOfText - 1;
					left.x = 0;
				}
				else if (left.x >= 50)//change the window's style/resolution
				{
					menuToUpdate->selectors[keyMaping.indexSelector].textDisplay.actualTextToDisplay = (menuToUpdate->selectors[keyMaping.indexSelector].textDisplay.actualTextToDisplay + 1) % menuToUpdate->selectors[keyMaping.indexSelector].textDisplay.numberOfText;
					left.x = 0;
				}

				if (JoystickIsButtonPressed(GetPortID(GetIdSend()), B) && menuToUpdate->selectors[keyMaping.indexSelector].isOn)
				{
					menuToUpdate->selectors[keyMaping.indexSelector].isOn = sfFalse;
					if (menuToUpdate->Index == 6)
					{
						if (keyMaping.indexSelector == 0)
						{
							menuToUpdate->selectors[keyMaping.indexSelector].textDisplay.actualTextToDisplay = GetIndexActualRes();
						}
						else if (keyMaping.indexSelector == 1)
						{
							menuToUpdate->selectors[keyMaping.indexSelector].textDisplay.actualTextToDisplay = GetIndexActualWinMode();
						}
					}
					else if (menuToUpdate->Index == 4)
					{
						menuToUpdate->selectors[keyMaping.indexSelector].textDisplay.actualTextToDisplay = GetActualLanguageSelected();
					}
					else if (menuToUpdate->Index == 5)
					{
						menuToUpdate->selectors[keyMaping.indexSelector].textDisplay.actualTextToDisplay = GetTypeHardware(keyMaping.indexSelector);
					}
					sfVector2f newPos = sfSprite_getPosition(menuToUpdate->button[menuToUpdate->selectionCursor.indexPos].sprite);
					sfSprite_setPosition(menuToUpdate->selectionCursor.indexDisplay, newPos);
					keyMaping.indexSelector = -1;
					keyMaping.Key = sfKeyUnknown;
					return keyMaping;
				}
			}
			else
			{
				if (left.y >= 50)
				{
					menuToUpdate->selectionCursor.indexPos = (menuToUpdate->selectionCursor.indexPos + 1) % menuToUpdate->buttonNumber;
					sfVector2f newPos = sfSprite_getPosition(menuToUpdate->button[menuToUpdate->selectionCursor.indexPos].sprite);
					sfSprite_setPosition(menuToUpdate->selectionCursor.indexDisplay, newPos);
					left.y = 0;
					PlayUISounds(scratch);
				}
				else if (left.y <= -50)
				{
					menuToUpdate->selectionCursor.indexPos = (menuToUpdate->selectionCursor.indexPos - 1) >= 0 ? menuToUpdate->selectionCursor.indexPos - 1 : menuToUpdate->buttonNumber - 1;
					sfVector2f newPos = sfSprite_getPosition(menuToUpdate->button[menuToUpdate->selectionCursor.indexPos].sprite);
					sfSprite_setPosition(menuToUpdate->selectionCursor.indexDisplay, newPos);
					left.y = 0;
					PlayUISounds(scratch);
				}
			}
		}
		eventList = eventList->Next;
	}

	if (menuToUpdate->Index == 7)
	{
		int tempID = -1;
		for (int i = 0; i < menuToUpdate->SliderSelectorsNumber; i++)
		{
			if (menuToUpdate->SliderSelectors[i].isSelected)
			{
				tempID = i;
				i = menuToUpdate->SliderSelectorsNumber;
			}
		}
		if (tempID != -1)
		{
			for (int i = 0; i < 2; i++)
			{
				menuToUpdate->SliderSelectors[tempID].Arrow[i].isActivated = sfFalse;
			}

			if (sfKeyboard_isKeyPressed(GetMapKey(MoveLeft)))
			{
				if (menuToUpdate->SliderSelectors[tempID].valueOfSlider > 0)
				{
					menuToUpdate->SliderSelectors[tempID].valueOfSlider--;
					menuToUpdate->SliderSelectors[tempID].Scursor.Pos.x = menuToUpdate->SliderSelectors[tempID].Scursor.minPos.x + (menuToUpdate->SliderSelectors[tempID].Scursor.maxPos.x - menuToUpdate->SliderSelectors[tempID].Scursor.minPos.x) * (float)(menuToUpdate->SliderSelectors[tempID].valueOfSlider / 100.0f);
					sfSprite_setPosition(menuToUpdate->SliderSelectors[tempID].Scursor.Sprite, menuToUpdate->SliderSelectors[tempID].Scursor.Pos);
					UpdateTextDisplay(menuToUpdate->SliderSelectors[tempID].ValueOfSlider, (int)menuToUpdate->SliderSelectors[tempID].valueOfSlider);
					menuToUpdate->SliderSelectors[tempID].Arrow[0].isActivated = sfTrue;
				}
			}

			if (sfKeyboard_isKeyPressed(GetMapKey(MoveRight)))
			{
				if (menuToUpdate->SliderSelectors[tempID].valueOfSlider < 100)
				{
					menuToUpdate->SliderSelectors[tempID].valueOfSlider++;
					menuToUpdate->SliderSelectors[tempID].Scursor.Pos.x = menuToUpdate->SliderSelectors[tempID].Scursor.minPos.x + (menuToUpdate->SliderSelectors[tempID].Scursor.maxPos.x - menuToUpdate->SliderSelectors[tempID].Scursor.minPos.x) * (float)(menuToUpdate->SliderSelectors[tempID].valueOfSlider / 100.0f);
					sfSprite_setPosition(menuToUpdate->SliderSelectors[tempID].Scursor.Sprite, menuToUpdate->SliderSelectors[tempID].Scursor.Pos);
					UpdateTextDisplay(menuToUpdate->SliderSelectors[tempID].ValueOfSlider, (int)menuToUpdate->SliderSelectors[tempID].valueOfSlider);
					menuToUpdate->SliderSelectors[tempID].Arrow[1].isActivated = sfTrue;
				}
			}

			sfVector2f Origin = sfVertexArray_getVertex(menuToUpdate->SliderSelectors[tempID].Scursor.sliderVertex, 0)->position;
			sfVertex* temp1 = sfVertexArray_getVertex(menuToUpdate->SliderSelectors[tempID].Scursor.sliderVertex, 1);
			sfVertex* temp2 = sfVertexArray_getVertex(menuToUpdate->SliderSelectors[tempID].Scursor.sliderVertex, 2);

			temp1->position.x = temp2->position.x = menuToUpdate->SliderSelectors[tempID].Scursor.spriteSize.x * (menuToUpdate->SliderSelectors[tempID].valueOfSlider / 100.0f) + Origin.x;
			temp1->texCoords.x = temp2->texCoords.x = (menuToUpdate->SliderSelectors[tempID].Scursor.spriteSize.x / menuToUpdate->SliderSelectors[tempID].Scursor.spriteScale.x) * (menuToUpdate->SliderSelectors[tempID].valueOfSlider / 100.0f);
		}
	}

	keyMaping.indexSelector = -1;
	keyMaping.Key = sfKeyUnknown;
	return keyMaping;
}

void UpdateMenuLoading(Menu *menuToUpdate, const sfVideoMode mode)
{
	float ratioX = mode.width / 1920.0f;
	float ratioY = mode.height / 1080.0f;
	int numberPlayer = GetPlayersInformations().NumberOfPlayer;
	for (int i = 0; i < menuToUpdate->buttonNumber - 1; i++)
	{
		if (menuToUpdate->button[i].text2 != NULL)
		{
			sfText_setString(menuToUpdate->button[i].text2, ChangeSaveNameButton(numberPlayer, i));
			sfVector2f pos = (sfVector2f) { menuToUpdate->button[i].rect.left + (menuToUpdate->button[i].rect.width) / 2, menuToUpdate->button[i].rect.top + (menuToUpdate->button[i].rect.height) / 2 + 20.f * ratioY };
			CenterText(menuToUpdate->button[i].text2, pos);
		}
		/*if (strcmp(menuToUpdate->button[i].text2, "Empty"))
		{
			if (GetDungeonIDInFile(i, GetPlayersInformations().NumberOfPlayer) == slimeDungeon)
				menuToUpdate->TextToDisplayLoad[i] = GetLocalizationString( 184);
			else
				menuToUpdate->TextToDisplayLoad[i] = GetLocalizationString( 185);
		}*/
	}
}

void UpdateMenuSaving(Menu *menuToUpdate, const sfVideoMode mode)
{
	float ratioX = mode.width / 1920.0f;
	float ratioY = mode.height / 1080.0f;
	int numberPlayer = GetPlayersInformations().NumberOfPlayer;
	for (int i = 0; i < menuToUpdate->buttonNumber - 1; i++)
	{
		if (menuToUpdate->button[i].text2 != NULL)
		{
			sfText_setString(menuToUpdate->button[i].text2, ChangeSaveNameButton(numberPlayer, i + 1));
			sfVector2f pos = (sfVector2f) { menuToUpdate->button[i].rect.left + (menuToUpdate->button[i].rect.width) / 2, menuToUpdate->button[i].rect.top + (menuToUpdate->button[i].rect.height) / 2 + 20.f * ratioY };
			CenterText(menuToUpdate->button[i].text2, pos);
		}
	}
}

void UpdateControlsMenu(Menu *menuToUpdate)
{
	printf("%d %d\n", GetPortID(0), GetPortID(1));
	if (GetPortID(0) != -1 && GetPortID(1) != -1)
	{
		printf("10\n");
		menuToUpdate->selectors[0].textDisplay.actualTextToDisplay = GetTypeHardware(0) + GetPortID(0);
		menuToUpdate->selectors[1].textDisplay.actualTextToDisplay = GetTypeHardware(1) + GetPortID(1);
	}
	else if (GetPortID(0) != -1)
	{
		printf("20\n");
		menuToUpdate->selectors[0].textDisplay.actualTextToDisplay = GetTypeHardware(0) + GetPortID(0);
		if (GetTypeHardware(1) != none)
		{
			menuToUpdate->selectors[1].textDisplay.actualTextToDisplay = GetTypeHardware(1);
		}
		else
		{
			menuToUpdate->selectors[1].textDisplay.actualTextToDisplay = 3;
			printf("je suis là\n");
		}
	}
	else
	{
		printf("30\n");
		menuToUpdate->selectors[0].textDisplay.actualTextToDisplay = GetTypeHardware(0);
		//printf("%d %d\n", GetTypeHardware(1), GetPortID(1));
		if (GetTypeHardware(1) != none)
			menuToUpdate->selectors[1].textDisplay.actualTextToDisplay = GetTypeHardware(1) + GetPortID(1);
		else
			menuToUpdate->selectors[1].textDisplay.actualTextToDisplay = 3;
	}
}

void DisplaySliderSelector(SliderSelector ToDisplay, sfRenderWindow *mainWindow)
{
	sfRenderWindow_drawSprite(mainWindow, ToDisplay.Background, NULL);
	if (ToDisplay.isSelected)
	{
		for (int i = 0; i < 2; i++)
		{
			sfRenderWindow_drawSprite(mainWindow, ToDisplay.Arrow[i].Sprites[ToDisplay.Arrow[i].isActivated], NULL);
		}
	}

	if (ToDisplay.Scursor.Sprite != NULL)
	{
		sfRenderWindow_drawVertexArray(mainWindow, ToDisplay.Scursor.sliderVertex, ToDisplay.Scursor.CursorRS);
	}

	sfRenderWindow_drawText(mainWindow, ToDisplay.ValueOfSlider, NULL);
	sfRenderWindow_drawText(mainWindow, ToDisplay.NameOfSlider, NULL);
}

void DisplayMenu(Menu menu, sfRenderWindow *window)
{
	if (menu.background != NULL)
	{
		sfRenderWindow_drawSprite(window, menu.background, NULL);
	}
	if (menu.title != NULL)
	{
		sfRenderWindow_drawSprite(window, menu.title, NULL);
	}
	if (menu.imagesNumber > 0)
	{
		for (int i = 0; i < menu.imagesNumber; i++)
		{
			sfRenderWindow_drawSprite(window, menu.images[i], NULL);
		}
	}
	if (menu.TextToDisplayLoad != NULL)
	{
		if (menu.button[menu.selectionCursor.indexPos].text2 != NULL && strcmp(sfText_getString(menu.button[menu.selectionCursor.indexPos].text2), GetLocalizationString(145)))
		{
			if (GetDungeonIDInFile(menu.selectionCursor.indexPos, GetPlayersInformations().NumberOfPlayer) == slimeDungeon)
			{
				sfText_setString(menu.TextToDisplayLoad, GetLocalizationString(184));
			}
			else
			{
				sfText_setString(menu.TextToDisplayLoad, GetLocalizationString(185));
			}
		}
		else
		{
			sfText_setString(menu.TextToDisplayLoad, "");
		}
		sfRenderWindow_drawText(window, menu.TextToDisplayLoad, NULL);
	}

	DisplayButtons(window, menu.button, menu.buttonNumber);

	if (menu.selectors != NULL)
	{
		sfBool isPaused = GetState().isPaused;
		for (int i = 0; i < menu.selectorsNumber; i++)
		{
			if (!isPaused)
			{
				sfRenderWindow_drawSprite(window, menu.selectors[i].selectorBackground, NULL);
			}
			sfRenderWindow_drawText(window, menu.selectors[i].textDisplay.textArray[menu.selectors[i].textDisplay.actualTextToDisplay], NULL);

			//Temporary
			if (menu.Index == 6)
			{
				for (int j = 0; j < 2; j++)
				{
					sfRenderWindow_drawSprite(window, menu.selectors[i].Arrows[j].Sprites[menu.selectors[i].Arrows[j].isActivated], NULL);
				}
			}
		}
	}

	if (menu.classDisplayer.Class != NULL)
	{
		sfRenderWindow_drawSprite(window, menu.classDisplayer.Class[menu.classDisplayer.ActualClassToDisplay], NULL);
	}

	if (menu.SliderSelectors != NULL)
	{
		for (int i = 0; i < menu.SliderSelectorsNumber; i++)
		{
			DisplaySliderSelector(menu.SliderSelectors[i], window);
		}
	}

	if (menu.Index == 7 && menu.selectionCursor.indexPos != 3)
	{
		sfRenderWindow_drawSprite(window, menu.SliderCursor, NULL);
	}
	else if (menu.selectionCursor.indexPos != -1)
	{
		sfRenderWindow_drawSprite(window, menu.selectionCursor.indexDisplay, NULL);
	}
}

void InitSliderSelector(SliderSelector* toInit, sfVector2f pos, sfVector2f size, char* spritePath, char* leftArrow, char* rightArrow, char* leftArrowH, char* rightArrowH, const bool isCentered, float ratioX, float ratioY)
{
	sfVector2f scale; // the button's scale
	sfTexture* texture; // the button's texture
	sfTexture* texture2; // the button's texture
	sfTexture* texture3; // the button's texture
	sfTexture* texture4;
	sfTexture* texture5;
	sfTexture* texture6;

	if (spritePath != NULL)
	{
		texture = sfTexture_createFromFile(spritePath, NULL); // load the button's texture
		sfTexture_setSmooth(texture, true);

		texture2 = sfTexture_createFromFile(leftArrow, NULL);
		toInit->Arrow[0].Sprites[0] = sfSprite_create();

		sfTexture_setSmooth(texture2, sfTrue);
		sfSprite_setTexture(toInit->Arrow[0].Sprites[0], texture2, 0);

		texture3 = sfTexture_createFromFile(rightArrow, NULL);
		toInit->Arrow[1].Sprites[0] = sfSprite_create();

		sfTexture_setSmooth(texture3, sfTrue);
		sfSprite_setTexture(toInit->Arrow[1].Sprites[0], texture3, 0);

		texture5 = sfTexture_createFromFile(leftArrowH, NULL);
		toInit->Arrow[0].Sprites[1] = sfSprite_create();

		sfTexture_setSmooth(texture5, sfTrue);
		sfSprite_setTexture(toInit->Arrow[0].Sprites[1], texture5, 0);

		texture6 = sfTexture_createFromFile(rightArrowH, NULL);
		toInit->Arrow[1].Sprites[1] = sfSprite_create();

		sfTexture_setSmooth(texture6, sfTrue);
		sfSprite_setTexture(toInit->Arrow[1].Sprites[1], texture6, 0);

	}
	else
	{
		toInit->Background = NULL;

		printf("Error Slider Selector couldn't load background\n");
		return;
	}

	toInit->Background = sfSprite_create();
	sfTexture_setSmooth(texture, sfTrue);
	sfSprite_setTexture(toInit->Background, texture, 0);

	sfVector2u slideSize = sfTexture_getSize(texture);
	// the scale's ratio (to rescale the texture according to the button's size)
	scale.x = size.x / (float)slideSize.x;
	scale.y = size.y / (float)slideSize.y;

	sfSprite_setScale(toInit->Background, scale);

	texture4 = sfTexture_createFromFile("Ressources/Sprites/UI/Jauge.png", NULL); // load the button's texture

	toInit->Scursor.Sprite = sfSprite_create();
	sfTexture_setSmooth(texture4, sfTrue);
	sfSprite_setTexture(toInit->Scursor.Sprite, texture4, 0);

	if (isCentered)
	{
		sfVector2f center = { (float)slideSize.x / 2.0f, (float)slideSize.y / 2.0f }; // get the texture's center
		sfVector2u tempSizeTexture2 = sfTexture_getSize(texture2);
		sfVector2u tempSizeTexture3 = sfTexture_getSize(texture3);
		sfVector2u tempSizeTexture4 = sfTexture_getSize(texture4);
		sfVector2u tempSizeTexture5 = sfTexture_getSize(texture5);
		sfVector2u tempSizeTexture6 = sfTexture_getSize(texture6);
		sfSprite_setOrigin(toInit->Background, center);

		center.x = (float)tempSizeTexture2.x / 2.0f;
		center.y = (float)tempSizeTexture2.y / 2.0f;

		sfSprite_setOrigin(toInit->Arrow[0].Sprites[0], center);

		center.x = (float)tempSizeTexture3.x / 2.0f;
		center.y = (float)tempSizeTexture3.y / 2.0f;

		sfSprite_setOrigin(toInit->Arrow[1].Sprites[0], center);

		center.x = (float)tempSizeTexture4.x / 2.0f;
		center.y = (float)tempSizeTexture4.y / 2.0f;

		sfSprite_setOrigin(toInit->Scursor.Sprite, center);

		center.x = (float)tempSizeTexture5.x / 2.0f;
		center.y = (float)tempSizeTexture5.y / 2.0f;

		sfSprite_setOrigin(toInit->Arrow[0].Sprites[1], center);

		center.x = (float)tempSizeTexture6.x / 2.0f;
		center.y = (float)tempSizeTexture6.y / 2.0f;

		sfSprite_setOrigin(toInit->Arrow[1].Sprites[1], center);
	}

	sfVector2u tempSize = sfTexture_getSize(sfSprite_getTexture(toInit->Scursor.Sprite));
	toInit->Scursor.spriteSize.x = (float)tempSize.x * scale.x;
	toInit->Scursor.spriteSize.y = (float)tempSize.y * scale.y;

	sfSprite_setScale(toInit->Scursor.Sprite, scale);

	toInit->Scursor.spriteScale = scale;

	toInit->Scursor.minPos.y = toInit->Scursor.maxPos.y = toInit->Scursor.Pos.y = pos.y;

	toInit->Scursor.minPos.x = pos.x - (slideSize.x * scale.x) / 2.0f;
	toInit->Scursor.maxPos.x = pos.x + (slideSize.x * scale.x) / 2.0f;

	toInit->Scursor.Pos.y = pos.y;
	toInit->Scursor.Pos.x = toInit->Scursor.minPos.x + (toInit->Scursor.maxPos.x - toInit->Scursor.minPos.x) * (float)(toInit->valueOfSlider / 100.0f);

	sfSprite_setPosition(toInit->Scursor.Sprite, toInit->Scursor.Pos);

	// get the texture's center
	sfSprite_setPosition(toInit->Background, pos);
	toInit->Pos = pos;


	sfVector2f scaleArrow = { ratioX, ratioY };

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			sfSprite_setScale(toInit->Arrow[i].Sprites[j], scaleArrow);
		}
	}

	pos.x -= toInit->Scursor.spriteSize.x / 2.0f + 85.0f * ratioX;
	sfSprite_setPosition(toInit->Arrow[0].Sprites[0], pos);
	sfSprite_setPosition(toInit->Arrow[0].Sprites[1], pos);

	pos.x += toInit->Scursor.spriteSize.x + 170.0f * ratioX;
	sfSprite_setPosition(toInit->Arrow[1].Sprites[0], pos);
	sfSprite_setPosition(toInit->Arrow[1].Sprites[1], pos);

	sfVector2f PosScursor = { toInit->Pos.x - toInit->Scursor.spriteSize.x / 2.0f, toInit->Pos.y + (2.0f*ratioY) - toInit->Scursor.spriteSize.y / 2.0f };

	toInit->Scursor.sliderVertex = sfVertexArray_create();
	sfVertexArray_setPrimitiveType(toInit->Scursor.sliderVertex, sfQuads);

	sfVertex v = { PosScursor, sfWhite, (sfVector2f) { 0.0f, 0.0f } };
	sfVertexArray_append(toInit->Scursor.sliderVertex, v);

	v.position.x += toInit->Scursor.spriteSize.x;
	v.texCoords.x += toInit->Scursor.spriteSize.x / scale.x;

	sfVertexArray_append(toInit->Scursor.sliderVertex, v);

	v.position.y += toInit->Scursor.spriteSize.y;
	v.texCoords.y += toInit->Scursor.spriteSize.y / scale.y;

	sfVertexArray_append(toInit->Scursor.sliderVertex, v);

	v.position.x -= toInit->Scursor.spriteSize.x;
	v.texCoords.x -= toInit->Scursor.spriteSize.x / scale.x;

	sfVertexArray_append(toInit->Scursor.sliderVertex, v);

	toInit->Scursor.CursorRS = (sfRenderStates*)calloc(1, sizeof(sfRenderStates));

	sfVector2f Origin = sfVertexArray_getVertex(toInit->Scursor.sliderVertex, 0)->position;
	sfVertex* temp1 = sfVertexArray_getVertex(toInit->Scursor.sliderVertex, 1);
	sfVertex* temp2 = sfVertexArray_getVertex(toInit->Scursor.sliderVertex, 2);

	temp1->position.x = temp2->position.x = toInit->Scursor.spriteSize.x * (toInit->valueOfSlider / 100.0f) + Origin.x;
	temp1->texCoords.x = temp2->texCoords.x = (toInit->Scursor.spriteSize.x / scale.x) * (toInit->valueOfSlider / 100.0f);

	toInit->Scursor.CursorRS->blendMode = sfBlendAlpha;
	toInit->Scursor.CursorRS->shader = NULL;
	toInit->Scursor.CursorRS->texture = sfTexture_copy(sfSprite_getTexture(toInit->Scursor.Sprite));
	toInit->Scursor.CursorRS->transform = sfTransform_Identity;
}

void ResponsiveSliderSelector(SliderSelector* toUpdate, sfVector2f newPos, sfVector2f newSize, float ratioX, float ratioY, const bool isCentered)
{
	sfVector2f scale;

	sfTexture *temp = (sfTexture *)sfSprite_getTexture(toUpdate->Background);
	sfVector2u tempSize = sfTexture_getSize(temp);
	scale.x = newSize.x / (float)tempSize.x;
	scale.y = newSize.y / (float)tempSize.y;

	sfSprite_setScale(toUpdate->Background, scale);


	if (isCentered)
	{
		sfVector2f center = { (float)tempSize.x / 2, (float)tempSize.y / 2 }; // get the texture's center
		sfSprite_setOrigin(toUpdate->Background, center);
	}

	sfSprite_setPosition(toUpdate->Background, newPos);

	sfSprite_setScale(toUpdate->Scursor.Sprite, (sfVector2f) { 1.0f, 1.0f });
	sfVector2u tempSize2 = sfTexture_getSize(sfSprite_getTexture(toUpdate->Scursor.Sprite));

	toUpdate->Scursor.spriteSize.x = (float)tempSize2.x * scale.x;
	toUpdate->Scursor.spriteSize.y = (float)tempSize2.y * scale.y;

	sfSprite_setScale(toUpdate->Scursor.Sprite, scale);

	toUpdate->Scursor.spriteScale = scale;

	toUpdate->Scursor.minPos.y = toUpdate->Scursor.maxPos.y = toUpdate->Scursor.Pos.y = newPos.y;

	toUpdate->Scursor.minPos.x = newPos.x - (tempSize.x * scale.x) / 2.0f;
	toUpdate->Scursor.maxPos.x = newPos.x + (tempSize.x * scale.x) / 2.0f;

	toUpdate->Scursor.Pos.y = newPos.y;
	toUpdate->Scursor.Pos.x = toUpdate->Scursor.minPos.x + (toUpdate->Scursor.maxPos.x - toUpdate->Scursor.minPos.x) * (float)(toUpdate->valueOfSlider / 100.0f);

	sfSprite_setPosition(toUpdate->Scursor.Sprite, toUpdate->Scursor.Pos);

	toUpdate->Pos = newPos;

	sfVector2f scaleArrow = { ratioX, ratioY };

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			sfSprite_setScale(toUpdate->Arrow[i].Sprites[j], scaleArrow);
		}
	}

	newPos.x -= toUpdate->Scursor.spriteSize.x / 2.0f + 85.0f * ratioX;
	sfSprite_setPosition(toUpdate->Arrow[0].Sprites[0], newPos);
	sfSprite_setPosition(toUpdate->Arrow[0].Sprites[1], newPos);

	newPos.x += toUpdate->Scursor.spriteSize.x + 170.0f * ratioX;
	sfSprite_setPosition(toUpdate->Arrow[1].Sprites[0], newPos);
	sfSprite_setPosition(toUpdate->Arrow[1].Sprites[1], newPos);

	sfVertexArray_clear(toUpdate->Scursor.sliderVertex);

	sfVector2f PosScursor = { toUpdate->Pos.x - toUpdate->Scursor.spriteSize.x / 2.0f, toUpdate->Pos.y + (2.0f*ratioY) - toUpdate->Scursor.spriteSize.y / 2.0f };

	sfVertex v = { PosScursor, sfWhite, (sfVector2f) { 0.0f, 0.0f } };
	sfVertexArray_append(toUpdate->Scursor.sliderVertex, v);

	v.position.x += toUpdate->Scursor.spriteSize.x;
	v.texCoords.x += toUpdate->Scursor.spriteSize.x / scale.x;

	sfVertexArray_append(toUpdate->Scursor.sliderVertex, v);

	v.position.y += toUpdate->Scursor.spriteSize.y;
	v.texCoords.y += toUpdate->Scursor.spriteSize.y / scale.y;

	sfVertexArray_append(toUpdate->Scursor.sliderVertex, v);

	v.position.x -= toUpdate->Scursor.spriteSize.x;
	v.texCoords.x -= toUpdate->Scursor.spriteSize.x / scale.x;

	sfVertexArray_append(toUpdate->Scursor.sliderVertex, v);

	sfVector2f Origin = sfVertexArray_getVertex(toUpdate->Scursor.sliderVertex, 0)->position;
	sfVertex* temp1 = sfVertexArray_getVertex(toUpdate->Scursor.sliderVertex, 1);
	sfVertex* temp2 = sfVertexArray_getVertex(toUpdate->Scursor.sliderVertex, 2);

	temp1->position.x = temp2->position.x = toUpdate->Scursor.spriteSize.x * (toUpdate->valueOfSlider / 100.0f) + Origin.x;
	temp1->texCoords.x = temp2->texCoords.x = (toUpdate->Scursor.spriteSize.x / scale.x) * (toUpdate->valueOfSlider / 100.0f);
}

void SetTitleMenu(Menu* menu, char* fileDirectory)
{
	sfTexture_destroy((sfTexture*)sfSprite_getTexture(menu->title));
	sfTexture* newTexture = sfTexture_createFromFile(fileDirectory, NULL);
	sfSprite_setTexture(menu->title, newTexture, 0);
}
