#ifndef _LOOT_H
#define _LOOT_H
#include <stdio.h>
#include <stdlib.h>
#include "SFML\System.h"
#include "..\Physics\FunctionsCollisions\Colision.h"

typedef enum
{
	littleBottleOfMana,
	mediumBottleOfMana,
	littleBottleOfLife,
	mediumBottleOfLife
}IdLoot;

typedef struct
{
	unsigned short Id;
	unsigned short Nbr;
}LootTransfer;

typedef struct Loot
{
	unsigned short Id;
	unsigned short Nbr;
	sfVector2f Pos;
	Circle Colidier;
	sfBool IsAttract;
	int IdPlayer;
}Loot;

typedef struct LootData
{
	Loot Loot[100];
	unsigned short NbrLoot;
	float RadiusAttraction;
	float Speed;
}LootData;

void InitLoot(LootData* lootData);

unsigned short SpawnMana();

unsigned short SpawnLife();

void AddLoot(LootData* lootData, const sfVector2f pos, const unsigned short id);

void RemoveLoot(LootData* lootData, const unsigned short index);

#endif 

