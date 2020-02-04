#include "ResourcesManager.h"

void LoadCommonResources();
void LoadResBeginningSlime();
void UnLoadBeginningSlime();
void LoadResDungeonSlime();
void UnLoadResDungeonSlime();
void LoadResDungeonElement();
void UnLoadResDungeonElement();
void LoadResDungeonRazar();
void UnLoadResDungeonRazar();

ResourcesManagerInfo resourcesManager[4];
ResourcesManagerInfo commonResources;

#define NUMBEROFDUNGEON 3

void InitResourceManagerInfo()
{
	resourcesManager[0].ID = beginningDungeon;
	resourcesManager[0].isLoaded = sfFalse;
	resourcesManager[1].ID = slimeDungeon;
	resourcesManager[1].isLoaded = sfFalse;
	resourcesManager[2].ID = elemDungeon;
	resourcesManager[2].isLoaded = sfFalse;
	resourcesManager[3].ID = razarDungeon;
	resourcesManager[3].isLoaded = sfFalse;

	resourcesManager[0].ptrLoad[0] = UnLoadBeginningSlime;
	resourcesManager[0].ptrLoad[1] = LoadResBeginningSlime;

	resourcesManager[1].ptrLoad[0] = UnLoadResDungeonSlime;
	resourcesManager[1].ptrLoad[1] = LoadResDungeonSlime;

	resourcesManager[2].ptrLoad[0] = UnLoadResDungeonElement;
	resourcesManager[2].ptrLoad[1] = LoadResDungeonElement;

	resourcesManager[3].ptrLoad[0] = UnLoadResDungeonRazar;
	resourcesManager[3].ptrLoad[1] = LoadResDungeonRazar;

	commonResources.ID = 0;
	commonResources.isLoaded = sfFalse;
	commonResources.ptrLoad[0] = NULL;
	commonResources.ptrLoad[1] = LoadCommonResources;
}

void LoadEveryLoad()
{
	InitPlayerGraphics(getConfigMode());//Common
	InitSpells();//Common
	if (GetActualDungeonId() == elemDungeon)
	{
		ManageBossControllerElem();
	}
}

void LoadCommonResources()
{
	InitLootsGraphics();//Common

	commonResources.isLoaded = sfTrue;
}

void LoadResBeginningSlime()
{
	LoadResDungeonSlime();
	resourcesManager[beginningDungeon].isLoaded = sfTrue;
	resourcesManager[slimeDungeon].isLoaded = sfFalse;
}

void UnLoadBeginningSlime()
{
	UnLoadResDungeonSlime();
	resourcesManager[beginningDungeon].isLoaded = sfFalse;
}

void LoadResDungeonSlime()
{
	sfVideoMode configMode = getConfigMode();
	InitShotsGraphics(configMode);
	ChangeLoadingProgress(0.1f);
	//loot
	ChangeLoadingProgress(0.1f);
	InitEnemySlimeController(configMode);
	ChangeLoadingProgress(0.1f);
	InitBossControllerSlime();
	ChangeLoadingProgress(0.1f);
	InitEnviDungeonSlime();
	ChangeLoadingProgress(0.1f);
	LoadSlimeTraps();

#if _DEBUG
	InitNodeSpritesDisplay();
#endif

	resourcesManager[slimeDungeon].isLoaded = sfTrue;
}

void UnLoadResDungeonSlime()
{
	UnloadEnemySlimeController();
	UnloadBossControllerSlime();
	UnLoadEnviDungeonSlime();
	UnloadShotsGraphicsSlime();
	UnLoadSlimeTraps();

	resourcesManager[slimeDungeon].isLoaded = sfFalse;
}

void LoadResDungeonElement()
{
	sfVideoMode configMode = getConfigMode();
	InitShotsGraphics(configMode);
	InitEnemyElemController(configMode);
	InitBossControllerElem();
	InitEnviDungeonElem();
	LoadElemTraps();

#if _DEBUG
	InitNodeSpritesDisplay();
#endif

	resourcesManager[elemDungeon].isLoaded = sfTrue;
}

void UnLoadResDungeonElement()
{
	UnloadEnemyElemController();
	UnloadBossControllerElem();
	UnLoadEnviDungeonElem();
	UnloadShotsGraphicsElem();
	UnLoadElemTraps();

	resourcesManager[elemDungeon].isLoaded = sfFalse;
}

void LoadResDungeonRazar()
{
	InitShotsGraphics(getConfigMode());
	InitEnviDungeonRazar();
	InitBossControllerRazar();

#if _DEBUG
	InitNodeSpritesDisplay();
#endif
}

void UnLoadResDungeonRazar()
{
	UnLoadEnviDungeonElem();
	UnloadEnemyElemController();
}


//Clean data when you load the same dungeon and/or data that is common that need clean up between two loading
void CleanUpResources()
{

}

DWORD WINAPI ProceedResourceManagement(void* data)
{
	sfBool Load = *((sfBool*)((void**)data)[0]);
	int IdToManipulate = *((int*)((void**)data)[1]);
	if (resourcesManager[IdToManipulate].isLoaded != Load)
	{
		if (!Load)
		{
			resourcesManager[IdToManipulate].ptrLoad[Load]();
		}
		else
		{
			if (!commonResources.isLoaded)
			{
				commonResources.ptrLoad[Load]();
			}

			for (int i = 0; i < NUMBEROFDUNGEON; i++)
			{
				if (i != IdToManipulate && resourcesManager[i].isLoaded)
				{
					resourcesManager[i].ptrLoad[0]();
				}
			}
			resourcesManager[IdToManipulate].ptrLoad[Load]();
		}
	}

	if (Load)
	{
		LoadEveryLoad();
	}

	return 0;
}
