#ifndef _PROCEDURAL_H
#define _PROCEDURAL_H


#include <stdio.h>
#include <stdlib.h>
#include <SFML\Graphics.h>
#include "..\Node\Node.h"
#include "..\..\Controller\State\State.h"
#include "..\..\Controller\GraphicsUtilities\Animation\Animation.h"

#define SIZE_CASE 64.0f
#define SIZE_HORIZONTAL_X_x64 4.0f
#define SIZE_HORIZONTAL_Y_x64 9.0f
#define SIZE_VERTICAL_X_x64 5.0f
#define SIZE_VERTICAL_Y_x64 4.0f
#define NBROFDIFFERENTROOMS 12

#define AMOUNT_PREFAB_1ST_DUNGEON 11

typedef struct ROOMGAMEDATA ROOMGAMEDATA;

typedef enum CARDINALDIRECTION
{
	N,
	E,
	S,
	W,
	ANY
}CardinalDirection;

typedef struct CORRIDORCHECKPOINTS
{
	sfVector2f pos;
	sfCircleShape* CircleA;
	sfBool isIntersect;
}CorridorCheckpoints;

typedef struct CORRIDORCHECKPOINTSLIST
{
	CorridorCheckpoints CP;
	struct CORRIDORCHECKPOINTSLIST* Next;
}CorridorCheckPointsList;

typedef struct DOOR DOOR;

typedef struct CORRIDOR
{
	unsigned char Index;
	unsigned short corridorSize;
	CorridorCheckPointsList *firstCheckPoint;
	DOOR* doorsBack[2];
}Corridor;

typedef struct ROOM ROOM;

typedef struct DOOR
{
	unsigned char Index;
	sfVector2f Position;
	sfCircleShape* Circle;
	ROOM* roomBack;
	CardinalDirection cardDir;
	Corridor *Corridor;
	DOOR* PortalRef;
}Door;

typedef struct ROOM
{
	unsigned short Index;
	unsigned short Width;
	unsigned short Height;
	unsigned char nbDoors;
	sfVector2f Position;
	Door *doors;
	sfRectangleShape* Rect;
	unsigned int GraphicsIndex;
}Room;

typedef struct ROOMLIST
{
	Room rm;
	struct ROOMLIST* Tail;
	struct ROOMLIST* Prev;
	struct ROOMLIST* Next;
}RoomList;

typedef struct EDGELIST
{
	Room* Rooms[2];
	Door* door[2];
	unsigned char Weight;
	unsigned char Index;
	short GroupIndex;
	struct EDGELIST* Tail;
	struct EDGELIST* Prev;
	struct EDGELIST* Next;
}EdgeList;

typedef struct
{
	RoomList *FirstRoom;
	EdgeList *FirstEdge;
	unsigned int edgesCount;
	unsigned int iterationsCount;
	unsigned int seed;
	unsigned short numberOfRoom;
}DungeonRawData;

//Modified for GraphicData
typedef struct VECTOR2FLIST VECTOR2FLIST;
typedef struct VECTOR2FLIST
{
	sfVector2f Position;
	VECTOR2FLIST* next;
	int id;
}Vector2fList;

//Neutral sfVector2fList
typedef struct SF_VECTOR2FLIST SF_VECTOR2FLIST;
typedef struct SF_VECTOR2FLIST
{
	sfVector2f Vector;
	SF_VECTOR2FLIST* Next;
}sf_Vector2fList;

typedef struct DOORGRAPHICDATA DOORGRAPHICDATA;

typedef enum
{
	BasicTorch
}TypeOfTorch;

typedef struct TORCHGRAPICDATA
{
	sfVector2f Pos;
	TypeOfTorch Type;
	AnimationKey KeyAnim;
}TorchGraphicData;

typedef struct CORRIDORGRAPHICDATA
{
	Vector2fList *firstElementPos;
	unsigned int uniqueID;
	DOORGRAPHICDATA *doorsRef[2];
}CorridorGraphicData;

typedef struct DOORGRAPHICDATA
{
	sfVector2f LocalPos;
	CorridorGraphicData *corridorRef;
	unsigned int uniqueID;
	sfBool isDebutOfCorridor;
	CardinalDirection Dir;
}DoorGraphicData;

typedef struct ROOMGRAPHICDATA
{
	sfVector2f Pos;
	DoorGraphicData* doors;
	unsigned short doorCount;
	unsigned short uniqueID;
	unsigned short IdEnemyRoom;
	unsigned short size;
	unsigned short TorchCount;
	TorchGraphicData* Torchs;
}RoomGraphicData;

typedef struct DUNGEONGRAPHICDATA
{
	RoomGraphicData* room;
	unsigned short numberOfRoom;
	unsigned int seed;
}DungeonGraphicData;

typedef struct CORRIDORGAMEDATA
{
	Node *CorridorNode;
}CorridorGameData;

typedef struct DOORGAMEDATA
{
	sfVector2f Pos;
	sfBool isClosed;
	Node *DoorsNode;
	CardinalDirection Dir;
}DoorGameData;

typedef struct DUNGEONGAMEDATA
{
	ROOMGAMEDATA *room;
	unsigned short numberOfRoom;
	unsigned int seed;
}DungeonGameData;

typedef struct DUNGEON
{
	DungeonGraphicData DGraphicData;
	DungeonGameData DGameData;
	sfBool NeedToClean;
	DungeonId ID;
}Dungeon;

typedef struct ROOMPREFAB
{
	char indexPrefab;
	sfBool hasContrain;
	sf_Vector2fList* AvailablePos[4];
	int ListLengths[4];
}RoomPrefab;

#endif // !_PROCEDURAL_H