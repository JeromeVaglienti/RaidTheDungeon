#include "SpawnEnemy.h"


void InitSpawnEnemy(SpawnEnemy* spawnEnemy)
{
	*spawnEnemy = (SpawnEnemy) { 0 };
}

void ChangePositonSpawnEnemy(SpawnEnemy* spawnEnemy, sfVector2f postion)
{
	spawnEnemy->Position = postion;
}

void SetSpawnEnemy(SpawnEnemy* spawnEnemy, sfVector2f position, unsigned short id, unsigned short AmountOfEnemies, float SpawnPerSecond)
{
	spawnEnemy->CooldownSpawn = 1.f / SpawnPerSecond;
	spawnEnemy->TotalAmountOfSpawnedEnemies = AmountOfEnemies;
	spawnEnemy->Id = id;
	spawnEnemy->Position = position;
	spawnEnemy->AmountOfSpawnedEnemies = 0;
	spawnEnemy->IsActive = sfTrue;
	spawnEnemy->Timer = 0.f;
}

void UpdateSpawnEnemy(Enemy** firstEnemy, SpawnEnemy* spawnEnemy, float deltaTime)
{
	if (spawnEnemy->IsActive)
	{
		spawnEnemy->Timer += deltaTime;
		if (spawnEnemy->Timer >= spawnEnemy->CooldownSpawn)
		{
			AddEnemy(firstEnemy, spawnEnemy->Id, spawnEnemy->Position);
			spawnEnemy->Timer -= spawnEnemy->CooldownSpawn;
			spawnEnemy->AmountOfSpawnedEnemies++;
		}
		if (spawnEnemy->AmountOfSpawnedEnemies >= spawnEnemy->TotalAmountOfSpawnedEnemies)
		{
			*spawnEnemy = (SpawnEnemy) { 0 };
		}
	}
}
