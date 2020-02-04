#ifndef _ENVIRONMENTCONTROLLER
#define _ENVIRONMENTCONTROLLER


#include <stdio.h>
#include <stdlib.h>
#include "..\..\GraphicsUtilities\CustomSprite\CustomSprite.h"
#include "..\..\..\Model\Procedural\ProceduralType.h"
#include "..\..\Camera\Camera.h"
#include "..\..\..\Model\GameData.h"

typedef struct
{
	sfVector2f Start;
	sfVector2f Pos_1;
	sfVector2f Pos_2;
	sfVector2f End;
	float speed;
}ScrollerBackGround;


typedef struct
{
	sfSprite** Room;
	sfSprite** FrontDisplay;
	sfSprite** BackGroundDisplay;
	ScrollerBackGround* Scroller;
}RoomDungeon;

typedef struct
{
	int index;
	sfVector2f scale;
	sfVector2f Pos;
	float Timer;
	float Height;
}LevitateRock;
typedef struct
{
	RoomDungeon* Room;
	sfSprite** RoomLight;
	sfSprite** Corridor;
	sfSprite** DoorsLayersNorth;
	sfSprite** DoorsLayerSouth;
	sfSprite** DoorsLayerWest;
	sfSprite** DoorsLayerEast;
	sfSprite** DoorsLayerAny;
	Animation* PortalTeleportingAnimation;
	Animation* PortalActive;
	Animation* PortalReceivingTeleport;
	sfSprite** PortalClosedStatic;
	sfSprite** LevitateRockSprite;
	LevitateRock* LevitateRock;
	int nbRocks;
	Animation* BigTeleport;
}EnvironmentDungeon;

void InitEnviDungeonSlime();
void UnLoadEnviDungeonSlime();
void InitEnviDungeonElem();
void UnLoadEnviDungeonElem();
void InitEnviDungeonRazar();
void UnLoadEnviDungeonRazar();
void UpdateLevitateRock(const float deltaTime);
void UpdateScroller(const int RoomID, const float deltaTime);
sfBool UpdateEndTeleporting(const float deltaTime);
void DisplayLevitateRock(sfRenderWindow* window, const DungeonGraphicData dungeonGameData, const sfVector2f vec);
void DisplayEnviDungeonLayer0(sfRenderWindow* window, const DungeonGraphicData dungeonGraphicData, const DungeonGameData dungeonGameData, const Camera Cam);
void DisplayEnviDungeonLayer1(sfRenderWindow* window, const DungeonGraphicData dungeonGraphicData, const DungeonGameData dungeonGameData, const Camera Cam);
void UpdateLayerLight(const float deltatime);
void DisplayCorridor(sfRenderWindow* mainWindow);
void DisplayScroller(sfRenderWindow* window, sfSprite* ScrollerSprite, const ScrollerBackGround Scroller, const sfVector2f Position);
void DisplayPortalLayer0(sfRenderWindow* mainWindow, const ROOMGAMEDATA* ToDisplay, const Camera Cam);
void DisplayPortals(sfRenderWindow* mainWindow, const ROOMGAMEDATA* ToDisplay, const Camera Cam);
void DisplayEndTeleporting(sfRenderWindow * mainWindow, const GameData ToDisplay, const Camera Cam);

#endif // !_ENVIRONMENTCONTROLLER
