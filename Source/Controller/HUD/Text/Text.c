#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SFML\Graphics.h>
#include "Text.h"

sfFont* font;

void InitFont()
{
	font = sfFont_createFromFile("Ressources/Font/FromWhereYouAre.ttf");
}

sfText* InitText(const char* message, const sfVector2f position, const sfColor color, const int size)
{
	if (font == NULL)
	{
		InitFont();
	}
	sfText *textToInit = NULL;
	textToInit = sfText_create();
	sfText_setFont(textToInit, font);
	sfText_setString(textToInit, message);
	sfText_setPosition(textToInit, position);
	sfText_setFillColor(textToInit, color);
	sfText_setCharacterSize(textToInit, size);
	sfText_setStyle(textToInit, sfTextRegular);
	return(textToInit);
}

void UpdateTextDisplay(sfText* text, const int dataTribe)
{
	char msgToDisplay[12];
	sprintf(msgToDisplay, "%d", dataTribe);
	sfText_setString(text, msgToDisplay);
}

void UpdateTextsDisplay(sfText** text, const int dataTribe[], const int index)
{
	for (int i = 0; i < index; i++)
	{
		char msgToDisplay[12];
		sprintf(msgToDisplay, "%d", dataTribe[i]);
		sfText_setString(text[i], msgToDisplay);
	}
}

void UpdateTextWithStringDisplay(sfText* text, const char* msg, const int data)
{
	char msgToDisplay[50];
	sprintf(msgToDisplay, "%s %d", msg, data);
	sfText_setString(text, msgToDisplay);
}

void UpdateTextWithSeparator(sfText* text, const int data1, const char* separator, const int data2)
{
	char msgToDisplay[50];
	sprintf(msgToDisplay, "%d %s %d", data1, separator, data2);
	sfText_setString(text, msgToDisplay);
}

void CenterText(sfText* text, sfVector2f posToCenterTo)
{
	sfFloatRect tempRect = sfText_getGlobalBounds(text);
	posToCenterTo.x -= tempRect.width / 2;
	posToCenterTo.y -= (sfText_getCharacterSize(text) / 2 + tempRect.height) / 2;
	sfText_setPosition(text, posToCenterTo);
}

void AddOutline(sfText* text, const float sizeOfOutLine, const sfColor color)
{
	sfText_setOutlineThickness(text, sizeOfOutLine);
	sfText_setOutlineColor(text, color);
}

void RescaleText(sfText* text, const float sizeX, const float sizeY, const float ratioX, const float ratioY)
{
	sfVector2f newScale = { 1.0f, 1.0f };
	sfText_setScale(text, newScale);
	sfFloatRect temp = sfText_getGlobalBounds(text);
	if (temp.width > sizeX * ratioX)
	{
		newScale.x = (sizeX * ratioX) / temp.width;
	}

	if (temp.height > sizeY * ratioY)
	{
		newScale.y = (sizeY * ratioY) / temp.height;
	}
	sfText_setScale(text, newScale);
}