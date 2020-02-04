#include "..\..\Model\Player\Player.h"
#include "..\..\Model\GameData.h"
#include "..\..\Model\Procedural\ProceduralType.h"
#include "..\MainSceneController\SkinsController\SkinsController.h"
#include "..\HUD\MiniMap\MiniMap.h"
#include "..\State\State.h"

typedef enum TypeOfSave
{
	manual,
	quickSave
}TypeOfSave;

typedef struct SaveData
{
	short NumberPlayer;
	Player* player;
	unsigned long int Seed;
	unsigned short NumberOfRoom;
	RoomGameData *Room;
	char SaveName[200];
	char date[80];
	sfBool IsSaved;
	sfBool IsLoaded;
	sfBool IsProceduralDone;
	TypeOfSave TypeOfSave;
	char ManualSaveChoosen;
	sfBool CanReloadSaveName;
	DungeonId IdDungeon;
	TypeOfGame TypeOfGame;
}SaveData;

void InitSaveData();

void SaveGame(const GameData gameData);

void LoadGame(const int numberPlayer, const int number);

sfBool SearchingSave(const int numberPlayer, const int number);

void DeleteSave(const int numberPlayer);

char* ChangeSaveNameButton(const int numberPlayer, const char number);

SaveData GetSaveData();

void SetSaveName(const char *name);

sfBool IsSaved();

void SetSave(const sfBool isSaving);

sfBool IsLoaded();

void SetLoad(const sfBool isLoaded);

sfBool IsProceduralDone();

void SetProceduralDone(const sfBool isDone);

TypeOfSave GetTypeOfSave();

void SetTypeOfSave(const TypeOfSave typeOfSave);

void SetManualSaveChoosen(const char manualSaveChoosen);

sfBool CanReloadSaveName();

void SetCanReloadSaveName(const char canReload);

DungeonId GetDungeonIdSaved();

DungeonId GetDungeonIDInFile(const int number, const int numberPlayer);