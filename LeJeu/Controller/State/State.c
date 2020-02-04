#include "State.h"
#include "../MainSceneController/MainSceneController.h"
#include <stdlib.h>
#include <Windows.h>


GameState gameState = { 0 };

void InitGameScene()
{
	gameState.actualScene = introduction;
	gameState.isPaused = sfFalse;
	gameState.actualDungeonId = beginningDungeon;
	gameState.typeOfGame = notAssigned;
	gameState.typeOfDungeon = dungeonCorridor;
	gameState.EndTransitioning = sfFalse;
}

Scene GetActualScene()
{
	return(gameState.actualScene);
}

GameState GetState()
{
	return(gameState);
}

void ChangeScene(const int id)
{
	gameState.actualScene = id;
}

DungeonId GetActualDungeonId()
{
	return gameState.actualDungeonId;
}

DWORD WINAPI ThreadDelayTransitioning(void *data)
{
	Sleep(10000);
	gameState.EndTransitioning = sfTrue;
	return 0;
}
void SetTransitioning(const sfBool value)
{
	static sfBool isWaiting = sfFalse;
	if (value == sfTrue && !isWaiting)
	{
		HANDLE waitThread = CreateThread(NULL, 0, ThreadDelayTransitioning, NULL, 0, NULL);
		isWaiting = sfTrue;
	}
	else if(value == sfFalse)
	{
		gameState.EndTransitioning = value;
		isWaiting = sfFalse;
	}
}

sfBool GetTransitioning()
{
	return(gameState.EndTransitioning);
}

void SetScene(const int newScene)
{
	sfBool mayNeedToInit = sfFalse;

	if (gameState.actualScene == mainMenu)
	{
		mayNeedToInit = sfTrue;
	}
	gameState.actualScene = newScene;

	if (mayNeedToInit && gameState.actualScene == mainScene)
	{
		if (gameState.typeOfGame != campaign)
		{
			gameState.actualScene = loading;
			LoadMainScene(getConfigMode(), gameState.actualDungeonId);
		}
		else
		{
			InitGamedataForCampaign();
			gameState.actualScene = worldMap;
		}
	}
}

void SetSceneAfterDungeon()
{
	gameState.actualScene = worldMap;
}

void SetSceneAfterWorldMap()
{
	gameState.actualScene = loading;
	LoadMainScene(getConfigMode(), gameState.actualDungeonId);
}

void SetTypeOfGame(const int newID)
{
	gameState.typeOfGame = newID;
}

int GetTypeOfDungeon()
{
	return(gameState.typeOfDungeon);
}

void UpdateState(sfRenderWindow* mainWindow, const float deltaTime)
{
	switch (gameState.actualScene)
	{
	case(mainScene):
		UpdateMainScene(mainWindow, deltaTime);
		break;
	case(worldMap):
		UpdateWorldMapController(deltaTime);
		break;
	default:
		break;
	}
}

void SetStatePause(const sfBool isPaused)
{
	gameState.isPaused = isPaused;
}

void SetIDDungeon(const DungeonId newDungeonId)
{
	gameState.actualDungeonId = newDungeonId;
	if (gameState.actualDungeonId == slimeDungeon || gameState.actualDungeonId == beginningDungeon)
	{
		gameState.typeOfDungeon = dungeonCorridor;
	}
	else
	{
		gameState.typeOfDungeon = dungeonPortal;
	}
}