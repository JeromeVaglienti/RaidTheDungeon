#ifndef _TRAP_H
#define _TRAP_H
#include <stdio.h>
#include <stdlib.h>
#include "SFML\System.h"
#include "..\Physics\FunctionsCollisions\Colision.h"
#include "..\..\Controller\Camera\Camera.h"

typedef enum IdTrap
{
	flask,
	lightning,
	rockSpike,
	rockSpikeBoss
}IdTrap;

typedef struct Trap
{
	AnimationKey KeyAnimation;
	IdTrap Id;
	sfVector2f Pos;
	sfVector2f Direction;
	Circle Collider;
	sfBool CanSlow;
	sfBool MustDamage;
	sfBool CanMove;
	float Speed;
	float PercentageSpeed;
	float TimerDestroy;
	unsigned short Damage;
#if _DEBUG
	sfCircleShape* Shape;
#endif
}Trap;

typedef struct TrapData
{
	Trap Traps[100];
	unsigned int NbrTrap;
}TrapData;


void AddTrap(const sfVector2f pos, const sfVector2f direction, const IdTrap id, const float radius, const sfBool canSlow, const float percentageSlow, const float lifeTime, const unsigned short damage, const float speed);

void RemoveTrap(const unsigned int index);

void UpdateTrap(const float deltaTime);

TrapData* GetPointerToTrapData();

TrapData GetTrapData();

#endif // !_TRAP_H
