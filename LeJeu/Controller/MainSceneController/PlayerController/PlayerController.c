#include "PlayerController.h"
#include "..\SpellsController\SpellsController.h"
#include "..\ShadersController\ShadersController.h"
#include "..\SkinsController\SkinsController.h"
#include <string.h>
#include "..\..\..\View\MapKey\MapKey.h"
#include "..\..\HUD\SkillTreeController\SkillTreeController.h"

Player *player[2] = { NULL, NULL };
PlayerGraphics playerGraphics = { 0 };
sfBool isLoaded = sfFalse;

void InitPlayerGraphics(const sfVideoMode mode)
{
	InitSkin();

	if (!isLoaded)
	{
		playerGraphics.PlayerCircle[0] = LoadSprite("Ressources/Sprites/Players/p1_area.png", 1);
		playerGraphics.PlayerCircle[1] = LoadSprite("Ressources/Sprites/Players/p2_area.png", 1);
		playerGraphics.Cursor = LoadSprite("Ressources/Sprites/Cursor/cursor.png", 1);
		playerGraphics.WetIndicator = LoadSprite("Ressources/Sprites/HUD/Silence_Icone.png", 1);
		playerGraphics.WetLightIndicator = LoadSprite("Ressources/Sprites/HUD/Silence_Icone_LightMap.png", 1);
		playerGraphics.DustParticle = LoadSprite("Ressources/Sprites/Players/Particle/projectile.png", 1);
		playerGraphics.WaveParticle = LoadSprite("Ressources/Sprites/Players/Particle/wave.png", 1);

		playerGraphics.DeathAnim = LoadAnimation(12, (6.0f / 12.0f), "Ressources/Sprites/Players/AnimDeath/", "png", (sfVector2f) { 0.0f, 0.0f }, sfFalse, 1);

		ReScaleAnimation(&playerGraphics.DeathAnim, 128.f, 304.f, mode.width / 1920.f, mode.height / 1080.f);
		for (int i = 0; i < 12; i++)
		{
			sfSprite_setOrigin(playerGraphics.DeathAnim.spriteArray[i], (sfVector2f) { 204.f, 787.f });
		}
		playerGraphics.ParticlesPlayer[0] = NULL;
		playerGraphics.ParticlesPlayer[1] = NULL;
		playerGraphics.ParticlesMovementPlayer[0] = NULL;
		playerGraphics.ParticlesMovementPlayer[1] = NULL;
		playerGraphics.ParticlesSlowPlayer[0] = NULL;
		playerGraphics.ParticlesSlowPlayer[1] = NULL;
		isLoaded = sfTrue;
	}
}

void ResetActionListener(ActionListener* toReset)
{
	toReset->ChangingWeapon = -1;
	toReset->isCastSpell = 0;
	toReset->isFiring = 0;
}

void InitPlayerPointer(const void *playerToPointTo, const int idPLayer)
{
	player[idPLayer] = (Player*)playerToPointTo;
}

Player *GetPlayerPointer(const int IdPlayer)
{
	return player[IdPlayer];
}

void TakeInputSkillTree(const sfEvent event, const int idPLayer)
{
	SkillTree* tempSkillTree = GetPointerToSkillTree(0);
	if (tempSkillTree->IsOpen)
	{
		if (event.type == sfEvtKeyPressed)
		{
			if (event.key.code == GetMapKey(MoveRight) && tempSkillTree->IndexTalent < NBRTALENTPERBRANCH - 1)
			{
				tempSkillTree->IndexTalent++;
				ChangeTextSkillTree(tempSkillTree, player[idPLayer]->Stats.XP);
			}
			else if (event.key.code == GetMapKey(MoveLeft) && tempSkillTree->IndexTalent > 0)
			{
				tempSkillTree->IndexTalent--;
				ChangeTextSkillTree(tempSkillTree, player[idPLayer]->Stats.XP);
			}
			else if (event.key.code == GetMapKey(MoveUp) && tempSkillTree->IndexBranch > 0)
			{
				tempSkillTree->IndexBranch--;
				tempSkillTree->IndexTalent = 0;
				ChangeTextSkillTree(tempSkillTree, player[idPLayer]->Stats.XP);
			}
			else if (event.key.code == GetMapKey(MoveDown) && tempSkillTree->IndexBranch < NBRBRANCH - 1)
			{
				tempSkillTree->IndexBranch++;
				tempSkillTree->IndexTalent = 0;
				ChangeTextSkillTree(tempSkillTree, player[idPLayer]->Stats.XP);
			}
			else if (event.key.code == sfKeyReturn)
			{
				int nbrPlayer = GetPlayersInformations().NumberOfPlayer;
				if (UpgradeTalent(tempSkillTree, &player[idPLayer]->Stats.XP))
				{
					ChangeTalentToUpdate(tempSkillTree->IndexBranch, tempSkillTree->IndexTalent);
					if (nbrPlayer == 2)
					{
						player[1]->SkillTree = player[0]->SkillTree;
					}
				}
				for (int i = 0; i < nbrPlayer; i++)
				{
					AssignSkillTreeToPlayer(player[i]);
				}
				ChangeTextSkillTree(tempSkillTree, player[idPLayer]->Stats.XP);
			}
		}
	}
	if (event.type == sfEvtKeyPressed)
	{
		if (event.key.code == sfKeyG)
		{
			tempSkillTree->IsOpen = !tempSkillTree->IsOpen;
			ChangeTextSkillTree(tempSkillTree, player[idPLayer]->Stats.XP);
		}
		if (tempSkillTree->IsOpen && event.key.code == sfKeyU)
		{
			int nbrPlayer = GetPlayersInformations().NumberOfPlayer;
			UpgradeAtMaxAllSkillTree(tempSkillTree);
			if (nbrPlayer == 2)
			{
				player[1]->SkillTree = player[0]->SkillTree;
			}
			for (int i = 0; i < nbrPlayer; i++)
			{
				AssignSkillTreeToPlayer(player[i]);
			}
			for (int i = 0; i < NBRBRANCH; i++)
			{
				for (int j = 0; j < NBRTALENTPERBRANCH; j++)
				{
					ChangeTalentToUpdate(i, j);
				}
			}
			ChangeTextSkillTree(tempSkillTree, player[idPLayer]->Stats.XP);
		}
	}
}

void TakeInputMovement(const sfVector2f dirMov, const int idPlayer)
{
	if (dirMov.x == 0 && dirMov.y == 0)
	{
		player[idPlayer]->States.IsIdle = sfTrue;
	}
	else
	{
		player[idPlayer]->States.IsIdle = sfFalse;
	}
	PlayerInputAffectationMovement(player[idPlayer], dirMov, 0);
}

void TakeInputViewMovement(const sfVector2f dirMov, const int idPlayer)
{
	PlayerInputAffectationViewMovement(player[idPlayer], dirMov);
}

//Manage all action receive from the view, which are everything except movement gestion
void TakeInputAction(const sfVector2f dirShot, const ActionListener actions)
{
	player[actions.IDPlayer]->States.IsAttacking = sfFalse;
	if (actions.isFiring && (dirShot.x != 0 && dirShot.y != 0))
	{
		player[actions.IDPlayer]->States.IsAttacking = sfTrue;
		PlayerFiring(player[actions.IDPlayer], dirShot);
	}

	if (actions.isDashing)
	{
		PlayerCastSpell(player[actions.IDPlayer], dirShot, Dash);
	}
	else if (actions.isCastSpell && (dirShot.x != 0 && dirShot.y != 0))
	{
		player[actions.IDPlayer]->States.IsAttacking = sfTrue;
		PlayerCastSpell(player[actions.IDPlayer], dirShot, player[actions.IDPlayer]->Spells.spellSelect);
	}

	if (player[actions.IDPlayer]->States.IsSpitFire)
	{
		player[actions.IDPlayer]->States.IsAttacking = sfTrue;
		PlayerUpdateAngleOfConeOfFire(player[actions.IDPlayer], dirShot);
	}

	if (actions.ChangingWeapon != -1)
	{
		PlayerChangeSpell(player[actions.IDPlayer], actions.ChangingWeapon);
	}
}

void UpdatePlayerController(const int numberOfPlayer, const float deltaTime)
{
	for (int i = 0; i < numberOfPlayer; i++)
	{
		if (player[i]->States.IsDashing && player[i]->Spells.DashState.TimerParticle > 0.0025f)
		{
			player[i]->Spells.DashState.TimerParticle = 0.0f;
			sfColor c1 = sfWhite;
			sfColor c2 = sfWhite;
			c2.a = 0;
			AddParticles(&playerGraphics.ParticlesPlayer[i]);
			SetParticlePosition(playerGraphics.ParticlesPlayer[i], player[i]->Physics.Rb.Position);
			SetParticleColor(playerGraphics.ParticlesPlayer[i], c1, c2);
			SetParticleTTL(playerGraphics.ParticlesPlayer[i], 0.13f);

			for (int j = 0; j < 10; j++)
			{
				AddParticles(&playerGraphics.ParticlesMovementPlayer[i]);
				sfVector2f dir = { -player[i]->Physics.Direction.x, -player[i]->Physics.Direction.y };
				float rotateAngle = DegreeToRadian(rand() % 45 - 22.5f);
				dir = Rotate(dir, rotateAngle);
				float distance = rand() / (float)RAND_MAX * 10.f + 5.f;
				sfVector2f posPlayer = { player[i]->Physics.Rb.Position.x + distance * dir.x, player[i]->Physics.Rb.Position.y + 50.f + distance * dir.y };
				SetParticlePosition(playerGraphics.ParticlesMovementPlayer[i], posPlayer);
				SetParticleColor(playerGraphics.ParticlesMovementPlayer[i], sfColor_fromRGB(150, 150, 150), sfTransparent);
				SetParticleTTL(playerGraphics.ParticlesMovementPlayer[i], 0.754f);
				float scale = rand() / (float)RAND_MAX * 0.1f + 0.05f;
				sfVector2f scale1 = { scale, scale };
				sfVector2f scale2 = { 0.f, 0.f };
				SetParticleScale(playerGraphics.ParticlesMovementPlayer[i], scale1, scale2);

				SetParticleDirection(playerGraphics.ParticlesMovementPlayer[i], dir);
				SetParticleSpeed(playerGraphics.ParticlesMovementPlayer[i], 1.f);
				sfVector2f delta = { 0.f, -(rand() / (float)RAND_MAX *50.f + 50.f) };
				delta = Rotate(delta, rotateAngle);
				SetParticleDelta(playerGraphics.ParticlesMovementPlayer[i], delta);
			}
		}
		else
		{
			player[i]->Spells.DashState.TimerParticle += deltaTime;
		}

		if (!player[i]->States.IsDashing && !player[i]->States.IsIdle && player[i]->Physics.TimerMovementParticles < 0.f)
		{
			if (player[i]->ActualNode->Mud)
			{
				player[i]->Physics.TimerMovementParticles = 0.15f;
				AddParticles(&playerGraphics.ParticlesSlowPlayer[i]);
				sfVector2f dir = { -player[i]->Physics.Direction.x, -player[i]->Physics.Direction.y };
				sfVector2f posPlayer = { player[i]->Physics.Rb.Position.x - dir.x * 12.f, player[i]->Physics.Rb.Position.y + 64.f - dir.y * 12.f };
				SetParticlePosition(playerGraphics.ParticlesSlowPlayer[i], posPlayer);
				//sfColor_fromRGB(49, 66, 20)
				SetParticleColor(playerGraphics.ParticlesSlowPlayer[i], sfWhite, sfTransparent);
				SetParticleTTL(playerGraphics.ParticlesSlowPlayer[i], 0.75f);
				float scale = 1.6f;
				sfVector2f scale1 = { scale, scale };
				sfVector2f scale2 = { 1.58f, 1.58f };
				SetParticleScale(playerGraphics.ParticlesSlowPlayer[i], (sfVector2f) { 1.58f, 1.58f }, (sfVector2f) { 1.6f, 1.6f });

				//SetParticleDirection(playerGraphics.ParticlesMovementPlayer[i], dir);
				//SetParticleSpeed(playerGraphics.ParticlesMovementPlayer[i], 3.f);
			}
			else if (player[i]->ActualNode->Ice)
			{
				player[i]->Physics.TimerMovementParticles = 0.150f;
				for (int j = 0; j < 5; j++)
				{
					AddParticles(&playerGraphics.ParticlesMovementPlayer[i]);
					sfVector2f dir = { -player[i]->Physics.Direction.x, -player[i]->Physics.Direction.y };
					float rotateAngle = DegreeToRadian((float)(rand() % 30 - 20));
					dir = Rotate(dir, rotateAngle);
					float distance = rand() / (float)RAND_MAX * 20.f + 5.f;
					sfVector2f posPlayer = { player[i]->Physics.Rb.Position.x + distance * dir.x, player[i]->Physics.Rb.Position.y + 50.f + distance * dir.y };
					SetParticlePosition(playerGraphics.ParticlesMovementPlayer[i], posPlayer);

					SetParticleColor(playerGraphics.ParticlesMovementPlayer[i], sfColor_fromRGB(178, 221, 240), sfTransparent);
					SetParticleTTL(playerGraphics.ParticlesMovementPlayer[i], 1.5f);
					float scale = rand() / (float)RAND_MAX * 0.15f + 0.15f;
					sfVector2f scale1 = { scale, scale / 4.f };
					sfVector2f scale2 = { 0.f, 0.f };
					SetParticleScale(playerGraphics.ParticlesMovementPlayer[i], scale1, scale2);
					dir.x *= rand() % 200 + 100;
					dir.y *= rand() % 100 + 200;
					SetParticleDirection(playerGraphics.ParticlesMovementPlayer[i], dir);
					SetParticleSpeed(playerGraphics.ParticlesMovementPlayer[i], 3.f);
				}
				/*sfVector2f delta = { 0.f, -(rand() / (float)RAND_MAX * 50.f + 50.f) };
				delta = Rotate(delta, rotateAngle);
				SetParticleDelta(playerGraphics.ParticlesMovementPlayer[i], delta);*/
			}
			else
			{
				player[i]->Physics.TimerMovementParticles = 0.150f;
				for (int j = 0; j < 5; j++)
				{
					AddParticles(&playerGraphics.ParticlesMovementPlayer[i]);
					sfVector2f dir = { -player[i]->Physics.Direction.x, -player[i]->Physics.Direction.y };
					float rotateAngle = DegreeToRadian(rand() % 45 - 22.5f);
					dir = Rotate(dir, rotateAngle);
					float distance = rand() / (float)RAND_MAX * 20.f + 5.f;
					sfVector2f posPlayer = { player[i]->Physics.Rb.Position.x + distance * dir.x, player[i]->Physics.Rb.Position.y + 50.f + distance * dir.y };
					SetParticlePosition(playerGraphics.ParticlesMovementPlayer[i], posPlayer);
					SetParticleColor(playerGraphics.ParticlesMovementPlayer[i], sfColor_fromRGB(150, 150, 150), sfTransparent);
					SetParticleTTL(playerGraphics.ParticlesMovementPlayer[i], 0.754f);
					float scale = rand() / (float)RAND_MAX * 0.15f + 0.15f;
					sfVector2f scale1 = { scale, scale };
					sfVector2f scale2 = { 0.f, 0.f };
					SetParticleScale(playerGraphics.ParticlesMovementPlayer[i], scale1, scale2);

					SetParticleDirection(playerGraphics.ParticlesMovementPlayer[i], dir);
					SetParticleSpeed(playerGraphics.ParticlesMovementPlayer[i], 1.f);
					sfVector2f delta = { 0.f, -(rand() / (float)RAND_MAX * 50.f + 50.f) };
					delta = Rotate(delta, rotateAngle);
					SetParticleDelta(playerGraphics.ParticlesMovementPlayer[i], delta);
				}
			}
		}
		else
		{
			player[i]->Physics.TimerMovementParticles -= deltaTime;
		}

		UpdateParticles(&playerGraphics.ParticlesPlayer[i], deltaTime);
		UpdateParticles(&playerGraphics.ParticlesSlowPlayer[i], deltaTime);
		
		UpdateParticles(&playerGraphics.ParticlesMovementPlayer[i], deltaTime);

		if (player[i]->States.IsSpitFire)
		{
			UpdateAnimation(&GetSpellsPtr()->coneOfFire, deltaTime);
		}

		if (!player[i]->States.IsIdle && !player[i]->States.IsDashing)
		{
			UpdateAnimation(&playerGraphics.AnimationMovement[i][player[i]->Physics.ActualDirection], deltaTime);
		}
	}
}

void DisplayPlayers(sfRenderWindow* mainWindow, const Camera Cam, const int numberOfPlayer)
{
	for (int i = 0; i < numberOfPlayer; i++)
	{
		if (!player[i]->States.IsTeleporting && !GetTransitioning())
		{
			sfShader* temp = NULL;

			if (player[i]->States.IsDead)
			{
				BlitSprite(mainWindow, playerGraphics.DeathAnim.spriteArray[player[i]->DeathAnimation.actualFrameToShow], player[i]->Physics.Rb.Position, NULL);
			}

			if (DrawOnCamera(Cam, playerGraphics.AnimationMovement[i][up].spriteArray[0], player[i]->Physics.Rb.Position))
			{
				ShadersContainer shaderList = GetShaderList();
				if (player[i]->States.IsWet)
				{
					BlitSprite(mainWindow, playerGraphics.WetIndicator, (sfVector2f) { player[i]->Physics.Rb.Position.x, (player[i]->Physics.Rb.Position.y - PLAYERHEIGHT) }, NULL);
					BlitRenderTexture(mainWindow, GetLightTexture(), playerGraphics.WetLightIndicator, (sfVector2f) { player[i]->Physics.Rb.Position.x, (player[i]->Physics.Rb.Position.y - PLAYERHEIGHT) });
				}

				if (player[i]->Spells.MineState.IsActivate)
				{
					MageSpell spells = GetSpells();
					sfColor col = sfWhite;
					if (GetPlayersInformations().SkinPlayer[i] == red)
					{
						col = sfColor_fromRGB(100, 0, 0);
					}
					if (GetPlayersInformations().SkinPlayer[i] == black)
					{
						col = sfColor_fromRGB(0, 0, 100);
					}
					if (GetPlayersInformations().SkinPlayer[i] == green)
					{
						col = sfColor_fromRGB(0, 100, 0);
					}

					sfSprite_setColor(spells.Mine.spriteArray[player[i]->Spells.MineState.AnimKey.actualFrameToShow], col);
					sfSprite_setRotation(spells.Mine.spriteArray[player[i]->Spells.MineState.AnimKey.actualFrameToShow], player[i]->Spells.MineState.Rotate);
					BlitSprite(mainWindow, spells.Mine.spriteArray[player[i]->Spells.MineState.AnimKey.actualFrameToShow], player[i]->Spells.MineState.Pos, NULL);
				}

				if (!player[i]->States.IsDead)
				{
					SetLigthPosition(i, player[i]->Physics.Rb.Position, player[i]->ActualNode);
					if (numberOfPlayer > 1)
					{
						sfVector2f posAureole = { player[i]->Physics.Rb.Position.x, player[i]->Physics.Rb.Position.y + PLAYERHEIGHT / 3.f };
						BlitSprite(mainWindow, playerGraphics.PlayerCircle[i], posAureole, NULL);
					}
				}

				if (player[i]->Stats.TimerInvulnerability > 0.f && !player[i]->States.IsDashing)
				{
					if ((int)(player[i]->Stats.TimerInvulnerability * 10) % 2 == 0)
					{
						sfShader_setFloatUniform(shaderList.Flash, "alpha", 1);
					}
				}

				/////////ALMOST SAFE

				if (player[i]->States.IsSlowDown)
				{
					sfShader_setFloatUniform(shaderList.Purple, "alpha", 1);
					temp = shaderList.Purple;
				}

				if (player[i]->Stats.TimerInvulnerability > 0.f && !player[i]->States.IsDashing)
				{
					if ((int)(player[i]->Stats.TimerInvulnerability * 10) % 2 != 0)
					{
						sfShader_setFloatUniform(shaderList.Flash, "alpha", 1);
						temp = shaderList.Flash;
					}
				}


				DisplayParticles(playerGraphics.ParticlesMovementPlayer[i], playerGraphics.DustParticle, mainWindow, NULL, NULL);
				DisplayParticles(playerGraphics.ParticlesSlowPlayer[i], playerGraphics.WaveParticle, mainWindow, NULL, NULL);
				DisplayParticles(playerGraphics.ParticlesPlayer[i],
					playerGraphics.AnimationMovement[i][player[i]->Physics.ActualDirection].spriteArray[playerGraphics.AnimationMovement[i][player[i]->Physics.ActualDirection].actualFrameToShow],
					mainWindow, temp, NULL);
				if (temp)
				{
					sfShader_setFloatUniform(temp, "alpha", 1);
				}


				if (!player[i]->States.IsDead)
				{
					sfSprite_setColor(playerGraphics.AnimationMovement[i][player[i]->Physics.ActualDirection].spriteArray[playerGraphics.AnimationMovement[i][player[i]->Physics.ActualDirection].actualFrameToShow], sfWhite);
					if (player[i]->States.IsIdle)
					{
						BlitSprite(mainWindow, playerGraphics.AnimationIdle[i][player[i]->Physics.ActualDirection], player[i]->Physics.Rb.Position, temp);
					}
					else
					{
						BlitSprite(mainWindow, playerGraphics.AnimationMovement[i][player[i]->Physics.ActualDirection].spriteArray[playerGraphics.AnimationMovement[i][player[i]->Physics.ActualDirection].actualFrameToShow], player[i]->Physics.Rb.Position, temp);
					}


					if (player[i]->States.IsSpitFire)
					{
						MageSpell spells = GetSpells();
						sfColor col = sfWhite;
						if (GetPlayersInformations().SkinPlayer[i] == red)
						{
							col = sfColor_fromRGB(100, 0, 0);
						}
						if (GetPlayersInformations().SkinPlayer[i] == black)
						{
							col = sfColor_fromRGB(0, 0, 100);
						}
						if (GetPlayersInformations().SkinPlayer[i] == green)
						{
							col = sfColor_fromRGB(0, 100, 0);
						}

						sfSprite_setColor(spells.coneOfFire.spriteArray[spells.coneOfFire.actualFrameToShow], col);
						sfSprite_setRotation(spells.coneOfFire.spriteArray[spells.coneOfFire.actualFrameToShow], player[i]->Spells.ConeState.Rotate);

						BlitSprite(mainWindow, spells.coneOfFire.spriteArray[spells.coneOfFire.actualFrameToShow], player[i]->Physics.Rb.Position, NULL);
					}
#if _DEBUG
					static sfBool toShow = sfFalse;


					if (sfKeyboard_isKeyPressed(sfKeyLShift) && sfKeyboard_isKeyPressed(sfKeyC))
					{
						toShow = sfTrue;
					}
					else if (sfKeyboard_isKeyPressed(sfKeyC))
					{
						toShow = sfFalse;
					}

					if (toShow)
					{
						sfCircleShape_setPosition(player[i]->Shape, player[i]->Physics.Collider.center);
						sfRenderWindow_drawCircleShape(mainWindow, player[i]->Shape, NULL);
					}
#endif // _DEBUG

				}
			}
		}
	}

}

PlayerGraphics* GetPlayerGraphics()
{
	return &playerGraphics;
}

void SetAnimationMovementPlayerGraphics(const char skinPlayer, const char* path, const int indexPlayer)
{
	sfVector2f pos = { 0,0 };
	char* direction[4] = { "Red", "Black", "White", "Green" };
	char directoryOfimage[255];
	sprintf(directoryOfimage, "%s/Down/%s/", path, direction[skinPlayer]);
	playerGraphics.AnimationMovement[indexPlayer][down] = LoadAnimation(24, (24.0f / 30.0f), directoryOfimage, "png", pos, sfTrue, 1);
	strcpy(directoryOfimage, "");
	sprintf(directoryOfimage, "%s/Up/%s/", path, direction[skinPlayer]);
	playerGraphics.AnimationMovement[indexPlayer][up] = LoadAnimation(24, (24.0f / 30.0f), directoryOfimage, "png", pos, sfTrue, 1);
	strcpy(directoryOfimage, "");
	sprintf(directoryOfimage, "%s/Right/%s/", path, direction[skinPlayer]);
	playerGraphics.AnimationMovement[indexPlayer][right] = LoadAnimation(24, (24.0f / 30.0f), directoryOfimage, "png", pos, sfTrue, 1);
	strcpy(directoryOfimage, "");
	sprintf(directoryOfimage, "%s/Left/%s/", path, direction[skinPlayer]);
	playerGraphics.AnimationMovement[indexPlayer][left] = LoadAnimation(24, (24.0f / 30.0f), directoryOfimage, "png", pos, sfTrue, 1);
}

void SetAnimationIdlePlayerGraphics(const char skinPlayer, const char* path, const int indexPlayer)
{
	sfVector2f pos = { 0,0 };
	char* direction[4] = { "Red", "Black", "White", "Green" };
	char directoryOfimage[255];
	sprintf(directoryOfimage, "%s/Down/%s/0000.png", path, direction[skinPlayer]);
	playerGraphics.AnimationIdle[indexPlayer][down] = LoadSprite(directoryOfimage, 1);
	strcpy(directoryOfimage, "");
	sprintf(directoryOfimage, "%s/Up/%s/0000.png", path, direction[skinPlayer]);
	playerGraphics.AnimationIdle[indexPlayer][up] = LoadSprite(directoryOfimage, 1);
	strcpy(directoryOfimage, "");
	sprintf(directoryOfimage, "%s/Right/%s/0000.png", path, direction[skinPlayer]);
	playerGraphics.AnimationIdle[indexPlayer][right] = LoadSprite(directoryOfimage, 1);
	strcpy(directoryOfimage, "");
	sprintf(directoryOfimage, "%s/Left/%s/0000.png", path, direction[skinPlayer]);
	playerGraphics.AnimationIdle[indexPlayer][left] = LoadSprite(directoryOfimage, 1);
}

void AllocateAnimation(const int numberOfPlayer)
{
	playerGraphics.AnimationMovement = (Animation**)calloc(numberOfPlayer, sizeof(Animation*));
	playerGraphics.AnimationIdle = (sfSprite***)calloc(numberOfPlayer, sizeof(sfSprite**));
	for (int i = 0; i < numberOfPlayer; i++)
	{
		playerGraphics.AnimationMovement[i] = (Animation*)calloc(4, sizeof(Animation));
		playerGraphics.AnimationIdle[i] = (sfSprite**)calloc(4, sizeof(sfSprite*));
	}
}

void UnloadAnimationPlayerGraphics(const int numberOfPlayer)
{
	for (int j = 0; j < numberOfPlayer; j++)
	{
		for (int i = 0; i < 4; i++)
		{
			UnLoadAnimation(&playerGraphics.AnimationMovement[j][i]);
		}
		for (int i = 0; i < 4; i++)
		{
			sfTexture_destroy((sfTexture*)sfSprite_getTexture(playerGraphics.AnimationIdle[j][i]));
			sfSprite_destroy(playerGraphics.AnimationIdle[j][i]);
		}
	}
}

void DisplayCursor(const int numberOfPlayer, sfRenderWindow* mainWindow, const sfView* view)
{
	for (int i = 0; i < numberOfPlayer; i++)
	{
		if (!player[i]->TypeInput && !player[i]->States.IsDead)
		{
			sfVector2i vec = sfRenderWindow_mapCoordsToPixel(mainWindow, player[i]->Hud.Cursor.Position, view);
			sfSprite_setScale(playerGraphics.Cursor, player[i]->Hud.Cursor.Scale);
			BlitSprite(mainWindow, playerGraphics.Cursor, (sfVector2f) { (float)vec.x, (float)vec.y }, NULL);
		}

	}
}