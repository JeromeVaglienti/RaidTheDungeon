#include "SpawnEnemy.h"


void InitSpawnEnemy(SpawnEnemy* spawnEnemy)
{
	SpawnEnemy tempSpawn = { 0 };
	(*spawnEnemy) = tempSpawn;
}

void ChangePositonSpawnEnemy(SpawnEnemy* spawnEnemy, const sfVector2f postion)
{
	spawnEnemy->Position = postion;
}

// remove all the spawner of the linked list
void ClearSpawnEnemy(SpawnEnemy** firstSpawn)
{
	if (*firstSpawn == NULL)
	{
		return;
	}
	else if ((*firstSpawn)->Next == NULL)
	{
		free(*firstSpawn);
		*firstSpawn = NULL;
		return;
	}
	SpawnEnemy* tempSpawn = (*firstSpawn);
	while (tempSpawn != NULL)
	{
		if (tempSpawn->Next != NULL)
		{
			SpawnEnemy* temp = tempSpawn->Next;
			free(tempSpawn);
			tempSpawn = temp;
		}
		else if (tempSpawn->Next == NULL)
		{
			free(tempSpawn);
			tempSpawn = NULL;
		}
	}
	*firstSpawn = NULL;
}

// add one spawner at the linked list
void AddSpawnEnemy(SpawnEnemy** firstSpawn)
{
	SpawnEnemy* tempSpawn = (SpawnEnemy*)calloc(1, sizeof(SpawnEnemy));
	if (*firstSpawn != NULL)
	{
		tempSpawn->Next = *firstSpawn;
	}
	else
	{
		tempSpawn->Next = NULL;
	}
	*firstSpawn = tempSpawn;
}

// assign all the needed value to a spawner
void SetSpawnEnemy(SpawnEnemy* spawnEnemy, const sfVector2f position, const unsigned short id, const unsigned short AmountOfEnemies, const float SpawnPerSecond)
{
	spawnEnemy->CooldownSpawn = 1.f / SpawnPerSecond;
	spawnEnemy->TotalAmountOfSpawnedEnemies = AmountOfEnemies;
	spawnEnemy->Id = id;
	spawnEnemy->Position = position;
	spawnEnemy->AmountOfSpawnedEnemies = 0;
	spawnEnemy->IsActive = sfTrue;
	spawnEnemy->Timer = 0.f;
}

//update the timer of the spawners and make enemies spawn
//disable the spawner when the number of enemies is reached
void UpdateSpawnEnemy(Enemy** firstEnemy, SpawnEnemy** spawnEnemy, const float deltaTime, Node* roomNode)
{
	SpawnEnemy* tempSpawn = *spawnEnemy;
	sfBool canRepeat = (tempSpawn != NULL);
	while (canRepeat)
	{
		if (tempSpawn->IsActive)
		{
			tempSpawn->Timer += deltaTime;
			if (tempSpawn->Timer >= tempSpawn->CooldownSpawn)
			{
				sfVector2f tempPos = tempSpawn->Position;
				float tempAngle = (float)rand() / RAND_MAX * M_PI * 2.f;
				float tempDistance = sqrtf((float)rand() / RAND_MAX) * 70.f;
				tempPos.x += tempDistance * cosf(tempAngle);
				tempPos.y += tempDistance * sinf(tempAngle);
				AddEnemy(firstEnemy, tempSpawn->Id, tempPos, roomNode);
				tempSpawn->Timer -= tempSpawn->CooldownSpawn;
				tempSpawn->AmountOfSpawnedEnemies++;
			}
			if (tempSpawn->AmountOfSpawnedEnemies >= tempSpawn->TotalAmountOfSpawnedEnemies)
			{
				tempSpawn->IsActive = sfFalse;
			}
		}
		if (tempSpawn != NULL)
		{
			if (tempSpawn->Next != NULL)
			{
				tempSpawn = tempSpawn->Next;
			}
			else
			{
				canRepeat = sfFalse;
			}
		}

	}
}
