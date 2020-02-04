#include <SFML\Graphics.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "Buttons.h"
#include "..\Text\Text.h"
#include "..\..\State\State.h"

void SwitchButtonState(Button* button, const bool isActive)
{
	sfColor enableColor = { 255, 255, 255, 255 };
	sfColor desableColor = { 130, 130, 130, 220 };

	button->isWorking = isActive;

	if (button->sprite != NULL)
	{
		if (button->isWorking)
		{
			sfSprite_setColor(button->sprite, enableColor);
		}
		else
		{
			sfSprite_setColor(button->sprite, desableColor);
		}
	}
}

void SwitchButtonDisplay(Button* button, const bool isDisplay)
{
	button->isDisplay = isDisplay;
	if (!isDisplay)
	{
		button->isWorking = false;
	}
}

int GetButtonValue(const sfVector2i mousePosition, const Button testedButton)
{
	int valueToReturn = -1; // the function will return -1 if the buttons is not pressed

	if (testedButton.isWorking && sfFloatRect_contains(&testedButton.rect, (float)mousePosition.x, (float)mousePosition.y)) // if the button is activated
	{
		valueToReturn = testedButton.numberToReturn; // gets the value of the pressed button
	}

	return valueToReturn;
}

int GetButtonsValue(const sfVector2i mousePosition, const Button testedButtons[], const int numberButtons)
{
	int valueToReturn = -1; // the function will return -1 if the buttons is not pressed
	for (int i = 0; i < numberButtons; i++) // checks all buttons of the array
	{
		valueToReturn = GetButtonValue(mousePosition, testedButtons[i]);
		if (valueToReturn != -1)
		{
			return valueToReturn;
		}
	}
	return valueToReturn;
}

Button InitButton(const sfVector2f size, const sfVector2f buttonPos, const int numberToReturn, const char* pathOfSpriteToLoad, const bool isCentered) // creates a button
{
	Button button; // the created button
	sfVector2f scale; // the button's scale
	sfTexture* texture; // the button's texture

	if (pathOfSpriteToLoad != NULL)
	{
		texture = sfTexture_createFromFile(pathOfSpriteToLoad, NULL); // load the button's texture
		sfTexture_setSmooth(texture, true);
	}
	else
	{
		button.isWorking = true;
		button.isDisplay = false;

		button.sprite = NULL;
		button.text = NULL;

		button.rect.top = buttonPos.y;
		button.rect.left = buttonPos.x;
		button.rect.width = size.x;
		button.rect.height = size.y;

		button.numberToReturn = numberToReturn;

		return button;
	}

	button.sprite = sfSprite_create();
	sfTexture_setSmooth(texture, sfTrue);
	sfSprite_setTexture(button.sprite, texture, 0);
	// the scale's ratio (to rescale the texture according to the button's size)
	sfVector2u tempSize = sfTexture_getSize(texture);
	scale.x = size.x / (float)tempSize.x;
	scale.y = size.y / (float)tempSize.y;

	sfSprite_setScale(button.sprite, scale);

	if (isCentered)
	{
		sfVector2f center = { (float)tempSize.x / 2, (float)tempSize.y / 2 }; // get the texture's center
		sfSprite_setOrigin(button.sprite, center);
	}

	sfSprite_setPosition(button.sprite, buttonPos);

	button.rect = sfSprite_getGlobalBounds(button.sprite); // sets the button's hitbox

	button.isWorking = true; // activates the button
	button.isDisplay = true; // the button can be display
	button.text = button.text2 = NULL;
	button.numberToReturn = numberToReturn; // sets the button's value

	return button;
}

void ResponsiveButton(Button* toUpdate, const sfVector2f newSize, const sfVector2f newButtonPos, const bool isCentered)
{
	if (toUpdate->sprite == NULL)
	{
		toUpdate->rect.top = newButtonPos.y;
		toUpdate->rect.left = newButtonPos.x;
		toUpdate->rect.height = newSize.y;
		toUpdate->rect.width = newSize.x;
	}
	else
	{
		sfVector2f scale; // the button's scale
		sfTexture *text = (sfTexture*)sfSprite_getTexture(toUpdate->sprite);
		sfVector2u tempSize = sfTexture_getSize(text);
		scale.x = newSize.x / (float)tempSize.x;
		scale.y = newSize.y / (float)tempSize.y;

		sfSprite_setScale(toUpdate->sprite, scale);

		if (isCentered)
		{
			sfVector2f center = { (float)tempSize.x / 2, (float)tempSize.y / 2 }; // get the texture's center
			sfSprite_setOrigin(toUpdate->sprite, center);
		}

		sfSprite_setPosition(toUpdate->sprite, newButtonPos);

		toUpdate->rect = sfSprite_getGlobalBounds(toUpdate->sprite);
	}
}

void DisplayButtonHUD(sfRenderWindow* window, const Button button)
{
	if (button.isDisplay && button.sprite != NULL && !GetState().isPaused)
	{
		sfRenderWindow_drawSprite(window, button.sprite, NULL);
	}
	if (button.isDisplay && button.text != NULL)
	{
		sfRenderWindow_drawText(window, button.text, NULL);
	}
	if (button.isDisplay && button.text2 != NULL)
	{
		sfRenderWindow_drawText(window, button.text2, NULL);
	}
}

void DisplayButtons(sfRenderWindow* window, const Button buttons[], const int numberSprites)
{
	for (int i = 0; i < numberSprites; i++)
	{
		DisplayButtonHUD(window, buttons[i]);
	}
}

void ChangeButtonPosition(Button* button, const sfVector2f newPosition)
{
	if (button->sprite != NULL)
	{
		sfSprite_setPosition(button->sprite, newPosition); // reset the button's position
		button->rect = sfSprite_getGlobalBounds(button->sprite); // reset the button's hitbox
	}
	else
	{
		//button->rect.left
	}
}

void InitTextButton(Button* button, const char* string, const sfColor color, const int size)
{
	int length = (int)strlen(string);
	sfVector2f pos = (sfVector2f) { 0.0f, 0.0f };
	button->text = InitText(string, pos, color, size);
}

void SetStringButton(Button* button, const char* string)
{
	sfText_setString(button->text, string);
}

void SetColorButton(Button* button, const sfColor color)
{
	sfText_setFillColor(button->text, color);
}