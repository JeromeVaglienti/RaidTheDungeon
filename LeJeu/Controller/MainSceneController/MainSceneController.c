#include "MainSceneController.h"
#include "..\GraphicsUtilities\Utilities.h"
#include "..\..\Model\GameData.h"
#include "..\Text\Text.h"
#include "..\..\Model\Procedural\ProceduralMain.h"
#include "..\Serialization\Serialization.h"
#include "..\ResourcesManager\ResourcesManager.h"
#include <Windows.h>

GameData gameData = { 0 };
Dungeon mainDungeon = { 0 };
sfBool isReloaded = sfFalse;
sfBool isDezoomed = sfFalse;

typedef struct
{
	sfVector2f Ratio;
	sfGlslVec3 transitionShader;
}MainSceneContainer;

MainSceneContainer MSContainer = { 0 };

DWORD WINAPI GenerationCalculation(void * data)
{
	SaveData tempSaveData = GetSaveData();
	if (tempSaveData.IsLoaded)
	{
		SetIDDungeon(GetDungeonIdSaved());
	}
	DungeonId tempId = GetActualDungeonId();


	if (tempId == beginningDungeon)
	{
		GenerateDungeon(&mainDungeon, 5, 5, 25, 40, tempId);
	}
	else if (tempId == slimeDungeon)
	{
		GenerateDungeon(&mainDungeon, 12, 5, 25, 40, tempId);
	}
	else if (tempId == elemDungeon)
	{
		GenerateDungeon(&mainDungeon, 11, 5, 50, 40, tempId);
	}
	else if (tempId == razarDungeon)
	{
		GenerateDungeon(&mainDungeon, 1, 5, 5, 5, tempId);
	}

	mainDungeon.NeedToClean = sfTrue;
	InitNewDungeonGameData(&gameData, &mainDungeon);
	if (tempSaveData.IsLoaded)
	{
		for (int i = 0; i < tempSaveData.NumberOfRoom; i++)
		{
			gameData.dungeon->room[i].IsClear = tempSaveData.Room[i].IsClear;
			SetRoomVisited(i, gameData.dungeon->room[i].IsClear);
		}
		SetLoad(sfFalse);
	}
	return 0;
}

//Init other thread and init gameData
DWORD WINAPI MainSecondThread(void *data)
{
	sfBool value = sfTrue;
	int valueInt = GetActualDungeonId();
	void* dataToSend[2] = { &(value), &valueInt };

	HANDLE LoadThread = CreateThread(NULL, 0, ProceedResourceManagement, dataToSend, 0, NULL);
	HANDLE ProceduralGenerationThread = CreateThread(NULL, 0, GenerationCalculation, NULL, 0, NULL);
	PlayersInformations tempPlayersInformations = GetPlayersInformations();


	if ((GetState().typeOfGame != campaign))
	{
		InitGameData(&gameData, tempPlayersInformations.NumberOfPlayer);
		InitHUD(getConfigMode(), tempPlayersInformations);
	}
	else
	{
		SetBossIdRoom(&gameData);
		InitLoot(&gameData.LootData);
	}

	//Hold the status of a Thread
	DWORD holder;
	DWORD holder2;
	do
	{
		GetExitCodeThread(LoadThread, &holder);
		GetExitCodeThread(ProceduralGenerationThread, &holder2);
		Sleep(20);
	} while (holder == STILL_ACTIVE || holder2 == STILL_ACTIVE);

	printf("Setting scene to mainScene\n");
	SetScene(2);
	StopMusic(mainMenuMusic);
	if (valueInt == razarDungeon)
	{
		PlayMusic(razarBattle);
	}
	else if (valueInt == elemDungeon)
	{
		PlayMusic(ambiantElem + rand() % 4);
	}
	else if (valueInt == slimeDungeon || valueInt == beginningDungeon)
	{
		PlayMusic(scary);
	}
	return 0;
}

void InitGamedataForCampaign()
{
	PlayersInformations tempPlayersInformations = GetPlayersInformations();
	InitGameData(&gameData, tempPlayersInformations.NumberOfPlayer);
	InitHUD(getConfigMode(), tempPlayersInformations);
	for (int i = 0; i < tempPlayersInformations.NumberOfPlayer; i++)
	{
		UpgradeAtMaxAllSkillTree(&gameData.Player[i].SkillTree);
		AssignSkillTreeToPlayer(&gameData.Player[i]);
	}
}

void LoadMainScene(const sfVideoMode mode, const DungeonId ID)
{
	float ratioX = mode.width / 1920.0f;
	float ratioY = mode.height / 1080.0f;

	if (!isReloaded)
	{
		mainDungeon.NeedToClean = sfFalse;
		InitShadersContainer(getConfigMode());
		InitializeSounds();

		MSContainer.Ratio.x = ratioX;
		MSContainer.Ratio.y = ratioY;
	}
	else
	{
		Restart(&gameData);
	}

	InitLights(0);

	HANDLE WaitingThread = CreateThread(NULL, 0, MainSecondThread, NULL, 0, NULL);
	PlayersInformations playerInfo = GetPlayersInformations();
	for (int i = 0; i < playerInfo.NumberOfPlayer; i++)
	{
		ActivateLigth(i, 1);
		SetIndexSprite(i, 2);
	}

	if (!isReloaded)
	{
		isReloaded = sfTrue;
	}
}

DWORD WINAPI DelayThreadUnZoom(void* data)
{
	Sleep(1000);
	return 0;
}

void UpdateMainScene(sfRenderWindow* window, const float deltaTime)
{
	DungeonId tempDungeonId = GetActualDungeonId();
	GameState gameState = GetState();
	if (gameState.isPaused)
	{
		ShadersContainer shaderList = GetShaderList();
		MSContainer.transitionShader.x = Lerp(MSContainer.transitionShader.x, 0.2126f, 0.005f);
		MSContainer.transitionShader.y = Lerp(MSContainer.transitionShader.y, 0.7152f, 0.005f);
		MSContainer.transitionShader.z = Lerp(MSContainer.transitionShader.z, 0.0722f, 0.005f);
		sfShader_setVec3Uniform(shaderList.Greyscale, "RGB", MSContainer.transitionShader);
		sfShader_setFloatUniform(shaderList.Greyscale, "alpha", 1);
	}
	else MSContainer.transitionShader = (sfGlslVec3) { 0.0f, 0.0f, 0.0f };

	if (gameData.Cam.isZoomed && gameData.Room->RoomId == gameData.IdBossRoom && tempDungeonId == elemDungeon && !gameData.Boss.IsDead)
	{
		SetZoom(&gameData.Cam, sfTrue);
		gameData.Cam.isZoomed = sfFalse;
	}

	if (!gameState.isPaused)
	{
		UpdatePlayerController(gameData.numberPlayer, deltaTime);
		UpdateGameData(&gameData, deltaTime, window);
		UpdateHUD(gameData, deltaTime, getConfigMode());
		if (gameData.Boss.IsSpawned)
		{
			UpdateBossController(&gameData.Boss, deltaTime);
		}

		if (tempDungeonId == elemDungeon)
		{
			UpdateScroller((int)mainDungeon.DGraphicData.room[gameData.Room->RoomId].uniqueID, deltaTime);
		}

		if (GetTransitioning())
		{
			if (UpdateEndTeleporting(deltaTime))
			{
				StopAllMusic();
				if (gameState.typeOfGame == campaign && tempDungeonId != razarDungeon)
				{
					if (tempDungeonId == beginningDungeon)
					{
						for (int i = 0; i < gameData.numberPlayer; i++)
						{
							InitPlayer(&gameData.Player[i], gameData.Player[i].Physics.Rb.Position, gameData.Player[i].ID);
						}
					}
					SetIDDungeon(tempDungeonId + 1);
					SetSceneAfterDungeon();
				}
				else
				{
					SetScene(mainMenu);
					PlayMusic(mainMenuMusic);
				}
				SetTransitioning(sfFalse);
			}
		}
	}

	if (GetRestart())
	{
		Restart(&gameData);
	}
	if (IsSaved())
	{
		sfBool isPlayerFighting = IsPlayerFighting();
		if ((gameData.numberPlayer == 2 && !gameData.Player[0].States.IsDead && !gameData.Player[1].States.IsDead
			&& !isPlayerFighting)
			|| (gameData.numberPlayer == 1 && !gameData.Player[0].States.IsDead && !isPlayerFighting))
		{
			SaveGame(gameData);
			SetCanReloadSaveName(sfTrue);
		}

		SetSave(sfFalse);
	}
}

void DisplayMainSceneSlime(sfRenderWindow *mainWindow)
{
	DisplayCorridor(mainWindow);

	DisplayEnemyParticles(mainWindow);

	DrawTrapFirstLayer(mainWindow);

#if _DEBUG

	static sfBool toShow = sfFalse;

	if (sfKeyboard_isKeyPressed(sfKeyN))
	{
		toShow = sfTrue;
	}
	else if (sfKeyboard_isKeyPressed(sfKeyB))
	{
		toShow = sfFalse;
	}

	if (toShow)
	{
		DisplayOneRoomNodes(mainWindow, gameData.Room->RoomNode, gameData.Cam, gameData.Room, gameData.Player[0].ActualNode);
	}
#endif

	DisplayLoots(gameData.LootData, mainWindow, gameData.Cam);

	DisplayEnemiesSlime(gameData.Room->firstEnemy, mainWindow, gameData.Cam);

	DisplayEnemyVertexArraySlime(mainWindow);

	DisplayPlayers(mainWindow, gameData.Cam, gameData.numberPlayer);

	if (gameData.Boss.IsSpawned)
	{
		DisplayBossController(gameData.Boss, mainWindow, gameData.Cam, mainDungeon.DGraphicData);
	}

	for (int i = 0; i < gameData.numberPlayer; i++)
	{
		SetShots(gameData.Player[i].Weapons.FirstShot, mainWindow, gameData.Cam);
		if (!gameData.Player[i].States.IsDead)
		{
			sfVector2f pos = { gameData.Player[i].Physics.Rb.Position.x / 64.f, gameData.Player[i].Physics.Rb.Position.y / 64.f };
			SetPlayerCirclePos(i, pos);
		}
	}


	SetShots(gameData.Room->FirstShot, mainWindow, gameData.Cam);

	SetShots(gameData.Boss.FirstShot, mainWindow, gameData.Cam);
	DisplayShotVertexArray(mainWindow);
}

void DisplayMainSceneElem(sfRenderWindow *mainWindow)
{
	DisplayPortalLayer0(mainWindow, gameData.Room, gameData.Cam);

#if _DEBUG

	static sfBool toShow = sfFalse;

	if (sfKeyboard_isKeyPressed(sfKeyN))
	{
		toShow = sfTrue;
	}
	else if (sfKeyboard_isKeyPressed(sfKeyB))
	{
		toShow = sfFalse;
	}

	if (toShow)
	{
		DisplayOneRoomNodes(mainWindow, gameData.Room->RoomNode, gameData.Cam, gameData.Room, gameData.Player[0].ActualNode);
	}
#endif

	DisplayLoots(gameData.LootData, mainWindow, gameData.Cam);

	DrawTrapFirstLayer(mainWindow);

	DisplayEnemiesElem(gameData.Room->firstEnemy, mainWindow, gameData.Cam);

	DisplayEnemyVertexArrayElem(mainWindow);

	DisplayPlayers(mainWindow, gameData.Cam, gameData.numberPlayer);

	DisplayPortals(mainWindow, gameData.Room, gameData.Cam);

	for (int i = 0; i < gameData.numberPlayer; i++)
	{
		SetShots(gameData.Player[i].Weapons.FirstShot, mainWindow, gameData.Cam);
		if (!gameData.Player[i].States.IsDead)
		{
			sfVector2f pos = { gameData.Player[i].Physics.Rb.Position.x / 64.f, gameData.Player[i].Physics.Rb.Position.y / 64.f };
			SetPlayerCirclePos(i, pos);

		}
	}

	SetShots(gameData.Room->FirstShot, mainWindow, gameData.Cam);

	SetShots(gameData.Boss.FirstShot, mainWindow, gameData.Cam);

	DisplayShotVertexArray(mainWindow);
	if (gameData.Boss.IsSpawned)
	{
		DisplayBossControllerLayer1(gameData.Boss, mainWindow, gameData.Cam);
	}
	DrawTrapSecondLayer(mainWindow);
}

void DisplayMainSceneRazar(sfRenderWindow * mainWindow)
{

#if _DEBUG

	static sfBool toShow = sfFalse;

	if (sfKeyboard_isKeyPressed(sfKeyN))
	{
		toShow = sfTrue;
	}
	else if (sfKeyboard_isKeyPressed(sfKeyB))
	{
		toShow = sfFalse;
	}

	if (toShow)
	{
		DisplayOneRoomNodes(mainWindow, gameData.Room->RoomNode, gameData.Cam, gameData.Room, gameData.Player[0].ActualNode);
	}
#endif


	DisplayPlayers(mainWindow, gameData.Cam, gameData.numberPlayer);


	for (int i = 0; i < gameData.numberPlayer; i++)
	{
		SetShots(gameData.Player[i].Weapons.FirstShot, mainWindow, gameData.Cam);
	}

	SetShots(gameData.Room->FirstShot, mainWindow, gameData.Cam);

	if (gameData.Boss.IsSpawned)
	{
		DisplayBossController(gameData.Boss, mainWindow, gameData.Cam, mainDungeon.DGraphicData);
	}

	SetShots(gameData.Boss.FirstShot, mainWindow, gameData.Cam);

	DisplayShotVertexArray(mainWindow);
}

void DisplayMainScene(sfRenderWindow *mainWindow)
{
	DungeonId tempDungeonId = GetActualDungeonId();
	DisplayCamera(&gameData.Cam, mainWindow);
	sfRenderTexture* tempLightTexture = GetLightTexture();

	sfRenderTexture_clear(tempLightTexture, sfBlack);
	sfRenderTexture_setView(tempLightTexture, gameData.Cam.View);

	if (tempDungeonId == elemDungeon || tempDungeonId == razarDungeon)
	{
		sfRenderTexture* tempLavaLightTexture = GetLavaLightTexture();
		sfRenderTexture* tempEnemyLightTexture = GetEnemiLightTexture();
		sfRenderTexture_clear(tempLavaLightTexture, sfBlack);
		sfRenderTexture_clear(tempEnemyLightTexture, sfBlack);
		sfRenderTexture_setView(tempLavaLightTexture, gameData.Cam.View);
		sfRenderTexture_setView(tempEnemyLightTexture, gameData.Cam.View);
	}

	if (gameData.Boss.IsSpawned && tempDungeonId == elemDungeon)
	{
		DisplayBossController(gameData.Boss, mainWindow, gameData.Cam, mainDungeon.DGraphicData);
	}

	DisplayEnviDungeonLayer0(mainWindow, mainDungeon.DGraphicData, mainDungeon.DGameData, gameData.Cam);


	if (tempDungeonId == slimeDungeon || tempDungeonId == beginningDungeon)
	{
		DisplayMainSceneSlime(mainWindow);
	}
	else if (tempDungeonId == elemDungeon)
	{
		DisplayMainSceneElem(mainWindow);
	}
	else if (tempDungeonId == razarDungeon)
	{
		DisplayMainSceneRazar(mainWindow);
	}

	if (GetTransitioning())
	{
		DisplayEndTeleporting(mainWindow, gameData, gameData.Cam);
	}

	DisplayEnviDungeonLayer1(mainWindow, mainDungeon.DGraphicData, mainDungeon.DGameData, gameData.Cam);

	//DrawSupp(mainWindow);

	DisplayHUD(&gameData.Cam, gameData.Player, gameData.numberPlayer, mainDungeon, mainWindow);
}
