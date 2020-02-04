#include "Room.h"
#include "..\..\Controller\State\State.h"

// init the segments collider of a room
void InitSegmentsOfRoom(RoomGameData* room)
{
	sfVector2f tempPos = { room->Position.x * 64.f , room->Position.y * 64.f };
	room->Segment[0] = CreateSegment((sfVector2f) { tempPos.x - room->Width * 32.f, tempPos.y - room->Height * 32.f }, (sfVector2f) { tempPos.x - room->Width * 32.f, tempPos.y + room->Height * 32.f });
	room->Segment[1] = CreateSegment((sfVector2f) { tempPos.x - room->Width * 32.f, tempPos.y + room->Height * 32.f }, (sfVector2f) { tempPos.x + room->Width * 32.f, tempPos.y + room->Height * 32.f });
	room->Segment[2] = CreateSegment((sfVector2f) { tempPos.x + room->Width * 32.f, tempPos.y + room->Height * 32.f }, (sfVector2f) { tempPos.x + room->Width * 32.f, tempPos.y - room->Height * 32.f });
	room->Segment[3] = CreateSegment((sfVector2f) { tempPos.x + room->Width * 32.f, tempPos.y - room->Height * 32.f }, (sfVector2f) { tempPos.x - room->Width * 32.f, tempPos.y - room->Height * 32.f });
}

//init the spawners of the room by text file
void LoadRoomsEnemies(RoomGameData* room, Enemy** firstEnemy, SpawnEnemy* spawnEnemy[], Boss* boss, const int nbrPlayer)
{
	if (room->IsClear)
	{
		return;
	}

	char pathFolder[40] = { 0 };
	FILE* f;
	char str[50] = { 0 };
	// 12 is the id of the boss slime
	// 13 is room for test
	DungeonId tempDungeonId = GetActualDungeonId();
	if (tempDungeonId == slimeDungeon || tempDungeonId == beginningDungeon)
	{
		int tempId = -1;
		if (room->RoomId == 11)
		{
			tempId = 11;
		}
		printf("salle numero : %d donjon slime\n", room->RoomId);
		sprintf(str, "Room/Slime/%d.txt", room->RoomId);
	}
	else if (tempDungeonId == elemDungeon)
	{
		int tempId = -1;
		if (room->RoomId == 10)
		{
			tempId = 10;
		}
		printf("salle numero : %d donjon elem\n", room->RoomId);
		sprintf(str, "Room/Elem/%d.txt", room->RoomId);
	}
	else if (tempDungeonId == razarDungeon)
	{
		printf("salle numero : %d donjon razar\n", room->RoomId);
		sprintf(str, "Room/Razar/%d.txt", room->RoomId);
	}

	if (!(f = fopen(str, "rt")))
	{
		printf("Erreur d'acces au fichier...\n");
		printf("%s\n", str);
	}
	else
	{
		int numberOfWave = 0;
		// type contains the keywords
		char type[30] = { 0 };
		int wave = 0;
		while (fscanf(f, "%s", type) != EOF)
		{
			// if the keyword is NBRWAVE
			if (!strcmp("NBRWAVE", type))
			{
				fscanf(f, "%hhu", &room->TotalWave);
				room->IsClear = sfFalse;
			}
			// if the keyword is WAVE
			else if (!strcmp("WAVE", type))
			{
				int numberOfSpawn = 0;
				fscanf(f, "%d", &numberOfSpawn);
				char s[30] = { 0 };
				for (int i = 0; i < numberOfSpawn; i++)
				{
					int tempId = 0;
					int tempNbrOfEnemies = 0;
					float tempTotalTimeSpawn = 0.f;
					sfVector2f tempPos = { 0.f };
					float tempSpawnPerSecond = 0.f;
					fscanf(f, "%d%d%f%f%f", &tempId, &tempNbrOfEnemies, &tempTotalTimeSpawn, &tempPos.x, &tempPos.y);
					tempSpawnPerSecond = (float)tempNbrOfEnemies / tempTotalTimeSpawn;
					AddSpawnEnemy(&spawnEnemy[wave]);
					tempPos.x += (room->Position.x - room->Width / 2.f) * 64.f;
					tempPos.y += (room->Position.y - room->Height / 2.f)* 64.f;
					SetSpawnEnemy(spawnEnemy[wave], tempPos, tempId, tempNbrOfEnemies, tempSpawnPerSecond);
				}
				wave++;
			}
			// if the keyword is BOSS
			else if (!strcmp("BOSS", type))
			{
				int numberOfBoss = 0;
				fscanf(f, "%d", &numberOfBoss);
				char s[30] = { 0 };
				for (int i = 0; i < numberOfBoss; i++)
				{
					int tempId;
					sfVector2f tempPos = { 0.f };
					fscanf(f, "%d%f%f", &tempId, &tempPos.x, &tempPos.y);

					if (tempDungeonId == elemDungeon)
					{
						tempPos.x += (room->Position.x)* 64.f;
						tempPos.y += (room->Position.y)* 64.f - 35.f /*- 9.f * 64.f - 35.f*/;
					}
					else if (GetTypeOfDungeon() == dungeonCorridor || tempDungeonId == razarDungeon)
					{
						tempPos.x += (room->Position.x - room->Width / 2.f)* 64.f;
						tempPos.y += (room->Position.y - room->Height / 2.f)* 64.f;
					}
					room->IsClear = sfFalse;
					SpawnBoss(boss, tempId, tempPos, nbrPlayer);
				}
			}
		}
	}
	fclose(f);
}

// if the actual room is clear then check if the player is in another room
int CheckWhatIsTheActualRoom(ROOMGAMEDATA** actualRoom, const DungeonGameData dungeon, const Player player[], const int nbrPlayer)
{
	if ((*actualRoom)->IsClear)
	{
		for (int i = 0; i < dungeon.numberOfRoom; i++)
		{
			if (!dungeon.room[i].IsClear)
			{
				sfBool allPlayerIsInTheRoom = sfTrue;
				for (int j = 0; j < nbrPlayer; j++)
				{
					if (!player[j].States.IsDead)
					{
						sfVector2i tempPos = { (int)(dungeon.room[i].Position.x * 64) , (int)(dungeon.room[i].Position.y * 64) + 100 };
						int distanceX = abs(tempPos.x - (int)player[j].Physics.Rb.Position.x);
						int distanceY = abs(tempPos.y - (int)player[j].Physics.Rb.Position.y);
						if (distanceX > 1200 || distanceY > 975)
						{
							allPlayerIsInTheRoom = sfFalse;
						}
					}
				}
				if (allPlayerIsInTheRoom)
				{
					*actualRoom = &dungeon.room[i];
					return 1;
				}
			}
		}
	}
	return 0;
}

int GetActualRoomForTeleport(ROOMGAMEDATA** actualRoom, const DungeonGameData dungeon, const Player player[], const int nbrPlayer)
{
	for (int i = 0; i < dungeon.numberOfRoom; i++)
	{
		sfBool allPlayerIsInTheRoom = sfTrue;
		for (int j = 0; j < nbrPlayer; j++)
		{
			if (!player[j].States.IsDead)
			{
				sfVector2i tempPos = { (int)(dungeon.room[i].Position.x * 64) , (int)(dungeon.room[i].Position.y * 64) + 100 };
				int distanceX = abs(tempPos.x - (int)player[j].Physics.Rb.Position.x);
				int distanceY = abs(tempPos.y - (int)player[j].Physics.Rb.Position.y);
				if (distanceX > dungeon.room[i].Width * 32 || distanceY > dungeon.room[i].Height * 32)
				{
					allPlayerIsInTheRoom = sfFalse;
				}
			}
		}
		if (allPlayerIsInTheRoom)
		{
			*actualRoom = &dungeon.room[i];
			return 1;
		}
	}
	return 0;
}

int CheckIdRoom(const DungeonGameData dungeon, const Player player[], const int nbrPlayer)
{
	for (int i = 0; i < dungeon.numberOfRoom; i++)
	{
		sfBool allPlayerIsInTheRoom = sfTrue;
		for (int j = 0; j < nbrPlayer; j++)
		{
			if (!player[j].States.IsDead)
			{
				int distanceX = abs((int)dungeon.room[i].Position.x * 64 - (int)player[j].Physics.Rb.Position.x);
				int distanceY = abs((int)dungeon.room[i].Position.y * 64 - (int)player[j].Physics.Rb.Position.y);
				if (distanceX > dungeon.room[i].Width * 64 / 2 - 150 || distanceY > dungeon.room[i].Height * 64 / 2 - 250)
				{
					allPlayerIsInTheRoom = sfFalse;
				}
			}
		}
		if (allPlayerIsInTheRoom)
		{
			return i;
		}
	}
	return -1;
}