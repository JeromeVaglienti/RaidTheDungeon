#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include <time.h>
#include <SFML\System.h>
#include <SFML\Window.h>
#include <SFML\Graphics.h>
#include "Controller\GraphicsUtilities\Utilities.h"
#include "Controller\HUD\HUDController.h"
#include "Controller\MainSceneController\MainSceneController.h"
#include "View\ViewManager.h"
#include "Controller\State\State.h"
#include "Model\Config\Config.h"
#include "Controller\Transition\Transition.h"
#include "Controller\MainSceneController\ShadersController\ShadersController.h"
#include "Controller\Serialization\Serialization.h"
#include "Controller\Sounds\Sounds.h"
#include "Controller\ResourcesManager\ResourcesManager.h" 
#include "Controller\WorldMapController\WorldMapController.h"

void Display(sfRenderWindow *mainWindow)
{
	sfRenderWindow_clear(mainWindow, sfBlack);
	/*sfRenderTexture* tempTexture = GetNormalMap();
	if (tempTexture)
	{
		sfRenderTexture_clear(tempTexture, sfRed);
	}*/
	Scene tempScene = GetActualScene();

	switch (tempScene)
	{
		case introduction:
		case loading:
			DisplayTransition(mainWindow, tempScene);
			break;
		case mainMenu:

			DisplayUI(mainWindow);
			break;
		case mainScene:
			DisplayMainScene(mainWindow);
			break;
		case worldMap:
			DisplayWorldMapController(mainWindow);
			break;
		default:
			break;
	}

	sfRenderWindow_display(mainWindow);
}

int main()
{
	sfClock *myClock = NULL;

	sfRenderWindow *window;

	window = NULL;

	InitConfig(&window);

	sfRenderWindow_setMouseCursorVisible(window, sfFalse);

	myClock = sfClock_create();

	srand((unsigned int)time(NULL));

	InitSaveData();
	InitResourceManagerInfo();
	InitSoundManager();
	InitGameScene();
	InitUI(getConfigMode());
	InitWorldMapController(getConfigMode());
	//Use to Init Inputs Management
	OnAwakeViewManager();
	//LoadTest();
	while (sfRenderWindow_isOpen(window))
	{
		sfJoystick_update();
		const float deltaTime = sfTime_asSeconds(sfClock_restart(myClock));
		
#if _DEBUG	
		if (sfKeyboard_isKeyPressed(sfKeyP))
		{
			sfRenderWindow_close(window);
		}
#endif
		UpdateViewManager(window, deltaTime, UpdateUIOnEvent);

		UpdateState(window, deltaTime);

		Display(window);
	}
	return 0;
}


