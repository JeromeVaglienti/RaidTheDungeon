#ifndef _SPAWNENEMY_H
#define _SPAWNENEMY_H
#include "SFML\System.h"
#include "..\..\Enemy\Enemy.h"
#include "..\..\Node\Node.h"

typedef struct SpawnEnemy SpawnEnemy;
struct SpawnEnemy
{
	sfVector2f Position;
	sfBool IsActive;
	float Timer;
	float CooldownSpawn;
	unsigned short Id;
	unsigned short AmountOfSpawnedEnemies;
	unsigned short TotalAmountOfSpawnedEnemies;
	SpawnEnemy* Next;
};


void ClearSpawnEnemy(SpawnEnemy** firstSpawn);

void AddSpawnEnemy(SpawnEnemy** firstSpawn);

void UpdateSpawnEnemy(Enemy** firstEnemy, SpawnEnemy** spawnEnemy, const float deltaTime, Node* roomNode);

void ChangePositonSpawnEnemy(SpawnEnemy* spawnEnemy, const sfVector2f postion);

void SetSpawnEnemy(SpawnEnemy* spawnEnemy, const sfVector2f position, const unsigned short id, const unsigned short AmountOfEnemies, const float SpawnPerSecond);

void InitSpawnEnemy(SpawnEnemy* spawnEnemy);


#endif 
