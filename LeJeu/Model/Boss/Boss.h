#ifndef  _BOSS_H
#define _BOSS_H
#include <SFML\System.h>
#include "..\Physics\FunctionsCollisions\Colision.h"
#include "..\Math\MathUtilities.h"
#include "..\Physics\RigidBody\RigidBody.h"
#include "..\Player\Player.h"
#include "..\Physics\Projectile\Shot.h"

typedef enum TypeBoss
{
	slime,
	elem,
	razar
}TypeBoss;

typedef enum PhaseBoss
{
	spawnPhase,
	p1,
	p2,
	p3,
	p4,
	p5,
	idleBoss,
	death
}PhaseBoss;

typedef struct StatBoss
{
	float Radius;
	float Speed;
	unsigned int XpGive;
	short LifeMax;
	unsigned short Damage;
}StatBoss;

typedef struct Boss
{
	Shot* FirstShot;
	RigidBody RigidBody;
	sfVector2f Direction;
	Circle Circle;
	sfBool IsMoving;
	sfBool IsSpawned;
	sfBool IsDead;
	sfBool HasTrashesAlive;
	sfBool TrashesNeedToSpawn;
	sfBool IsAttacking;
	sfBool CanBeEngaged;
	sfBool CanSpawn;
	TypeBoss Id;
	sfBool IsHit;
	unsigned int XpGive;
	float HitTimer;
	float CooldownShot[5];
	float TimerMovement;
	float timerSin;
	float Speed;
	float Life;
	short LifeMax;
	unsigned short Damage;
	char DirectionAnimation;
	PhaseBoss CurrentPhase;
	int RealPhase;
	int PaternChoosen;
#if _DEBUG
	sfCircleShape* Shape;
#endif
}Boss;

void InitBoss(Boss* boss);
void SpawnBoss(Boss* boss, const int id, const sfVector2f pos, const int nbrPlayer);
void UpdateBoss(Boss* boss, const Player player[], const short nbrOfPlayer, const float dt, const sfVector2f posRoom, const sfVector2f sizeRoom);
void PaternBoss(Boss* boss, const Player player[], const short nbrOfPlayer, const float dt, const sfVector2f posRoom, const sfVector2f sizeRoom);
void DamageBoss(Boss* boss, const int damage, int* playerXp);
void ResetBoss(Boss* boss);
#endif // ! _BOSS_H
