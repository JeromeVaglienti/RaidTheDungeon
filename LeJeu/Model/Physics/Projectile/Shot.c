#include "shot.h"
#include "..\..\..\Controller\MainSceneController\ShadersController\ShadersController.h"
#include "..\..\..\Controller\MainSceneController\SkinsController\SkinsController.h"
#include "..\Collision\Collision.h"

//IdShot, Speed, Radius, RemainingDistanceForDestroy, Damage
ProtoShot protoShots[28] =
{
	{ matches, 650.f, 5.f, 800.f, 5},
	{ matchesB, 650.f, 5.f, 800.f, 5 },
	{ matchesW, 650.f, 5.f, 800.f, 5 },
	{ matchesG, 650.f, 5.f, 800.f, 5 },
	{ fireBall, 900.f, 32.f, 1300.f, 30 },
	{ fireBallB, 900.f, 32.f, 1300.f, 30 },
	{ fireBallW, 900.f, 32.f, 1300.f, 30 },
	{ fireBallG, 900.f, 32.f, 1300.f, 30 },
	{ enemiesShot, 450.f, 10.f, 1500.f, 1 },
	{ bossShot, 300.f, 10.f, 2000.f, 5 },
	{ miniBossShot, 400.f, 10.f, 2000.f, 4 },
	{ kamikazeShot, 600.f, 15.f, 2000.f, 4 },
	{ fireElemShot, 400.f, 15.f, 1300.f, 2 },
	{ waterElemShot, 600.f, 15.f, 2000.f, 2 },
	{ iceElemShot, 500.f, 15.f, 1000.f, 5 },
	{ iceShard, 600.f, 15.f, 2000.f, 3 },
	{ windElemShot, 700.f, 15.f, 50000.f, 2 },
	{ dirtElemShot, 400.f, 15.f, 2000.f, 2 },
	{ lavaElemShot, 450.f, 15.f, 2000.f, 2 },
	{ iceBossElemShot, 600.f, 15.f, 200.f, 10 },
	{ fireBallBossElemShot, 600.f, 55.f, 2500.f, 10 },
	{ razarShot, 500.f, 20.f, 1500.f, 5 },
	{ razarWindShot, 1000.f, 15.f, 8500.f, 5 },
	{ razarBigFragShot, 300.f, 45.f, 500.f, 15 },
	{ razarMediumFragShot, 450.f, 30.f, 500.f, 10 },
	{ razarLittleFragShot, 600.f, 15.f, 1500.f, 5 },
	{ razarBoomerangShot, 500.f, 20.f, 1000.f, 10 },
	{ razarBlockShot, 0.f, 50.f, 0.f, 50 }
};

void InitAnimKeyOfShot(Shot* tempShot, const int id)
{
	tempShot->shotAnimKey = (AnimationKey*)calloc(1, sizeof(AnimationKey));
	if (id == matches)
	{
		tempShot->IdLight = GetAvaibleLight();
		ActivateLigth(tempShot->IdLight, 1);
		SetIndexSprite(tempShot->IdLight, 2);

		SetLigthColor(tempShot->IdLight, REDLIGHT);


		*tempShot->shotAnimKey = InitAnimKey(12, .5f, sfTrue);
	}
	else if (id == matchesB)
	{
		tempShot->IdLight = GetAvaibleLight();
		ActivateLigth(tempShot->IdLight, 1);
		SetIndexSprite(tempShot->IdLight, 2);

		SetLigthColor(tempShot->IdLight, REDLIGHTBLACK);


		*tempShot->shotAnimKey = InitAnimKey(12, .5f, sfTrue);
	}
	else if (id == matchesW)
	{
		tempShot->IdLight = GetAvaibleLight();
		ActivateLigth(tempShot->IdLight, 1);
		SetIndexSprite(tempShot->IdLight, 2);

		SetLigthColor(tempShot->IdLight, REDLIGHTWHITE);


		*tempShot->shotAnimKey = InitAnimKey(12, .5f, sfTrue);
	}
	else if (id == matchesG)
	{
		tempShot->IdLight = GetAvaibleLight();
		ActivateLigth(tempShot->IdLight, 1);
		SetIndexSprite(tempShot->IdLight, 2);

		SetLigthColor(tempShot->IdLight, REDLIGHTGREEN);


		*tempShot->shotAnimKey = InitAnimKey(12, .5f, sfTrue);
	}
	else if (id == fireBall)
	{
		tempShot->IdLight = GetAvaibleLight();
		ActivateLigth(tempShot->IdLight, 1);
		SetIndexSprite(tempShot->IdLight, 0);
		SetLigthColor(tempShot->IdLight, FIREBALLLIGHT);
		*tempShot->shotAnimKey = InitAnimKey(16, 1.f, sfTrue);
	}
	else if (id == fireBallB)
	{
		tempShot->IdLight = GetAvaibleLight();
		ActivateLigth(tempShot->IdLight, 1);
		SetIndexSprite(tempShot->IdLight, 0);
		SetLigthColor(tempShot->IdLight, FIREBALLLIGHTBLACK);
		*tempShot->shotAnimKey = InitAnimKey(16, 1.f, sfTrue);
	}
	else if (id == fireBallW)
	{
		tempShot->IdLight = GetAvaibleLight();
		ActivateLigth(tempShot->IdLight, 1);
		SetIndexSprite(tempShot->IdLight, 0);
		SetLigthColor(tempShot->IdLight, FIREBALLLIGHTWHITE);
		*tempShot->shotAnimKey = InitAnimKey(16, 1.f, sfTrue);
	}
	else if (id == fireBallG)
	{
		tempShot->IdLight = GetAvaibleLight();
		ActivateLigth(tempShot->IdLight, 1);
		SetIndexSprite(tempShot->IdLight, 0);
		SetLigthColor(tempShot->IdLight, FIREBALLLIGHTGREEN);
		*tempShot->shotAnimKey = InitAnimKey(16, 1.f, sfTrue);
	}
	else if (id == enemiesShot)
	{
		*tempShot->shotAnimKey = InitAnimKey(12, .5f, sfTrue);
	}
	else if (id == kamikazeShot)
	{
		*tempShot->shotAnimKey = InitAnimKey(12, .5f, sfTrue);
	}
	else if (id == fireElemShot || id == fireBallBossElemShot)
	{
		*tempShot->shotAnimKey = InitAnimKey(12, .5f, sfTrue);
	}
	else if (id == waterElemShot)
	{
		*tempShot->shotAnimKey = InitAnimKey(16, 1.f, sfTrue);
	}
	else if (id == dirtElemShot)
	{
		*tempShot->shotAnimKey = InitAnimKey(16, 1.f, sfTrue);
	}
	else if (id == windElemShot)
	{
		*tempShot->shotAnimKey = InitAnimKey(12, 1.f, sfTrue);
	}
	else if (id == razarShot)
	{
		*tempShot->shotAnimKey = InitAnimKey(12, 1.f, sfTrue);
	}
	else if (id == razarBlockShot)
	{
		*tempShot->shotAnimKey = InitAnimKey(24, 24.f / 12.f, sfTrue);
	}
	else if (id == razarBoomerangShot)
	{
		*tempShot->shotAnimKey = InitAnimKey(12, 1.f, sfTrue);
	}
	else if (id == razarWindShot)
	{
		*tempShot->shotAnimKey = InitAnimKey(12, 1.f, sfTrue);
	}
	else if (id == razarLittleFragShot)
	{
		*tempShot->shotAnimKey = InitAnimKey(9, 9.f / 12.f, sfTrue);
	}
	else if (id == razarMediumFragShot)
	{
		*tempShot->shotAnimKey = InitAnimKey(9, 9.f / 12.f, sfTrue);
	}
	else if (id == razarBigFragShot)
	{
		*tempShot->shotAnimKey = InitAnimKey(9, 9.f / 12.f, sfTrue);
	}
	else
	{
		*tempShot->shotAnimKey = InitAnimKey(12, 1.0f, sfTrue);
	}
}

// add one shot at the linked list
void AddShot(Shot** firstShot, const sfVector2f pos, const sfVector2f direction, const unsigned short id)
{
	Shot* tempShot = (Shot*)calloc(1, sizeof(Shot));
	tempShot->IdShot = id;
	tempShot->Circle.center = pos;
	tempShot->Circle.radius = protoShots[id].Radius;
	tempShot->Direction = direction;
	tempShot->RigidBody.Position.x = pos.x;
	tempShot->RigidBody.Position.y = pos.y;
	if (id == bossShot && id == miniBossShot)
	{
		tempShot->RigidBody.Position.x += 45.f * direction.x;
		tempShot->RigidBody.Position.y += 45.f * direction.y;
	}
	tempShot->RigidBody.Velocity.x = protoShots[id].Speed * direction.x;
	tempShot->RigidBody.Velocity.y = protoShots[id].Speed * direction.y;
	tempShot->RigidBody.Mass = 1;
	tempShot->Speed = protoShots[id].Speed;
	tempShot->Damage = protoShots[id].Damage;
	tempShot->RemainingDistanceForDestroy = protoShots[id].RemainingDistanceForDestroy;
	tempShot->IdLight = -1;
	tempShot->Timer = 1.f;
	tempShot->ActualNode = NULL;
#if _DEBUG
	tempShot->Shape = sfCircleShape_create();
	sfCircleShape_setOutlineThickness(tempShot->Shape, 1.f);
	sfCircleShape_setOutlineColor(tempShot->Shape, sfRed);
	sfCircleShape_setFillColor(tempShot->Shape, sfTransparent);
	sfCircleShape_setRadius(tempShot->Shape, tempShot->Circle.radius);
	sfCircleShape_setOrigin(tempShot->Shape, (sfVector2f) { tempShot->Circle.radius, tempShot->Circle.radius });
#endif
	InitAnimKeyOfShot(tempShot, id);
	if (id == iceBossElemShot)
	{
		tempShot->RemainingDistanceForDestroy = 30.f + rand() % 80;
	}
	if (*firstShot != NULL)
	{
		(*firstShot)->Previous = tempShot;
		tempShot->Next = *firstShot;
	}
	else
	{
		tempShot->Next = NULL;
	}
	tempShot->Previous = NULL;
	*firstShot = tempShot;
}

void AddShotInMiddle(Shot* shot, const sfVector2f pos, const sfVector2f direction, const unsigned short id)
{
	Shot* tempShot = (Shot*)calloc(1, sizeof(Shot));
	tempShot->IdShot = id;
	tempShot->Circle.center = pos;
	tempShot->Circle.radius = protoShots[id].Radius;
	tempShot->Direction = direction;
	tempShot->RigidBody.Position.x = pos.x;
	tempShot->RigidBody.Position.y = pos.y;
	if (id == bossShot && id == miniBossShot)
	{
		tempShot->RigidBody.Position.x += 45.f * direction.x;
		tempShot->RigidBody.Position.y += 45.f * direction.y;
	}
	tempShot->RigidBody.Velocity.x = protoShots[id].Speed * direction.x;
	tempShot->RigidBody.Velocity.y = protoShots[id].Speed * direction.y;
	tempShot->RigidBody.Mass = 1;
	tempShot->Speed = protoShots[id].Speed;
	tempShot->Damage = protoShots[id].Damage;
	tempShot->RemainingDistanceForDestroy = protoShots[id].RemainingDistanceForDestroy;
	tempShot->IdLight = -1;
	tempShot->Timer = 1.f;
	tempShot->ActualNode = NULL;
#if _DEBUG
	tempShot->Shape = sfCircleShape_create();
	sfCircleShape_setOutlineThickness(tempShot->Shape, 1.f);
	sfCircleShape_setOutlineColor(tempShot->Shape, sfRed);
	sfCircleShape_setFillColor(tempShot->Shape, sfTransparent);
	sfCircleShape_setRadius(tempShot->Shape, tempShot->Circle.radius);
	sfCircleShape_setOrigin(tempShot->Shape, (sfVector2f) { tempShot->Circle.radius, tempShot->Circle.radius });
#endif
	InitAnimKeyOfShot(tempShot, id);

	tempShot->Next = shot->Next;
	tempShot->Previous = shot;
	if (tempShot->Next != NULL)
	{
		tempShot->Next->Previous = tempShot;
	}
	shot->Next = tempShot;
}

void AddWindElemShot(Shot** firstShot, const sfVector2f pos, const sfVector2f playerPos, const sfVector2f direction)
{
	AddShot(firstShot, pos, direction, windElemShot);
	(*firstShot)->EndPos = playerPos;
	(*firstShot)->PivotPoint = pos;
	(*firstShot)->Timer = 0.f;
	(*firstShot)->DirectionRotate = rand() % 2 ? 1 : -1;
}

void AddRazarWindElemShot(Shot** firstShot, const sfVector2f pos, const sfVector2f playerPos, const sfVector2f direction, const char DirectionRotate)
{
	AddShot(firstShot, pos, direction, razarWindShot);
	(*firstShot)->EndPos = playerPos;
	(*firstShot)->PivotPoint = pos;
	(*firstShot)->Timer = 0.f;
	(*firstShot)->DirectionRotate = DirectionRotate;
}

void AddRazarBoomerangShot(Shot** firstShot, sfVector2f* pos, const sfVector2f direction)
{
	AddShot(firstShot, *pos, direction, razarBoomerangShot);
	(*firstShot)->LauncherPos = pos;
	(*firstShot)->Timer = 1.f;
}

void AddShotInExplosionInMiddle(Shot* shot, const sfVector2f pos, const unsigned short id, const unsigned short nbrOfShots)
{
	float basicAngle = (float)(rand() % (int)(360 / nbrOfShots));
	for (int i = 0; i <= 360; i += (int)(360 / nbrOfShots))
	{
		float angle = DegreeToRadian(i + basicAngle);
		sfVector2f dirShot = { cosf(angle), sinf(angle) };
		AddShotInMiddle(shot, pos, dirShot, id);
	}
}

void AddShotInExplosion(Shot** firstShot, const sfVector2f pos, const unsigned short id, const unsigned short nbrOfShots)
{
	float basicAngle = (float)(rand() % (int)(360 / nbrOfShots));
	for (int i = 0; i <= 360; i += (int)(360 / nbrOfShots))
	{
		float angle = DegreeToRadian(i + basicAngle);
		sfVector2f dirShot = { cosf(angle), sinf(angle) };
		AddShot(firstShot, pos, dirShot, id);
	}
}

void AddShotWithDistanceMultiplier(Shot** firstShot, const sfVector2f pos, const sfVector2f direction, const unsigned short id, const float distanceMultiplier)
{
	AddShot(firstShot, pos, direction, id);
	(*firstShot)->RemainingDistanceForDestroy *= distanceMultiplier;
}

void AddShotWithDamageMultiplier(Shot** firstShot, const sfVector2f pos, const sfVector2f direction, const unsigned short id, const float damageMultiplier)
{
	AddShot(firstShot, pos, direction, id);
	(*firstShot)->Damage = (short)((float)(*firstShot)->Damage * damageMultiplier);
}

void AddShotWithSpeedMultiplier(Shot** firstShot, const sfVector2f pos, const sfVector2f direction, const unsigned short id, const float speedMultiplier)
{
	AddShot(firstShot, pos, direction, id);
	(*firstShot)->Speed *= speedMultiplier;
	(*firstShot)->RigidBody.Velocity.x = (*firstShot)->Speed * direction.x;
	(*firstShot)->RigidBody.Velocity.y = (*firstShot)->Speed * direction.y;
}

void AddShotWithSpeedAndDamageMultiplier(Shot** firstShot, const sfVector2f pos, const sfVector2f direction, const unsigned short id, const float speedMultiplier, const float damageMultiplier)
{
	AddShot(firstShot, pos, direction, id);
	(*firstShot)->Speed *= speedMultiplier;
	(*firstShot)->RigidBody.Velocity.x = (*firstShot)->Speed * direction.x;
	(*firstShot)->RigidBody.Velocity.y = (*firstShot)->Speed * direction.y;
	(*firstShot)->Damage = (short)((float)(*firstShot)->Damage * damageMultiplier);
}

void AddMiniBossShot(Shot** firstShot, const sfVector2f pos, const sfVector2f direction, const float speed, const float remainingDistanceForDestroy, const int DirectionRotate, const long long int adressRelative)
{
	Shot* tempShot = (Shot*)calloc(1, sizeof(Shot));
	tempShot->IdShot = miniBossShot;
	tempShot->Circle.center = pos;
	tempShot->Circle.radius = protoShots[miniBossShot].Radius;
	tempShot->Direction = direction;
	tempShot->RigidBody.Position.x = pos.x;
	tempShot->RigidBody.Position.y = pos.y;
	tempShot->RigidBody.Velocity.x = speed * direction.x;
	tempShot->RigidBody.Velocity.y = speed * direction.y;
	tempShot->RigidBody.Mass = 1;
	tempShot->Speed = speed;
	tempShot->Damage = protoShots[miniBossShot].Damage;
	tempShot->RemainingDistanceForDestroy = remainingDistanceForDestroy;
	tempShot->Timer = 1.1f;
	tempShot->DirectionRotate = DirectionRotate;
	tempShot->PivotPoint = pos;
	tempShot->AdressRelative = adressRelative;

	InitAnimKeyOfShot(tempShot, miniBossShot);

#if _DEBUG
	tempShot->Shape = sfCircleShape_create();
	sfCircleShape_setOutlineThickness(tempShot->Shape, 1.f);
	sfCircleShape_setOutlineColor(tempShot->Shape, sfRed);
	sfCircleShape_setFillColor(tempShot->Shape, sfTransparent);
	sfCircleShape_setRadius(tempShot->Shape, tempShot->Circle.radius);
	sfCircleShape_setOrigin(tempShot->Shape, (sfVector2f) { tempShot->Circle.radius, tempShot->Circle.radius });
#endif

	if (*firstShot != NULL)
	{
		(*firstShot)->Previous = tempShot;
		tempShot->Next = *firstShot;
	}
	else
	{
		tempShot->Next = NULL;
	}
	tempShot->Previous = NULL;
	*firstShot = tempShot;
}

void AddRazarBlockShot(Shot** firstShot, const sfVector2f pos, const sfVector2f direction, const float speed, const float remainingDistanceForDestroy)
{
	AddShot(firstShot, pos, direction, razarBlockShot);
	(*firstShot)->Speed = speed;
	(*firstShot)->RemainingDistanceForDestroy = remainingDistanceForDestroy;
	(*firstShot)->RigidBody.Velocity.x = speed * direction.x;
	(*firstShot)->RigidBody.Velocity.y = speed * direction.y;
}

// remove a shot of the linked list
Shot* RemoveShot(Shot* shot)
{
	Shot tempShot = *shot;
#if _DEBUG
	sfCircleShape_destroy((*shot).Shape);
#endif // _DEBUG

	if (shot->IdLight > 0)
	{
		ActivateLigth(shot->IdLight, 0);
	}

	if (shot->shotAnimKey != NULL)
	{
		free(shot->shotAnimKey);
	}

	if (shot->Previous != NULL && shot->Next != NULL)
	{
		shot->Previous->Next = shot->Next;
		shot->Next->Previous = shot->Previous;
		free(shot);
		shot = NULL;
		return tempShot.Next;
	}
	else if (shot->Previous == NULL && shot->Next == NULL)
	{
		free(shot);
		shot = NULL;
		return NULL;
	}
	else if (shot->Next == NULL && shot->Previous != NULL)
	{
		shot->Previous->Next = NULL;
		free(shot);
		shot = NULL;
		return tempShot.Previous;
	}
	else if (shot->Previous == NULL && shot->Next != NULL)
	{
		shot->Next->Previous = NULL;
		free(shot);
		shot = NULL;
		return tempShot.Next;
	}
	return NULL;
}

// remove all the shots of the linked list
void ClearShots(Shot** firstShot)
{
	if ((*firstShot) == NULL)
	{
		(*firstShot) = NULL;
		return;
	}
	else if ((*firstShot)->Next == NULL)
	{
#if _DEBUG
		sfCircleShape_destroy((*firstShot)->Shape);
#endif // _DEBUG

		if ((*firstShot)->shotAnimKey != NULL)
		{
			free((*firstShot)->shotAnimKey);
		}
		free(*firstShot);
		(*firstShot) = NULL;
		return;
	}
	Shot* tempShot = (*firstShot)->Next;
	while (tempShot != NULL)
	{
		if (tempShot->IdLight > 0)
		{
			ActivateLigth(tempShot->IdLight, 0);
			tempShot->IdLight = -1;
		}
		if (tempShot->Previous != NULL)
		{
#if _DEBUG
			sfCircleShape_destroy(tempShot->Previous->Shape);
#endif // _DEBUG
			free(tempShot->Previous->shotAnimKey);
			free(tempShot->Previous);
			if (tempShot->Next == NULL)
			{
#if _DEBUG
				sfCircleShape_destroy(tempShot->Shape);
#endif // _DEBUG
				free(tempShot->shotAnimKey);
				free(tempShot);
				tempShot = NULL;
			}
			else
			{
				tempShot = tempShot->Next;
			}
		}
		else if (tempShot->Next == NULL)
		{
#if _DEBUG
			sfCircleShape_destroy(tempShot->Shape);
#endif // _DEBUG
			free(tempShot->shotAnimKey);
			free(tempShot);
			tempShot = NULL;
		}
	}
	(*firstShot) = NULL;
}

void SetInitNodeShot(Shot *shot, Node* firstNode)
{
	shot->ActualNode = GetNodeInArray(shot->RigidBody.Position.x, shot->RigidBody.Position.y, firstNode);
}

// updates shooting physics and remove 
void UpdateShots(Shot** firstShot, const float deltaTime, Node* firstNode, const sfVector2f posPlayer[], const int nbrPlayer, Node* playerNode)
{
	if (*firstShot == NULL)
	{
		return;
	}
	short indexNearestPlayer = 0;
	Shot* tempShot = *firstShot;
	sfBool canRepeat = (tempShot != NULL);
	while (canRepeat)
	{
		if (tempShot->ActualNode == NULL)
		{
			if (tempShot->IdShot == fireBall || tempShot->IdShot == matches)
			{
				SetInitNodeShot(tempShot, playerNode);
			}
			else
			{
				SetInitNodeShot(tempShot, firstNode);
			}
		}
		if (tempShot->IdLight > 0)
		{
			SetLigthPosition(tempShot->IdLight, tempShot->RigidBody.Position, playerNode);
		}
		tempShot->RemainingDistanceForDestroy -= Magnitude(tempShot->RigidBody.Velocity) * deltaTime;
		if (tempShot->RemainingDistanceForDestroy > 0)
		{
			if (tempShot->IdShot == fireElemShot)
			{
				sfVector2f vectorToPlayer;
				if (nbrPlayer == 2 && Distance(tempShot->RigidBody.Position, posPlayer[1]) < Distance(tempShot->RigidBody.Position, posPlayer[0]))
				{
					indexNearestPlayer = 1;
				}
				else
				{
					indexNearestPlayer = 0;
				}
				vectorToPlayer = Normalize(AddTwoVectors(tempShot->RigidBody.Position, posPlayer[indexNearestPlayer]));
				if (Dot(vectorToPlayer, tempShot->Direction) >= 0)
				{
					float percentage = 80.f;
					sfVector2f tempV = { (vectorToPlayer.x + tempShot->Direction.x * percentage) / (percentage + 1), (vectorToPlayer.y + tempShot->Direction.y * percentage) / (percentage + 1) };
					tempV = Normalize(tempV);
					tempShot->Direction = tempV;
					tempShot->RigidBody.Velocity.x = tempShot->Speed * tempShot->Direction.x;
					tempShot->RigidBody.Velocity.y = tempShot->Speed * tempShot->Direction.y;
				}
			}
			UpdateRigidBody(&tempShot->RigidBody, deltaTime);
			if (tempShot->IdShot == windElemShot || tempShot->IdShot == razarWindShot)
			{
				float distance = Distance(tempShot->PivotPoint, tempShot->EndPos);
				float circonference = distance * M_PI;
				tempShot->Timer += deltaTime * ((M_PI * 2.f) / (circonference / tempShot->Speed));
				tempShot->RigidBody.Position = EllipticalTrajectory(tempShot->Timer, tempShot->PivotPoint, tempShot->EndPos, tempShot->DirectionRotate);
				tempShot->Circle.center = tempShot->RigidBody.Position;
				if (tempShot->Timer >= M_PI * 2)
				{
					tempShot->Timer = 0.f;
					tempShot->RemainingDistanceForDestroy = 0.f;
				}
			}

		}
		sfVector2f tempPos = { tempShot->RigidBody.Position.x, tempShot->RigidBody.Position.y };
		tempShot->Circle.center = tempPos;

		if (tempShot->shotAnimKey != NULL)
		{
			if (tempShot->IdShot == fireBall || tempShot->IdShot == (IdShot)enemiesShot || tempShot->IdShot == (IdShot)kamikazeShot || tempShot->IdShot == (IdShot)waterElemShot
				|| tempShot->IdShot == (IdShot)dirtElemShot)
			{
				UpdateAnimationKeyWithSpecialLoop(tempShot->shotAnimKey, deltaTime, 5);
			}
			else if (tempShot->IdShot == razarShot)
			{
				UpdateAnimationKeyWithSpecialLoop(tempShot->shotAnimKey, deltaTime, 8);
			}
			else if (tempShot->IdShot == razarBlockShot)
			{
				if (tempShot->RemainingDistanceForDestroy <= 0)
				{
					UpdateAnimationKeyWithSpecialLoop(tempShot->shotAnimKey, deltaTime, 10);
				}
			}
			else
			{
				UpdateAnimationKey(tempShot->shotAnimKey, deltaTime);
			}
		}
		if (tempShot->RemainingDistanceForDestroy <= 0 || CheckCollisionShotTrap(tempShot) ||
			(tempShot->ActualNode != NULL && !tempShot->ActualNode->isWalkable && !tempShot->ActualNode->isShootable && protoShots[tempShot->IdShot].RemainingDistanceForDestroy - tempShot->RemainingDistanceForDestroy >= 60))
		{
			if ((tempShot->IdShot != miniBossShot && tempShot->IdShot != razarBoomerangShot && tempShot->IdShot != razarBlockShot) || (tempShot->ActualNode != NULL && !tempShot->ActualNode->isWalkable && !tempShot->ActualNode->isShootable))
			{
				if (tempShot->IdShot == iceElemShot)
				{
					if (tempShot->ActualNode != NULL && !tempShot->ActualNode->isWalkable)
					{
						Node* node = SearchWalkableNodeAround(tempShot->ActualNode);
						sfVector2f pos = { node->posX * 64.f, node->posY * 64.f };
						AddShotInExplosionInMiddle(tempShot, pos, iceShard, 5);
					}
					else
					{
						AddShotInExplosionInMiddle(tempShot, tempShot->RigidBody.Position, iceShard, 5);
					}
					tempShot = RemoveShot(tempShot);
				}
				else if (tempShot->IdShot == iceBossElemShot)
				{
					if (tempShot->ActualNode != NULL && !tempShot->ActualNode->isWalkable)
					{
						Node* node = SearchWalkableNodeAround(tempShot->ActualNode);
						sfVector2f pos = { node->posX * 64.f, node->posY * 64.f };
						AddShotInExplosionInMiddle(tempShot, pos, iceShard, 10);
					}
					else
					{
						AddShotInExplosionInMiddle(tempShot, tempShot->RigidBody.Position, iceShard, 10);
					}
					tempShot = RemoveShot(tempShot);
				}
				else if (tempShot->IdShot == razarBigFragShot)
				{
					if (tempShot->ActualNode != NULL && !tempShot->ActualNode->isWalkable)
					{
						Node* node = SearchWalkableNodeAround(tempShot->ActualNode);
						sfVector2f pos = { node->posX * 64.f, node->posY * 64.f };
						AddShotInExplosionInMiddle(tempShot, pos, razarMediumFragShot, 5);
					}
					else
					{
						AddShotInExplosionInMiddle(tempShot, tempShot->RigidBody.Position, razarMediumFragShot, 5);
					}
					tempShot = RemoveShot(tempShot);
				}
				else if (tempShot->IdShot == razarMediumFragShot)
				{
					if (tempShot->ActualNode != NULL && !tempShot->ActualNode->isWalkable)
					{
						Node* node = SearchWalkableNodeAround(tempShot->ActualNode);
						sfVector2f pos = { node->posX * 64.f, node->posY * 64.f };
						AddShotInExplosionInMiddle(tempShot, pos, razarLittleFragShot, 5);
					}
					else
					{
						AddShotInExplosionInMiddle(tempShot, tempShot->RigidBody.Position, razarLittleFragShot, 5);
					}
					tempShot = RemoveShot(tempShot);
				}
				if (tempShot != NULL)
				{

					tempShot = RemoveShot(tempShot);
				}
				if (tempShot == NULL)
				{
					*firstShot = NULL;
					return;
				}
				else if (tempShot->Previous == NULL)
				{
					*firstShot = tempShot;
				}
			}
			else if (tempShot->IdShot == miniBossShot)
			{
				tempShot->Timer -= deltaTime;

				if (tempShot->Timer <= 0)
				{
					tempShot = RemoveShot(tempShot);
					if (tempShot == NULL)
					{
						*firstShot = NULL;
						return;
					}
					else if (tempShot->Previous == NULL)
					{
						*firstShot = tempShot;
					}
				}
				else if (tempShot->Timer >= 0.1)
				{
					tempShot->RigidBody.Position = RotateAround(tempShot->RigidBody.Position, tempShot->PivotPoint, DegreeToRadian(30.f * tempShot->DirectionRotate) * deltaTime);
					tempShot->Circle.center = tempShot->RigidBody.Position;
				}
			}
			else if (tempShot->IdShot == razarBoomerangShot)
			{
				tempShot->Timer -= deltaTime;
				if (tempShot->Timer <= 0)
				{
					tempShot->Direction = Normalize(AddTwoVectors(tempShot->RigidBody.Position, *tempShot->LauncherPos));
					tempShot->RigidBody.Velocity.x = protoShots[razarBoomerangShot].Speed * tempShot->Direction.x * 3.f;
					tempShot->RigidBody.Velocity.y = protoShots[razarBoomerangShot].Speed * tempShot->Direction.y * 3.f;
					UpdateRigidBody(&tempShot->RigidBody, deltaTime);
					tempShot->Circle.center = tempShot->RigidBody.Position;
				}
				if (Distance(tempShot->RigidBody.Position, *tempShot->LauncherPos) <= 10)
				{
					tempShot = RemoveShot(tempShot);
				}
			}
		}
		if (tempShot != NULL)
		{
			if (tempShot->ActualNode == NULL)
			{
				if (tempShot->IdShot == fireBall || tempShot->IdShot == matches)
				{
					SetInitNodeShot(tempShot, playerNode);
				}
				else
				{
					SetInitNodeShot(tempShot, firstNode);
				}
			}
			else
			{
				ChangeNode(&tempShot->ActualNode, tempShot->RigidBody.Position);
			}
		}



		if (tempShot != NULL && tempShot->Next != NULL)
		{
			tempShot = tempShot->Next;
		}
		else
		{
			canRepeat = sfFalse;
		}
	}
	if (tempShot != NULL)
	{
		while (tempShot->Previous != NULL)
		{
			tempShot = tempShot->Previous;
		}
	}
	*firstShot = tempShot;
}
