#include "GameData.h"
#include "Config\Config.h"
#include "..\View\CustomXinput\CustomXinput.h"
#include "..\Controller\State\State.h"
#include "..\Controller\HUD\HUDController.h"
#include "Physics\Collision\Collision.h"
#include <SFML\Graphics.h>
#include "..\Controller\MainSceneController\PlayerController\PlayerController.h"
#include "..\Controller\MainSceneController\EnemyController\EnemyController.h"
#include "..\Controller\MainSceneController\EnvironmentController\EnvironmentController.h"
#include "Trap\Trap.h"
#include "..\View\ViewManager.h"
#include "..\Controller\Serialization\Serialization.h"
#include "..\Controller\Sounds\Sounds.h"
#include "Portal\Portal.h"
#include "..\Controller\MainSceneController\BossController\BossController.h"

float timerCollision = 0.f;
//check collision like if the game is in 60 fps
float cooldownCollision = 1.f / 60.f;
///changer apres press


void SetBossIdRoom(GameData* gameData)
{
	switch (GetActualDungeonId())
	{
	case beginningDungeon:
		gameData->IdBossRoom = -2;
		break;
	case slimeDungeon:
		gameData->IdBossRoom = 11;
		break;
	case elemDungeon:
		gameData->IdBossRoom = 10;
		break;
	case razarDungeon:
		gameData->IdBossRoom = 0;
		break;
	}
}

void InitGameDataForCampaign(GameData *gameData, const int numberOfPlayer)
{
	gameData->numberPlayer = numberOfPlayer;
	gameData->Player = (Player*)calloc(numberOfPlayer, sizeof(Player));
	for (int i = 0; i < gameData->numberPlayer; i++)
	{
		InitPlayer(&gameData->Player[i], (sfVector2f) { 50.0f, 50.0f }, i);
		InitPlayerPointer(&gameData->Player[i], i);
	}
}

void InitGameData(GameData *gameData, const int numberOfPlayer)
{
	sfVideoMode temp = getConfigMode();
	SaveData saveData = GetSaveData();
	SetStatePause(sfFalse);
	gameData->Cam = InitializeCamera((sfVector2f) { 1920.0f * 1.f, 1080.0f * 1.f }, (sfVector2f) { (float)temp.width, (float)temp.height }, 5.0f);
	gameData->numberPlayer = numberOfPlayer;
	gameData->Player = (Player*)calloc(numberOfPlayer, sizeof(Player));
	gameData->TimerEndingGame = 0.f;
	gameData->TimerEngageBoss = 0.f;
	if (saveData.IsLoaded)
	{
		for (int i = 0; i < saveData.NumberPlayer; i++)
		{
			gameData->Player[i] = saveData.player[i];
			ResetPlayer(&gameData->Player[i]);
			InitPlayerPointer(&gameData->Player[i], i);
		}
	}
	else
	{
		for (int i = 0; i < gameData->numberPlayer; i++)
		{
			InitPlayer(&gameData->Player[i], (sfVector2f) { 50.0f, 50.0f }, i);
			InitPlayerPointer(&gameData->Player[i], i);
		}
	}
	for (int i = 0; i < gameData->numberPlayer; i++)
	{
		InitPointerSkillTree(&gameData->Player[i].SkillTree, i);
	}
	InitLoot(&gameData->LootData);
	InitStatEnemies();
	InitBoss(&gameData->Boss);
	InitializeSounds();
	sfView_setCenter(gameData->Cam.View, gameData->Player[0].Physics.Rb.Position);

	SetBossIdRoom(gameData);
	InitDirectionalArray();
}

void ChangeDoorsState(RoomGameData *RoomDataDG, const sfBool Open)
{
	for (int i = 0; i < RoomDataDG->nbDoors; i++)
	{
		if (RoomDataDG->doors[i].Dir == S)
		{
			Node* temp = RoomDataDG->doors[i].DoorsNode->edges[4];
			for (int i = 0; i < 3; i++)
			{
				temp->isWalkable = Open;
				temp = temp->edges[0];
			}
		}
		else if (RoomDataDG->doors[i].Dir == N)
		{
			Node* temp = RoomDataDG->doors[i].DoorsNode->edges[4];
			for (int i = 0; i < 4; i++)
			{
				Node *second = temp;
				for (int i = 0; i < 3; i++)
				{
					temp->isWalkable = Open;
					temp = temp->edges[0];
				}
				temp = second->edges[6];
			}
		}
		else
		{
			Node* temp = RoomDataDG->doors[i].DoorsNode;
			for (int i = 0; i < 4; i++)
			{
				temp->isWalkable = Open;
				temp = temp->edges[6];
			}
		}
		RoomDataDG->doors[i].isClosed = !Open;
	}

	if (Open)
	{
		PlaySounds(openDoor);
		StopMusic(battle1);
		StopMusic(battle2);
		StopMusic(battle3);
		StopMusic(battle4);
		StopMusic(battle5);
		StopMusic(bossSlime);
		PlayMusic(scary);
	}
	else
	{
		PlaySounds(closeDoor);
		if (RoomDataDG->RoomId == 11)
		{
			PlayMusic(bossSlime);
		}
		else
		{
			PlayMusic(battle1 + rand() % 5);
		}
		StopMusic(scary);
	}
}

void ChangePortalsState(RoomGameData* RoomDataDG, const sfBool Open)
{
	if (Open)
	{
		StopMusic(battleElem);
		StopMusic(bossElem);
		PlayMusic(ambiantElem + rand() % 4);
	}
	else
	{
		if (RoomDataDG->RoomId == 10)
		{
			PlayMusic(bossElem);
		}
		else
		{
			PlayMusic(battleElem);
		}
		StopMusic(ambiantElem);
		StopMusic(ambiantElem2);
		StopMusic(ambiantElem3);
		StopMusic(ambiantElem4);
	}
	for (int i = 0; i < RoomDataDG->nbPortals; i++)
	{
		RoomDataDG->portals[i].IsNOTActive = !Open;
	}
}

void InitNewDungeonGameData(GameData *gameData, Dungeon *newDungeon)
{
	MiniMap miniMap;
	SaveData saveData = GetSaveData();
	gameData->dungeon = &newDungeon->DGameData;
	int id = CheckIdRoom(*gameData->dungeon, gameData->Player, saveData.NumberPlayer);

	if (saveData.IsLoaded)
		gameData->Room = &gameData->dungeon->room[id];
	else
		gameData->Room = &gameData->dungeon->room[0];

	miniMap.IDDungeon = newDungeon->ID;
	miniMap.nbRoom = gameData->dungeon->numberOfRoom;
	miniMap.RoomMap = (RoomMap*)calloc(miniMap.nbRoom, sizeof(RoomMap));
	miniMap.PlayerIsFighting = sfFalse;
	for (int i = 0; i < gameData->dungeon->numberOfRoom; i++)
	{
		InitSegmentsOfRoom(&gameData->dungeon->room[i]);
		miniMap.RoomMap[i].Index = gameData->dungeon->room[i].RoomId;
		miniMap.RoomMap[i].Position = gameData->dungeon->room[i].Position;
		miniMap.RoomMap[i].Size = (sfVector2f) { gameData->dungeon->room[i].Width, gameData->dungeon->room[i].Height };
		miniMap.RoomMap[i].HasBeenVisited = sfFalse;
		miniMap.RoomMap[i].UniqueID = gameData->dungeon->room[i].RoomId;
		if (newDungeon->ID == elemDungeon)
		{
			miniMap.RoomMap[i].NbDoors = 0;
			miniMap.RoomMap[i].DoorMap = NULL;
			miniMap.RoomMap[i].nbPortals = (unsigned short)gameData->dungeon->room[i].nbPortals;
			miniMap.RoomMap[i].Portals = (PortalMap*)calloc(miniMap.RoomMap[i].nbPortals, sizeof(PortalMap));
			for (int j = 0; j < miniMap.RoomMap[i].nbPortals; j++)
			{
				miniMap.RoomMap[i].Portals[j].Circle = NULL;
				miniMap.RoomMap[i].Portals[j].Position = gameData->dungeon->room[i].portals[j].PosCenter;
				miniMap.RoomMap[i].Portals[j].ID = gameData->dungeon->room[i].portals[j].ID;
				miniMap.RoomMap[i].Portals[j].Room = &miniMap.RoomMap[i];
			}
		}
		else if (newDungeon->ID == slimeDungeon || newDungeon->ID == beginningDungeon)
		{
			miniMap.RoomMap[i].Portals = NULL;
			miniMap.RoomMap[i].NbDoors = (unsigned short)gameData->dungeon->room[i].nbDoors;
			miniMap.RoomMap[i].DoorMap = (DoorMap*)calloc(miniMap.RoomMap[i].NbDoors, sizeof(DoorMap));
			for (int j = 0; j < miniMap.RoomMap[i].NbDoors; j++)
			{
				if (newDungeon->DGraphicData.room[i].doors[j].Dir == S)
				{
					miniMap.RoomMap[i].DoorMap[j].PosDoors.x = (newDungeon->DGraphicData.room[i].Pos.x + newDungeon->DGraphicData.room[i].doors[j].LocalPos.x) + 1.f;
					miniMap.RoomMap[i].DoorMap[j].PosDoors.y = (newDungeon->DGraphicData.room[i].Pos.y + newDungeon->DGraphicData.room[i].doors[j].LocalPos.y);
				}
				else if (newDungeon->DGraphicData.room[i].doors[j].Dir == N)
				{
					miniMap.RoomMap[i].DoorMap[j].PosDoors.x = (newDungeon->DGraphicData.room[i].Pos.x + newDungeon->DGraphicData.room[i].doors[j].LocalPos.x) + 1.f;
					miniMap.RoomMap[i].DoorMap[j].PosDoors.y = (newDungeon->DGraphicData.room[i].Pos.y + newDungeon->DGraphicData.room[i].doors[j].LocalPos.y) - 1.5f;
				}
				else
				{
					miniMap.RoomMap[i].DoorMap[j].PosDoors.x = (newDungeon->DGraphicData.room[i].Pos.x + newDungeon->DGraphicData.room[i].doors[j].LocalPos.x);
					miniMap.RoomMap[i].DoorMap[j].PosDoors.y = (newDungeon->DGraphicData.room[i].Pos.y + newDungeon->DGraphicData.room[i].doors[j].LocalPos.y) + 1.f;
				}
				miniMap.RoomMap[i].DoorMap[j].Dir = newDungeon->DGraphicData.room[i].doors[j].Dir;
				miniMap.RoomMap[i].DoorMap[j].ID = newDungeon->DGraphicData.room[i].doors[j].uniqueID;
				miniMap.RoomMap[i].DoorMap[j].Room = &miniMap.RoomMap[i];
				if (newDungeon->DGraphicData.room[i].doors[j].isDebutOfCorridor)
				{
					miniMap.RoomMap[i].DoorMap[j].IsDebutOfCorridor = sfTrue;
					miniMap.RoomMap[i].DoorMap[j].CorridorMap.VertexArray = sfVertexArray_create();
					sfVertexArray_setPrimitiveType(miniMap.RoomMap[i].DoorMap[j].CorridorMap.VertexArray, sfLineStrip);
					for (Vector2fList *tempList = newDungeon->DGraphicData.room[i].doors[j].corridorRef->firstElementPos; tempList != NULL; tempList = tempList->next)
					{
						sfVector2f tempPos;
						tempPos.x = tempList->Position.x / 64.f + 1.f;
						tempPos.y = tempList->Position.y / 64.f + 1.f;
						sfVertex vertex = { tempPos, sfWhite, (sfVector2f) { 0.0f, 0.0f } };
						sfVertexArray_append(miniMap.RoomMap[i].DoorMap[j].CorridorMap.VertexArray, vertex);
					}
				}
				else
				{
					miniMap.RoomMap[i].DoorMap[j].IsDebutOfCorridor = sfFalse;
					miniMap.RoomMap[i].DoorMap[j].CorridorMap.VertexArray = NULL;
				}
			}
		}
	}

	CopyDataIntoMiniMap(miniMap);
	if (newDungeon->ID == elemDungeon)
	{
		for (int i = 0; i < miniMap.nbRoom; i++)
		{
			for (int j = 0; j < miniMap.RoomMap[i].nbPortals; j++)
			{
				SearchIDPortalLinked(gameData->dungeon->room[i].portals[j].LinkPortal->ID, i, j);
			}
		}
	}

	if (GetTypeOfDungeon() == dungeonCorridor)
	{
		for (int i = 0; i < miniMap.nbRoom; i++)
		{
			for (int j = 0; j < miniMap.RoomMap[i].NbDoors; j++)
			{
				SearchIDDoorOfCorridor(newDungeon->DGraphicData.room[i].doors[j].corridorRef->doorsRef[0]->uniqueID, i, j, 0);
				SearchIDDoorOfCorridor(newDungeon->DGraphicData.room[i].doors[j].corridorRef->doorsRef[1]->uniqueID, i, j, 1);
			}
		}
	}

	InitializeMiniMap(gameData->numberPlayer);

	CheckWhatIsTheActualRoom(&gameData->Room, *gameData->dungeon, gameData->Player, gameData->numberPlayer);

	if (!saveData.IsLoaded)
	{
		gameData->Boss.RigidBody.Position.x = gameData->Cam.Position.x = gameData->Player->Physics.Rb.Position.x = (float)gameData->Player->Physics.PositionWorld.x = (int)gameData->dungeon->room[0].Position.x * 64.f;
		gameData->Boss.RigidBody.Position.y = gameData->Cam.Position.y = gameData->Player->Physics.Rb.Position.y = (float)gameData->Player->Physics.PositionWorld.y = (int)gameData->dungeon->room[0].Position.y * 64.f;


		gameData->Player[0].Physics.Rb.Position.x = (float)gameData->Player[0].Physics.PositionWorld.x = (int)(gameData->dungeon->room[0].Position.x + 5.f) * 64.f;
		gameData->Player[0].Physics.Rb.Position.y = (float)gameData->Player[0].Physics.PositionWorld.y = (int)(gameData->dungeon->room[0].Position.y + 5.f) * 64.f;

		if (gameData->numberPlayer > 1)
		{
			gameData->Player[1].Physics.Rb.Position = gameData->Player[0].Physics.Rb.Position;
		}
	}

	for (int i = 0; i < gameData->numberPlayer; i++)
	{
		SetInitNodePlayer(&gameData->Player[i], gameData->Room->RoomNode);
	}

	if (gameData->dungeon->numberOfRoom != 1)
	{
		gameData->Room->IsClear = sfTrue;
	}
	else
	{
		gameData->Room->IsClear = sfFalse;
		LoadRoomsEnemies(gameData->Room, &gameData->Room->firstEnemy, gameData->SpawEnemy, &gameData->Boss, gameData->numberPlayer);
	}
}

void UpdateGameData(GameData *gameData, const float deltaTime, sfRenderWindow* window)
{
	char id;
	DungeonId tempDungeonId = GetActualDungeonId();
	if (tempDungeonId == elemDungeon)
	{
		UpdateLayerLight(deltaTime);
		UpdateLevitateRock(deltaTime);
	}

	if (tempDungeonId == razarDungeon)
	{
		UpdateLayerLight(deltaTime*2.0f);
	}

	if (sfKeyboard_isKeyPressed(sfKeyO))
	{
		for (int i = 0; i < gameData->numberPlayer; i++)
		{
			gameData->Player[i].Stats.Life = gameData->Player[i].Stats.LifeMax;
			gameData->Player[i].Stats.Mana = gameData->Player[i].Stats.ManaMax;
		}
	}

	timerCollision += deltaTime;
	for (int i = 0; i < gameData->numberPlayer; i++)
	{
		gameData->Player[i].TypeInput = GetTypeHardware(i);
		UpdatePlayerHUD(gameData->Player[i], i, deltaTime);

		if (gameData->Boss.CanBeEngaged || gameData->Room->RoomId != gameData->IdBossRoom)
		{

			UpdatePlayer(&gameData->Player[i], deltaTime, i, gameData->Room->RoomNode);
		}

		if (!gameData->Player[i].States.IsDead)
		{
			if (gameData->Player[i].Spells.ConeState.MustDamage == sfTrue)
			{
				if (gameData->Boss.IsSpawned && !gameData->Boss.IsDead)
				{
					CheckCollisionPlayerConeOfFireBoss(&gameData->Player[i], &gameData->Boss);
				}
			}
			gameData->Player[i].Physics.PositionWorld = sfRenderWindow_mapCoordsToPixel(window, (sfVector2f) { gameData->Player[i].Physics.Rb.Position.x, gameData->Player[i].Physics.Rb.Position.y }, gameData->Cam.View);

			if (gameData->Player[i].TypeInput)
			{
				sfVector2u v = sfRenderWindow_getSize(window);
				gameData->Player[i].Hud.Cursor.Position.x = Lerp(gameData->Player[i].Hud.Cursor.Position.x, gameData->Player[i].Physics.Rb.Position.x, deltaTime * 2);
				gameData->Player[i].Hud.Cursor.Position.y = Lerp(gameData->Player[i].Hud.Cursor.Position.y, gameData->Player[i].Physics.Rb.Position.y, deltaTime * 2);
				gameData->Player[i].Hud.Cursor.Position.x = Clamp(gameData->Player[i].Hud.Cursor.Position.x, gameData->Player[i].Physics.Rb.Position.x - v.x / 4, gameData->Player[i].Physics.Rb.Position.x + v.x / 4);
				gameData->Player[i].Hud.Cursor.Position.y = Clamp(gameData->Player[i].Hud.Cursor.Position.y, gameData->Player[i].Physics.Rb.Position.y - v.y / 4, gameData->Player[i].Physics.Rb.Position.y + v.y / 4);
			}
			else
			{
				sfVector2i posMouse = sfMouse_getPositionRenderWindow(window);
				gameData->Player[i].Hud.Cursor.Position = sfRenderWindow_mapPixelToCoords(window, posMouse, gameData->Cam.View);
			}

			UpdateScaleCursor(&gameData->Player[i].Hud.Cursor, 0.5f, deltaTime * 10);
		}

		if (gameData->Player[i].States.IsHit && !gameData->Player[i].States.IsDead)
		{
			static float t = 0.0f;
			ShakeCamera(&gameData->Cam, 50, 1.f, deltaTime);
			t += deltaTime;
			if (t >= 0.2f)
			{
				t = 0.f;
				gameData->Player[i].States.IsHit = sfFalse;
			}
			SetVibration(i, 1.0f, 1.0f);
		}
		else
		{
			SetVibration(i, 0.f, 0.f);
		}

		if (gameData->Player[i].Stats.TimerInvulnerability > 0)
		{

			gameData->Player[i].Stats.TimerInvulnerability -= deltaTime;
		}
		if (timerCollision >= cooldownCollision)
		{
			CheckCollisionPlayerLoots(&gameData->Player[i], &gameData->LootData);
			CheckCollisionPlayerTrap(&gameData->Player[i]);
		}

		if (gameData->Player[i].States.IsFiring)
		{
			gameData->Player[i].States.IsFiring = sfFalse;
		}
		if (gameData->Player[i].States.IsCastingFireBall)
		{
			PlaySounds(fireball);
			gameData->Player[i].States.IsCastingFireBall = sfFalse;
		}
		if (gameData->Player[i].Spells.ConeState.Timer == 2.f - deltaTime)
		{
			PlaySounds(flameThrower);
		}
	}
	for (int i = 0; i < gameData->Room->nbPortals; i++)
	{
		if (UpdatePortals(&gameData->Room->portals[i], gameData->Player, gameData->numberPlayer, deltaTime))
		{
			if (GetActualRoomForTeleport(&gameData->Room, *gameData->dungeon, gameData->Player, gameData->numberPlayer) && !gameData->Room->IsClear)
			{
				LoadRoomsEnemies(gameData->Room, &gameData->Room->firstEnemy, gameData->SpawEnemy, &gameData->Boss, gameData->numberPlayer);
				ChangePortalsState(gameData->Room, sfFalse);
				if (!gameData->Cam.isZoomed && gameData->Room->RoomId != gameData->IdBossRoom)
				{
					SetZoom(&gameData->Cam, sfFalse);
					gameData->Cam.isZoomed = sfTrue;
				}
			}
			for (int j = 0; j < gameData->numberPlayer; j++)
			{
				SetInitNodePlayer(&gameData->Player[j], gameData->Room->RoomNode);
			}
			return;
		}
	}

	sfVector2f posPlayer[2];
	for (int i = 0; i < gameData->numberPlayer; i++)
	{
		posPlayer[i] = gameData->Player[i].Physics.Rb.Position;
	}
	UpdateShots(&gameData->Room->FirstShot, deltaTime, gameData->Room->RoomNode, posPlayer, gameData->numberPlayer, NULL);
	if (gameData->Boss.IsSpawned && !gameData->Boss.IsDead)
	{
		sfVector2f sizeRoom = { gameData->Room->Height * 64.f, gameData->Room->Width *64.f };
		sfVector2f posRoom = { gameData->Room->Position.x * 64.f, gameData->Room->Position.y * 64.f };
		UpdateBoss(&gameData->Boss, gameData->Player, gameData->numberPlayer, deltaTime, posRoom, sizeRoom);
		if (tempDungeonId == elemDungeon)
		{
			if (gameData->Boss.CurrentPhase == p1 && gameData->Boss.IsAttacking)
			{
				ShakeCamera(&gameData->Cam, 150, 2.f, deltaTime);
			}
		}
		UpdateShots(&gameData->Boss.FirstShot, deltaTime, gameData->Room->RoomNode, posPlayer, gameData->numberPlayer, NULL);
		if (timerCollision >= cooldownCollision)
		{
			CheckCollisionEnemyShot(&gameData->Boss.FirstShot, gameData->Player, gameData->numberPlayer, gameData->Room);
		}

		if (gameData->Room->firstEnemy == NULL)
		{
			gameData->Boss.HasTrashesAlive = sfFalse;
		}
	}
	if (timerCollision >= cooldownCollision)
	{
		if (!gameData->Room->IsClear)
		{
			CheckCollisionEnemyShot(&gameData->Room->FirstShot, gameData->Player, gameData->numberPlayer, gameData->Room);
			CheckCollisionEnemies(&gameData->Room->firstEnemy, gameData->Player, gameData->numberPlayer, gameData->Room, &gameData->Boss);
		}
	}
	if (tempDungeonId == slimeDungeon || tempDungeonId == beginningDungeon || tempDungeonId == razarDungeon)
	{
		FollowObject(&gameData->Cam, gameData->Player, gameData->numberPlayer, *gameData->Room, deltaTime);
	}
	else if (tempDungeonId == elemDungeon)
	{
		if (gameData->Room->RoomId != gameData->IdBossRoom)
		{
			FollowObject(&gameData->Cam, gameData->Player, gameData->numberPlayer, *gameData->Room, deltaTime);
		}
		else
		{
			if (gameData->Boss.CanBeEngaged)
			{
				if (!CanSendInput())
				{
					SetCanSendInput(sfTrue);
				}
				FollowObjectSpecialBoy(&gameData->Cam, gameData->Player, gameData->Boss.RigidBody.Position, gameData->numberPlayer, *gameData->Room, deltaTime);
			}
			else
			{
				if (CanSendInput())
				{
					SetCanSendInput(sfFalse);
				}
				if (gameData->TimerEngageBoss < 0.5f)
				{
					FollowObject(&gameData->Cam, gameData->Player, gameData->numberPlayer, *gameData->Room, deltaTime);
				}
				gameData->TimerEngageBoss += deltaTime;
				if (gameData->TimerEngageBoss > 1.5f)
				{
					MovementToASpecificPoint(&gameData->Cam, gameData->Boss.RigidBody.Position, deltaTime);
					if (gameData->TimerEngageBoss > 3.5f)
					{
						gameData->Boss.CanSpawn = sfTrue;
					}
				}
			}
		}
	}

	if (!IsEnableMap())
	{
		FollowObjectMiniMap(&gameData->Cam, gameData->Player, gameData->numberPlayer, deltaTime);
	}
	sfVector2f tempPosRoom = { gameData->Room->Position.x * 64.f, gameData->Room->Position.y * 64.f };
	sfVector2f tempSizeRoom = { gameData->Room->Width * 64.f, gameData->Room->Height * 64.f };
	UpdateEnemies(gameData->Player, gameData->numberPlayer, &gameData->Room->firstEnemy, &gameData->Room->FirstShot, deltaTime, &gameData->LootData, gameData->Room->RoomNode, tempPosRoom, tempSizeRoom, GetParticlesEnnemies());
	if (tempDungeonId == slimeDungeon || tempDungeonId == beginningDungeon)
	{
		UpdateParticles(&GetParticlesEnnemies()[0], deltaTime);
		UpdateParticles(&GetParticlesEnnemies()[1], deltaTime);
	}
	if (gameData->Room->IsClear && CheckWhatIsTheActualRoom(&gameData->Room, *gameData->dungeon, gameData->Player, gameData->numberPlayer))
	{
		LoadRoomsEnemies(gameData->Room, &gameData->Room->firstEnemy, gameData->SpawEnemy, &gameData->Boss, gameData->numberPlayer);

		ChangeDoorsState(gameData->Room, sfFalse);
	}

	id = CheckIdRoom(*gameData->dungeon, gameData->Player, gameData->numberPlayer);

	if (!IsAlreadyEnableMap() || (id != -1 && !IsRoomVisited(id)))
	{
		ChangeMiniMap(&gameData->Cam, gameData->numberPlayer);
		SetAlreadyEnableMap(sfTrue);
	}

	if (id != -1 && !IsRoomVisited(id))
	{
		SetRoomVisited(id, sfTrue);
	}

	if (gameData->Room->RoomId != gameData->IdBossRoom)
	{
		if (!gameData->Room->IsClear && gameData->Room->firstEnemy == NULL && gameData->SpawEnemy[gameData->Room->ActualWave] != NULL && !(gameData->SpawEnemy[gameData->Room->ActualWave]->IsActive))
		{
			if (gameData->Room->ActualWave < gameData->Room->TotalWave - 1)
			{
				ClearSpawnEnemy(&(gameData->SpawEnemy[gameData->Room->ActualWave]));
				gameData->Room->ActualWave++;
			}
			else
			{
				gameData->Room->ActualWave = 0;
				gameData->Room->IsClear = sfTrue;
				SetPlayerFighting(sfFalse);
				GetTypeOfDungeon() == dungeonCorridor ? ChangeDoorsState(gameData->Room, sfTrue) : ChangePortalsState(gameData->Room, sfTrue);
				if (gameData->numberPlayer == 2)
				{
					for (int i = 0; i < gameData->numberPlayer; i++)
					{
						if (gameData->Player[i].States.IsDead)
						{
							gameData->Player[i].States.IsDead = sfFalse;
							gameData->Player[i].Stats.Life = 1;
							gameData->Player[i].Physics.Rb.Position = gameData->Player[(i + 1) % 2].Physics.Rb.Position;
							gameData->Player[i].Physics.Collider.center = gameData->Player[(i + 1) % 2].Physics.Rb.Position;
							gameData->Player[i].Stats.TimerInvulnerability = 3.f;
						}
					}
				}

				sfBool done = sfTrue;
				for (int i = 0; i < gameData->dungeon->numberOfRoom; i++)
				{
					if (!gameData->dungeon->room[i].IsClear)
					{
						done = sfFalse;
						i = gameData->dungeon->numberOfRoom;
					}
				}

				if (done)
				{
					for (int i = 0; i < gameData->numberPlayer; i++)
					{
						gameData->Player[i].States.ISLeavingActualDungeon = sfTrue;
					}
					if (!GetTransitioning())
					{
						SetTransitioning(sfTrue);
					}
				}
			}
		}
	}
	else
	{
		if (gameData->Boss.IsSpawned && gameData->Boss.IsDead && gameData->Room->firstEnemy == NULL)
		{
			gameData->Room->ActualWave = 0;
			gameData->Room->IsClear = sfTrue;
			SetPlayerFighting(sfFalse);
			GetTypeOfDungeon() == dungeonCorridor ? ChangeDoorsState(gameData->Room, sfTrue) : ChangePortalsState(gameData->Room, sfTrue);
			for (int i = 0; i < gameData->numberPlayer; i++)
			{
				if (gameData->Player[i].States.IsDead && gameData->numberPlayer == 2)
				{
					gameData->Player[i].States.IsDead = sfFalse;
					gameData->Player[i].Stats.Life = 1;
					gameData->Player[i].Physics.Rb.Position = gameData->Player[(i + 1) % 2].Physics.Rb.Position;
				}

				gameData->Player[i].States.ISLeavingActualDungeon = sfTrue;
			}
			if (!GetTransitioning())
			{
				SetTransitioning(sfTrue);
			}
		}
	}
	if (!gameData->Room->IsClear)
	{
		if (!IsPlayerFighting())
		{
			SetPlayerFighting(sfTrue);
		}
		UpdateSpawnEnemy(&gameData->Room->firstEnemy, &(gameData->SpawEnemy[gameData->Room->ActualWave]), deltaTime, gameData->Room->RoomNode);
	}

	//death of the players
	if ((gameData->numberPlayer == 1 && gameData->Player[0].States.IsDead)
		|| (gameData->numberPlayer == 2 && gameData->Player[0].States.IsDead && gameData->Player[1].States.IsDead))
	{
		gameData->TimerEndingGame += deltaTime;
	}

	AttractLoot(&gameData->LootData, gameData->Player, gameData->numberPlayer, deltaTime);
	UpdateShotsOfMiniBoss(gameData->Room->firstEnemy, gameData->Room->FirstShot, deltaTime);
	UpdateTrap(deltaTime);

	if (timerCollision >= cooldownCollision)
	{
		timerCollision -= cooldownCollision;
	}

	ActiveLightOnCamera(gameData->Cam);
	if (gameData->TimerEndingGame > 2.f)
	{
		ChangeScene(mainMenu);
		PlayMusic(mainMenuMusic);
	}
}

void Restart(GameData *gameData)
{
	ClearEnemies(&gameData->Room->firstEnemy);
	ClearShots(&gameData->Room->FirstShot);
	ClearShots(&gameData->Boss.FirstShot);
	for (int i = 0; i < gameData->Room->TotalWave; i++)
	{
		ClearSpawnEnemy(&gameData->SpawEnemy[i]);
	}

	for (int i = 0; i < gameData->numberPlayer; i++)
	{
		ClearParticle(&(GetPlayerGraphics()->ParticlesPlayer[i]));
		ClearShots(&gameData->Player[i].Weapons.FirstShot);
	}
	ClearCamera(&gameData->Cam, gameData->numberPlayer);
	ClearPointLights();
	if (GetState().typeOfGame != campaign)
	{
		free(gameData->Player);
		UnloadAnimationPlayerGraphics(gameData->numberPlayer);
	}
	else
	{
		gameData->Cam = InitializeCamera((sfVector2f) { 1920.0f * 1.f, 1080.0f * 1.f }, (sfVector2f) { (float)getConfigMode().width, (float)getConfigMode().height }, 5.0f);
	}

	ClearLights();
	StopAllMusic();
	SetRestart(false);
}