#ifndef _GAMEDATA_H
#define _GAMEDATA_H

#include <stdio.h>
#include <stdlib.h>
#include <SFML\Graphics.h>
#include "..\Controller\Sounds\Sounds.h"
#include "Player\Player.h"
#include "Room\Room.h"
#include "../Controller/Camera/Camera.h"
#include "Physics\FunctionsCollisions\Colision.h"
#include "Math\MathUtilities.h"
#include "Loot\Loot.h"
#include "..\Controller\State\State.h"
#include "..\Controller\Particles\Particles.h"
#include "Procedural\ProceduralType.h"


typedef struct
{
	Camera Cam;
	int numberPlayer;
	Player *Player;
	DungeonGameData *dungeon;
	RoomGameData *Room;//actualRoom
	SpawnEnemy* SpawEnemy[7];
	LootData LootData;
	float TimerEndingGame;
	float TimerEngageBoss;
	int IdBossRoom;
	Boss Boss;
}GameData;

void InitGameData(GameData *gameData, const int numberOfPlayer);
void SetBossIdRoom(GameData* gameData);
void InitNewDungeonGameData(GameData *gameData, Dungeon *newDungeon);
void UpdateGameData(GameData *gameData, const float deltaTime, sfRenderWindow* window);
void Restart(GameData *gameData);
#endif // !_GAMEDATA_H
