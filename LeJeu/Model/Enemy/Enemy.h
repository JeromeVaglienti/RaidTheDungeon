#ifndef _ENEMY_H
#define _ENEMY_H
#include <stdio.h>
#include <stdlib.h>
#include <SFML\System.h>
#include "..\Math\MathUtilities.h"
#include "..\Math\Vector\VectorUtilities.h"
#include "..\..\Controller\GraphicsUtilities\Utilities.h"
#include "..\Player\Player.h"
#include "..\Loot\Loot.h"
#include "..\..\Controller\GraphicsUtilities\Animation\Animation.h"
#include "..\AI\PathFinding\A_Star.h"
#include "..\..\Controller\Particles\Particles.h"

typedef enum TypeEnemySlime
{
	unknownSlime = -1,
	cac,
	distance,
	kamikaze,
	cacUpgrade,
	distanceUpgrade,
	kamikazeUpgrade,
	miniBoss,
}TypeEnemySlime;

typedef enum TypeEnemyElem
{
	unknownElem = -1,
	fireElem,
	waterElem,
	windElem,
	dirtElem,
	lavaElem,
	iceElem,
	stormElem,
	rockElem,
	littleElemOfLava
}TypeEnemyElem;

typedef enum StateAnimEnemy
{
	spawn,
	move,
	deathAnim
}StateAnimEnemy;

typedef enum DirectionEnemy
{
	idle,
	east,
	southEast,
	south,
	southWest,
	west,
	northWest,
	north,
	northEast,
}DirectionEnemy;

typedef struct StatEnemy
{
	float radius;
	float Speed;
	short Life;
	unsigned short Damage;
	float CooldownShot;
	int XPGives;
}StatEnemy;

typedef struct Timer
{
	float Hit;
	float Flask;
	float Movement;
	float CooldownShot[3];
	float PathFinding;
	float Smudge;
}Timer;

typedef struct Enemy Enemy;
struct Enemy
{
	AnimationKey DeathAnimKey;
	AnimationKey SpawnAnimKey;
	AnimationKey MoveAnimKey;
	float TransitionTimer;
	sfBool IsTransitioning;
	char TransitionDirection;
	AnimationKey AttackAnimKey;
	StateAnimEnemy StateAnim;
	RigidBody RigidBody;
	sfVector2f Direction;
	Circle Circle;
	//sfBool IsKnockBacked;
	sfBool IsMoving;
	sfBool IsHit;
	sfBool Attack;
	sfBool MustShot;
	int Id;
	float CooldownShot;
	float Speed;
	//float TimerKnock;
	float TimerSideWalk;
	Timer timer;
	int XPGives;
	float Life;
	unsigned short Damage;
	char DirectionAnimation;
	char DirectionSideWalk;
	Node *ActualNode;
	Enemy* Next;
	Enemy* Previous;
	NodeList *PathList;

#if _DEBUG
	sfCircleShape* Shape;
#endif
};

void InitStatEnemies();

void AddEnemy(Enemy** firstEnemy, const int id, const sfVector2f pos, Node* nodeArray);

void AddEnemyInMiddle(Enemy* enemy, const int id, const sfVector2f pos, Node* nodeArray);

void UpdateEnemies(Player player[], const short nbrOfPlayer, Enemy** firstEnemy, Shot** firstShot, const float deltaTime, LootData* lootData, Node* firstNode, const sfVector2f posRoom, const sfVector2f sizeRoom, Particles** particles);

Enemy* RemoveEnemy(Enemy* Enemy);

void ClearEnemies(Enemy** firstEnemy);

void DamageEnemy(Enemy* enemy, const float damage);

void UpdateShotsOfMiniBoss(Enemy* firstEnemy, Shot* firstShot, const float deltaTime);

#endif



