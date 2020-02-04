#ifndef INTRODUCTION_H
#define INTRODUCTION_H
#include "..\GraphicsUtilities\CustomSprite\CustomSprite.h"
#include "..\State\State.h"

typedef struct IntroComponents
{
	sfSprite* Start;
	float Timer;
	float Alpha;
	sfColor Color;
}IntroComponents;

void InitTransitions(const sfVideoMode mode);
void ChangeLoadingProgress(const float value);
void UpdateTransition(const float deltaTime, const Scene actualScene);
void DisplayTransition(sfRenderWindow *mainWindow, const Scene actualScene);
#endif
