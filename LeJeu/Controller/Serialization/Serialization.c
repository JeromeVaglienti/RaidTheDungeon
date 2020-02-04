#include "Serialization.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include "dirent.h"
#include "..\Localization\Localization.h"

SaveData saveData;

void InitSaveData()
{
	saveData.IsLoaded = sfFalse;
	saveData.IsSaved = sfFalse;
	saveData.NumberOfRoom = 0;
	saveData.player = NULL;
	saveData.Room = NULL;
	saveData.Seed = 0;
	saveData.CanReloadSaveName = sfFalse;
	saveData.TypeOfGame = notAssigned;
}

void SaveGame(const GameData gameData)
{
	FILE* ptrFile;
	char path[200];
	char buffer[80];
	time_t rawtime;
	struct tm * timeinfo;
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	strftime(buffer, 80, "%Y/%m/%d %H:%M:%S", timeinfo);

	if (saveData.TypeOfSave == manual)
	{
		if (gameData.numberPlayer == 2)
		{
			sprintf(path, "Ressources/Saves/TwoPlayers/manualSave%d.save", saveData.ManualSaveChoosen);
		}
		else
		{
			sprintf(path, "Ressources/Saves/OnePlayer/manualSave%d.save", saveData.ManualSaveChoosen);
		}
	}
	else if (saveData.TypeOfSave == quickSave)
	{
		if (gameData.numberPlayer == 2)
		{
			sprintf(path, "Ressources/Saves/TwoPlayers/quickSave.save");
		}
		else
		{
			sprintf(path, "Ressources/Saves/OnePlayer/quickSave.save");
		}
	}
	DeleteSave(gameData.numberPlayer);
	if (!(ptrFile = fopen(path, "wb")))
	{
		printf("error saving file\n");
		free(ptrFile);
		return;
	}
	else
	{
		fwrite(&buffer, sizeof(char), 80, ptrFile);
		DungeonId idDungeon = GetActualDungeonId();
		fwrite(&idDungeon, sizeof(DungeonId), 1, ptrFile);
		TypeOfGame typeGame = GetState().typeOfGame;
		fwrite(&typeGame, sizeof(TypeOfGame), 1, ptrFile);
		fwrite(&gameData.numberPlayer, sizeof(int), 1, ptrFile);
		PlayersInformations playerInfo = GetPlayersInformations();
		for (int i = 0; i < gameData.numberPlayer; i++)
		{
			fwrite(&gameData.Player[i], sizeof(Player), 1, ptrFile);
			fwrite(&playerInfo.SkinPlayer[i], sizeof(SkinPlayer), 1, ptrFile);
		}
		fwrite(&gameData.dungeon->seed, sizeof(unsigned int), 1, ptrFile);
		fwrite(&gameData.dungeon->numberOfRoom, sizeof(unsigned short), 1, ptrFile);
		for (int i = 0; i < gameData.dungeon->numberOfRoom; i++)
		{
			fwrite(&gameData.dungeon->room[i].IsClear, sizeof(sfBool), 1, ptrFile);
		}
	}
	fclose(ptrFile);
	printf("Save done !\n");
}

sfBool SearchingSave(const int numberPlayer, const int number)
{
	char path[200];
	char saveName[100];
	if (numberPlayer == 1)
	{
		sprintf(path, "Ressources/Saves/OnePlayer/");
	}
	else
	{
		sprintf(path, "Ressources/Saves/TwoPlayers/");
	}

	if (number == 0)
	{
		sprintf(saveName, "quickSave");
	}
	else if (number > 0)
	{
		sprintf(saveName, "manualSave%d", number);
	}

	struct dirent *lecture;
	DIR *rep;
	int i = 0;
	if (numberPlayer == 1)
	{
		rep = opendir("Ressources/Saves/OnePlayer/");
	}
	else
	{
		rep = opendir("Ressources/Saves/TwoPlayers/");
	}

	while ((lecture = readdir(rep))) {
		if (number >= 0)
		{
			if (i > 1)
			{
				if (strstr(lecture->d_name, saveName) != NULL)
				{
					return sfTrue;
				}
			}
		}
		i++;
	}
	closedir(rep);
	if (number < 0)
		return (i > 2);
	else
		return sfFalse;
}

void DeleteSave(const int numberPlayer)
{
	struct dirent *lecture;
	DIR *rep;
	int i = 0;
	char nameSave[50];
	if (saveData.TypeOfSave == manual)
	{
		sprintf(nameSave, "manualSave%d", saveData.ManualSaveChoosen);
	}
	else
	{
		strcpy(nameSave, "quickSave");
	}
	char path[200];
	if (numberPlayer == 1)
	{
		rep = opendir("Ressources/Saves/OnePlayer/");
		strcpy(path, "Ressources/Saves/OnePlayer/");
	}
	else
	{
		rep = opendir("Ressources/Saves/TwoPlayers/");
		strcpy(path, "Ressources/Saves/TwoPlayers/");
	}
	while ((lecture = readdir(rep)))
	{
		if (i > 1)
		{
			if (strstr(lecture->d_name, nameSave) != NULL)
			{
				char fileDirectory[200];
				sprintf(fileDirectory, "%s%s", path, lecture->d_name);
				remove(fileDirectory);
				return;
			}
		}
		i++;
	}
}

char* ChangeSaveNameButton(const int numberPlayer, const char number)
{
	struct dirent *lecture;
	DIR *rep;
	int i = 0;
	char path[200];
	char path2[200];
	if (number == 0)
	{
		strcpy(path, "quickSave");
	}
	else
	{
		sprintf(path, "manualSave%d", number);
	}
	if (numberPlayer == 1)
	{
		rep = opendir("Ressources/Saves/OnePlayer/");
		sprintf(path2, "Ressources/Saves/OnePlayer/");
	}
	else
	{
		rep = opendir("Ressources/Saves/TwoPlayers/");
		sprintf(path2, "Ressources/Saves/TwoPlayers/");
	}

	char* nameFile = malloc(sizeof(char) * 200);
	while ((lecture = readdir(rep))) {
		if (i > 1)
		{
			if (strstr(lecture->d_name, path) != NULL)
			{
				FILE* ptrFile;
				strcat(path2, lecture->d_name);
				if (!(ptrFile = fopen(path2, "rb")))
				{
					printf("error reading file\n");
					free(ptrFile);
					return NULL;
				}
				else
				{
					fread(nameFile, sizeof(char), 80, ptrFile);
				}

				closedir(rep);
				return nameFile;
			}
		}
		i++;
	}
	closedir(rep);
	strcpy(nameFile, GetLocalizationString(145));
	return nameFile;
}

void LoadGame(const int numberPlayer, const int number)
{
	FILE* ptrFile;
	char path[200];
	char saveName[100];
	if (numberPlayer == 1)
	{
		sprintf(path, "Ressources/Saves/OnePlayer/");
	}
	else
	{
		sprintf(path, "Ressources/Saves/TwoPlayers/");
	}

	if (number == 0)
	{
		sprintf(saveName, "quickSave");
	}
	else
	{
		sprintf(saveName, "manualSave%d", number);
	}

	struct dirent *lecture;
	DIR *rep;
	int i = 0;
	if (numberPlayer == 1)
	{
		rep = opendir("Ressources/Saves/OnePlayer/");
	}
	else
	{
		rep = opendir("Ressources/Saves/TwoPlayers/");
	}

	while ((lecture = readdir(rep))) {
		if (i > 1)
		{
			if (strstr(lecture->d_name, saveName) != NULL)
			{
				strcat(path, lecture->d_name);
			}
		}
		i++;
	}
	closedir(rep);

	printf("%s\n", path);

	if (!(ptrFile = fopen(path, "rb")))
	{
		printf("error loading file\n");
		free(ptrFile);
		return;
	}
	else
	{
		fread(&saveData.date, sizeof(char), 80, ptrFile);
		fread(&saveData.IdDungeon, sizeof(DungeonId), 1, ptrFile);
		fread(&saveData.TypeOfGame, sizeof(TypeOfGame), 1, ptrFile);
		printf("%d\n", saveData.TypeOfGame);
		fread(&saveData.NumberPlayer, sizeof(int), 1, ptrFile);
		SetNumberPlayer(saveData.NumberPlayer);
		saveData.player = malloc(sizeof(Player) * saveData.NumberPlayer);

		fread(&saveData.player[0], sizeof(Player), 1, ptrFile);
		SkinPlayer skin;
		fread(&skin, sizeof(SkinPlayer), 1, ptrFile);
		SetPlayer1Informations(skin);
		if (saveData.NumberPlayer == 2)
		{
			fread(&saveData.player[1], sizeof(Player), 1, ptrFile);
			SkinPlayer skin;
			fread(&skin, sizeof(SkinPlayer), 1, ptrFile);
			SetPlayer2Informations(skin);
		}
		fread(&saveData.Seed, sizeof(unsigned int), 1, ptrFile);
		printf("%u\n", saveData.Seed);
		fread(&saveData.NumberOfRoom, sizeof(unsigned short), 1, ptrFile);
		saveData.Room = (RoomGameData*)malloc(sizeof(RoomGameData) * saveData.NumberOfRoom);
		for (int i = 0; i < saveData.NumberOfRoom; i++)
		{
			fread(&saveData.Room[i].IsClear, sizeof(sfBool), 1, ptrFile);
		}
	}
	SetLoad(sfTrue);
	fclose(ptrFile);
}

void SetSaveName(const char *name)
{
	strcpy(saveData.SaveName, name);
}

SaveData GetSaveData()
{
	return saveData;
}

sfBool IsSaved()
{
	return saveData.IsSaved;
}

void SetSave(const sfBool isSaving)
{
	saveData.IsSaved = isSaving;
}

sfBool IsLoaded()
{
	return saveData.IsLoaded;
}

void SetLoad(const sfBool isLoaded)
{
	saveData.IsLoaded = isLoaded;
}

sfBool IsProceduralDone()
{
	return saveData.IsProceduralDone;
}

void SetProceduralDone(const sfBool isDone)
{
	saveData.IsProceduralDone = isDone;
}

TypeOfSave GetTypeOfSave()
{
	return saveData.TypeOfSave;
}

void SetTypeOfSave(const TypeOfSave typeOfSave)
{
	saveData.TypeOfSave = typeOfSave;
}

void SetManualSaveChoosen(const char manualSaveChoosen)
{
	saveData.ManualSaveChoosen = manualSaveChoosen;
}

sfBool CanReloadSaveName()
{
	return saveData.CanReloadSaveName;
}

void SetCanReloadSaveName(const char canReload)
{
	saveData.CanReloadSaveName = canReload;
}

DungeonId GetDungeonIdSaved()
{
	return saveData.IdDungeon;
}

DungeonId GetDungeonIDInFile(const int number, const int numberPlayer)
{
	FILE* ptrFile;
	char path[200];
	DungeonId id;
	if (numberPlayer == 1)
	{
		sprintf(path, "Ressources/Saves/OnePlayer/");
	}
	else
	{
		sprintf(path, "Ressources/Saves/TwoPlayers/");
	}
	if (number == 0)
	{
		strcat(path, "quickSave.save");
	}
	else
	{
		char name[50];
		sprintf(name, "manualSave%d.save", number);
		strcat(path, name);;
	}
	if (!(ptrFile = fopen(path, "rb")))
	{
		free(ptrFile);
		return -1;
	}
	else
	{
		char date[200];
		fread(&date, sizeof(char), 80, ptrFile);
		fread(&id, sizeof(DungeonId), 1, ptrFile);
		fclose(ptrFile);
		return id;
	}
	return -1;
}