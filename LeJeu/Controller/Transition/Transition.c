#include "Transition.h"
#include "..\State\State.h"
#include "..\GraphicsUtilities\Utilities.h"
#include "..\GraphicsUtilities\Animation\Animation.h"
#include "..\Sounds\Sounds.h"
#include <Windows.h>

IntroComponents introComponents;
IntroComponents logoComponents;
IntroComponents fadeComponents;
Animation loadingAnimation;
float progress;
int StatusConvertedInFrame;
float ActualIdIntro;

DWORD WINAPI LoadAnimationThread(void* data)
{
	loadingAnimation = LoadAnimation(29, (29.0f / 12.0f), "Ressources/Sprites/Transition/LoadingAnimation/", "png", (sfVector2f) { 0.0f, 0.0f }, sfTrue, 1);
	sfVideoMode mode = *((sfVideoMode*)data);
	sfVector2f scaleIntro = { (float)mode.width / 1920.f , (float)mode.height / 1080.f };
	sfVector2f centerOfScreen = { mode.width / 2.f, mode.height / 2.f };
	for (int i = 0; i < loadingAnimation.numberOfFrame; i++)
	{
		sfSprite_setScale(loadingAnimation.spriteArray[i], scaleIntro);
		sfSprite_setPosition(loadingAnimation.spriteArray[i], centerOfScreen);
	}
	return 0;
}


void InitTransitions(const sfVideoMode mode)
{
	HANDLE loadAnimThread = CreateThread(NULL, 0, LoadAnimationThread, (void*)&mode, 0, NULL);

	introComponents.Start = LoadSprite("Ressources/Sprites/Transition/start.png", 0);
	logoComponents.Start = LoadSprite("Ressources/Sprites/Transition/Insomniarts Logo.png", 0);
	sfVector2f scaleIntro = { mode.width / 1920.f ,  mode.height / 1080.f };
	sfSprite_setScale(introComponents.Start, scaleIntro);
	sfSprite_setScale(logoComponents.Start, scaleIntro);
	logoComponents.Timer = introComponents.Timer = 0.f;
	logoComponents.Alpha = introComponents.Alpha = 255.f;
	introComponents.Color = sfColor_fromRGBA(255, 255, 255, (sfUint8)introComponents.Alpha);
	logoComponents.Color = sfColor_fromRGBA(255, 255, 255, (sfUint8)logoComponents.Alpha);
	progress = 0.0f;
	StatusConvertedInFrame = 0;
	ActualIdIntro = 0;
}

void ChangeLoadingProgress(const float value)
{
	progress += value;
	printf("Progress %f \n", progress);
	StatusConvertedInFrame = (int)(progress * loadingAnimation.numberOfFrame);
	if (value == -1.0f)
	{
		loadingAnimation.actualDuration = 0.0f;
		loadingAnimation.actualFrameToShow = 0;
	}
}

void UpdateTransition(const float deltaTime, const Scene actualScene)
{
	switch (actualScene)
	{
	case(introduction):
		if (sfKeyboard_isKeyPressed(sfKeySpace))
		{
			SetScene(mainMenu);
			PlayMusic(mainMenuMusic);
		}
		if (introComponents.Timer <= 1.f)
		{
			introComponents.Timer += deltaTime;
		}
		else //change the alpha of the introduction's sprite
		{
			if (ActualIdIntro == 0)
			{
				if (introComponents.Alpha > 0)
				{
					introComponents.Alpha -= 80 * deltaTime;
					introComponents.Color.a = (sfUint8)introComponents.Alpha;
					sfSprite_setColor(introComponents.Start, introComponents.Color);
				}
				else
				{
					introComponents.Timer = 0.f;
					introComponents.Alpha = 255;
					introComponents.Color.a = (sfUint8)introComponents.Alpha;
					ActualIdIntro = 1;
				}
			}
			else
			{
				if (logoComponents.Alpha > 0)
				{
					logoComponents.Alpha -= 80 * deltaTime;
					logoComponents.Color.a = (sfUint8)logoComponents.Alpha;
					sfSprite_setColor(logoComponents.Start, logoComponents.Color);
				}
				else
				{
					logoComponents.Timer = 0.f;
					logoComponents.Alpha = 255;
					logoComponents.Color.a = (sfUint8)logoComponents.Alpha;
					SetScene(mainMenu);
					PlayMusic(mainMenuMusic);
				}
			}

		}
		break;
	case(loading):
		UpdateAnimation(&loadingAnimation, deltaTime);
		break;
	default:
		break;
	}
}

void DisplayTransition(sfRenderWindow *mainWindow, const Scene actualScene)
{
	switch (actualScene)
	{
	case(introduction):
		sfSprite_setPosition(introComponents.Start, (sfVector2f) { 0.0f, 0.0f });
		if (ActualIdIntro == 0)
		{
			sfRenderWindow_drawSprite(mainWindow, introComponents.Start, NULL);
		}
		else
		{
			sfRenderWindow_drawSprite(mainWindow, logoComponents.Start, NULL);
		}
		break;
	case(loading):
		sfRenderWindow_drawSprite(mainWindow, loadingAnimation.spriteArray[loadingAnimation.actualFrameToShow], NULL);
		break;
	default:
		break;
	}
}