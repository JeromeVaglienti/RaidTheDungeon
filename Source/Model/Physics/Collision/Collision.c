#include "Collision.h"
#include "..\..\..\Controller\HUD\HUDController.h"
#include "..\..\Trap\Trap.h"

void CheckCollisionBossRoom(Boss *boss, const RoomGameData *room)
{
	if (boss->Id == razar)
	{
		for (int i = 0; i < 4; i++)
		{
			Hit tempHit = IsCollindingCircleSegment(boss->Circle, room->Segment[i]);
			if (tempHit.IsCollided)
			{
				boss->Direction = (sfVector2f) { -boss->Direction.x, -boss->Direction.y };
				boss->RigidBody.Velocity.x = boss->Direction.x * boss->Speed;
				boss->RigidBody.Velocity.y = boss->Direction.y * boss->Speed;
				boss->TimerMovement = 1.5f + (float)rand() / RAND_MAX * 2.f;
				UpdateRigidBody(&boss->RigidBody, 0.05f);
			}
		}
	}
}

void CheckColisionShotRoom(Shot **shot, const RoomGameData *room)
{
	if (*shot == NULL || room->IsClear)
	{
		return;
	}
	for (int i = 0; i < 4; i++)
	{
		Hit tempHit = IsCollindingCircleSegment((*shot)->Circle, room->Segment[i]);
		if (tempHit.IsCollided)
		{
			*shot = RemoveShot(*shot);
			if (*shot == NULL)
			{
				return;
			}
		}
	}
}

void CheckColisionPlayerShotBoss(Shot **shot, Boss* boss, Player* player)
{
	if (*shot == NULL || boss->IsDead || !boss->IsSpawned)
	{
		return;
	}
	Hit tempHit = IsCollidingCircleCircle(boss->Circle, (*shot)->Circle);
	if (tempHit.IsCollided)
	{
		DamageBoss(boss, (*shot)->Damage, &player->Stats.XP);
		*shot = RemoveShot(*shot);
		if (*shot == NULL)
		{
			return;
		}
	}
}

void CheckColisionPlayerShots(Player *player, Enemy* enemy, const RoomGameData *room, Boss *boss, const sfBool firstTimeColliding)
{
	if (player->Weapons.FirstShot == NULL)
	{
		return;
	}
	Shot* tempShot = player->Weapons.FirstShot;
	sfBool canRepeatShot = (tempShot != NULL);
	while (canRepeatShot)
	{
		CheckColisionPlayerShotEnemies(&tempShot, enemy, player);
		if (firstTimeColliding)
		{
			CheckColisionPlayerShotBoss(&tempShot, boss, player);
			CheckColisionShotRoom(&tempShot, room);
		}
		if (tempShot == NULL)
		{
			player->Weapons.FirstShot = NULL;
			canRepeatShot = sfFalse;
		}
		else if (tempShot->Previous == NULL)
		{
			player->Weapons.FirstShot = tempShot;
		}
		if (tempShot != NULL && tempShot->Next != NULL)
		{
			tempShot = tempShot->Next;
		}
		else
		{
			canRepeatShot = sfFalse;
		}
	}
}

void CheckColisionPlayerShotEnemies(Shot **shot, Enemy* enemy, Player* player)
{
	if (*shot == NULL || enemy == NULL)
	{
		return;
	}
	if (enemy->StateAnim == move)
	{
		Hit tempHit = IsCollidingCircleCircle(enemy->Circle, (*shot)->Circle);
		if (tempHit.IsCollided)
		{
			if (enemy->Id != miniBoss)
			{
				//enemy->IsKnockBacked = sfTrue;
				//enemy->TimerKnock = 0.f;
				/*enemy->RigidBody.Position.x += tempHit.Normal.x * 5.f;
				enemy->RigidBody.Position.y += tempHit.Normal.y * 5.f;*/
				enemy->RigidBody.KnockBackForce.x += tempHit.Normal.x * 150;
				enemy->RigidBody.KnockBackForce.y += tempHit.Normal.y * 150;
				enemy->Circle.center = enemy->RigidBody.Position;
				enemy->RigidBody.Velocity = (sfVector2f) { 0.f, 0.f };
			}
			DamageEnemy(enemy, (*shot)->Damage);
			if (enemy->StateAnim == deathAnim)
			{
				AddXPToPlayer(player, enemy->XPGives);
			}
			*shot = RemoveShot(*shot);
		}
	}
}

void CheckCollisionEnemyShot(Shot** firstShot, Player player[], const int nbrOfPlayer, const RoomGameData *room)
{
	if (*firstShot == NULL)
	{
		return;
	}
	Shot* tempShot = *firstShot;
	sfBool canRepeatShot = (tempShot != NULL);
	while (canRepeatShot)
	{
		for (int i = 0; i < nbrOfPlayer; i++)
		{
			CheckCollisionEnemyShotPlayer(&player[i], &tempShot);
		}
		//CheckColisionShotRoom(&tempShot, room);
		if (tempShot == NULL)
		{
			*firstShot = NULL;
			canRepeatShot = sfFalse;
		}
		else if (tempShot->Previous == NULL)
		{
			*firstShot = tempShot;
		}
		if (tempShot != NULL && tempShot->Next != NULL)
		{
			tempShot = tempShot->Next;
		}
		else
		{
			canRepeatShot = sfFalse;
		}
	}
}

sfBool CheckCollisionShotTrap(Shot* shot)
{
	TrapData* trapData = GetPointerToTrapData();
	for (int i = 0; i < (int)trapData->NbrTrap; i++)
	{
		Hit tempHit = IsCollidingCircleCircle(shot->Circle, trapData->Traps[i].Collider);
		if ((trapData->Traps[i].Id == rockSpike || trapData->Traps[i].Id == rockSpikeBoss) && trapData->Traps[i].KeyAnimation.actualFrameToShow >= 11 && tempHit.IsCollided)
		{
			shot->RigidBody.Position.x = tempHit.Point.x + tempHit.Normal.x * (shot->Circle.radius + 5.f);
			shot->RigidBody.Position.y = tempHit.Point.y + tempHit.Normal.y * (shot->Circle.radius + 5.f);
			shot->Circle.center = shot->RigidBody.Position;
			return sfTrue;
		}
	}
	return sfFalse;
}

void CheckCollisionEnemyShotPlayer(Player *player, Shot** shot)
{
	if (*shot == NULL || player->States.IsDead)
	{
		return;
	}
	Hit tempHit = IsCollidingCircleCircle(player->Physics.Collider, (*shot)->Circle);
	if (tempHit.IsCollided)
	{
		if (!player->States.IsDashing)
		{
			DamagePlayer(player, (*shot)->Damage);
			if ((*shot)->IdShot == enemiesShot || (*shot)->IdShot == dirtElemShot)
			{
				SlowPlayer(player, 50.f);
			}
			if ((*shot)->IdShot == waterElemShot)
			{
				for (int i = 0; i < 3; i++)
				{
					if (player->Spells.spell[i].isAvailable || player->Spells.spell[i].timerCooldown > player->Spells.spell[i].cooldown - 5.f)
					{
						player->Spells.spell[i].isAvailable = sfFalse;
						player->Spells.spell[i].timerCooldown = player->Spells.spell[i].cooldown - 5.f;
						UpdatePlayerCDHUD(*player, player->ID);
					}
				}

				player->States.IsWet = sfTrue;
			}
			if ((*shot)->IdShot != razarBlockShot)
			{
				*shot = RemoveShot(*shot);
			}
		}
	}
}

void CheckCollisionEnemies(Enemy **firstEnemy, Player player[], const int nbrOfPlayer, const RoomGameData *room, Boss *boss)
{
	sfBool firsTimeCheckColliding = sfTrue;
	if (*firstEnemy == NULL)
	{
		for (int i = 0; i < nbrOfPlayer; i++)
		{
			CheckColisionPlayerShots(&player[i], NULL, room, boss, firsTimeCheckColliding);
		}
	}
	CheckCollisionBossRoom(boss, room);
	Enemy* tempEnemy = *firstEnemy;
	sfBool canRepeatEnemy = (tempEnemy != NULL);
	while (canRepeatEnemy)
	{
		Enemy* tempEnemyTwo = *firstEnemy;
		sfBool canRepeatEnemyTwo = (tempEnemyTwo != NULL);
		if (tempEnemy->StateAnim == move)
		{
			for (int i = 0; i < nbrOfPlayer; i++)
			{
				if (!player[i].States.IsDead)
				{
					CheckCollisionPlayerEnemy(&player[i], tempEnemy);
					if (player[i].Spells.ConeState.MustDamage == sfTrue)
					{
						CheckCollisionPlayerConeOfFireEnemy(&player[i], tempEnemy);
					}
				}
				CheckColisionPlayerShots(&player[i], tempEnemy, room, boss, firsTimeCheckColliding);
				CheckCollisionMineEnemy(&player[i].Spells.MineState, tempEnemy, player);
			}
			firsTimeCheckColliding = sfFalse;
			while (canRepeatEnemyTwo)
			{
				if (tempEnemyTwo->StateAnim == move)
				{
					CheckCollisionEnemyEnemy(tempEnemy, tempEnemyTwo);
				}
				if (tempEnemyTwo != NULL && tempEnemyTwo->Next != NULL)
				{
					tempEnemyTwo = tempEnemyTwo->Next;
				}
				else
				{
					canRepeatEnemyTwo = sfFalse;
				}
			}

		}
		if (tempEnemy != NULL && tempEnemy->Next != NULL)
		{
			tempEnemy = tempEnemy->Next;
		}
		else
		{
			canRepeatEnemy = sfFalse;
		}
	}

	tempEnemy = *firstEnemy;
	canRepeatEnemy = (tempEnemy != NULL);

	while (canRepeatEnemy)
	{
		CheckCollisionEnemyRoom(tempEnemy, room);
		if (tempEnemy->ActualNode != NULL && !tempEnemy->ActualNode->isWalkableForMob)
		{
			tempEnemy->ActualNode = SearchWalkableNodeForMobAround(tempEnemy->ActualNode);
			tempEnemy->RigidBody.Position.x = tempEnemy->ActualNode->posX * 64.f;
			tempEnemy->RigidBody.Position.y = tempEnemy->ActualNode->posY * 64.f;
			tempEnemy->Circle.center = tempEnemy->RigidBody.Position;
			tempEnemy->timer.PathFinding = 0.f;
		}
		if (tempEnemy != NULL && tempEnemy->Next != NULL)
		{
			tempEnemy = tempEnemy->Next;
		}
		else
		{
			canRepeatEnemy = sfFalse;
		}
	}
	for (int i = 0; i < nbrOfPlayer; i++)
	{
		if (!player[i].States.IsDead && player[i].Spells.ConeState.MustDamage)
		{
			player[i].Spells.ConeState.MustDamage = sfFalse;
		}
	}
}

void CheckCollisionMineEnemy(PlayerMineStat* mineStat, Enemy *enemy, Player* player)
{
	if (mineStat->IsActivate)
	{
		if (!mineStat->Explodes && Distance(enemy->Circle.center, mineStat->Collider.center) <= mineStat->Collider.radius + enemy->Circle.radius / 2.f && mineStat->AnimKey.actualFrameToShow >= 5)
		{
			mineStat->Explodes = sfTrue;
		}
		else if (mineStat->Explodes && mineStat->ExplosionKey.actualFrameToShow == mineStat->ExplosionKey.numberOfFrame - 2 &&
			Distance(enemy->Circle.center, mineStat->Collider.center) <= enemy->Circle.radius + mineStat->RadiusExplosion)
		{
			Hit tempHit = IsCollidingCircleCircle(enemy->Circle, mineStat->Collider);
			float dot = Dot(enemy->RigidBody.Velocity, tempHit.Normal);
			enemy->RigidBody.Velocity.x -= tempHit.Normal.x * dot * 2.0f;
			enemy->RigidBody.Velocity.y -= tempHit.Normal.y * dot * 2.0f;

			enemy->RigidBody.Position.x = tempHit.Point.x + tempHit.Normal.x * enemy->Circle.radius;
			enemy->RigidBody.Position.y = tempHit.Point.y + tempHit.Normal.y * enemy->Circle.radius;
			enemy->Circle.center = enemy->RigidBody.Position;
			DamageEnemy(enemy, mineStat->Damage);
			if (enemy->StateAnim == deathAnim)
			{
				AddXPToPlayer(player, enemy->XPGives);
			}
			if (enemy->Id == kamikaze || enemy->Id == kamikazeUpgrade)
			{
				enemy->StateAnim = death;
				PlaySounds(explosion);
			}
		}
	}
}

void CheckCollisionPlayerEnemy(Player *player, Enemy *enemy)
{
	Hit tempHit = IsCollidingCircleCircle(enemy->Circle, player->Physics.Collider);
	if (tempHit.IsCollided)
	{
		//to change cause it sucks
		float dot = Dot(enemy->RigidBody.Velocity, tempHit.Normal);
		enemy->RigidBody.Velocity.x -= tempHit.Normal.x * dot * 2.0f;
		enemy->RigidBody.Velocity.y -= tempHit.Normal.y * dot * 2.0f;

		enemy->RigidBody.Position.x = tempHit.Point.x + tempHit.Normal.x * enemy->Circle.radius;
		enemy->RigidBody.Position.y = tempHit.Point.y + tempHit.Normal.y * enemy->Circle.radius;
		enemy->Circle.center = enemy->RigidBody.Position;
		DamagePlayer(player, enemy->Damage / 2);
	}
}

void CheckCollisionPlayerRoom(Player *player, const RoomGameData *room)
{
	for (int i = 0; i < 4; i++)
	{
		Hit tempHit = IsCollindingCircleSegment(player->Physics.Collider, room->Segment[i]);
		if (tempHit.IsCollided)
		{
			player->Physics.Rb.Position.x = tempHit.Point.x + tempHit.Normal.x * player->Physics.Collider.radius;
			player->Physics.Rb.Position.y = tempHit.Point.y + tempHit.Normal.y * player->Physics.Collider.radius;
			player->Physics.Collider.center = player->Physics.Rb.Position;
		}
	}
}

void CheckCollisionEnemyEnemy(Enemy *firstEnemy, const Enemy* secondEnemy)
{
	if (firstEnemy == secondEnemy)
	{
		return;
	}
	DungeonId temp = GetActualDungeonId();
	if (temp == slimeDungeon || temp == beginningDungeon)
	{
		if (secondEnemy->Id == miniBoss || secondEnemy->Speed >= firstEnemy->Speed)
		{
			Hit tempHit = IsCollidingCircleCircle(firstEnemy->Circle, secondEnemy->Circle);
			if (tempHit.IsCollided)
			{
				//float dot = Dot(firstEnemy->RigidBody.Velocity, tempHit.Normal);
				//firstEnemy->RigidBody.Velocity.x -= tempHit.Normal.x * dot * 2.0f;
				//firstEnemy->RigidBody.Velocity.y -= tempHit.Normal.y * dot * 2.0f;

				firstEnemy->RigidBody.Position.x = tempHit.Point.x + tempHit.Normal.x * firstEnemy->Circle.radius;
				firstEnemy->RigidBody.Position.y = tempHit.Point.y + tempHit.Normal.y * firstEnemy->Circle.radius;
				firstEnemy->Circle.center = firstEnemy->RigidBody.Position;
			}
		}
	}
	else if (temp == elemDungeon)
	{
		if (secondEnemy->Speed >= firstEnemy->Speed && secondEnemy->Id != littleElemOfLava)
		{
			Hit tempHit = IsCollidingCircleCircle(firstEnemy->Circle, secondEnemy->Circle);
			if (tempHit.IsCollided)
			{
				//float dot = Dot(firstEnemy->RigidBody.Velocity, tempHit.Normal);
				//firstEnemy->RigidBody.Velocity.x -= tempHit.Normal.x * dot * 2.0f;
				//firstEnemy->RigidBody.Velocity.y -= tempHit.Normal.y * dot * 2.0f;

				firstEnemy->RigidBody.Position.x = tempHit.Point.x + tempHit.Normal.x * firstEnemy->Circle.radius;
				firstEnemy->RigidBody.Position.y = tempHit.Point.y + tempHit.Normal.y * firstEnemy->Circle.radius;
				firstEnemy->Circle.center = firstEnemy->RigidBody.Position;
			}
		}
	}

}

void CheckCollisionEnemyRoom(Enemy *enemy, const RoomGameData *room)
{
	for (int i = 0; i < 4; i++)
	{
		Hit tempHit = IsCollindingCircleSegment(enemy->Circle, room->Segment[i]);
		if (tempHit.IsCollided)
		{
			enemy->RigidBody.Position.x = tempHit.Point.x + tempHit.Normal.x * enemy->Circle.radius;
			enemy->RigidBody.Position.y = tempHit.Point.y + tempHit.Normal.y * enemy->Circle.radius;
			enemy->Circle.center = enemy->RigidBody.Position;
			if (enemy->Id == miniBoss)
			{
				float tempAngle = (float)rand() / RAND_MAX * 360.f;
				enemy->Direction = (sfVector2f) { cosf(tempAngle), sinf(tempAngle) };
				enemy->RigidBody.Velocity.x = enemy->Direction.x * enemy->Speed;
				enemy->RigidBody.Velocity.y = enemy->Direction.y * enemy->Speed;
				enemy->timer.Movement = 1.f + (float)rand() / RAND_MAX * 2.f;
			}
		}
	}
}

void CheckCollisionPlayerConeOfFireEnemy(Player *player, Enemy *enemy)
{
	float distance = Distance(player->Physics.Rb.Position, enemy->RigidBody.Position);
	sfVector2f direction = { cosf(player->Spells.ConeState.Rotate), sinf(player->Spells.ConeState.Rotate) };
	if (distance <= 240 + enemy->Circle.radius && CheckAngle(player->Physics.Rb.Position, NearestPointOfVector(enemy->Circle, player->Physics.Rb.Position, direction), player->Spells.ConeState.Rotate, 30))
	{
		DamageEnemy(enemy, (float)player->Spells.spell[ConeOfFire].damage * player->Spells.spell[ConeOfFire].fireRate * player->Stats.DamageMultiplier);
		if (enemy->StateAnim == deathAnim)
		{
			AddXPToPlayer(player, enemy->XPGives);
		}
	}

}

void CheckCollisionPlayerConeOfFireBoss(Player *player, Boss *boss)
{
	float distance = Distance(player->Physics.Rb.Position, boss->RigidBody.Position);
	sfVector2f direction = { cosf(player->Spells.ConeState.Rotate), sinf(player->Spells.ConeState.Rotate) };
	if (distance <= 240 + boss->Circle.radius && CheckAngle(player->Physics.Rb.Position, NearestPointOfVector(boss->Circle, player->Physics.Rb.Position, direction), player->Spells.ConeState.Rotate, 30))
	{
		DamageBoss(boss, (int)(player->Spells.spell[ConeOfFire].damage * player->Spells.spell[ConeOfFire].fireRate * player->Stats.DamageMultiplier), &player->Stats.XP);
		if (boss->IsDead)
		{
			AddXPToPlayer(player, boss->XpGive);
		}
	}
}

void CheckCollisionPlayerLoots(Player* player, LootData* lootdata)
{
	for (int i = 0; i < lootdata->NbrLoot; i++)
	{
		Hit tempHit = IsCollidingCircleCircle(lootdata->Loot[i].Colidier, player->Physics.Collider);
		if (tempHit.IsCollided)
		{
			LootTransfer temp;
			temp.Id = lootdata->Loot[i].Id;
			temp.Nbr = lootdata->Loot[i].Nbr;
			RemoveLoot(lootdata, i);
			AddLootToPlayer(player, temp);
			UpdatePlayerHUD(*player, player->ID, 0);
		}
	}
}

void CheckCollisionPlayerTrap(Player *player)
{
	TrapData* trapData = GetPointerToTrapData();
	for (int i = 0; i < (int)trapData->NbrTrap; i++)
	{
		if (trapData->Traps[i].MustDamage)
		{
			Hit tempHit = IsCollidingCircleCircle(player->Physics.Collider, trapData->Traps[i].Collider);
			if (tempHit.IsCollided)
			{
				if (trapData->Traps[i].CanSlow)
				{
					SlowPlayer(player, trapData->Traps[i].PercentageSpeed);
				}
				if (trapData->Traps[i].Damage > 0)
				{
					DamagePlayer(player, trapData->Traps[i].Damage);
				}
				if (trapData->Traps[i].Id == flask)
				{

				}
				else if (trapData->Traps[i].Id == rockSpike || trapData->Traps[i].Id == rockSpikeBoss)
				{
					/*	player->Physics.Rb.Position.x = tempHit.Point.x + tempHit.Normal.x * (player->Physics.Collider.radius + 15.f);
						player->Physics.Rb.Position.y = tempHit.Point.y + tempHit.Normal.y * (player->Physics.Collider.radius + 15.f);
						player->Physics.Collider.center = player->Physics.Rb.Position;*/
					player->Physics.Rb.KnockBackForce.x += tempHit.Normal.x * 150;
					player->Physics.Rb.KnockBackForce.y += tempHit.Normal.y * 150;
				}
			}
		}
		else if ((trapData->Traps[i].Id == rockSpike || trapData->Traps[i].Id == rockSpikeBoss) && trapData->Traps[i].KeyAnimation.actualFrameToShow >= 11)
		{
			Hit tempHit = IsCollidingCircleCircle(player->Physics.Collider, trapData->Traps[i].Collider);
			if (tempHit.IsCollided)
			{
				/*player->Physics.Rb.Position.x = tempHit.Point.x + tempHit.Normal.x * (player->Physics.Collider.radius);
				player->Physics.Rb.Position.y = tempHit.Point.y + tempHit.Normal.y * (player->Physics.Collider.radius);
				player->Physics.Collider.center = player->Physics.Rb.Position;*/
				player->Physics.Rb.KnockBackForce.x += tempHit.Normal.x * 150;
				player->Physics.Rb.KnockBackForce.y += tempHit.Normal.y * 150;
			}
		}
	}
}