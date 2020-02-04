#ifndef MINIMAP_H
#define MINIMAP_H
#include <SFML\Graphics.h>
#include "..\..\GraphicsUtilities\Utilities.h"
#include "..\..\..\Model\Player\Player.h"

typedef struct RoomMap RoomMap;

typedef struct DoorMap DoorMap;

typedef enum Direction
{
	No,
	Ea,
	So,
	We
}Direction;

typedef struct CorridorMap
{
	sfVertexArray* VertexArray;
	DoorMap *Doors[2];
}CorridorMap;

typedef struct PortalMap
{
	int ID;
	sfVector2f Position;
	sfCircleShape* Circle;
	RoomMap* Room;
	struct PortalMap* LinkPortal;
}PortalMap;

typedef struct DoorMap
{
	sfVector2f PosDoors;
	sfRectangleShape* Rect;
	CorridorMap CorridorMap;
	RoomMap* Room;
	sfBool IsDebutOfCorridor;
	Direction Dir;
	int ID;
}DoorMap;

typedef struct RoomMap
{
	sfVector2f Position;
	sfVector2f Size;
	DoorMap *DoorMap;
	PortalMap *Portals;
	sfBool HasBeenVisited;
	unsigned int UniqueID;
	unsigned int Index;
	unsigned short NbDoors;
	unsigned short nbPortals;
}RoomMap;

typedef struct MiniMap
{
	int nbRoom;
	int IDDungeon;
	RoomMap *RoomMap;
	sfSprite** PlayerCircle;
	sfVector2f* PosPlayer;
	sfBool PlayerIsFighting;
	sfBool EnableMap;
	sfBool AlreadyEnableMap;
	sfBool NeedToDezoom;
	sfSprite** RoomMiniMap;
}MiniMap;


void CopyDataIntoMiniMap(const MiniMap miniMapToCopy);
//create and set position of every circle and rectangle
void InitializeMiniMap(const int numberOfPlayer);
//destroy shape and free sprite
void ClearMiniMap(const int numberOfPlayer);

void SearchIDPortalLinked(const int id, const int roomNumber, const int portalNumber);
//search the id of the doors connected to the corridor
void SearchIDDoorOfCorridor(const int id, const int numberRoom, const int numberDoor, const int doorNumber);

sfVector2f CalculateCenterDiscoveryMap();

sfVector2f GetPlayerCirclePos(const int index);

void SetPlayerCirclePos(const int index, const sfVector2f pos);

sfBool IsEnableMap();

void SetEnableMap(const sfBool enable);

sfBool IsRoomVisited(const int index);

void SetRoomVisited(const int index, const sfBool visited);

sfBool IsPlayerFighting();

void SetPlayerFighting(const sfBool enabled);

sfBool IsAlreadyEnableMap();

void SetAlreadyEnableMap(const sfBool enabled);

void DisplayMiniMap(const Player* player, const int numberOfPlayer, sfRenderWindow* pWindow, sfShader* shd);

#endif