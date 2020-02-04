#ifndef _BUTTONS_H
#define _BUTTONS_H

typedef struct BUTTON
{
	sfFloatRect rect; // the button's hitbox
	int numberToReturn; // number returned by the button
	sfSprite* sprite; // explicit enough
	bool isWorking; // false : disables the button, true : activates it
	bool isDisplay;
	sfText* text;
	sfText* text2;
}Button;

// changes the button's state
// if the button is active, disables it, else enables it
void SwitchButtonState(Button* button, const bool isActive);

// Display or not the button
// If the button is desable, the button is automaticaly desactivated ( bool isWorking = false ).
void SwitchButtonDisplay(Button* button, const bool isDisplay);

// detects which button is pressed and returns its value
// if none is, returns -1
int GetButtonValue(const sfVector2i mousePosition, const Button testedButton);

// detects which button is pressed and returns its value
// if none is, returns -1
int GetButtonsValue(const sfVector2i mousePosition, const Button testedButtons[], const int numberButtons);

// creates a button
Button InitButton(const sfVector2f size, const sfVector2f buttonPos, const int numberToReturn, const char* pathOfSpriteToLoad, const bool isCentered);

void ResponsiveButton(Button* toUpdate, const sfVector2f newSize, const sfVector2f newButtonPos, const bool isCentered);

// display a button if it is enabled
void DisplayButtonHUD(sfRenderWindow* window, const Button button);

// display buttons if they are enabled
void DisplayButtons(sfRenderWindow* window, const Button buttons[], const int numberSprites);

// changes the button's position and its hitbox
void ChangeButtonPosition(Button* button, const sfVector2f newPosition);

void InitTextButton(Button* button, const char* string, const sfColor color, const int size);

void SetStringButton(Button* button, const char* string);

void SetColorButton(Button* button, const sfColor color);
#endif