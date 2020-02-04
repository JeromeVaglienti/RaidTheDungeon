#ifndef _SPAWNENEMY_H
#define _SPAWNENEMY_H
#include "SFML\System.h"
#include "..\Enemy.h"

typedef struct SpawnEnemy
{
	sfVector2f Position;
	sfBool IsActive;
	float Timer;
	float CooldownSpawn;
	unsigned short Id;
	unsigned short AmountOfSpawnedEnemies;
	unsigned short TotalAmountOfSpawnedEnemies;
}SpawnEnemy;

void UpdateSpawnEnemy(Enemy** firstEnemy, SpawnEnemy* spawnEnemy, float deltaTime);

void ChangePositonSpawnEnemy(SpawnEnemy* spawnEnemy, sfVector2f postion);

void SetSpawnEnemy(SpawnEnemy* spawnEnemy, sfVector2f position, unsigned short id, unsigned short AmountOfEnemies, float SpawnPerSecond);

void InitSpawnEnemy(SpawnEnemy* spawnEnemy);


#endif 
