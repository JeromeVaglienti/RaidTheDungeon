#ifndef _RESOURCESMANAGER_H
#define _RESOURCESMANAGER_H

#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include "..\State\State.h"
#include "..\MainSceneController\EnvironmentController\EnvironmentController.h"
#include "..\MainSceneController\ShadersController\ShadersController.h"
#include "..\MainSceneController\SpellsController\SpellsController.h"
#include "..\MainSceneController\BossController\BossController.h"
#include "..\MainSceneController\ShotsController\ShotsController.h"
#include "..\MainSceneController\LootsController\LootsController.h"
#include "..\..\DevTools\NodeDisplayer.h"
#include "..\..\Model\Config\Config.h"
#include "..\MainSceneController\PlayerController\PlayerController.h"
#include "..\MainSceneController\EnemyController\EnemyController.h"
#include "..\Transition\Transition.h"
#include "..\MainSceneController\TrapController\TrapController.h"

typedef struct
{
	DungeonId ID;
	sfBool isLoaded;
	void (*ptrLoad[2])();
}ResourcesManagerInfo;


void InitResourceManagerInfo();
DWORD WINAPI ProceedResourceManagement(void* data);


#endif // !_RESOURCESMANAGER_H