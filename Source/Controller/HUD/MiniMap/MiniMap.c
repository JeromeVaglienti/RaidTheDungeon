#include "MiniMap.h"
#include <stdlib.h>
#include "..\..\MainSceneController\ShadersController\ShadersController.h"
#include "..\..\State\State.h"
#include "..\..\MainSceneController\SkinsController\SkinsController.h"

MiniMap miniMap = { 0 };

void CopyDataIntoMiniMap(const MiniMap miniMapToCopy)
{
	miniMap = miniMapToCopy;
}

void InitializeMiniMap(const int numberOfPlayer)
{
	//create circle for each player on the map
	sfColor color1[14] = {
		sfColor_fromRGB(255, 0, 0),
		sfColor_fromRGB(0, 0, 255),
		sfColor_fromRGB(0, 255, 0),
		sfColor_fromRGB(255, 0, 222),
		sfColor_fromRGB(132, 0, 255),
		sfColor_fromRGB(255, 108, 0),
		sfColor_fromRGB(0, 255, 162),
		sfColor_fromRGB(0, 186, 255),
		sfColor_fromRGB(231, 58, 123),
		sfColor_fromRGB(205, 231, 58),
		sfColor_fromRGB(122, 100, 62),
		sfColor_fromRGB(245, 144, 211),
		sfColor_fromRGB(123, 0, 32),
		sfColor_fromRGB(48, 4, 98)
	};
	sfColor colorDoor = sfColor_fromRGB(141, 36, 222);
	miniMap.PlayerCircle = (sfSprite**)calloc(numberOfPlayer, sizeof(sfSprite*));
	miniMap.PosPlayer = (sfVector2f*)calloc(numberOfPlayer, sizeof(sfVector2f));
	miniMap.EnableMap = sfFalse;
	miniMap.AlreadyEnableMap = sfTrue;
	miniMap.NeedToDezoom = sfFalse;
	PlayersInformations playersInformations = GetPlayersInformations();
	for (int i = 0; i < numberOfPlayer; i++)
	{
		if (playersInformations.SkinPlayer[i] == (SkinPlayer)red)
		{
			miniMap.PlayerCircle[i] = LoadSprite("Ressources/Sprites/HUD/MiniMap/SmallHead/Logo_Perso_Rouge.png", 1);
		}
		else if (playersInformations.SkinPlayer[i] == (SkinPlayer)black)
		{
			miniMap.PlayerCircle[i] = LoadSprite("Ressources/Sprites/HUD/MiniMap/SmallHead/Logo_Perso_Noir.png", 1);
		}
		else if (playersInformations.SkinPlayer[i] == (SkinPlayer)white)
		{
			miniMap.PlayerCircle[i] = LoadSprite("Ressources/Sprites/HUD/MiniMap/SmallHead/Logo_Perso_Blanc.png", 1);
		}
		else if (playersInformations.SkinPlayer[i] == (SkinPlayer)green)
		{
			miniMap.PlayerCircle[i] = LoadSprite("Ressources/Sprites/HUD/MiniMap/SmallHead/Logo_Perso_Vert.png", 1);
		}
		sfSprite_setScale(miniMap.PlayerCircle[i], (sfVector2f) { 0.325f, 0.325f });
	}

	if (miniMap.IDDungeon == elemDungeon)
	{
		miniMap.RoomMiniMap = (sfSprite**)calloc(13, sizeof(sfSprite*));
		miniMap.RoomMiniMap[0] = LoadSprite("Ressources/Sprites/HUD/MiniMap/Dungeon2/Spawn.png", 1);
		miniMap.RoomMiniMap[1] = LoadSprite("Ressources/Sprites/HUD/MiniMap/Dungeon2/Lave1.png", 1);
		miniMap.RoomMiniMap[2] = LoadSprite("Ressources/Sprites/HUD/MiniMap/Dungeon2/Lave2.png", 1);
		miniMap.RoomMiniMap[3] = LoadSprite("Ressources/Sprites/HUD/MiniMap/Dungeon2/Lave3.png", 1);
		miniMap.RoomMiniMap[4] = LoadSprite("Ressources/Sprites/HUD/MiniMap/Dungeon2/Wind1.png", 1);
		miniMap.RoomMiniMap[5] = LoadSprite("Ressources/Sprites/HUD/MiniMap/Dungeon2/Ice1.png", 1);
		miniMap.RoomMiniMap[6] = LoadSprite("Ressources/Sprites/HUD/MiniMap/Dungeon2/Ice2.png", 1);
		miniMap.RoomMiniMap[7] = LoadSprite("Ressources/Sprites/HUD/MiniMap/Dungeon2/Ice3.png", 1);
		miniMap.RoomMiniMap[8] = LoadSprite("Ressources/Sprites/HUD/MiniMap/Dungeon2/Dirt1.png", 1);
		miniMap.RoomMiniMap[9] = LoadSprite("Ressources/Sprites/HUD/MiniMap/Dungeon2/Wind2.png", 1);
		miniMap.RoomMiniMap[10] = LoadSprite("Ressources/Sprites/HUD/MiniMap/Dungeon2/Boss.png", 1);
		miniMap.RoomMiniMap[11] = LoadSprite("Ressources/Sprites/HUD/MiniMap/Room_BossElem.png", 1);
		miniMap.RoomMiniMap[12] = LoadSprite("Ressources/Sprites/HUD/MiniMap/unknownRoom.png", 1);
		int boy = 0;
		for (int i = 0; i < miniMap.nbRoom; i++)
		{
			for (int j = 0; j < miniMap.RoomMap[i].nbPortals; j++)
			{
				if (miniMap.RoomMap[i].Portals[j].Circle == NULL)
				{
					miniMap.RoomMap[i].Portals[j].Circle = sfCircleShape_create();
					miniMap.RoomMap[i].Portals[j].LinkPortal->Circle = sfCircleShape_create();
					sfCircleShape_setRadius(miniMap.RoomMap[i].Portals[j].Circle, 2.f);
					sfCircleShape_setRadius(miniMap.RoomMap[i].Portals[j].LinkPortal->Circle, 2.f);
					sfVector2f newOrigin = { 1.f, 1.f };
					sfCircleShape_setOrigin(miniMap.RoomMap[i].Portals[j].Circle, newOrigin);
					sfCircleShape_setOrigin(miniMap.RoomMap[i].Portals[j].LinkPortal->Circle, newOrigin);

					sfCircleShape_setPosition(miniMap.RoomMap[i].Portals[j].Circle, miniMap.RoomMap[i].Portals[j].Position);
					sfCircleShape_setPosition(miniMap.RoomMap[i].Portals[j].LinkPortal->Circle, miniMap.RoomMap[i].Portals[j].LinkPortal->Position);


					sfCircleShape_setFillColor(miniMap.RoomMap[i].Portals[j].Circle, color1[boy]);
					sfCircleShape_setFillColor(miniMap.RoomMap[i].Portals[j].LinkPortal->Circle, color1[boy]);
					boy++;
				}
			}
		}
	}
	else if (miniMap.IDDungeon == slimeDungeon || miniMap.IDDungeon == beginningDungeon)
	{
		miniMap.RoomMiniMap = (sfSprite**)calloc(13, sizeof(sfSprite*));
		miniMap.RoomMiniMap[0] = LoadSprite("Ressources/Sprites/HUD/MiniMap/Dungeon1/startMiniMap.png", 1);
		miniMap.RoomMiniMap[1] = LoadSprite("Ressources/Sprites/HUD/MiniMap/Dungeon1/dortoir1MiniMap.png", 1);
		miniMap.RoomMiniMap[2] = LoadSprite("Ressources/Sprites/HUD/MiniMap/Dungeon1/dortoir2MiniMap.png", 1);
		miniMap.RoomMiniMap[3] = LoadSprite("Ressources/Sprites/HUD/MiniMap/Dungeon1/entrepot1MiniMap.png", 1);
		miniMap.RoomMiniMap[4] = LoadSprite("Ressources/Sprites/HUD/MiniMap/Dungeon1/entrepot2MiniMap.png", 1);
		miniMap.RoomMiniMap[5] = LoadSprite("Ressources/Sprites/HUD/MiniMap/Dungeon1/cantine1MiniMap.png", 1);
		miniMap.RoomMiniMap[6] = LoadSprite("Ressources/Sprites/HUD/MiniMap/Dungeon1/cantine2MiniMap.png", 1);
		miniMap.RoomMiniMap[7] = LoadSprite("Ressources/Sprites/HUD/MiniMap/Dungeon1/bibli1MiniMap.png", 1);
		miniMap.RoomMiniMap[8] = LoadSprite("Ressources/Sprites/HUD/MiniMap/Dungeon1/bibli2MiniMap.png", 1);
		miniMap.RoomMiniMap[9] = LoadSprite("Ressources/Sprites/HUD/MiniMap/Dungeon1/miniBossMiniMap.png", 1);
		miniMap.RoomMiniMap[10] = LoadSprite("Ressources/Sprites/HUD/MiniMap/Dungeon1/miniBossMiniMap.png", 1);
		miniMap.RoomMiniMap[11] = LoadSprite("Ressources/Sprites/HUD/MiniMap/Room_Boss.png", 1);
		miniMap.RoomMiniMap[12] = LoadSprite("Ressources/Sprites/HUD/MiniMap/unknownRoom.png", 1);

		for (int i = 0; i < miniMap.nbRoom; i++)
		{
			for (int j = 0; j < miniMap.RoomMap[i].NbDoors; j++)
			{
				miniMap.RoomMap[i].DoorMap[j].Rect = sfRectangleShape_create();
				sfVector2f newOrigin;
				if (miniMap.RoomMap[i].DoorMap[j].Dir == So ||
					miniMap.RoomMap[i].DoorMap[j].Dir == No)
				{
					sfRectangleShape_setSize(miniMap.RoomMap[i].DoorMap[j].Rect, (sfVector2f) { 7.5f, 1.5f });
					newOrigin = (sfVector2f) { 3.75f, 0.75f };
				}
				else
				{
					sfRectangleShape_setSize(miniMap.RoomMap[i].DoorMap[j].Rect, (sfVector2f) { 1.5f, 7.5f });
					newOrigin = (sfVector2f) { 0.75f, 3.75f };
				}
				sfRectangleShape_setOrigin(miniMap.RoomMap[i].DoorMap[j].Rect, newOrigin);
				sfRectangleShape_setPosition(miniMap.RoomMap[i].DoorMap[j].Rect, miniMap.RoomMap[i].DoorMap[j].PosDoors);
				sfRectangleShape_setFillColor(miniMap.RoomMap[i].DoorMap[j].Rect, colorDoor);
			}
		}
	}
	else if (miniMap.IDDungeon == razarDungeon)
	{

	}
}

void SearchIDPortalLinked(const int id, const int roomNumber, const int portalNumber)
{
	for (int i = 0; i < miniMap.nbRoom; i++)
	{
		for (int j = 0; j < miniMap.RoomMap[i].nbPortals; j++)
		{
			if (miniMap.RoomMap[i].Portals[j].ID == id)
			{
				miniMap.RoomMap[roomNumber].Portals[portalNumber].LinkPortal = &miniMap.RoomMap[i].Portals[j];
			}
		}
	}
}

void SearchIDDoorOfCorridor(const int id, const int numberRoom, const int numberDoor, const int doorNumber)
{
	for (int i = 0; i < miniMap.nbRoom; i++)
	{
		for (int j = 0; j < miniMap.RoomMap[i].NbDoors; j++)
		{
			if (miniMap.RoomMap[i].DoorMap[j].ID == id)
			{
				miniMap.RoomMap[numberRoom].DoorMap[numberDoor].CorridorMap.Doors[doorNumber] = &miniMap.RoomMap[i].DoorMap[j];
				return;
			}
		}
	}
}

void ClearMiniMap(const int numberOfPlayer)
{
	for (int i = 0; i < miniMap.nbRoom; i++)
	{
		if (miniMap.IDDungeon == slimeDungeon || miniMap.IDDungeon == beginningDungeon)
		{
			for (int j = 0; j < miniMap.RoomMap[i].NbDoors; j++)
			{
				sfRectangleShape_destroy(miniMap.RoomMap[i].DoorMap[j].Rect);
				if (miniMap.RoomMap[i].DoorMap[j].CorridorMap.VertexArray)
				{
					sfVertexArray_clear(miniMap.RoomMap[i].DoorMap[j].CorridorMap.VertexArray);
				}
				sfVertexArray_destroy(miniMap.RoomMap[i].DoorMap[j].CorridorMap.VertexArray);
			}
			free(miniMap.RoomMap[i].DoorMap);
		}
		else if (miniMap.IDDungeon == elemDungeon)
		{
			for (int j = 0; j < miniMap.RoomMap[i].nbPortals; j++)
			{
				sfCircleShape_destroy(miniMap.RoomMap[i].Portals[j].Circle);
			}
			free(miniMap.RoomMap[i].Portals);
		}
	}
	free(miniMap.RoomMap);
	free(miniMap.PlayerCircle);
	free(miniMap.PosPlayer);
}

sfVector2f CalculateCenterDiscoveryMap()
{
	sfVector2f center = { 0 };
	int cpt = 0;
	for (int i = 0; i < miniMap.nbRoom; i++)
	{
		if (miniMap.RoomMap[i].HasBeenVisited)
		{
			center.x += miniMap.RoomMap[i].Position.x;
			center.y += miniMap.RoomMap[i].Position.y;
			cpt++;
		}
	}
	center.x /= cpt;
	center.y /= cpt;
	return center;
}

sfVector2f GetPlayerCirclePos(const int index)
{
	return miniMap.PosPlayer[index];
}

void SetPlayerCirclePos(const int index, const sfVector2f pos)
{
	miniMap.PosPlayer[index] = pos;
}

sfBool IsEnableMap()
{
	return miniMap.EnableMap;
}

void SetEnableMap(const sfBool enable)
{
	miniMap.EnableMap = enable;
}

sfBool IsRoomVisited(const int index)
{
	return miniMap.RoomMap[index].HasBeenVisited;
}

void SetRoomVisited(const int index, const sfBool visited)
{
	miniMap.RoomMap[index].HasBeenVisited = visited;
}

sfBool IsPlayerFighting()
{
	return miniMap.PlayerIsFighting;
}

void SetPlayerFighting(const sfBool enabled)
{
	miniMap.PlayerIsFighting = enabled;
}

sfBool IsAlreadyEnableMap()
{
	return miniMap.AlreadyEnableMap;
}

void SetAlreadyEnableMap(const sfBool enabled)
{
	miniMap.AlreadyEnableMap = enabled;
}

void DisplayMiniMap(const Player* player, const int numberOfPlayer, sfRenderWindow* pWindow, sfShader* shd)
{
	if (miniMap.IDDungeon == razarDungeon)
	{
		return;
	}

	if (IsEnableMap())
	{
		shd = NULL;
	}
	DungeonId dungeonId = GetActualDungeonId();
	sfRenderStates* renderStates = GetRenderState();
	for (int i = 0; i < miniMap.nbRoom; i++)
	{
		//draw only the room visited
		if (miniMap.RoomMap[i].HasBeenVisited)
		{
			BlitSprite(pWindow, miniMap.RoomMiniMap[miniMap.RoomMap[i].UniqueID], miniMap.RoomMap[i].Position, shd);

			if (dungeonId == slimeDungeon || dungeonId == beginningDungeon)
			{
				for (int j = 0; j < miniMap.RoomMap[i].NbDoors; j++)
				{
					if (miniMap.RoomMap[i].DoorMap[j].CorridorMap.Doors[0] != NULL && (miniMap.RoomMap[i].DoorMap[j].CorridorMap.Doors[0]->CorridorMap.VertexArray != NULL || miniMap.RoomMap[i].DoorMap[j].CorridorMap.Doors[1]->CorridorMap.VertexArray != NULL))
					{
						//draw in grey rooms connected to all that are explored
						if (miniMap.RoomMap[i].UniqueID != miniMap.RoomMap[i].DoorMap[j].CorridorMap.Doors[0]->Room->UniqueID
							&& !miniMap.RoomMap[i].DoorMap[j].CorridorMap.Doors[0]->Room->HasBeenVisited)
						{
							//draw room with skull for the boss
							if (miniMap.RoomMap[i].DoorMap[j].CorridorMap.Doors[0]->Room->Index == 11)
							{
								BlitSprite(pWindow, miniMap.RoomMiniMap[11], miniMap.RoomMap[i].DoorMap[j].CorridorMap.Doors[0]->Room->Position, shd);
							}
							else
							{
								BlitSprite(pWindow, miniMap.RoomMiniMap[12], miniMap.RoomMap[i].DoorMap[j].CorridorMap.Doors[0]->Room->Position, shd);
							}
						}
						if (miniMap.RoomMap[i].UniqueID != miniMap.RoomMap[i].DoorMap[j].CorridorMap.Doors[1]->Room->UniqueID
							&& !miniMap.RoomMap[i].DoorMap[j].CorridorMap.Doors[1]->Room->HasBeenVisited)
						{
							if (miniMap.RoomMap[i].DoorMap[j].CorridorMap.Doors[0]->Room->Index == 11)
							{
								BlitSprite(pWindow, miniMap.RoomMiniMap[11], miniMap.RoomMap[i].DoorMap[j].CorridorMap.Doors[1]->Room->Position, shd);
							}
							else
							{
								BlitSprite(pWindow, miniMap.RoomMiniMap[12], miniMap.RoomMap[i].DoorMap[j].CorridorMap.Doors[1]->Room->Position, shd);
							}
						}
						//draw the doors
						GetRenderState()->shader = shd;
						sfRenderWindow_drawRectangleShape(pWindow, miniMap.RoomMap[i].DoorMap[j].CorridorMap.Doors[0]->Rect, renderStates);
						sfRenderWindow_drawRectangleShape(pWindow, miniMap.RoomMap[i].DoorMap[j].CorridorMap.Doors[1]->Rect, renderStates);
						//draw all the vertex
						if (miniMap.RoomMap[i].DoorMap[j].CorridorMap.Doors[0]->CorridorMap.VertexArray != NULL)
						{
							sfRenderWindow_drawVertexArray(pWindow, miniMap.RoomMap[i].DoorMap[j].CorridorMap.Doors[0]->CorridorMap.VertexArray, renderStates);
						}
						else
						{
							sfRenderWindow_drawVertexArray(pWindow, miniMap.RoomMap[i].DoorMap[j].CorridorMap.Doors[1]->CorridorMap.VertexArray, renderStates);
						}
					}
				}
			}
			else if (dungeonId == elemDungeon)
			{
				for (int j = 0; j < miniMap.RoomMap[i].nbPortals; j++)
				{
					renderStates->shader = shd;
					sfRenderWindow_drawCircleShape(pWindow, miniMap.RoomMap[i].Portals[j].Circle, renderStates);
					if (miniMap.RoomMap[i].UniqueID != miniMap.RoomMap[i].Portals[j].LinkPortal->Room->UniqueID
						&& !miniMap.RoomMap[i].Portals[j].LinkPortal->Room->HasBeenVisited)
					{
						//draw room with skull for the boss
						if (miniMap.RoomMap[i].Portals[j].LinkPortal->Room->Index == 10)
						{
							BlitSprite(pWindow, miniMap.RoomMiniMap[11], miniMap.RoomMap[i].Portals[j].LinkPortal->Room->Position, shd);
						}
						else
						{
							BlitSprite(pWindow, miniMap.RoomMiniMap[12], miniMap.RoomMap[i].Portals[j].LinkPortal->Room->Position, shd);
						}
						sfRenderWindow_drawCircleShape(pWindow, miniMap.RoomMap[i].Portals[j].LinkPortal->Circle, renderStates);
					}

				}
			}
		}
	}
	//draw players
	for (int i = 0; i < numberOfPlayer; i++)
	{
		if (!player[i].States.IsDead)
		{
			BlitSprite(pWindow, miniMap.PlayerCircle[i], miniMap.PosPlayer[i], shd);
		}
	}
}