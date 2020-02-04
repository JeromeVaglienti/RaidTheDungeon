#include "Player.h"
#include "..\..\View\ViewManager.h"
#include "..\..\Controller\HUD\HUDController.h"
#include "..\..\Controller\MainSceneController\ShadersController\ShadersController.h"
#include "..\..\Controller\Sounds\Sounds.h"

void InitPlayer(Player *player, const sfVector2f position, const int id)
{
	player->Hud.Cursor = InitializeCursor();

	player->ID = id;
	player->Stats.LifeMax = PLAYERBASICLIFE;
	player->Stats.Life = player->Stats.LifeMax;
	player->Stats.SpeedMax = PLAYERBASICSPEED;
	player->Stats.speed = PLAYERBASICSPEED;
	player->Stats.TimerInvulnerability = 0.f;
	player->Stats.ManaMax = PLAYERBASICMANA;
	player->Stats.Mana = player->Stats.ManaMax;
	player->Stats.ShotPerSecond = PLAYERBASICSHOTPERSECOND;
	player->Stats.TimeOfInvincibility = PLAYERBASICTIMERINVU;
	player->Stats.CostManaMultiplier = 1.f;
	player->Stats.DamageMultiplier = 1.f;
	player->Stats.HealthRegenMultiplier = 1.f;
	player->Stats.ManaRegenMultiplier = 1.f;
	player->Stats.FallingTimer = 0.0f;
	player->Stats.XP = 0;

	InitializeRigidBody(&player->Physics.Rb);
	player->Physics.Rb.Position = position;
	player->Physics.LastPosition = position;
	player->Physics.Collider.center = player->Physics.Rb.Position;
	player->Physics.Collider.radius = 20.f;
	player->Physics.PositionWorld.x = (int)player->Physics.Rb.Position.x;
	player->Physics.PositionWorld.y = (int)player->Physics.Rb.Position.y;
	player->Physics.TimerMovementParticles = 0.f;

	player->Weapons.FirstShot = NULL;
	player->Weapons.CDShot = 1.f / PLAYERBASICSHOTPERSECOND;

	player->Spells.spell[FireBall] = ReturnProtoSpell(FireBall);
	player->Spells.spell[ConeOfFire] = ReturnProtoSpell(ConeOfFire);
	player->Spells.spell[mine] = ReturnProtoSpell(mine);
	player->Spells.spell[Dash] = ReturnProtoSpell(Dash);
	player->Spells.spellSelect = 0;
	player->Spells.MineState.AnimKey = InitAnimKey(14, 16.f / 12.f, sfTrue);
	player->Spells.MineState.ExplosionKey = InitAnimKey(10, 0.3f, sfFalse);
	player->Spells.MineState.Collider.radius = 130.f / 2.f * 1.5f;
	player->Spells.MineState.RadiusExplosion = (short)(player->Spells.MineState.Collider.radius * 2.f);
	player->Spells.MineState.Damage = player->Spells.spell[mine].damage;
	player->Spells.MineState.IdLight = -1;

	player->States.IsHit = sfFalse;
	player->States.IsDead = sfFalse;
	player->States.IsIdle = sfTrue;
	player->States.IsSlowDown = sfFalse;
	player->States.IsTeleporting = sfFalse;
	player->States.ISLeavingActualDungeon = sfFalse;
	player->States.IsWet = sfFalse;
	player->States.IsStopXl = sfFalse;
	player->States.IsStopYu = sfFalse;
	player->States.IsStopXr = sfFalse;
	player->States.IsStopYd = sfFalse;
	player->TypeInput = GetTypeOfHardware(id);

	player->Spells.DashState = (PlayerDashStat) { 0 };
	player->Spells.DashState.NbrDashMax = 1;
	player->Spells.DashState.NbrDashRemaining = 1;
	player->Spells.DashState.TimerParticle = 0.0f;
	player->ActualNode = NULL;
	player->Physics.ActualDirection = down;

	player->IdLight = -1;

	player->DeathAnimation = InitAnimKey(12, (24.0f / 12.0f), sfFalse);

	InitSkillTree(&player->SkillTree);
	AssignSkillTreeToPlayer(player);
	InitPointerSkillTree(&player->SkillTree, id);

#if _DEBUG

	if (player->Spells.MineState.Shape == NULL)
	{
		player->Spells.MineState.Shape = sfCircleShape_create();
		sfCircleShape_setOutlineThickness(player->Spells.MineState.Shape, 1.f);
		sfCircleShape_setOutlineColor(player->Spells.MineState.Shape, sfWhite);
		sfCircleShape_setFillColor(player->Spells.MineState.Shape, sfTransparent);
		sfCircleShape_setRadius(player->Spells.MineState.Shape, player->Spells.MineState.Collider.radius);
		sfCircleShape_setOrigin(player->Spells.MineState.Shape, (sfVector2f) { player->Spells.MineState.Collider.radius, player->Spells.MineState.Collider.radius });

		player->Shape = sfCircleShape_create();
		sfCircleShape_setOutlineThickness(player->Shape, 1.f);
		sfCircleShape_setOutlineColor(player->Shape, sfWhite);
		sfCircleShape_setFillColor(player->Shape, sfTransparent);
		sfCircleShape_setRadius(player->Shape, player->Physics.Collider.radius);
		sfCircleShape_setOrigin(player->Shape, (sfVector2f) { player->Physics.Collider.radius, player->Physics.Collider.radius });

	}
#endif
}

void SetInitNodePlayer(Player *player, Node* firstNode)
{
	player->ActualNode = GetClosestNodeInArray(player->Physics.Rb.Position.x, player->Physics.Rb.Position.y, firstNode);
}

// update the position of the player while his dashing
void UpdateDash(Player *player, const float deltaTime)
{
	if (player->States.IsDashing)
	{
		player->Spells.DashState.Timer -= deltaTime;
		if (player->Spells.DashState.Timer <= 0)
		{
			player->Spells.DashState.Timer = 0.f;
			player->States.IsDashing = sfFalse;
			player->Stats.TimerInvulnerability = 0.f;
		}
		player->Physics.Rb.Velocity.x = player->Physics.Direction.x * player->Stats.speed * 1.5f;
		player->Physics.Rb.Velocity.y = player->Physics.Direction.y * player->Stats.speed * 1.5f;

	}
}

// update the firerate of the cone and the timer
void UpdateConeOfFire(Player *player, const float deltaTime, const sfVector2f dirCone)
{
	if (player->States.IsSpitFire)
	{
		player->Physics.DirectionShot = dirCone;
		player->Spells.ConeState.Timer -= deltaTime;
		player->Spells.ConeState.TimerFireRate -= deltaTime;
		if (player->Spells.ConeState.TimerFireRate <= 0)
		{
			player->Spells.ConeState.TimerFireRate += player->Spells.spell[ConeOfFire].fireRate;
			player->Spells.ConeState.MustDamage = sfTrue;
		}
		if (player->Spells.ConeState.Timer <= 0)
		{
			ActivateLigth(player->IdLight, 0);
			player->IdLight = -1;
			player->States.IsSpitFire = sfFalse;
		}
	}
}

void AddShotColor(Shot** firstShot, const sfVector2f pos, const sfVector2f direction, unsigned short id, const int idPlayer, const float speedMultiplier, const float damageMultiplier)
{
	PlayersInformations playerInfo = GetPlayersInformations();
	if (playerInfo.SkinPlayer[idPlayer] == red)
	{
		AddShotWithSpeedAndDamageMultiplier(firstShot, pos, direction, id, speedMultiplier, damageMultiplier);
	}
	else if (playerInfo.SkinPlayer[idPlayer] == black)
	{
		AddShotWithSpeedAndDamageMultiplier(firstShot, pos, direction, id + 1, speedMultiplier, damageMultiplier);
	}
	else if (playerInfo.SkinPlayer[idPlayer] == white)
	{
		AddShotWithSpeedAndDamageMultiplier(firstShot, pos, direction, id + 1, speedMultiplier, damageMultiplier);
	}
	else if (playerInfo.SkinPlayer[idPlayer] == green)
	{
		AddShotWithSpeedAndDamageMultiplier(firstShot, pos, direction, id + 1, speedMultiplier, damageMultiplier);
	}
}

void UpdateMine(PlayerMineStat* mineStat, const float deltaTime, Shot** firstShot, const float damageMultiplier, const int id, const sfBool isUpgrade)
{
	if (mineStat->IsActivate)
	{
		if (mineStat->Explodes)
		{
			if (mineStat->ExplosionKey.actualDuration == 0.f)
			{
				StopSounds(fireTrap, 0);
				PlaySounds(explosion);
				if (isUpgrade)
				{
					float basicAngle = (float)(rand() % (int)(360.f / 5.f));
					for (float i = 0; i < 360; i += 360.f / 5.f)
					{
						float angle = DegreeToRadian(i + basicAngle);
						sfVector2f dirShot = { cosf(angle), sinf(angle) };
						AddShotColor(firstShot, mineStat->Collider.center, dirShot, fireBall, id, 1.f, damageMultiplier);
					}
				}
			}
			UpdateAnimationKey(&mineStat->ExplosionKey, deltaTime);
			if (mineStat->ExplosionKey.actualFrameToShow == mineStat->ExplosionKey.numberOfFrame - 1)
			{
				mineStat->IsActivate = sfFalse;
				ActivateLigth(mineStat->IdLight, 0);
			}
		}
		else
		{
			UpdateAnimationKeyWithSpecialLoop(&mineStat->AnimKey, deltaTime, 9);
			mineStat->Timer -= deltaTime;
			if (mineStat->Timer <= 0.f)
			{
				mineStat->Explodes = sfTrue;
			}
		}
	}
}

//update the angle of the cone of fire 
void PlayerUpdateAngleOfConeOfFire(Player *player, sfVector2f dirCone)
{
	if (player->TypeInput == 0)
	{
		dirCone.x = dirCone.x - player->Physics.PositionWorld.x;
		dirCone.y = dirCone.y - player->Physics.PositionWorld.y;
		dirCone = Normalize(dirCone);
	}
	else if (player->TypeInput == 1)
	{
		dirCone = Normalize(dirCone);
	}
	if (player->IdLight > 0)
	{
		SetLigthPosition(player->IdLight, (sfVector2f) { player->Physics.Rb.Position.x + dirCone.x * 140, player->Physics.Rb.Position.y + dirCone.y * 140 }, player->ActualNode);
	}
	player->Physics.DirectionShot = dirCone;
	player->Spells.ConeState.Rotate = RadianToDegree(atan2f(dirCone.y, dirCone.x)) + 90;
}

void ComeBackFromTheDead(Player *player)
{
	sfVector2f newVec = (sfVector2f) { player->Physics.LastPosition.x * 64.0f, player->Physics.LastPosition.y *64.0f };
	player->Physics.Rb.Position = newVec;
	player->Physics.PositionWorld.x = (int)newVec.x;
	player->Physics.PositionWorld.y = (int)newVec.y;
	player->Physics.Collider.center = player->Physics.Rb.Position;
	ChangeNodePlayer(&player->ActualNode, player->Physics.Rb.Position);
	DamagePlayer(player, (int)(player->Stats.LifeMax*0.2f));
}
//Called every frame
void UpdatePlayer(Player *player, const float deltaTime, const int index, Node* firstNode)
{
	if (!player->ActualNode->isFalling)
	{
		player->Physics.LastPosition.x = player->ActualNode->posX;
		player->Physics.LastPosition.y = player->ActualNode->posY - ((1 / 3.0f)* PLAYERHEIGHT) / 64.0f;
	}


	if (!player->States.IsTeleporting)
	{

		if (!player->States.IsDead)
		{
			if (player->Stats.FallingTimer > 0.18f)
			{
				player->Stats.FallingTimer = 0.0f;
				ComeBackFromTheDead(player);
			}
			if (player->ActualNode->lava)
			{
				DamagePlayer(player, 1);
			}
			if (player->States.IsSlowDown)
			{
				player->Stats.speed += (player->Stats.SpeedMax / 4.0f) * deltaTime;

				if (player->Stats.speed >= player->Stats.SpeedMax)
				{
					player->Stats.speed = player->Stats.SpeedMax;
					player->States.IsSlowDown = sfFalse;
				}
			}

			//Collision system with node 
			PlayerInputAffectationMovement(player, player->Physics.Direction, deltaTime);

			if (CheckNextNodeP(player->ActualNode, &player->Physics.Rb, player->Physics.Rb.Position, sfTrue))
			{
				ChangeNodePlayer(&player->ActualNode, player->Physics.Rb.Position);
			}
			else if (player->States.IsDashing)
			{
				player->States.IsIdle = sfTrue;
				player->States.IsDashing = sfFalse;
				player->Stats.TimerInvulnerability = 0.0f;
			}
			else
			{
				player->States.IsIdle = sfTrue;
			}

			if (player->States.IsStopXl && player->Physics.Rb.Velocity.x < 0.0f)
			{
				player->Physics.Rb.Velocity.x = 0.0f;
			}
			if (player->States.IsStopXr && player->Physics.Rb.Velocity.x > 0.0f)
			{
				player->Physics.Rb.Velocity.x = 0.0f;
			}

			if (player->States.IsStopYu && player->Physics.Rb.Velocity.y < 0.0f)
			{
				player->Physics.Rb.Velocity.y = 0.0f;
			}
			if (player->States.IsStopYd && player->Physics.Rb.Velocity.y > 0.0f)
			{
				player->Physics.Rb.Velocity.y = 0.0f;
			}


			UpdateRigidBody(&player->Physics.Rb, deltaTime);

			player->Physics.Collider.center = player->Physics.Rb.Position;

			player->Weapons.CDShot -= deltaTime;
			player->Physics.PositionWorld.x = (int)player->Physics.Rb.Position.x;
			player->Physics.PositionWorld.y = (int)player->Physics.Rb.Position.y;
			UpdateCooldownSpell(player, deltaTime);
			UpdateDash(player, deltaTime);
			UpdateConeOfFire(player, deltaTime, player->Physics.Direction);
			UpdateMine(&player->Spells.MineState, deltaTime, &player->Weapons.FirstShot, player->Stats.DamageMultiplier, player->ID, player->Spells.spell[mine].isUpgrade);

		}
		sfVector2f playerPos[1] = { player->Physics.Rb.Position };
		UpdateShots(&player->Weapons.FirstShot, deltaTime, firstNode, playerPos, 0, player->ActualNode);

		if (player->States.IsDead)
		{
			UpdateAnimationKey(&player->DeathAnimation, deltaTime);
		}
		else if (player->States.IsAttacking)
		{
			player->Physics.ActualDirection = (int)((int)(RadianToDegree(atan2f(-player->Physics.DirectionShot.y, -player->Physics.DirectionShot.x)) + 225) % 360 / 90);
		}
		else if (!player->States.IsIdle && !player->States.IsDashing)
		{
			player->Physics.ActualDirection = (int)((int)(RadianToDegree(atan2f(-player->Physics.Direction.y, -player->Physics.Direction.x)) + 225) % 360 / 90);
		}
		if (player->Stats.Life <= 0)
		{
			ActivateLigth(index, 0);
			player->States.IsDead = sfTrue;
			if (player->IdLight > 0)
			{
				ActivateLigth(player->IdLight, 0);
				player->IdLight = -1;
			}
			player->States.IsSpitFire = sfFalse;
			player->States.IsSpitingFire = sfFalse;
		}

	}

}

//Manage player's movement
void PlayerInputAffectationMovement(Player *player, sfVector2f dirMov, const float dt)
{
	if (!player->States.IsDashing)
	{
		if (player->ActualNode->isFalling)
		{
			player->Stats.FallingTimer += dt;
		}


		if (player->ActualNode->Ice)
		{
			float X = dirMov.x * player->Stats.speed * player->ActualNode->speedOfWalk;
			float Y = dirMov.y * player->Stats.speed * player->ActualNode->speedOfWalk;

			if (Magnitude(player->Physics.Rb.Velocity) >= player->Stats.SpeedMax)
			{
				player->Physics.Rb.Velocity.x *= 0.99f;
				player->Physics.Rb.Velocity.y *= 0.99f;
			}


			dirMov = Normalize(dirMov);
			player->Physics.Rb.Velocity.x = Lerp(player->Physics.Rb.Velocity.x, X, dt*0.5f);
			player->Physics.Rb.Velocity.y = Lerp(player->Physics.Rb.Velocity.y, Y, dt*0.5f);
			player->Physics.Direction = dirMov;

			if (player->Physics.Rb.Velocity.x != 0)
			{
				player->Physics.Rb.Friction.x = player->Physics.Rb.Velocity.x*1.1f;
			}
			if (player->Physics.Rb.Velocity.y != 0)
			{
				player->Physics.Rb.Friction.y = player->Physics.Rb.Velocity.y*1.1f;
			}

		}
		else
		{
			dirMov = Normalize(dirMov);
			player->Physics.Rb.Velocity.x = dirMov.x * player->Stats.speed * player->ActualNode->speedOfWalk;
			player->Physics.Rb.Velocity.y = dirMov.y * player->Stats.speed * player->ActualNode->speedOfWalk;
			if (player->ActualNode->speedOfWalk <= 0.5f)
			{
				player->Physics.Rb.Velocity.x *= 0.8f;
				player->Physics.Rb.Velocity.y *= 0.8f;
			}
			else
			{
				player->Physics.Rb.Friction.x = 0.0f;
				player->Physics.Rb.Friction.y = 0.0f;
			}
			player->Physics.Direction = dirMov;
		}
	}
}

//Manage camera's movement based on the player's position
void PlayerInputAffectationViewMovement(Player *player, sfVector2f dirMov)
{
	//mvt view gamepad
	sfVector2f tmpDir = dirMov;
	dirMov = Normalize(dirMov);

	if (tmpDir.x > 30 || tmpDir.x < -30)
	{
		player->Hud.Cursor.Position.x += dirMov.x * 2;
	}
	if (tmpDir.y > 30 || tmpDir.y < -30)
	{
		player->Hud.Cursor.Position.y += dirMov.y * 2;
	}
}

//Manage Firing system Logic
void PlayerFiring(Player *player, sfVector2f dirShot)
{
	if (!player->States.IsDead)
	{
		//0 is KeyBoard ; 1 is GamePad
		if (player->TypeInput == 0)
		{
			dirShot.x = dirShot.x - player->Physics.PositionWorld.x;
			dirShot.y = dirShot.y - player->Physics.PositionWorld.y;
			dirShot = Normalize(dirShot);
		}
		else if (player->TypeInput == 1)
		{
			dirShot = Normalize(dirShot);
		}

		if (player->Weapons.CDShot <= 0.f && (dirShot.x != 0 || dirShot.y != 0))
		{
			float magnitudeDirectionPlayer = Magnitude(player->Physics.Direction);
			float speedMultiplier;
			if (magnitudeDirectionPlayer > 0.0001)
			{
				speedMultiplier = 1.f + Magnitude((sfVector2f) { player->Physics.Direction.x + dirShot.x, player->Physics.Direction.y + dirShot.y }) / 3.f;
			}
			else
			{
				speedMultiplier = 1;
			}
			player->Hud.Cursor.Scale.x = 1.0f;
			player->Hud.Cursor.Scale.y = 1.0f;
			player->States.IsFiring = sfTrue;
			PlaySounds(match);
			AddShotColor(&player->Weapons.FirstShot, player->Physics.Rb.Position, dirShot, matches, player->ID, speedMultiplier, player->Stats.DamageMultiplier);
			player->Weapons.CDShot = 1.f / player->Stats.ShotPerSecond;
		}
		player->Physics.DirectionShot = dirShot;
	}
}

void CastFireball(Player *player, const sfVector2f dirShot)
{
	float magnitudeDirectionPlayer = Magnitude(player->Physics.Direction);
	float speedMultiplier;
	if (magnitudeDirectionPlayer > 0.0001)
	{
		speedMultiplier = 1.f + Magnitude((sfVector2f) { player->Physics.Direction.x + dirShot.x, player->Physics.Direction.y + dirShot.y }) / 5.f;
	}
	else
	{
		speedMultiplier = 1;
	}
	if (!player->Spells.spell[FireBall].isUpgrade)
	{
		sfVector2f tempPos = { player->Physics.Rb.Position.x + dirShot.x * 30.f,  player->Physics.Rb.Position.y + dirShot.y * 30.f };
		AddShotColor(&player->Weapons.FirstShot, tempPos, dirShot, fireBall, player->ID, speedMultiplier, player->Stats.DamageMultiplier);
	}
	else
	{
		sfVector2f tempDir = dirShot;
		sfVector2f tempPos = { player->Physics.Rb.Position.x + dirShot.x * 30.f,  player->Physics.Rb.Position.y + dirShot.y * 30.f };
		float tempAngle = 10;
		float angleRotate = DegreeToRadian(tempAngle);

		AddShotColor(&player->Weapons.FirstShot, tempPos, dirShot, fireBall, player->ID, speedMultiplier, player->Stats.DamageMultiplier);
		tempDir = Rotate(dirShot, -angleRotate);
		tempPos = (sfVector2f) { player->Physics.Rb.Position.x + tempDir.x * 30.f, player->Physics.Rb.Position.y + tempDir.y * 30.f };
		AddShotColor(&player->Weapons.FirstShot, tempPos, tempDir, fireBall, player->ID, speedMultiplier, player->Stats.DamageMultiplier);
		tempDir = Rotate(dirShot, angleRotate);
		tempPos = (sfVector2f) { player->Physics.Rb.Position.x + tempDir.x * 30.f, player->Physics.Rb.Position.y + tempDir.y * 30.f };
		AddShotColor(&player->Weapons.FirstShot, tempPos, tempDir, fireBall, player->ID, speedMultiplier, player->Stats.DamageMultiplier);


	}
	player->Physics.DirectionShot = dirShot;
	player->Stats.Mana -= (int)(player->Spells.spell[FireBall].cost * player->Stats.CostManaMultiplier);
	player->Spells.spell[FireBall].isAvailable = sfFalse;
}

void CastDash(Player *player, const sfVector2f dirShot)
{
	player->Spells.DashState.Timer = 0.5f;
	player->States.IsDashing = sfTrue;
	player->Stats.TimerInvulnerability = 1.f;
	player->Spells.DashState.NbrDashRemaining--;
	player->Stats.Mana -= player->Spells.spell[Dash].cost;
	if (player->Spells.DashState.NbrDashRemaining == 0)
	{
		player->Spells.spell[Dash].isAvailable = sfFalse;
	}
}

void CastMine(Player *player, const sfVector2f dirShot)
{
	player->Spells.MineState.Timer = 9.f;
	player->Spells.MineState.Explodes = sfFalse;
	player->Spells.MineState.IsActivate = sfTrue;
	player->Spells.MineState.Pos = player->Physics.Rb.Position;
	player->Spells.MineState.Collider.center.x = player->Physics.Rb.Position.x + (345.f - player->Spells.MineState.Collider.radius) * dirShot.x;
	player->Spells.MineState.Collider.center.y = player->Physics.Rb.Position.y + (345.f - player->Spells.MineState.Collider.radius) * dirShot.y;
	player->Spells.MineState.Rotate = RadianToDegree(atan2f(dirShot.y, dirShot.x));
	player->Stats.Mana -= (int)(player->Spells.spell[mine].cost * player->Stats.CostManaMultiplier);
	player->Spells.spell[mine].isAvailable = sfFalse;
	player->Spells.MineState.AnimKey.actualDuration = 0.f;
	player->Spells.MineState.AnimKey.actualFrameToShow = 0;
	player->Spells.MineState.ExplosionKey.actualDuration = 0.f;
	player->Spells.MineState.ExplosionKey.actualFrameToShow = 0;
	player->Spells.MineState.IdLight = GetAvaibleLight();
	ActivateLigth(player->Spells.MineState.IdLight, 1);
	SetIndexSprite(player->Spells.MineState.IdLight, 0);

	PlayersInformations playerInfo = GetPlayersInformations();
	if (playerInfo.SkinPlayer[player->ID] == red)
	{
		SetLigthColor(player->Spells.MineState.IdLight, CANDLELIGHTRED);
	}
	else if (playerInfo.SkinPlayer[player->ID] == black)
	{
		SetLigthColor(player->Spells.MineState.IdLight, CANDLELIGHTBLACK);
	}
	else if (playerInfo.SkinPlayer[player->ID] == white)
	{
		SetLigthColor(player->Spells.MineState.IdLight, CANDLELIGHTWHITE);
	}
	else if (playerInfo.SkinPlayer[player->ID] == green)
	{
		SetLigthColor(player->Spells.MineState.IdLight, CANDLELIGHTGREEN);
	}
	sfVector2f posLight = (sfVector2f) { player->Spells.MineState.Pos.x + 230.0f*dirShot.x, player->Spells.MineState.Pos.y + 230.0f*dirShot.y };
	SetLigthPosition(player->Spells.MineState.IdLight, posLight, player->ActualNode);
}

void CastConeOfFire(Player *player, const sfVector2f dirShot)
{
	player->IdLight = GetAvaibleLight();
	ActivateLigth(player->IdLight, 1);
	SetIndexSprite(player->IdLight, 2);

	PlayersInformations playerInfo = GetPlayersInformations();
	if (playerInfo.SkinPlayer[player->ID] == red)
	{
		SetLigthColor(player->IdLight, CONELIGHT);
	}
	else if (playerInfo.SkinPlayer[player->ID] == black)
	{
		SetLigthColor(player->IdLight, CONELIGHTBLACK);
	}
	else if (playerInfo.SkinPlayer[player->ID] == white)
	{
		SetLigthColor(player->IdLight, CONELIGHTWHITE);
	}
	else if (playerInfo.SkinPlayer[player->ID] == green)
	{
		SetLigthColor(player->IdLight, CONELIGHTGREEN);
	}

	player->Spells.spell[ConeOfFire].isAvailable = sfFalse;
	player->Stats.Mana -= (int)(player->Spells.spell[ConeOfFire].cost * player->Stats.CostManaMultiplier);
	player->Spells.ConeState.Rotate = RadianToDegree(atan2f(dirShot.x, dirShot.y)) + 90;
	player->Spells.ConeState.Timer = 2.f;
	player->Spells.ConeState.TimerFireRate = player->Spells.spell[ConeOfFire].fireRate;
	player->States.IsSpitFire = sfTrue;
	player->Spells.ConeState.MustDamage = sfTrue;
}

// check which spell the player want to cast
void PlayerCastSpell(Player *player, sfVector2f dirShot, const unsigned short id)
{
	if (!player->States.IsDead)
	{
		//0 is KeyBoard ; 1 is GamePad
		if (player->TypeInput == 0)
		{
			dirShot.x = dirShot.x - player->Physics.PositionWorld.x;
			dirShot.y = dirShot.y - player->Physics.PositionWorld.y;
			dirShot = Normalize(dirShot);
		}
		else if (player->TypeInput == 1)
		{
			dirShot = Normalize(dirShot);
		}

		if (!player->States.IsDashing && player->Spells.spell[id].isUnlocked && player->Spells.spell[id].isAvailable && player->Stats.Mana >= player->Spells.spell[id].cost * player->Stats.CostManaMultiplier)
		{
			if (id == FireBall)
			{
				CastFireball(player, dirShot);
				player->States.IsCastingFireBall = sfTrue;
			}
			else if (id == ConeOfFire)
			{
				CastConeOfFire(player, dirShot);
			}
			else if (id == mine)
			{
				PlaySounds(fireTrap);
				CastMine(player, dirShot);
			}
			else if (id == Dash && player->Spells.DashState.NbrDashRemaining > 0)
			{
				CastDash(player, player->Physics.Direction);
			}
			else
			{
				return;
			}
			UpdatePlayerHUD(*player, player->ID, 0);
			UpdatePlayerCDHUD(*player, player->ID);
		}
	}
}

// reduce spell cooldown
void UpdateCooldownSpell(Player *player, const float deltaTime)
{
	for (int i = 0; i < 4; i++)
	{
		if (!player->Spells.spell[i].isAvailable && player->Spells.spell[i].cost <= player->Stats.Mana)
		{
			player->Spells.spell[i].timerCooldown += deltaTime;
			if (player->Spells.spell[i].timerCooldown >= player->Spells.spell[i].cooldown)
			{
				if (player->States.IsWet)
				{
					player->States.IsWet = sfFalse;
				}
				player->Spells.spell[i].isAvailable = sfTrue;
				player->Spells.spell[i].timerCooldown = 0.f;
				UpdatePlayerCDHUD(*player, player->ID);
				if (i == Dash)
				{
					player->Spells.DashState.NbrDashRemaining = player->Spells.DashState.NbrDashMax;
				}
			}
		}
		if (player->Spells.spell[i].isAvailable && player->Spells.spell[i].cost > player->Stats.Mana)
		{
			player->Spells.spell[i].isAvailable = sfFalse;
			player->Spells.spell[i].timerCooldown = 0.f;
			UpdatePlayerCDHUD(*player, player->ID);
		}
	}
}

// assign the new spell index at the selected spell
void PlayerChangeSpell(Player *player, const int newspellIndex)
{
	player->Spells.spellSelect = newspellIndex;
	UpdatePlayerSpellSelectedHUD(*player, player->ID);
}

// inflicts damage to the player and activates invulnerability
void DamagePlayer(Player* player, const unsigned short damage)
{
	if (player->Stats.TimerInvulnerability <= 0)
	{
		player->States.IsHit = sfTrue;
		player->Stats.Life -= damage;
		player->Stats.TimerInvulnerability = player->Stats.TimeOfInvincibility;
		if (player->Stats.Life < 0)
		{
			player->Stats.Life = 0;
			player->States.IsDead = sfTrue;
		}
	}
}

void AddXPToPlayer(Player* player, const int xp)
{
	player->Stats.XP += xp;
}
// affect the loot to the player
void AddLootToPlayer(Player* player, const LootTransfer lootTransfer)
{
	if (lootTransfer.Id == littleBottleOfMana || lootTransfer.Id == mediumBottleOfMana)
	{
		player->Stats.Mana += (int)(lootTransfer.Nbr * player->Stats.ManaRegenMultiplier);
		if (player->Stats.Mana > player->Stats.ManaMax)
		{
			player->Stats.Mana = player->Stats.ManaMax;
		}
	}
	else if (lootTransfer.Id == littleBottleOfLife || lootTransfer.Id == mediumBottleOfLife)
	{
		player->Stats.Life += (int)(lootTransfer.Nbr * player->Stats.HealthRegenMultiplier);
		if (player->Stats.Life > player->Stats.LifeMax)
		{
			player->Stats.Life = player->Stats.LifeMax;
		}
	}
}

void MoveLoot(Loot* loot, Player* players, const float speed)
{
	sfVector2f vec = Normalize(AddTwoVectors(loot->Pos, players[loot->IdPlayer].Physics.Rb.Position));
	loot->Pos.x += vec.x * speed;
	loot->Pos.y += vec.y * speed;
	loot->Colidier.center = loot->Pos;
}

void SetAttractionLoot(Loot* loot, const Player player, const float radius)
{
	if (Distance(player.Physics.Rb.Position, loot->Pos) < radius)
	{
		loot->IsAttract = sfTrue;
		loot->IdPlayer = player.ID;
	}
}

void AttractLoot(LootData* lootData, Player* players, const int numberOfPlayer, const float dt)
{
	for (int i = 0; i < lootData->NbrLoot; i++)
	{
		if (!lootData->Loot[i].IsAttract)
		{
			for (int j = 0; j < numberOfPlayer; j++)
			{
				if (!players[j].States.IsDead)
				{
					SetAttractionLoot(&lootData->Loot[i], players[j], lootData->RadiusAttraction);
				}
			}
		}
		else
		{
			if (!players[lootData->Loot[i].IdPlayer].States.IsDead)
			{
				MoveLoot(&lootData->Loot[i], players, lootData->Speed * dt);
			}
			else
			{
				lootData->Loot[i].IsAttract = sfFalse;
				lootData->Loot[i].IdPlayer = -1;
			}
		}
	}
}

void AssignSkillTreeToPlayer(Player* player)
{
	//assign life branch to player
	for (int i = 0; i < NBRTALENTPERBRANCH; i++)
	{
		//check if the skill tree have one point. exit the loop if there is no point 
		if (player->SkillTree.Branch[lifeBranch].NbrUpgrade == 0)
		{
			i = NBRTALENTPERBRANCH;
		}
		//assign the first upgrade of life branch : more regen when we pick up life potion
		else if (i == regenLife)
		{
			// change a percentage for a multiplier
			// ex : 65% = 1.65f
			float nextHealthRegenMultiplier = 1.f + (player->SkillTree.Branch[lifeBranch].Talent[i].ActualUpgrade * player->SkillTree.Branch[lifeBranch].Talent[i].PercentageBuffPerUpgrade / 100.f);

			player->Stats.HealthRegenMultiplier = nextHealthRegenMultiplier;
		}
		//assign the second upgrade of life branch : more duration on invulnerability
		else if (i == moreInvulnerability)
		{
			// increes the duration of the timer of invincibility based on the basic player invulnerability
			player->Stats.TimeOfInvincibility = PLAYERBASICTIMERINVU * (1.f + (player->SkillTree.Branch[lifeBranch].Talent[i].ActualUpgrade * player->SkillTree.Branch[lifeBranch].Talent[i].PercentageBuffPerUpgrade / 100.f));
		}
		//assign the third upgrade of life branch : more life max
		else if (i == moreLifeMax)
		{
			// increase the maximum life of the player according to the basic maximum life of the player
			player->Stats.LifeMax = (int)(PLAYERBASICLIFE * (1.f + (player->SkillTree.Branch[lifeBranch].Talent[i].ActualUpgrade * player->SkillTree.Branch[lifeBranch].Talent[i].PercentageBuffPerUpgrade / 100.f)));
			player->Stats.Life = player->Stats.LifeMax;
		}

		//check if the player have unlock the next skill
		//exit the loop if the player have not unlock the next skill
		if (player->SkillTree.Branch[lifeBranch].Talent[i].ActualUpgrade < player->SkillTree.Branch[lifeBranch].Talent[i].NbrUpgradeToUnlockNextOne)
		{
			i = NBRTALENTPERBRANCH;
		}
	}

	//assign mana branch to player
	for (int i = 0; i < NBRTALENTPERBRANCH; i++)
	{
		//check if the skill tree have one point. exit the loop if there is no point 
		if (player->SkillTree.Branch[manaBranch].NbrUpgrade == 0)
		{
			i = NBRTALENTPERBRANCH;
		}
		//assign the first upgrade of mana branch : more regen when we pick up mana potion
		else if (i == regenMana)
		{
			// change a percentage for a multiplier
			// ex : 40% = 1.4f
			float nextManaRegenMultiplier = 1.f + (player->SkillTree.Branch[manaBranch].Talent[i].ActualUpgrade * player->SkillTree.Branch[manaBranch].Talent[i].PercentageBuffPerUpgrade / 100.f);

			player->Stats.ManaRegenMultiplier = nextManaRegenMultiplier;
		}
		//assign the second upgrade of mana branch : less cost of mana on spells
		else if (i == lessCostMana)
		{
			// change a percentage for a multiplier
			// ex : 50% = 0.5f
			player->Stats.CostManaMultiplier = 1.f - (player->SkillTree.Branch[manaBranch].Talent[i].ActualUpgrade * player->SkillTree.Branch[manaBranch].Talent[i].PercentageBuffPerUpgrade / 100.f);
		}
		//assign the third upgrade of mana branch : more mana max
		else if (i == moreManaMax)
		{
			// increase the maximum mana of the player according to the basic maximum mana of the player
			player->Stats.ManaMax = (int)(PLAYERBASICMANA * (1.f + (player->SkillTree.Branch[manaBranch].Talent[i].ActualUpgrade * player->SkillTree.Branch[manaBranch].Talent[i].PercentageBuffPerUpgrade / 100.f)));
			player->Stats.Mana = player->Stats.ManaMax;
		}

		//check if the player have unlock the next skill
		//exit the loop if the player have not unlock the next skill
		if (player->SkillTree.Branch[manaBranch].Talent[i].ActualUpgrade < player->SkillTree.Branch[manaBranch].Talent[i].NbrUpgradeToUnlockNextOne)
		{
			i = NBRTALENTPERBRANCH;
		}
	}

	//assign mobility branch to player
	for (int i = 0; i < NBRTALENTPERBRANCH; i++)
	{
		//check if the skill tree have one point. exit the loop if there is no point 
		if (player->SkillTree.Branch[mobilityBranch].NbrUpgrade == 0)
		{
			i = NBRTALENTPERBRANCH;
		}
		//assign the first upgrade of mobility branch : increase the speed of the player
		else if (i == speedBuff)
		{
			// change a percentage for a multiplier
			// ex : 40% = 1.4f
			player->Stats.SpeedMax = PLAYERBASICSPEED * (1.f + (player->SkillTree.Branch[mobilityBranch].Talent[i].ActualUpgrade * player->SkillTree.Branch[mobilityBranch].Talent[i].PercentageBuffPerUpgrade / 100.f));
			player->Stats.speed = player->Stats.SpeedMax;
		}
		//assign the second upgrade of mobility branch : less slowdown on the player
		else if (i == lessSlowDown)
		{
			//Tenacity is a percentage
			player->Stats.Tenacity = player->SkillTree.Branch[mobilityBranch].Talent[i].ActualUpgrade * player->SkillTree.Branch[mobilityBranch].Talent[i].PercentageBuffPerUpgrade;
		}
		//assign the third upgrade of mobility branch : unlock a second dash
		else if (i == doubleDash && player->SkillTree.Branch[mobilityBranch].Talent[i].ActualUpgrade == 1)
		{
			player->Spells.DashState.NbrDashMax = 2;
			player->Spells.DashState.NbrDashRemaining = 2;
		}

		//check if the player have unlock the next skill
		//exit the loop if the player have not unlock the next skill
		if (player->SkillTree.Branch[mobilityBranch].Talent[i].ActualUpgrade < player->SkillTree.Branch[mobilityBranch].Talent[i].NbrUpgradeToUnlockNextOne)
		{
			i = NBRTALENTPERBRANCH;
		}
	}

	//assign damage branch to player
	for (int i = 0; i < NBRTALENTPERBRANCH; i++)
	{
		//check if the skill tree have one point. exit the loop if there is no point 
		if (player->SkillTree.Branch[damageBranch].NbrUpgrade == 0)
		{
			i = NBRTALENTPERBRANCH;
		}
		//assign the first upgrade of damage branch : unlock the cone of fire
		else if (i == unlockConeOfFire && player->SkillTree.Branch[damageBranch].Talent[i].ActualUpgrade == 1)
		{
			player->Spells.spell[mine].isUpgrade = sfTrue;
		}
		//assign the second upgrade of damage branch : upgrade the fire ball (3 fire balls)
		else if (i == upgradeFireBall && player->SkillTree.Branch[damageBranch].Talent[i].ActualUpgrade == 1)
		{
			player->Spells.spell[FireBall].isUpgrade = sfTrue;
		}
		//assign the third upgrade of damage branch : upgrade the damage of player
		else if (i == moreDamage)
		{
			// change a percentage for a multiplier
			// ex : 150% = 2.5f
			player->Stats.DamageMultiplier = 1.f + (player->SkillTree.Branch[damageBranch].Talent[i].ActualUpgrade * player->SkillTree.Branch[damageBranch].Talent[i].PercentageBuffPerUpgrade / 100.f);
		}

		//check if the player have unlock the next skill
		//exit the loop if the player have not unlock the next skill
		if (player->SkillTree.Branch[damageBranch].Talent[i].ActualUpgrade < player->SkillTree.Branch[damageBranch].Talent[i].NbrUpgradeToUnlockNextOne)
		{
			i = NBRTALENTPERBRANCH;
		}
	}
}

void SlowPlayer(Player* player, const float percentageSlow)
{
	player->States.IsSlowDown = sfTrue;
	player->Stats.speed = player->Stats.SpeedMax * (100.f - percentageSlow + percentageSlow * player->Stats.Tenacity / 100.f) / 100.f;
}

void SetPlayerPos(Player* player, const sfVector2f NewPos)
{
	player->Physics.Rb.Position = NewPos;
	player->Physics.Collider.center = player->Physics.Rb.Position;
}

void ResetPlayer(Player* player)
{
	ResetAnimKey(&player->Spells.MineState.AnimKey);
	ResetAnimKey(&player->Spells.MineState.ExplosionKey);
	player->Spells.spell[mine].timerCooldown = 0.f;
	player->Spells.spell[mine].isAvailable = sfTrue;
	player->Spells.spell[ConeOfFire].timerCooldown = 0.f;
	player->Spells.spell[ConeOfFire].isAvailable = sfTrue;
	player->Spells.spell[Dash].timerCooldown = 0.f;
	player->Spells.spell[Dash].isAvailable = sfTrue;
	player->Spells.spell[FireBall].isAvailable = sfTrue;
	player->Spells.spell[FireBall].timerCooldown = 0.f;
	player->Stats.TimerInvulnerability = 0.f;
	player->Spells.MineState.IsActivate = sfFalse;
	player->Spells.ConeState.Timer = 0.f;
	player->Spells.MineState.Timer = 0.f;
	player->States.IsHit = sfFalse;
	player->States.IsDead = sfFalse;
	player->States.IsIdle = sfTrue;
	player->States.IsSlowDown = sfFalse;
	player->States.IsDashing = sfFalse;
	player->States.IsSpitFire = sfFalse;
	player->States.IsSpitingFire = sfFalse;
	player->States.IsFiring = sfFalse;
	player->States.IsCastingFireBall = sfFalse;
	player->States.IsAttacking = sfFalse;
	player->States.IsTeleporting = sfFalse;
	player->States.ISLeavingActualDungeon = sfFalse;
	//player->ActualNode = NULL;
	player->Physics.ActualDirection = down;
	player->Weapons.FirstShot = NULL;
	player->Stats.speed = player->Stats.SpeedMax;
}