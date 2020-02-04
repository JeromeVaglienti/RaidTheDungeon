#include "Loot.h"
#include "..\Math\Vector\VectorUtilities.h"
#include "..\..\Model\Player\Player.h"

void InitLoot(LootData* lootData)
{
	lootData->NbrLoot = 0;
	lootData->RadiusAttraction = 150.0f;
	lootData->Speed = 900.f;
}

unsigned short SpawnMana()
{
	unsigned short random = rand() % 100;
	unsigned short id = 0;
	if (random < 70)
	{
		id = littleBottleOfMana;
	}
	else if (random < 100)
	{
		id = mediumBottleOfMana;
	}
	return id;
}

unsigned short SpawnLife()
{
	unsigned short random = rand() % 100;
	unsigned short id = 0;
	if (random < 70)
	{
		id = littleBottleOfLife;
	}
	else if (random < 100)
	{
		id = mediumBottleOfLife;
	}
	return id;
}

void AddLoot(LootData* lootData, const sfVector2f pos, const unsigned short id)
{
	if (lootData->NbrLoot < 100)
	{
		lootData->Loot[lootData->NbrLoot].Colidier.center = pos;
		lootData->Loot[lootData->NbrLoot].Colidier.radius = 32;
		lootData->Loot[lootData->NbrLoot].Pos = pos;
		lootData->Loot[lootData->NbrLoot].Id = id;
		lootData->Loot[lootData->NbrLoot].IsAttract = sfFalse;
		lootData->Loot[lootData->NbrLoot].IdPlayer = -1;
		if (id == littleBottleOfMana)
		{
			lootData->Loot[lootData->NbrLoot].Nbr = 50;
		}
		else if (id == mediumBottleOfMana)
		{
			lootData->Loot[lootData->NbrLoot].Nbr = 100;
		}
		else if (id == littleBottleOfLife)
		{
			lootData->Loot[lootData->NbrLoot].Nbr = 5;
		}
		else if (id == mediumBottleOfLife)
		{
			lootData->Loot[lootData->NbrLoot].Nbr = 10;
		}
		lootData->NbrLoot++;
	}
}

void RemoveLoot(LootData* lootData, const unsigned short index)
{
	lootData->NbrLoot--;
	lootData->Loot[index] = lootData->Loot[lootData->NbrLoot];
}

