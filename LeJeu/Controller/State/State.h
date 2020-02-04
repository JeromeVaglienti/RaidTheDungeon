#ifndef _STATE_H
#define _STATE_H

#include <stdio.h>
#include <stdlib.h>
#include <SFML\Graphics.h>

typedef enum
{
	introduction,
	mainMenu,
	mainScene,
	loading,
	worldMap
}Scene;

typedef enum DungeonId
{
	beginningDungeon,
	slimeDungeon,
	elemDungeon,
	razarDungeon
}DungeonId;

typedef enum DungeonType
{
	dungeonCorridor,
	dungeonPortal
}DungeonType;

typedef enum TypeOfGame
{
	notAssigned,
	campaign,
	oneLevel
}TypeOfGame;

typedef struct
{
	Scene actualScene;
	sfBool isPaused;
	DungeonId actualDungeonId;
	TypeOfGame typeOfGame;
	DungeonType typeOfDungeon;
	sfBool EndTransitioning;
}GameState;

void InitGameScene();
Scene GetActualScene();
GameState GetState();
DungeonId GetActualDungeonId();
void SetTransitioning(const sfBool value);
sfBool GetTransitioning();
void ChangeScene(const int id);
void SetScene(const int newScene);
void SetSceneAfterDungeon();
void SetSceneAfterWorldMap();
void SetTypeOfGame(const int newID);
void SetStatePause(const sfBool isPaused);
void SetIDDungeon(const DungeonId newDungeonId);
int GetTypeOfDungeon();
void UpdateState(sfRenderWindow* mainWindow, const float deltaTime);

#endif // !_STATE_H
