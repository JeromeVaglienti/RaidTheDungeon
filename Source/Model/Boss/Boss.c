#include <Windows.h>
#include "Boss.h"
#include "..\..\Controller\GraphicsUtilities\Animation\Animation.h"
#include "..\Trap\Trap.h"
#include "..\Math\Vector\VectorUtilities.h"
#include "..\..\Controller\Sounds\Sounds.h"

// radius, speed, xp give, life, damage

StatBoss statBoss[3] = { {150.f, 0.f, 300, 1500, 5}, { 178.5f, 0.f, 1000, 5000, 20 } ,{ 105.f, 200.f, 5000, 10000, 20 } };

//init all variable of boos at 0
void InitBoss(Boss* boss)
{
	Boss tempBoss = { 0 };
	*boss = tempBoss;
}

void SpawnBoss(Boss* boss, const int id, const sfVector2f pos, const int nbrPlayer)
{
	boss->Id = id;
	InitializeRigidBody(&boss->RigidBody);
	boss->RigidBody.Position = pos;
	boss->Circle.radius = statBoss[id].Radius;
	boss->Damage = statBoss[id].Damage;
	boss->LifeMax = statBoss[id].LifeMax;
	boss->XpGive = statBoss[id].XpGive;
	boss->IsHit = sfFalse;
	boss->HitTimer = 0.0f;
	if (nbrPlayer == 2)
	{
		boss->LifeMax = (short)(boss->LifeMax * 2.5f);
	}
	boss->Life = boss->LifeMax;
	boss->IsSpawned = sfTrue;
	boss->IsDead = sfFalse;
	boss->HasTrashesAlive = sfFalse;
	boss->TrashesNeedToSpawn = sfFalse;
	boss->IsAttacking = sfFalse;
	if (id == elem)
	{
		boss->CanBeEngaged = sfFalse;
		boss->CanSpawn = sfFalse;
		boss->Circle.center.x = pos.x;
		boss->Circle.center.y = pos.y - 210.f;
	}
	else
	{
		boss->CanBeEngaged = sfTrue;
		boss->CanSpawn = sfTrue;
		boss->Circle.center = pos;
	}
	boss->Speed = statBoss[id].Speed;
	boss->timerSin = 2.f;
	boss->PaternChoosen = 0;
	boss->RealPhase = spawnPhase;
	boss->CurrentPhase = spawnPhase;
	for (int i = 0; i < 5; i++)
	{
		boss->CooldownShot[i] = 3.f + 2.f * i;
	}
	boss->IsMoving = sfFalse;
#if _DEBUG
	if (boss->Shape == NULL)
	{
		boss->Shape = sfCircleShape_create();
		sfCircleShape_setOutlineThickness(boss->Shape, 1.f);
		sfCircleShape_setOutlineColor(boss->Shape, sfGreen);
		sfCircleShape_setFillColor(boss->Shape, sfTransparent);
		sfCircleShape_setRadius(boss->Shape, boss->Circle.radius);
		sfCircleShape_setOrigin(boss->Shape, (sfVector2f) { boss->Circle.radius, boss->Circle.radius });
	}
#endif
}

void UpdateBoss(Boss* boss, const Player player[], const short nbrOfPlayer, const float dt, const sfVector2f posRoom, const sfVector2f sizeRoom)
{
	if (boss->CanBeEngaged)
	{
		PaternBoss(boss, player, nbrOfPlayer, dt, posRoom, sizeRoom);

		if (boss->Id == razar)
		{
			if (boss->CurrentPhase == 0)
			{
				boss->Circle.center = boss->RigidBody.Position;
				boss->Circle.center.y -= 20.f;
				boss->Circle.center.x += 20.f;
			}
			else if (boss->CurrentPhase == 1)
			{
				if (boss->IsMoving)
				{
					boss->RigidBody.Position.y -= 450.f * dt;
				}
				boss->Circle.center = boss->RigidBody.Position;
				boss->Circle.center.y += 75.f;
			}
			else if (boss->CurrentPhase == 2)
			{
				boss->Circle.center = boss->RigidBody.Position;
				boss->Circle.center.y += 75.f;
			}
		}
		else if (boss->Id == slime)
		{
			boss->Circle.center = boss->RigidBody.Position;
		}

		if (boss->IsHit)
		{
			boss->HitTimer += dt;
			if (boss->HitTimer > 0.1f)
			{
				boss->HitTimer = 0.f;
				boss->IsHit = sfFalse;
			}
		}
	}
}

// assign the good patern to the boss
void PaternBoss(Boss* boss, const Player player[], const short nbrOfPlayer, const float dt, const sfVector2f posRoom, const sfVector2f sizeRoom)
{
	float percentageLife = (float)boss->Life / (float)boss->LifeMax * 100.f;
	short targetPlayer = 0;
	float distanceTargetPlayer = 0;;
	if (nbrOfPlayer > 1)
	{
		targetPlayer = rand() % 2;
		if (player[0].States.IsDead)
		{
			targetPlayer = 1;
		}
		if (player[1].States.IsDead)
		{
			targetPlayer = 0;
		}
	}

	distanceTargetPlayer = Distance(player[targetPlayer].Physics.Rb.Position, boss->RigidBody.Position);
	if (boss->Id == slime)
	{
		for (int i = 0; i < 5; i++)
		{
			boss->CooldownShot[i] -= dt;
		}
		boss->timerSin -= dt;

		// check what is the actual boss phase and apply the corresponding shot patern
		// phase 1
		if (percentageLife > 80)
		{
			boss->CurrentPhase = spawnPhase;
			if (boss->CooldownShot[0] <= 0)
			{
				sfVector2f dirShot;
				dirShot = Normalize(AddTwoVectors(boss->RigidBody.Position, player[targetPlayer].Physics.Rb.Position));
				sfVector2f tempDir = dirShot;
				float tempAngle = 10;
				float rotateAngle = DegreeToRadian(tempAngle);
				AddShot(&boss->FirstShot, boss->RigidBody.Position, dirShot, bossShot);
				tempDir = Rotate(dirShot, -rotateAngle);
				AddShot(&boss->FirstShot, boss->RigidBody.Position, tempDir, bossShot);
				tempDir = Rotate(dirShot, rotateAngle);
				AddShot(&boss->FirstShot, boss->RigidBody.Position, tempDir, bossShot);
				tempDir = Rotate(dirShot, -rotateAngle * 2.f);
				AddShot(&boss->FirstShot, boss->RigidBody.Position, tempDir, bossShot);
				tempDir = Rotate(dirShot, rotateAngle * 2.f);
				AddShot(&boss->FirstShot, boss->RigidBody.Position, tempDir, bossShot);
				boss->CooldownShot[0] = ((float)rand() / RAND_MAX) * 0.5f + 0.5f;
			}
		}
		// phase 2
		else if (percentageLife > 60)
		{
			boss->CurrentPhase = p1;
			if (boss->CooldownShot[0] <= 0)
			{
				float tempAngle = ((float)rand() / RAND_MAX) * 360.f;
				sfVector2f tempDir = { cosf(tempAngle), sinf(tempAngle) };
				AddShot(&boss->FirstShot, boss->RigidBody.Position, tempDir, bossShot);
				boss->CooldownShot[0] = 0.05f;
			}
		}
		// phase 3
		else if (percentageLife > 40)
		{
			if (boss->CooldownShot[0] <= 0)
			{
				float tempAngle = 0.f + sinf(boss->timerSin) * DegreeToRadian(60.f);
				float rotateAngle = DegreeToRadian(90.f);
				sfVector2f tempDir = { cosf(tempAngle), sinf(tempAngle) };
				AddShot(&boss->FirstShot, boss->RigidBody.Position, tempDir, bossShot);
				tempDir = Rotate(tempDir, rotateAngle);
				AddShot(&boss->FirstShot, boss->RigidBody.Position, tempDir, bossShot);
				tempDir = Rotate(tempDir, rotateAngle);
				AddShot(&boss->FirstShot, boss->RigidBody.Position, tempDir, bossShot);
				tempDir = Rotate(tempDir, rotateAngle);
				AddShot(&boss->FirstShot, boss->RigidBody.Position, tempDir, bossShot);
				boss->CooldownShot[0] = 0.02f;
			}
		}
		// phase 4
		else if (percentageLife > 10)
		{
			// first patern shot
			if (boss->CooldownShot[0] <= 0)
			{
				float tempAngle = DegreeToRadian(40.f) + sinf(boss->timerSin / 2) * DegreeToRadian(60.f);
				sfVector2f tempDir = { cosf(tempAngle), sinf(tempAngle) };
				float rotateAngle = DegreeToRadian(90.f);
				AddShot(&boss->FirstShot, boss->RigidBody.Position, tempDir, bossShot);
				tempDir = Rotate(tempDir, rotateAngle);
				AddShot(&boss->FirstShot, boss->RigidBody.Position, tempDir, bossShot);
				tempDir = Rotate(tempDir, rotateAngle);
				AddShot(&boss->FirstShot, boss->RigidBody.Position, tempDir, bossShot);
				tempDir = Rotate(tempDir, rotateAngle);
				AddShot(&boss->FirstShot, boss->RigidBody.Position, tempDir, bossShot);
				tempDir = Rotate(tempDir, rotateAngle);
				AddShot(&boss->FirstShot, boss->RigidBody.Position, tempDir, bossShot);
				tempDir = Rotate(tempDir, rotateAngle);
				AddShot(&boss->FirstShot, boss->RigidBody.Position, tempDir, bossShot);
				tempDir = Rotate(tempDir, rotateAngle);
				AddShot(&boss->FirstShot, boss->RigidBody.Position, tempDir, bossShot);
				tempDir = Rotate(tempDir, rotateAngle);
				AddShot(&boss->FirstShot, boss->RigidBody.Position, tempDir, bossShot);
				boss->CooldownShot[0] = 0.02f;
			}
			// second patern shot
			if (boss->CooldownShot[1] <= 0)
			{
				for (int i = 0; i < 360; i++)
				{
					sfVector2f tempDir = { cosf(DegreeToRadian((float)i)), sinf(DegreeToRadian((float)i)) };
					AddShot(&boss->FirstShot, boss->RigidBody.Position, tempDir, bossShot);
				}
				boss->CooldownShot[1] = ((float)rand() / RAND_MAX) * 2.f + 3;
			}
		}
		// phase 5
		else if (percentageLife > 0)
		{
			// first patern shot
			if (boss->CooldownShot[2] <= 0)
			{
				sfVector2f dirShot;
				dirShot = Normalize(AddTwoVectors(boss->RigidBody.Position, player[targetPlayer].Physics.Rb.Position));
				sfVector2f tempDir = dirShot;
				float tempAngle = 10;
				float rotateAngle = DegreeToRadian(tempAngle);
				AddShot(&boss->FirstShot, boss->RigidBody.Position, dirShot, bossShot);
				tempDir = Rotate(dirShot, -rotateAngle);
				AddShot(&boss->FirstShot, boss->RigidBody.Position, tempDir, bossShot);
				tempDir = Rotate(dirShot, rotateAngle);
				AddShot(&boss->FirstShot, boss->RigidBody.Position, tempDir, bossShot);
				tempDir = Rotate(dirShot, -rotateAngle * 2.f);
				AddShot(&boss->FirstShot, boss->RigidBody.Position, tempDir, bossShot);
				tempDir = Rotate(dirShot, rotateAngle * 2.f);
				AddShot(&boss->FirstShot, boss->RigidBody.Position, tempDir, bossShot);
				boss->CooldownShot[2] = ((float)rand() / RAND_MAX) * 0.5f + 0.5f;
			}

			// second patern shot
			if (boss->CooldownShot[0] <= 0)
			{
				float tempAngle = DegreeToRadian(40.f) + sinf(boss->timerSin / 2) * DegreeToRadian(60.f);
				sfVector2f tempDir = { cosf(tempAngle), sinf(tempAngle) };
				float rotateAngle = DegreeToRadian(90.f);
				AddShot(&boss->FirstShot, boss->RigidBody.Position, tempDir, bossShot);
				tempDir = Rotate(tempDir, rotateAngle);
				AddShot(&boss->FirstShot, boss->RigidBody.Position, tempDir, bossShot);
				tempDir = Rotate(tempDir, rotateAngle);
				AddShot(&boss->FirstShot, boss->RigidBody.Position, tempDir, bossShot);
				tempDir = Rotate(tempDir, rotateAngle);
				AddShot(&boss->FirstShot, boss->RigidBody.Position, tempDir, bossShot);
				tempDir = Rotate(tempDir, rotateAngle);
				AddShot(&boss->FirstShot, boss->RigidBody.Position, tempDir, bossShot);
				tempDir = Rotate(tempDir, rotateAngle);
				AddShot(&boss->FirstShot, boss->RigidBody.Position, tempDir, bossShot);
				tempDir = Rotate(tempDir, rotateAngle);
				AddShot(&boss->FirstShot, boss->RigidBody.Position, tempDir, bossShot);
				tempDir = Rotate(tempDir, rotateAngle);
				AddShot(&boss->FirstShot, boss->RigidBody.Position, tempDir, bossShot);
			}

			// third patern shot
			if (boss->CooldownShot[1] <= 0)
			{
				for (int i = 0; i < 360; i++)
				{
					sfVector2f tempDir = { cosf(DegreeToRadian((float)i)), sinf(DegreeToRadian((float)i)) };
					AddShot(&boss->FirstShot, boss->RigidBody.Position, tempDir, bossShot);
				}
				boss->CooldownShot[1] = ((float)rand() / RAND_MAX) * 2.f + 3;
			}

			// fourth patern shot
			if (boss->CooldownShot[3] <= 0)
			{
				float tempAngle = ((float)rand() / RAND_MAX) * 360.f;
				sfVector2f tempDir = { cosf(tempAngle), sinf(tempAngle) };
				AddShot(&boss->FirstShot, boss->RigidBody.Position, tempDir, bossShot);
				boss->CooldownShot[3] = 0.05f;
			}
		}
		else
		{
			boss->CurrentPhase = p3;
		}
	}
	else if (boss->Id == elem)
	{
		for (int i = 0; i < 5; i++)
		{
			boss->CooldownShot[i] -= dt;
		}
		//first phase
		if (percentageLife > 75)
		{
			if (boss->RealPhase < p1)
			{
				boss->RealPhase = p1;
			}
			/*if (boss->TrashesNeedToSpawn)
			{
				boss->HasTrashesAlive = sfTrue;
				boss->TrashesNeedToSpawn = sfFalse;
			}
			else if (!boss->HasTrashesAlive && boss->CooldownShot[3] <= 0)
			{
				boss->TrashesNeedToSpawn = sfTrue;
				boss->CooldownShot[3] = 30.f;
			}*/
			/*if (boss->CooldownShot[3] <= 0.f)
			{
				boss->CooldownShot[3] = 8.f;
			}
			if (boss->CooldownShot[3] > 0.f && boss->CooldownShot[3] < 5.f)
			{
				boss->CurrentPhase = p1;*/
			if (boss->CooldownShot[1] <= 0)
			{
				if (boss->IsAttacking)
				{
					/*for (int i = 0; i < 10; i++)
					{
						sfVector2f tempPos;
						do
						{
							tempPos = (sfVector2f){ player[targetPlayer].Physics.Rb.Position.x + 200.f * cosf(DegreeToRadian((float)(rand() % 360))), player[targetPlayer].Physics.Rb.Position.y + 200.f * sinf(DegreeToRadian((float)(rand() % 360))) };
							tempPos.x += (float)(rand() % (int)(sizeRoom.x - 200.f) - (sizeRoom.x / 2.f));
							tempPos.y += (float)(rand() % (int)(sizeRoom.y / 2.f) - 150.f);
						} while (tempPos.y < posRoom.y + 100.f && tempPos.y > (posRoom.y + sizeRoom.y) - 300.f);
						AddTrap(tempPos, (sfVector2f) { 0, 0 }, rockSpike, 100.f, sfTrue, 50.f, 2.f, 10, 0);
					}*/ //TRUC NUL
					if (boss->PaternChoosen == 0 && boss->IsAttacking)
					{
						sfVector2f tempPos = player[targetPlayer].Physics.Rb.Position;
						tempPos.y -= 400;
						for (int i = 0; i < 5; i++)
						{
							if (tempPos.y > posRoom.y + 100.f && tempPos.y < posRoom.y + (sizeRoom.y - 200.f))
							{
								AddTrap(tempPos, (sfVector2f) { 0, 0 }, rockSpikeBoss, 112.5f, sfTrue, 50.f, 2.f, 10, 0);
							}
							tempPos.y += 200;
						}
						tempPos.y = player[targetPlayer].Physics.Rb.Position.y;
						tempPos.x -= 400;
						for (int i = 0; i < 5; i++)
						{
							if (tempPos.x > posRoom.x - (sizeRoom.x + 100.f) && tempPos.x < posRoom.x + (sizeRoom.x + 100.f))
							{
								AddTrap(tempPos, (sfVector2f) { 0, 0 }, rockSpikeBoss, 112.5f, sfTrue, 50.f, 2.f, 10, 0);
							}
							tempPos.x += 200;
						}
						boss->CooldownShot[1] = 1.f;
					}
					else if (boss->PaternChoosen == 1 && boss->IsAttacking)
					{
						sfVector2f tempPos = player[targetPlayer].Physics.Rb.Position;
						AddTrap(tempPos, (sfVector2f) { 0, 0 }, rockSpikeBoss, 112.5f, sfTrue, 50.f, 2.f, 10, 0);
						sfVector2f* directionalArray = GetDirectionalArray();
						for (float angle = 0.f; angle < 360.f; angle += 60.f)
						{
							sfVector2f center = player[targetPlayer].Physics.Rb.Position;
							tempPos.x = center.x + 400.f * directionalArray[(int)angle].x;
							tempPos.y = center.y + 400.f * directionalArray[(int)angle].y;
							if (tempPos.y > posRoom.y + 100.f && tempPos.y < posRoom.y + (sizeRoom.y - 200.f) &&
								tempPos.x > posRoom.x - (sizeRoom.x + 100.f) && tempPos.x < posRoom.x + (sizeRoom.x + 100.f))
							{
								AddTrap(tempPos, (sfVector2f) { 0, 0 }, rockSpikeBoss, 112.5f, sfTrue, 50.f, 2.f, 10, 0);
							}
						}
						boss->CooldownShot[1] = 1.f;
					}
				}
			}
		}
		//second phase
		else if (percentageLife > 50)
		{
			if (boss->RealPhase < p2)
			{
				boss->RealPhase = p2;
				boss->CurrentPhase = p5;
			}
			if (boss->PaternChoosen == 0 && boss->IsAttacking)
			{
				if (boss->CooldownShot[1] <= 0)
				{
					boss->IsAttacking = sfTrue;
					sfVector2f tempPos = player[targetPlayer].Physics.Rb.Position;
					AddTrap(tempPos, (sfVector2f) { 0, 0 }, lightning, 100.f, sfFalse, 0.f, 2.f, 10, 0);
					boss->CooldownShot[1] = 0.4f;
				}
				if (boss->CooldownShot[2] <= 0)
				{
					sfVector2f posPlayerAfterFewSeconds = player[targetPlayer].Physics.Rb.Position;
					posPlayerAfterFewSeconds.x += player[targetPlayer].Physics.Rb.Velocity.x * 0.5f;
					posPlayerAfterFewSeconds.y += player[targetPlayer].Physics.Rb.Velocity.y * 0.5f;
					if (posPlayerAfterFewSeconds.y > posRoom.y + 100.f && posPlayerAfterFewSeconds.y < posRoom.y + (sizeRoom.y - 200.f) &&
						posPlayerAfterFewSeconds.x > posRoom.x - (sizeRoom.x + 100.f) && posPlayerAfterFewSeconds.x < posRoom.x + (sizeRoom.x + 100.f))
					{
						AddTrap(posPlayerAfterFewSeconds, (sfVector2f) { 0, 0 }, rockSpikeBoss, 112.5f, sfTrue, 50.f, 5.f, 10, 0);
					}
					boss->CooldownShot[2] = 1.5f;
				}
			}
			else if (boss->PaternChoosen == 1 && boss->IsAttacking)
			{
				if (boss->CooldownShot[1] <= 0)
				{
					sfVector2f tempPos = player[targetPlayer].Physics.Rb.Position;

					AddTrap(tempPos, (sfVector2f) { 0, 0 }, lightning, 100.f, sfFalse, 0.f, 2.f, 10, 0);
					sfVector2f center = player[targetPlayer].Physics.Rb.Position;
					sfVector2f* directionalArray = GetDirectionalArray();
					for (float angle = 0.f; angle < 360.f; angle += 60.f)
					{
						tempPos.x = center.x + 200.f * directionalArray[(int)angle].x;
						tempPos.y = center.y + 200.f * directionalArray[(int)angle].y;
						if (tempPos.y > posRoom.y + 100.f && tempPos.y < posRoom.y + (sizeRoom.y - 200.f) &&
							tempPos.x > posRoom.x - (sizeRoom.x + 100.f) && tempPos.x < posRoom.x + (sizeRoom.x + 100.f))
						{
							AddTrap(tempPos, (sfVector2f) { 0, 0 }, lightning, 100.f, sfFalse, 0.f, 2.f, 10, 0);
						}
					}
					boss->CooldownShot[1] = 1.f;
				}
			}
		}
		//third phase
		else if (percentageLife > 25)
		{
			if (boss->RealPhase < p3)
			{
				boss->RealPhase = p3;
				boss->CurrentPhase = p5;
			}
			static float angle = -30.f;
			if (boss->IsAttacking)
			{
				if (boss->CooldownShot[0] <= 0)
				{
					sfVector2f dirShot;
					//sfVector2f tempDir;
					sfVector2f posShot = { boss->RigidBody.Position.x - 460.f, boss->RigidBody.Position.y - 60.f };

					// shoot on the player
					dirShot = Normalize(AddTwoVectors(posShot, player[targetPlayer].Physics.Rb.Position));
					dirShot = Rotate(dirShot, DegreeToRadian(angle));
					AddShotWithDistanceMultiplier(&boss->FirstShot, posShot, dirShot, iceBossElemShot, 10.f);
					angle += 15.f;
					boss->CooldownShot[0] = 0.15f;
				}
				if (boss->CooldownShot[1] <= 0)
				{
					for (int i = 0; i < 10; i++)
					{
						sfVector2f tempPos = { posRoom.x + 100.f, posRoom.y + 100.f };
						tempPos.x += (float)(rand() % (int)(sizeRoom.x - 200.f) - (sizeRoom.x / 2.f));
						tempPos.y += (float)(rand() % (int)(sizeRoom.y / 2.f) - 150.f);
						if (tempPos.y > posRoom.y + 100.f && tempPos.y < posRoom.y + (sizeRoom.y - 200.f) &&
							tempPos.x > posRoom.x - (sizeRoom.x + 100.f) && tempPos.x < posRoom.x + (sizeRoom.x + 100.f))
						{
							AddTrap(tempPos, (sfVector2f) { 0, 0 }, lightning, 100.f, sfFalse, 0.f, 2.f, 10, 0);
						}
						boss->CooldownShot[1] = 0.8f;
					}
				}
			}
			else
			{
				angle = -30.f;
			}
		}
		//fourth phase
		else if (percentageLife > 0)
		{
			if (boss->RealPhase < p4)
			{
				boss->RealPhase = p4;
				boss->CurrentPhase = p5;
			}

			if (boss->PaternChoosen == 0 && boss->IsAttacking)
			{
				if (boss->CooldownShot[2] <= 0)
				{
					boss->IsAttacking = sfTrue;
					sfVector2f tempPos = posRoom;
					tempPos.x -= (sizeRoom.x / 2.f) - 100.f;
					tempPos.y += 25.f;
					AddShot(&boss->FirstShot, tempPos, (sfVector2f) { 1, 0 }, fireBallBossElemShot);
					for (int i = 0; i < 9; i++)
					{
						tempPos.y += 150.f;
						AddShot(&boss->FirstShot, tempPos, (sfVector2f) { 1, 0 }, fireBallBossElemShot);
					}
					boss->CooldownShot[2] = 1.5f;
					boss->CooldownShot[1] = 0.25f;
				}

				if (boss->CooldownShot[1] <= 0)
				{
					sfVector2f tempPos = posRoom;
					tempPos.x += (sizeRoom.x / 2.f) - 100.f;
					tempPos.y += 50.f;
					AddShot(&boss->FirstShot, tempPos, (sfVector2f) { -1, 0 }, fireBallBossElemShot);
					for (int i = 0; i < 9; i++)
					{
						tempPos.y += 150.f;
						AddShot(&boss->FirstShot, tempPos, (sfVector2f) { -1, 0 }, fireBallBossElemShot);
					}
					boss->CooldownShot[1] = 4.0f;
				}
			}
			//TOO HARD BOY
			/*else if (boss->PaternChoosen == 1 && boss->IsAttacking)
			{
				if (boss->CooldownShot[3] <= 0)
				{
					boss->IsAttacking = sfTrue;
					for (int i = 0; i < 5; i++)
					{
						sfVector2f center = { posRoom.x + 100.f, posRoom.y + 100.f };
						center.x += (float)(rand() % (int)(sizeRoom.x - 200.f) - (sizeRoom.x / 2.f));
						center.y += (float)(rand() % (int)(sizeRoom.y / 2.f) - 150.f);

						for (float angle = 0.f; angle < 360.f; angle += 60.f)
						{
							sfVector2f tempPos;
							sfVector2f dirShot = { cosf(DegreeToRadian(angle)), sinf(DegreeToRadian(angle)) };
							tempPos.x = center.x + 150.f * dirShot.x;
							tempPos.y = center.y + 150.f * dirShot.y;
							AddShot(&boss->FirstShot, tempPos, dirShot, fireBallBossElemShot);
						}
					}
					boss->CooldownShot[3] = 2.0f;
				}
			}*/
		}
		//last phase
		else
		{
			if (boss->RealPhase < idleBoss)
			{
				boss->RealPhase = idleBoss;
				boss->CurrentPhase = idleBoss;
			}
		}
	}
	else if (boss->Id == razar)
	{
		boss->timerSin -= dt;
		if (percentageLife > 80)
		{
			boss->CurrentPhase = 0;
			for (int i = 0; i < 5; i++)
			{
				boss->CooldownShot[i] -= dt;
			}
			if (boss->CooldownShot[0] <= 0)
			{
				for (int i = 0; i < nbrOfPlayer; i++)
				{
					if (!player[i].States.IsDead)
					{
						sfVector2f dirShot;
						dirShot = Normalize(AddTwoVectors(boss->Circle.center, player[i].Physics.Rb.Position));
						float tempAngle = 10;
						AddShot(&boss->FirstShot, boss->Circle.center, dirShot, razarShot);
					}
				}
				boss->CooldownShot[0] += 1.2f;
				boss->CooldownShot[1] = 0.25f;
			}
			if (boss->CooldownShot[1] <= 0)
			{
				for (int i = 0; i < nbrOfPlayer; i++)
				{
					if (!player[i].States.IsDead)
					{
						sfVector2f dirShot;
						dirShot = Normalize(AddTwoVectors(boss->Circle.center, player[i].Physics.Rb.Position));
						sfVector2f tempDir = dirShot;
						float tempAngle = 10;
						float angleRotate = DegreeToRadian(tempAngle);
						tempDir = Rotate(dirShot, -angleRotate);
						AddShot(&boss->FirstShot, boss->Circle.center, tempDir, razarShot);
						tempDir = Rotate(dirShot, angleRotate);
						AddShot(&boss->FirstShot, boss->Circle.center, tempDir, razarShot);
					}
				}
				boss->CooldownShot[1] = 5.5f;
				boss->CooldownShot[2] = 0.25f;
			}
			if (boss->CooldownShot[2] <= 0)
			{
				for (int i = 0; i < nbrOfPlayer; i++)
				{
					if (!player[i].States.IsDead)
					{
						sfVector2f dirShot;
						dirShot = Normalize(AddTwoVectors(boss->Circle.center, player[i].Physics.Rb.Position));
						sfVector2f tempDir = dirShot;
						float tempAngle = 10;
						float angleRotate = DegreeToRadian(tempAngle);
						tempDir = Rotate(dirShot, angleRotate * 2.f);
						AddShot(&boss->FirstShot, boss->Circle.center, tempDir, razarShot);
						tempDir = Rotate(dirShot, -angleRotate * 2.f);
						AddShot(&boss->FirstShot, boss->Circle.center, tempDir, razarShot);
					}
				}
				boss->CooldownShot[2] = 5.5f;
				boss->CooldownShot[3] = 0.25f;
			}
			if (boss->CooldownShot[3] <= 0)
			{
				for (int i = 0; i < nbrOfPlayer; i++)
				{
					if (!player[i].States.IsDead)
					{
						sfVector2f dirShot;
						dirShot = Normalize(AddTwoVectors(boss->Circle.center, player[i].Physics.Rb.Position));
						sfVector2f tempDir = dirShot;
						float tempAngle = 10;
						float angleRotate = DegreeToRadian(tempAngle);
						tempDir = Rotate(dirShot, angleRotate * 3.f);
						AddShot(&boss->FirstShot, boss->Circle.center, tempDir, razarShot);
						tempDir = Rotate(dirShot, -angleRotate * 3.f);
						AddShot(&boss->FirstShot, boss->Circle.center, tempDir, razarShot);
					}
				}
				boss->CooldownShot[3] = 5.5f;
				boss->CooldownShot[4] = 0.20f;
			}
			if (boss->CooldownShot[4] <= 0)
			{
				for (int i = 0; i < nbrOfPlayer; i++)
				{
					if (!player[i].States.IsDead)
					{
						sfVector2f dirShot;
						dirShot = Normalize(AddTwoVectors(boss->Circle.center, player[i].Physics.Rb.Position));
						sfVector2f tempDir = dirShot;
						float tempAngle = 10;
						float angleRotate = DegreeToRadian(tempAngle);
						tempDir = Rotate(dirShot, angleRotate * 4.f);
						AddShot(&boss->FirstShot, boss->Circle.center, tempDir, razarShot);
						tempDir = Rotate(dirShot, -angleRotate * 4.f);
						AddShot(&boss->FirstShot, boss->Circle.center, tempDir, razarShot);
					}
				}
				boss->CooldownShot[4] = 5.5f;
			}
		}
		else if (percentageLife > 60)
		{
			for (int i = 0; i < 5; i++)
			{
				boss->CooldownShot[i] -= dt;
			}
			if (boss->CooldownShot[0] <= 0)
			{
				float basicAngle = 0.f;
				for (int i = 0; i <= 360; i += (int)(360 / 25))
				{
					sfVector2f dirShot = { cosf(DegreeToRadian(i + basicAngle)), sinf(DegreeToRadian(i + basicAngle)) };
					AddShot(&boss->FirstShot, boss->Circle.center, dirShot, razarShot);
				}
				boss->CooldownShot[0] += 10.6f;
				boss->CooldownShot[1] = 0.6f;
			}
			if (boss->CooldownShot[1] <= 0)
			{
				float basicAngle = 7.2f;
				for (int i = 0; i <= 360; i += (int)(360 / 25))
				{
					sfVector2f dirShot = { cosf(DegreeToRadian(i + basicAngle)), sinf(DegreeToRadian(i + basicAngle)) };
					AddShot(&boss->FirstShot, boss->Circle.center, dirShot, razarShot);
				}
				boss->CooldownShot[1] += 10.2f;
				boss->CooldownShot[0] = 0.6f;
			}
		}
		else if (percentageLife > 50)
		{
			if (boss->CurrentPhase == 0)
			{
				boss->CurrentPhase = 1;
				for (int i = 0; i < 5; i++)
				{
					boss->CooldownShot[i] = 3.f;
				}
				boss->RigidBody.Position.x += 14.f;
				boss->RigidBody.Position.y -= 99.f;
			}
			for (int i = 0; i < 5; i++)
			{
				boss->CooldownShot[i] -= dt;
			}
			sfVector2f dirShot;
			// shoot on the player
			if (boss->CooldownShot[4] <= 0)
			{
				dirShot = Normalize(AddTwoVectors(boss->Circle.center, player[targetPlayer].Physics.Rb.Position));
				for (int i = -15; i < 15; i++)
				{
					sfVector2f tempPos = player[targetPlayer].Physics.Rb.Position;
					tempPos.x += 20.f * i * dirShot.x;
					tempPos.y += 20.f * i * dirShot.y;
					AddRazarWindElemShot(&boss->FirstShot, boss->Circle.center, tempPos, dirShot, -1);
					AddRazarWindElemShot(&boss->FirstShot, boss->Circle.center, tempPos, dirShot, 1);
				}
				boss->CooldownShot[4] = 2.f;
			}
		}
		else if (percentageLife > 30)
		{
			for (int i = 0; i < 5; i++)
			{
				boss->CooldownShot[i] -= dt;
			}
			if (boss->CooldownShot[0] <= 0)
			{
				sfVector2f dirShot;
				dirShot = Normalize(AddTwoVectors(boss->Circle.center, player[targetPlayer].Physics.Rb.Position));
				AddShot(&boss->FirstShot, boss->Circle.center, dirShot, razarBigFragShot);
				boss->CooldownShot[0] = 1.5f;
			}

			sfVector2f dirShot;
			// shoot on the player
			if (boss->CooldownShot[4] <= 0)
			{
				dirShot = Normalize(AddTwoVectors(boss->Circle.center, player[targetPlayer].Physics.Rb.Position));
				for (int i = -15; i < 15; i++)
				{
					sfVector2f tempPos = player[targetPlayer].Physics.Rb.Position;
					tempPos.x += 20.f * i * dirShot.x;
					tempPos.y += 20.f * i * dirShot.y;
					AddRazarWindElemShot(&boss->FirstShot, boss->Circle.center, tempPos, dirShot, -1);
					AddRazarWindElemShot(&boss->FirstShot, boss->Circle.center, tempPos, dirShot, 1);
				}
				boss->CooldownShot[4] = ((float)rand() / RAND_MAX) * 5.f + 5.f;
			}
		}
		else if (percentageLife > 10)
		{
			for (int i = 0; i < 5; i++)
			{
				boss->CooldownShot[i] -= dt;
			}
			sfVector2f dirShot;
			// shoot on the player
			if (boss->CooldownShot[4] <= 0)
			{
				dirShot = Normalize(AddTwoVectors(boss->Circle.center, player[targetPlayer].Physics.Rb.Position));
				for (int i = -15; i < 15; i++)
				{
					sfVector2f tempPos = player[targetPlayer].Physics.Rb.Position;
					tempPos.x += 20.f * i * dirShot.x;
					tempPos.y += 20.f * i * dirShot.y;
					AddRazarWindElemShot(&boss->FirstShot, boss->Circle.center, tempPos, dirShot, -1);
					AddRazarWindElemShot(&boss->FirstShot, boss->Circle.center, tempPos, dirShot, 1);
				}
				boss->CooldownShot[4] = ((float)rand() / RAND_MAX) * 5.f + 5.f;
			}

			if (boss->CooldownShot[0] <= 0)
			{
				sfVector2f dirShot;
				dirShot = Normalize(AddTwoVectors(boss->Circle.center, player[targetPlayer].Physics.Rb.Position));
				AddShot(&boss->FirstShot, boss->Circle.center, dirShot, razarBigFragShot);
				boss->CooldownShot[0] = 5.f;
			}

			if (boss->CooldownShot[1] <= 0)
			{
				sfVector2f dirShot;
				dirShot = Normalize(AddTwoVectors(boss->Circle.center, player[targetPlayer].Physics.Rb.Position));
				float basicAngle = (float)(rand() % (int)(360 / 30));
				for (int i = 0; i <= 360; i += (int)(360 / 30))
				{
					sfVector2f dirShot = { cosf(DegreeToRadian(i + basicAngle)), sinf(DegreeToRadian(i + basicAngle)) };
					AddRazarBoomerangShot(&boss->FirstShot, &boss->Circle.center, dirShot);
				}
				boss->CooldownShot[1] = 3.f;
			}
		}
		else if (percentageLife > 0)
		{
			sfVector2f tempPos = { posRoom.x, posRoom.y - 822.5f };
			if (boss->RigidBody.Position.x != tempPos.x || boss->RigidBody.Position.y != tempPos.y)
			{
				boss->RigidBody.Position.x = tempPos.x;
				boss->RigidBody.Position.y = tempPos.y;
				boss->Circle.center = boss->RigidBody.Position;
				boss->CooldownShot[0] = 1.f;
				boss->CooldownShot[1] = 5.f;
				boss->CooldownShot[2] = ((float)rand() / RAND_MAX) * 5.f + 5.f;
				boss->CooldownShot[4] = 0.f;
			}
			for (int i = 0; i < 4; i++)
			{
				boss->CooldownShot[i] -= dt;
			}
			boss->CooldownShot[4] += dt * ((M_PI / 2.f) / 60.f);
			sfVector2f dirShot;
			// shoot on the player
			if (boss->CooldownShot[0] <= 0)
			{
				for (int i = 0; i < nbrOfPlayer; i++)
				{
					if (!player[i].States.IsDead)
					{
						sfVector2f dirShot;
						dirShot = Normalize(AddTwoVectors(boss->Circle.center, player[i].Physics.Rb.Position));
						sfVector2f tempDir = dirShot;
						float tempAngle = 10;
						float rotateAngle = DegreeToRadian(tempAngle);
						AddShot(&boss->FirstShot, boss->Circle.center, dirShot, razarShot);
						tempDir = Rotate(dirShot, -rotateAngle);
						AddShot(&boss->FirstShot, boss->Circle.center, tempDir, razarShot);
						tempDir = Rotate(dirShot, rotateAngle);
						AddShot(&boss->FirstShot, boss->Circle.center, tempDir, razarShot);
						boss->CooldownShot[0] = ((float)rand() / RAND_MAX) * 0.5f + 0.5f;
					}
				}
			}

			if (boss->CooldownShot[1] <= 0)
			{
				sfVector2f dirShot;
				dirShot = Normalize(AddTwoVectors(boss->Circle.center, player[targetPlayer].Physics.Rb.Position));
				AddShot(&boss->FirstShot, boss->Circle.center, dirShot, razarBigFragShot);
				boss->CooldownShot[1] = 5.f;
			}

			if (boss->CooldownShot[2] <= 0)
			{
				dirShot = Normalize(AddTwoVectors(boss->Circle.center, player[targetPlayer].Physics.Rb.Position));
				for (int i = -15; i < 15; i++)
				{
					sfVector2f tempPos = player[targetPlayer].Physics.Rb.Position;
					tempPos.x += 20.f * i * dirShot.x;
					tempPos.y += 20.f * i * dirShot.y;
					AddRazarWindElemShot(&boss->FirstShot, boss->Circle.center, tempPos, dirShot, -1);
					AddRazarWindElemShot(&boss->FirstShot, boss->Circle.center, tempPos, dirShot, 1);
				}
				boss->CooldownShot[2] = ((float)rand() / RAND_MAX) * 5.f + 5.f;
			}

			if (boss->CooldownShot[3] <= 0)
			{
				float angle = boss->CooldownShot[4];
				float angle2 = M_PI - boss->CooldownShot[4];
				sfVector2f dirShot = { cosf(angle),sinf(angle) };
				sfVector2f dirShot2 = { cosf(angle2),sinf(angle2) };
				sfVector2f tempPos = boss->Circle.center;
				for (int i = 2; i < 25; i++)
				{
					float speed = 100.f * (float)i;
					float distanceForDestroy = 100.f * (float)i;
					AddRazarBlockShot(&boss->FirstShot, tempPos, dirShot, speed, distanceForDestroy);
					AddRazarBlockShot(&boss->FirstShot, tempPos, dirShot2, speed, distanceForDestroy);
				}
				boss->CooldownShot[3] += 3.f;
			}
		}
		else
		{
			boss->CurrentPhase = death;
		}
		if (percentageLife > 10 && percentageLife < 60 && boss->CurrentPhase == 2)
		{
			boss->IsMoving = sfTrue;
			boss->TimerMovement -= dt;
			if (boss->TimerMovement <= 0)
			{
				if (!(rand() % 3))
				{
					boss->Direction = AddTwoVectors(boss->RigidBody.Position, player[targetPlayer].Physics.Rb.Position);
					boss->Direction = Normalize(boss->Direction);
					boss->Direction = Rotate(boss->Direction, DegreeToRadian((float)(rand() % 180 - 90.f)));
				}
				else
				{
					float tempAngle = (float)rand() / RAND_MAX * 360.f;
					boss->Direction = (sfVector2f) { cosf(tempAngle), sinf(tempAngle) };
				}
				boss->RigidBody.Velocity.x = boss->Direction.x * boss->Speed;
				boss->RigidBody.Velocity.y = boss->Direction.y * boss->Speed;
				boss->TimerMovement = 1.f + (float)rand() / RAND_MAX * 2.f;
			}
			UpdateRigidBody(&boss->RigidBody, dt);
		}
	}
}

void DamageBoss(Boss* boss, const int damage, int* playerXp)
{
	boss->Life -= damage;
	if (boss->Life <= 0)
	{
		boss->IsDead = sfTrue;
		(*playerXp) += boss->XpGive;
		if (boss->Id == slime)
		{
			boss->CurrentPhase = p2;
		}
		if (boss->Id == elem)
		{
			boss->RealPhase = idleBoss;
			boss->CurrentPhase = idleBoss;
		}
		ClearShots(&boss->FirstShot);
	}
	else
	{
		boss->IsHit = sfTrue;
	}
}

void ResetBoss(Boss* boss)
{
	if (boss->Id == 1)
	{
		boss->CanBeEngaged = sfFalse;
		boss->CanSpawn = sfFalse;
		boss->CurrentPhase = spawnPhase;
		boss->HasTrashesAlive = sfFalse;
	}

}