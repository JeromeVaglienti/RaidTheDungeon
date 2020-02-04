#ifndef _MENU_H
#define _MENU_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SFML\Graphics.h>
#include "..\Text\Text.h"
#include "../Button/Buttons.h"
#include "../../../Model/Config/Config.h"
#include "..\..\GraphicsUtilities\Utilities.h"
#include "..\..\..\View\EventList\EventList.h"
#define TEXT_SIZE_MENU 45

typedef struct
{
	sfSprite* indexDisplay;
	int indexPos;
	sfVector2f offset;
	sfVector2f originCursor;
}Cursor;

typedef struct
{
	sfVector2f posFirst;
	sfText **textArray;
	int numberOfText;
	int actualTextToDisplay;
}TextDisplayer;

typedef struct
{
	sfVector2f Position;
	sfSprite** Class;
	int NumberOfClass;
	int ActualClassToDisplay;
	sfBool isOn;
}ClassDisplayer;

typedef struct
{
	sfSprite* Sprites[2];
	sfBool isActivated;
	sfVector2f Pos;
}DirectionalArrow;

typedef struct
{
	DirectionalArrow Arrows[2];
	TextDisplayer textDisplay;
	sfVector2f positionText;
	sfSprite* selectorBackground;
	sfFloatRect rect;
	sfBool isOn;
	sfText *Legend;
}Selector;

typedef struct
{
	sfSprite* Sprite;
	sfVector2f Pos;
	sfVector2f minPos;
	sfVector2f maxPos;
	sfVertexArray* sliderVertex;
	sfVector2f spriteSize;
	sfVector2f spriteScale;
	sfRenderStates *CursorRS;
}SliderCursor;


typedef struct
{
	SliderCursor Scursor;
	DirectionalArrow Arrow[2];
	sfVector2f Pos;
	sfSprite* Background;
	sfText* ValueOfSlider;
	sfText* NameOfSlider;
	float valueOfSlider;
	sfBool isSelected;
}SliderSelector;

typedef struct Menu
{
	sfSprite* background;
	sfSprite* title;
	sfSprite** images;
	Button* button;
	sfText* TextToDisplayLoad;
	int imagesNumber;
	int buttonNumber;
	sfVector2f Position;
	bool isActive;
	Cursor selectionCursor;
	Selector* selectors;
	int selectorsNumber;
	SliderSelector* SliderSelectors;
	int SliderSelectorsNumber;
	sfSprite* SliderCursor;
	ClassDisplayer classDisplayer;
	unsigned short Index;
}Menu;

typedef struct KeyMaping
{
	int indexSelector;
	sfKeyCode Key;
}KeyMaping;



Menu CreateGameMenu(const sfVector2f menuPosition, const Button* buttons, const int buttonNumber, const int imagesNumber, const bool isCentered);
void AddBackgroundToMenu(Menu* menu, const char* fileDirectory, const sfVector2f position, const bool isCentered);
void AddTitleToMenu(Menu* menu, char* fileDirectory, sfVector2f position, const bool isCentered);
void AddImageToMenu(Menu* menu, char* fileDirectory, int arrayIndex, sfVector2f position, const bool isCentered);
void AddImagesToMenu(Menu* menu, char* fileDirectories[], sfVector2f positions[], const bool isCentered);
void AlignMenuButtons(Menu* menu, int firstButtonIndex, sfVector2f initPosition, float offset, int offsetDir);
sfVector2f CenterInMiddleElements(Menu menu, sfVector2f actualCenter, int firstButtonIndex, int lastButtonIndex, int selectorIndex, float offset, int offsetDir);
void AlignButtonsAndSelector(Menu* menu, sfVector2f initPosition, int firstButtonIndex, int lastButtonIndex, int selectorIndex, float offset, int offsetDir);
void InitSelector(Selector* toInit, sfVector2f pos, sfVector2f size, char* spritePath, const bool isCentered);
void ResponsiveSelector(Selector* toUpdate, sfVector2f newPos, sfVector2f newSize, const bool isCentered);
void UpdateMenuPosition(Menu* menu, sfVector2f Position);
void InitArrow(DirectionalArrow *Arrow, sfVector2f PosCenter, float ratioX, float ratioY, float size, sfBool isCentered, char** Path, float Spacing);
KeyMaping UpdateMenu(Menu *menuToUpdate, EventList* eventList, const int typeOfGamePad);
void UpdateMenuLoading(Menu *menuToUpdate, const sfVideoMode mode);
void UpdateMenuSaving(Menu *menuToUpdate, const sfVideoMode mode);
void UpdateControlsMenu(Menu *menuToUpdate);
void DisplayMenu(Menu menu, sfRenderWindow *window);
void InitSliderSelector(SliderSelector* toInit, sfVector2f pos, sfVector2f size, char* spritePath, char* leftArrow, char* rightArrow, char* leftArrowH, char* rightArrowH, const bool isCentered, float ratioX, float ratioY);
void ResponsiveSliderSelector(SliderSelector* toUpdate, sfVector2f newPos, sfVector2f newSize, float ratioX, float ratioY, const bool isCentered);
void SetTitleMenu(Menu* menu, char* fileDirectory);

#endif // !HEADER_MENU
