#ifndef _SHOT_H
#define _SHOT_H
#include <stdio.h>
#include <stdlib.h>
#include "SFML\System.h"
#include "SFML\Graphics.h"
#include "../RigidBody/RigidBody.h"
#include "../../Math/MathUtilities.h"
#include "../../../Controller/GraphicsUtilities/Utilities.h"
#include "..\FunctionsCollisions\Colision.h"
#include "..\..\..\Controller\GraphicsUtilities\Animation\Animation.h"
#include "..\..\Node\Node.h"

typedef enum
{
	matches,
	matchesB,
	matchesW,
	matchesG,
	fireBall,
	fireBallB,
	fireBallW,
	fireBallG,
	enemiesShot,
	bossShot,
	miniBossShot,
	kamikazeShot,
	fireElemShot,
	waterElemShot,
	iceElemShot,
	iceShard,
	windElemShot,
	dirtElemShot,
	lavaElemShot,
	iceBossElemShot,
	fireBallBossElemShot,
	razarShot,
	razarWindShot,
	razarBigFragShot,
	razarMediumFragShot,
	razarLittleFragShot,
	razarBoomerangShot,
	razarBlockShot
}IdShot;

typedef struct
{
	IdShot IdShot;
	float Speed;
	float Radius;
	float RemainingDistanceForDestroy;
	unsigned short Damage;
}ProtoShot;

typedef struct Shot Shot;
struct Shot
{
	AnimationKey *shotAnimKey;
	IdShot IdShot;
	RigidBody RigidBody;
	sfVector2f Direction;
	sfVector2f PivotPoint;
	sfVector2f EndPos;
	sfVector2f* LauncherPos;
	Circle Circle;
	long long int AdressRelative;
	float Speed;
	float Timer;
	unsigned short Damage;
	char DirectionRotate;
	float RemainingDistanceForDestroy;
	Shot* Next;
	Shot* Previous;
	int IdLight;
	Node *ActualNode;
#if _DEBUG
	sfCircleShape* Shape;
#endif
};

void AddShot(Shot** firstShot, const sfVector2f pos, const sfVector2f direction, const unsigned short id);

void AddWindElemShot(Shot** firstShot, const sfVector2f pos, const sfVector2f playerPos, const sfVector2f direction);

void AddRazarWindElemShot(Shot** firstShot, const sfVector2f pos, const sfVector2f playerPos, const sfVector2f direction, const char DirectionRotate);

void AddRazarBoomerangShot(Shot** firstShot, sfVector2f* pos, const sfVector2f direction);

void AddShotInMiddle(Shot* shot, const sfVector2f pos, const sfVector2f direction, const unsigned short id);

void AddShotInExplosionInMiddle(Shot* shot, const sfVector2f pos, const unsigned short id, const unsigned short nbrOfShots);

void AddShotInExplosion(Shot** firstShot, const sfVector2f pos, const unsigned short id, const unsigned short nbrOfShots);

void AddShotWithDistanceMultiplier(Shot** firstShot, const sfVector2f pos, const sfVector2f direction, const unsigned short id, const float distanceMultiplier);

void AddShotWithDamageMultiplier(Shot** firstShot, const sfVector2f pos, const sfVector2f direction, const unsigned short id, const float damageMultiplier);

void AddShotWithSpeedMultiplier(Shot** firstShot, const sfVector2f pos, const sfVector2f direction, const unsigned short id, const float speedMultiplier);

void AddShotWithSpeedAndDamageMultiplier(Shot** firstShot, const sfVector2f pos, const sfVector2f direction, const unsigned short id, const float speedMultiplier, const float damageMultiplier);

void AddMiniBossShot(Shot** firstShot, const sfVector2f pos, const sfVector2f direction, const float speed, const float RemainingDistanceForDestroy, const int DirectionRotate, const long long int adressRelative);

void AddRazarBlockShot(Shot** firstShot, const sfVector2f pos, const sfVector2f direction, const float speed, const float remainingDistanceForDestroy);

void UpdateShots(Shot** firstShot, const float deltaTime, Node* firstNode, const sfVector2f posPlayer[], const int nbrPlayer, Node* playerNode);

Shot* RemoveShot(Shot* Shot);

void ClearShots(Shot** firstShot);
#endif



