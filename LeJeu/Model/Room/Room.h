#ifndef _ROOM_H
#define _ROOM_H
#include <stdlib.h>
#include <string.h>
#include "SpawnEnemy\SpawnEnemy.h"
#include "..\Boss\Boss.h"
#include "..\Procedural\ProceduralType.h"
#include "..\..\Controller\Particles\Particles.h"
#include "..\Portal\Portal.h"

typedef struct ROOMGAMEDATA
{
	Enemy* firstEnemy;
	Shot* FirstShot;
	sfVector2f Position;
	sfBool IsClear;
	float Width;
	float Height;
	unsigned char Type;
	unsigned char RoomId;
	unsigned char ActualWave;
	unsigned char TotalWave;
	unsigned short nbDoors;
	unsigned short nbPortals;
	Segment Segment[4];
	Node *RoomNode;
	Node **Adress;
	DoorGameData *doors;
	Portal* portals;
}RoomGameData;

void InitSegmentsOfRoom(RoomGameData* room);

void LoadRoomsEnemies(RoomGameData* room, Enemy** firstEnemy, SpawnEnemy* spawnEnemy[], Boss* boss, const int nbrPlayer);

int CheckWhatIsTheActualRoom(ROOMGAMEDATA** actualRoom, const DungeonGameData dungeon, const Player player[], const int nbrPlayer);

int GetActualRoomForTeleport(ROOMGAMEDATA** actualRoom, const DungeonGameData dungeon, const Player player[], const int nbrPlayer);

//return -1 if no room is found
int CheckIdRoom(const DungeonGameData dungeon, const Player player[], const int nbrPlayer);

#endif
