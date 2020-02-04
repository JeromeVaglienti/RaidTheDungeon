#include <Windows.h>
#include "BossController.h"
#include "..\ShadersController\ShadersController.h"
#include <Windows.h>

Animation* AnimationBoss = NULL;
Animation* AnimationBossLayer1 = NULL;
sfSprite* LayerTwoMoins = { 0 };

sfSprite* light = { 0 };

void InitBossControllerSlime()
{
	AnimationBoss = (Animation*)calloc(3, sizeof(Animation));
	AnimationBoss[0] = LoadAnimation(18, (18.f / 12.f), "Ressources/Sprites/Boss/Dungeon1/p1/Slim-boss_", "png", (sfVector2f) { 0 }, 1, 1);
	AnimationBoss[1] = LoadAnimation(29, (29.f / 12.f), "Ressources/Sprites/Boss/Dungeon1/p2/Slim-boss-p2_", "png", (sfVector2f) { 0 }, 1, 1);
	AnimationBoss[2] = LoadAnimation(18, (18.f / 12.f), "Ressources/Sprites/Boss/Dungeon1/death/Slim-boss_", "png", (sfVector2f) { 0 }, 0, 1);
}

void InitBossControllerRazar()
{
	AnimationBoss = (Animation*)calloc(3, sizeof(Animation));
	AnimationBoss[0] = LoadAnimation(33, (33.f / 12.f), "Ressources/Sprites/Boss/Razar/Idle/", "png", (sfVector2f) { 0 }, 1, 1);
	AnimationBoss[1] = LoadAnimation(33, (33.f / 24.f), "Ressources/Sprites/Boss/Razar/Transition/", "png", (sfVector2f) { 0 }, 0, 1);
	AnimationBoss[2] = LoadAnimation(33, (33.f / 12.f), "Ressources/Sprites/Boss/Razar/Vol/", "png", (sfVector2f) { 0 }, 1, 1);
	sfVector2f scale = { 0.5f * 1449.f / 962.f, 0.5f * 1449.f / 962.f };

	for (int i = 0; i < 33; i++)
	{
		sfSprite_setScale(AnimationBoss[0].spriteArray[i], scale);
		sfSprite_setScale(AnimationBoss[1].spriteArray[i], scale);
		sfSprite_setScale(AnimationBoss[2].spriteArray[i], scale);
	}
}

void InitBossControllerElem()
{
	//InitBossControllerRazar();
	LayerTwoMoins = LoadSprite("Ressources/Sprites/Environment/Dungeon2/Rooms/ID_10/cloud.png", 0);
	AnimationBoss = (Animation*)calloc(7, sizeof(Animation));
	AnimationBossLayer1 = (Animation*)calloc(7, sizeof(Animation));
	AnimationBoss[0] = LoadAnimation(33, 1.8f, "Ressources/Sprites/Boss/Dungeon2/Boss_Spawn/Layer-1/Armature_Boss_Spawn_", "png", (sfVector2f) { 0 }, 0, 1);
	AnimationBossLayer1[0] = LoadAnimation(33, 1.8f, "Ressources/Sprites/Boss/Dungeon2/Boss_Spawn/Layer+1/Armature_Boss_Spawn_", "png", (sfVector2f) { 0 }, 0, 1);
	AnimationBoss[1] = LoadAnimation(33, 2.f, "Ressources/Sprites/Boss/Dungeon2/Rock_attack/Layer-1/Armature_Rock_attack_", "png", (sfVector2f) { 12.f*1.75f, -(246.f*1.75f) }, 1, 1);
	AnimationBossLayer1[1] = LoadAnimation(33, 2.f, "Ressources/Sprites/Boss/Dungeon2/Rock_attack/Layer+1/Rock_Attack+FX_", "png", (sfVector2f) { 12.f*1.75f, -(246.f*1.75f) }, 1, 1);
	AnimationBoss[5] = LoadAnimation(33, 2.4f, "Ressources/Sprites/Boss/Dungeon2/Idle/Layer-1/Armature_Idle_", "png", (sfVector2f) { 8.f*1.75f, -(256.f*1.75f) }, 1, 1);
	AnimationBossLayer1[5] = LoadAnimation(33, 2.4f, "Ressources/Sprites/Boss/Dungeon2/Idle/Layer+1/Armature_Idle_", "png", (sfVector2f) { 8.f*1.75f, -(256.f*1.75f) }, 1, 1);
	light = LoadSprite("Ressources/Sprites/Boss/Dungeon2/light/head_light.png", 1);

	AnimationBoss[2].isLoaded = sfFalse;
	AnimationBoss[3].isLoaded = sfFalse;
	AnimationBoss[4].isLoaded = sfFalse;
	AnimationBoss[6].isLoaded = sfFalse;

	for (int i = 0; i < 33; i++)
	{
		sfSprite_setScale(AnimationBoss[0].spriteArray[i], (sfVector2f) { 1.75f, 1.75f });
		sfSprite_setScale(AnimationBossLayer1[0].spriteArray[i], (sfVector2f) { 1.75f, 1.75f });
		sfSprite_setScale(AnimationBoss[1].spriteArray[i], (sfVector2f) { 1.75f, 1.75f });
		sfSprite_setScale(AnimationBossLayer1[1].spriteArray[i], (sfVector2f) { 1.75f, 1.75f });
		sfSprite_setScale(AnimationBoss[5].spriteArray[i], (sfVector2f) { 1.75f, 1.75f });
		sfSprite_setScale(AnimationBossLayer1[5].spriteArray[i], (sfVector2f) { 1.75f, 1.75f });
	}

	sfSprite_setScale(light, (sfVector2f) { 1.75f, 1.75f });
}

DWORD WINAPI LoadBossElemAnim(void* data)
{
	printf("%d\n", (*(int*)data));
	int idUnload = (*(int*)data) - 1;
	printf("id Unload %d\n", idUnload);

	if (!AnimationBoss[idUnload].isLoaded)
	{
		free(data);
		return 0;
	}

	UnLoadAnimation(&AnimationBoss[idUnload]);
	UnLoadAnimation(&AnimationBossLayer1[idUnload]);

	int id;
	if ((*(int*)data) == 4)
	{
		id = idUnload + 3;
	}
	else
	{
		id = idUnload + 2;
	}

	if (id > 6 || AnimationBoss[id].isLoaded)
	{
		free(data);
		return 0;
	}

	char* Paths[7][2] =
	{
		{ "Ressources/Sprites/Boss/Dungeon2/Boss_Spawn/Layer-1/Armature_Boss_Spawn_" , "Ressources/Sprites/Boss/Dungeon2/Boss_Spawn/Layer+1/Armature_Boss_Spawn_" },
		{"Ressources/Sprites/Boss/Dungeon2/Rock_attack/Layer-1/Armature_Rock_attack_", "Ressources/Sprites/Boss/Dungeon2/Rock_attack/Layer+1/Rock_Attack+FX_" },
		{ "Ressources/Sprites/Boss/Dungeon2/Storm_attack/Layer-1/Armature_Storm_attack_" , "Ressources/Sprites/Boss/Dungeon2/Storm_attack/Layer+1/Storm_attack+FX_" },
		{ "Ressources/Sprites/Boss/Dungeon2/Ice_attack/Layer-1/Armature_Ice_attack_" , "Ressources/Sprites/Boss/Dungeon2/Ice_attack/Layer+1/Ice_Attack+FX_" },
		{ "Ressources/Sprites/Boss/Dungeon2/Lava_attack/Layer-1/Armature_Lava_attack_" , "Ressources/Sprites/Boss/Dungeon2/Lava_attack/Layer+1/Lava_attack+FX_" },
		{ "Ressources/Sprites/Boss/Dungeon2/Idle/Layer-1/Armature_Idle_" , "Ressources/Sprites/Boss/Dungeon2/Idle/Layer+1/Armature_Idle_" },
		{ "Ressources/Sprites/Boss/Dungeon2/Boss_Death/Layer-1/Armature_Boss_Death_" , "Ressources/Sprites/Boss/Dungeon2/Boss_Death/Layer+1/Armature_Boss_Death_" }
	};

	float Time[7] = { 1.8f, 2.0f, 2.0f, 2.0f, 2.0f, 2.4f, 3.0f };
	sfBool isLoop[7] = { sfFalse, sfTrue, sfTrue, sfTrue, sfTrue, sfTrue, sfFalse };
	sfVector2f posOffset[7] = { {0.f},{ 12.f*1.75f, -(246.f*1.75f) }, {-32.f*1.75f, -(293.f * 1.75f)}, {-7.f*1.75f, -(264.f * 1.75f)},{ 50.f*1.75f, -(261.f*1.75f) },{ 8.f*1.75f, -(256.f*1.75f) },{0.f} };

	int nbFrame = id == 6 ? 37 : 33;
	AnimationBoss[id] = LoadAnimation(nbFrame, Time[id], Paths[id][0],"png", posOffset[id], isLoop[id], 1);
	AnimationBossLayer1[id] = LoadAnimation(nbFrame, Time[id], Paths[id][1], "png", posOffset[id], isLoop[id], 1);

	for (int i = 0; i < nbFrame; i++)
	{
		sfSprite_setScale(AnimationBoss[id].spriteArray[i], (sfVector2f) { 1.75f, 1.75f });
		sfSprite_setScale(AnimationBossLayer1[id].spriteArray[i], (sfVector2f) { 1.75f, 1.75f });
	}

	free(data);
	return 0;
}


//Recharge Boss First Phases
void ManageBossControllerElem()
{
	if (LayerTwoMoins == NULL)
		LayerTwoMoins = LoadSprite("Ressources/Sprites/Environment/Dungeon2/Rooms/ID_10/cloud.png", 0);

	if (!AnimationBoss[0].isLoaded)
	{
		AnimationBoss[0] = LoadAnimation(33, 1.8f, "Ressources/Sprites/Boss/Dungeon2/Boss_Spawn/Layer-1/Armature_Boss_Spawn_", "png", (sfVector2f) { 0 }, 0, 1);
		AnimationBossLayer1[0] = LoadAnimation(33, 1.8f, "Ressources/Sprites/Boss/Dungeon2/Boss_Spawn/Layer+1/Armature_Boss_Spawn_", "png", (sfVector2f) { 0 }, 0, 1);

		for (int i = 0; i < 33; i++)
		{
			sfSprite_setScale(AnimationBoss[0].spriteArray[i], (sfVector2f) { 1.75f, 1.75f });
			sfSprite_setScale(AnimationBossLayer1[0].spriteArray[i], (sfVector2f) { 1.75f, 1.75f });
		}
	}

	if (!AnimationBoss[1].isLoaded)
	{
		AnimationBoss[1] = LoadAnimation(33, 2.f, "Ressources/Sprites/Boss/Dungeon2/Rock_attack/Layer-1/Armature_Rock_attack_", "png", (sfVector2f) { 12.f*1.75f, -(246.f*1.75f) }, 1, 1);
		AnimationBossLayer1[1] = LoadAnimation(33, 2.f, "Ressources/Sprites/Boss/Dungeon2/Rock_attack/Layer+1/Rock_Attack+FX", "png", (sfVector2f) { 12.f*1.75f, -(246.f*1.75f) }, 1, 1);

		for (int i = 0; i < 33; i++)
		{
			sfSprite_setScale(AnimationBoss[1].spriteArray[i], (sfVector2f) { 1.75f, 1.75f });
			sfSprite_setScale(AnimationBossLayer1[1].spriteArray[i], (sfVector2f) { 1.75f, 1.75f });
		}
	}

	if (!AnimationBoss[5].isLoaded)
	{
		AnimationBoss[5] = LoadAnimation(33, 2.4f, "Ressources/Sprites/Boss/Dungeon2/Idle/Layer-1/Armature_Idle_","png", (sfVector2f) { 8.f*1.75f, -(256.f*1.75f) }, 1, 1);
		AnimationBossLayer1[5] = LoadAnimation(33, 2.4f, "Ressources/Sprites/Boss/Dungeon2/Idle/Layer+1/Armature_Idle_", "png", (sfVector2f) { 8.f*1.75f, -(256.f*1.75f) }, 1, 1);

		for (int i = 0; i < 33; i++)
		{
			sfSprite_setScale(AnimationBoss[5].spriteArray[i], (sfVector2f) { 1.75f, 1.75f });
			sfSprite_setScale(AnimationBossLayer1[5].spriteArray[i], (sfVector2f) { 1.75f, 1.75f });
		}
	}

}

void UnloadBossControllerSlime()
{
	if (AnimationBoss != NULL)
	{
		for (int i = 0; i < 3; i++)
		{
			UnLoadAnimation(&AnimationBoss[i]);
		}

		free(AnimationBoss);

		AnimationBoss = NULL;

	}
	if (AnimationBossLayer1 != NULL)
	{
		for (int i = 0; i < 3; i++)
		{
			UnLoadAnimation(&AnimationBossLayer1[i]);
		}
		free(AnimationBossLayer1);
		AnimationBossLayer1 = NULL;
	}
}

void UnloadBossControllerElem()
{
	if (AnimationBoss != NULL)
	{
		for (int i = 0; i < 7; i++)
		{
			if (AnimationBoss[i].isLoaded)
			{
				UnLoadAnimation(&AnimationBoss[i]);
			}
		}

		free(AnimationBoss);

		AnimationBoss = NULL;

	}
	if (AnimationBossLayer1 != NULL)
	{
		for (int i = 0; i < 7; i++)
		{
			if (AnimationBossLayer1[i].isLoaded)
			{
				UnLoadAnimation(&AnimationBossLayer1[i]);
			}
		}
		free(AnimationBossLayer1);
		AnimationBossLayer1 = NULL;
	}

	if (LayerTwoMoins != NULL)
	{
		DestroySfSprite(&LayerTwoMoins);
	}
}

void UnloadBossControllerRazar()
{
	return;
	if (AnimationBoss != NULL)
	{
		for (int i = 0; i < 7; i++)
		{
			UnLoadAnimation(&AnimationBoss[i]);
		}

		free(AnimationBoss);

		AnimationBoss = NULL;

	}
	if (AnimationBossLayer1 != NULL)
	{
		for (int i = 0; i < 7; i++)
		{
			UnLoadAnimation(&AnimationBossLayer1[i]);
		}
		free(AnimationBossLayer1);
		AnimationBossLayer1 = NULL;
	}
}

void UpdateAnimationSyncWithAttack(Boss* boss, const float deltaTime)
{
	if (GetActualDungeonId() == elemDungeon)
	{
		if (boss->RealPhase == p1)
		{
			if (boss->CooldownShot[4] <= 0.f)
			{
				boss->CooldownShot[4] = 9.f;
				float pouet = ((float)(rand() / (float)RAND_MAX) * 100);
				boss->PaternChoosen = pouet > 50.f ? 1 : 0;
			}
			if (boss->CooldownShot[4] > 0.f && boss->CooldownShot[4] < 6.f)
			{
				if (boss->CurrentPhase == p5)
				{
					AnimationBoss[boss->CurrentPhase].actualFrameToShow = 0;
					AnimationBoss[boss->CurrentPhase].actualDuration = 0.f;
					AnimationBossLayer1[boss->CurrentPhase].actualFrameToShow = 0;
					AnimationBossLayer1[boss->CurrentPhase].actualDuration = 0;
					boss->CurrentPhase = p1;
				}
			}
			else
			{
				if (boss->CurrentPhase == p1)
				{
					AnimationBoss[boss->CurrentPhase].actualFrameToShow = 0;
					AnimationBoss[boss->CurrentPhase].actualDuration = 0.f;
					AnimationBossLayer1[boss->CurrentPhase].actualFrameToShow = 0;
					AnimationBossLayer1[boss->CurrentPhase].actualDuration = 0;
					boss->IsAttacking = sfFalse;
					boss->CurrentPhase = p5;
				}
			}
		}
		else if (boss->RealPhase == p2)
		{
			if (boss->CooldownShot[4] <= 0.f)
			{
				boss->CooldownShot[4] = 9.f;
				float pouet = ((float)(rand() / (float)RAND_MAX) * 100);
				boss->PaternChoosen = pouet > 50.f ? 1 : 0;
			}
			if (boss->CooldownShot[4] > 0.f && boss->CooldownShot[4] < 6.f)
			{
				if (boss->CurrentPhase == p5)
				{
					AnimationBoss[boss->CurrentPhase].actualFrameToShow = 0;
					AnimationBoss[boss->CurrentPhase].actualDuration = 0.f;
					AnimationBossLayer1[boss->CurrentPhase].actualFrameToShow = 0;
					AnimationBossLayer1[boss->CurrentPhase].actualDuration = 0;
					boss->CurrentPhase = p2;
				}
			}
			else
			{
				if (boss->CurrentPhase == p2)
				{
					AnimationBoss[boss->CurrentPhase].actualFrameToShow = 0;
					AnimationBoss[boss->CurrentPhase].actualDuration = 0.f;
					AnimationBossLayer1[boss->CurrentPhase].actualFrameToShow = 0;
					AnimationBossLayer1[boss->CurrentPhase].actualDuration = 0;
					boss->IsAttacking = sfFalse;
					boss->CurrentPhase = p5;
				}
			}
		}
		else if (boss->RealPhase == p3)
		{
			if (boss->CooldownShot[4] <= 0.f)
			{
				boss->CooldownShot[4] = 8.f;
			}
			if (boss->CooldownShot[4] > 0.f && boss->CooldownShot[4] < 6.f)
			{
				if (boss->CurrentPhase == p5)
				{
					AnimationBoss[boss->CurrentPhase].actualFrameToShow = 0;
					AnimationBoss[boss->CurrentPhase].actualDuration = 0.f;
					AnimationBossLayer1[boss->CurrentPhase].actualFrameToShow = 0;
					AnimationBossLayer1[boss->CurrentPhase].actualDuration = 0;
					boss->CurrentPhase = p3;
				}
			}
			else
			{
				if (boss->CurrentPhase == p3)
				{
					AnimationBoss[boss->CurrentPhase].actualFrameToShow = 0;
					AnimationBoss[boss->CurrentPhase].actualDuration = 0.f;
					AnimationBossLayer1[boss->CurrentPhase].actualFrameToShow = 0;
					AnimationBossLayer1[boss->CurrentPhase].actualDuration = 0;
					boss->IsAttacking = sfFalse;
					boss->CurrentPhase = p5;
				}
			}
		}
		else if (boss->RealPhase == p4)
		{
			if (boss->CooldownShot[4] <= 0.f)
			{
				boss->CooldownShot[4] = 8.f;
				float pouet = ((float)(rand() / (float)RAND_MAX) * 100);
				boss->PaternChoosen = 0;//pouet > 50.f ? 1 : 0;
			}
			if (boss->CooldownShot[4] > 0.f && boss->CooldownShot[4] < 6.f)
			{
				if (boss->CurrentPhase == p5)
				{
					AnimationBoss[boss->CurrentPhase].actualFrameToShow = 0;
					AnimationBoss[boss->CurrentPhase].actualDuration = 0.f;
					AnimationBossLayer1[boss->CurrentPhase].actualFrameToShow = 0;
					AnimationBossLayer1[boss->CurrentPhase].actualDuration = 0;
					boss->CurrentPhase = p4;
				}
			}
			else
			{
				if (boss->CurrentPhase == p4)
				{
					AnimationBoss[boss->CurrentPhase].actualFrameToShow = 0;
					AnimationBoss[boss->CurrentPhase].actualDuration = 0.f;
					AnimationBossLayer1[boss->CurrentPhase].actualFrameToShow = 0;
					AnimationBossLayer1[boss->CurrentPhase].actualDuration = 0;
					boss->IsAttacking = sfFalse;
					boss->CurrentPhase = p5;
				}
			}
		}
		else if (boss->RealPhase == idleBoss)
		{
			//if (boss->CurrentPhase > idleBoss)
			//{
			/*AnimationBoss[boss->CurrentPhase].actualFrameToShow = 0;
			AnimationBoss[boss->CurrentPhase].actualDuration = 0.f;
			AnimationBossLayer1[boss->CurrentPhase].actualFrameToShow = 0;
			AnimationBossLayer1[boss->CurrentPhase].actualDuration = 0;
			boss->IsAttacking = sfFalse;*/
			//boss->CurrentPhase = idleBoss;
			//}
		}
	}
}

void UpdateAnimationFirstLayer(Boss* boss, const float deltaTime)
{
	if (GetActualDungeonId() == elemDungeon)
	{
		UpdateAnimation(&AnimationBossLayer1[boss->CurrentPhase], deltaTime);

		if (boss->CurrentPhase == p1)
		{
			if (AnimationBoss[boss->CurrentPhase].actualFrameToShow == 10)
				boss->IsAttacking = sfTrue;
			if (AnimationBoss[boss->CurrentPhase].actualFrameToShow == 20)
				boss->IsAttacking = sfFalse;
		}

		if (boss->CurrentPhase == p2)
		{
			if (boss->PaternChoosen == 1)
			{
				if (AnimationBoss[boss->CurrentPhase].actualFrameToShow == 17)
					boss->IsAttacking = sfTrue;
				if (AnimationBoss[boss->CurrentPhase].actualFrameToShow == 30)
					boss->IsAttacking = sfFalse;
			}
			else if (boss->PaternChoosen == 0)
			{
				if (AnimationBoss[boss->CurrentPhase].actualFrameToShow == 0)
					boss->IsAttacking = sfTrue;
				if (AnimationBoss[boss->CurrentPhase].actualFrameToShow == 30)
					boss->IsAttacking = sfFalse;
			}
		}

		if (boss->CurrentPhase == p3)
		{
			if (AnimationBoss[boss->CurrentPhase].actualFrameToShow == 16)
				boss->IsAttacking = sfTrue;
			if (AnimationBoss[boss->CurrentPhase].actualFrameToShow == 25)
				boss->IsAttacking = sfFalse;
		}

		if (boss->CurrentPhase == p4)
		{
			if (AnimationBoss[boss->CurrentPhase].actualFrameToShow == 19)
				boss->IsAttacking = sfTrue;
			if (AnimationBoss[boss->CurrentPhase].actualFrameToShow == 27)
				boss->IsAttacking = sfFalse;
		}
	}
}

void UpdateBossController(Boss* boss, const float deltaTime)
{
	if (boss->Id == razar)
	{
		UpdateAnimation(&AnimationBoss[boss->CurrentPhase], deltaTime);
		if (boss->CurrentPhase == 1)
		{
			if (AnimationBoss[boss->CurrentPhase].actualDuration >= AnimationBoss[boss->CurrentPhase].totalDuration)
			{
				boss->CurrentPhase = 2;
			}
			if (AnimationBoss[boss->CurrentPhase].actualFrameToShow >= 13 && AnimationBoss[boss->CurrentPhase].actualFrameToShow <= 19)
			{
				boss->IsMoving = sfTrue;
			}
			else
			{
				boss->IsMoving = sfFalse;
			}
		}
	}
	else
	{
		if (boss->CanBeEngaged)
		{
			UpdateAnimationSyncWithAttack(boss, deltaTime);
			UpdateAnimation(&AnimationBoss[boss->CurrentPhase], deltaTime);
			UpdateAnimationFirstLayer(boss, deltaTime);
		}
		else
		{
			if (boss->CanSpawn)
			{
				UpdateAnimation(&AnimationBoss[boss->CurrentPhase], deltaTime);
				UpdateAnimation(&AnimationBossLayer1[boss->CurrentPhase], deltaTime);
				if (AnimationBoss[boss->CurrentPhase].actualFrameToShow == 32 && boss->CurrentPhase == spawnPhase)
				{
					AnimationBoss[boss->CurrentPhase].actualFrameToShow = 0;
					AnimationBoss[boss->CurrentPhase].actualDuration = 0.f;
					AnimationBossLayer1[boss->CurrentPhase].actualFrameToShow = 0;
					AnimationBossLayer1[boss->CurrentPhase].actualDuration = 0;
					boss->CurrentPhase = p5;
					boss->CooldownShot[4] = 9.f;
				}
				if (AnimationBoss[boss->CurrentPhase].actualFrameToShow == 15 && boss->CurrentPhase == p5)
				{
					boss->CanBeEngaged = sfTrue;
				}
			}
		}

		for (int i = 0; i < boss->RealPhase; i++)
		{
			if (AnimationBoss[i].isLoaded)
			{
				printf("real phase%d\n", boss->RealPhase);
				int* ptr = calloc(1, sizeof(int));
				*ptr = i + 1;
				HANDLE LoadBossAnimThread = CreateThread(NULL, 0, LoadBossElemAnim, (void*)(ptr), 0, NULL);
			}
		}
	}
}

void DisplayBossController(const Boss boss, sfRenderWindow* window, const Camera pCam, const DungeonGraphicData dungeonGraphicData)
{
	sfVector2f temp;
	DungeonId dungeonId = GetActualDungeonId();
	for (int i = 0; i < dungeonGraphicData.numberOfRoom; i++)
	{
		temp = (sfVector2f) { dungeonGraphicData.room[dungeonGraphicData.room[i].uniqueID].Pos.x * 64.f - 1280.f, dungeonGraphicData.room[dungeonGraphicData.room[i].uniqueID].Pos.y * 64.f - 1440.f };
	}
	if (dungeonId == elemDungeon)
	{
		BlitSprite(window, LayerTwoMoins, temp, NULL);
		temp.y += 1440.f;
		BlitSprite(window, LayerTwoMoins, temp, NULL);
	}
	if (!boss.IsDead)
	{
		if (DrawOnCamera(pCam, AnimationBoss[boss.CurrentPhase].spriteArray[AnimationBoss[boss.CurrentPhase].actualFrameToShow], boss.RigidBody.Position))
		{
			sfVector2f actualPosition = { boss.RigidBody.Position.x + AnimationBoss[boss.CurrentPhase].animPosition.x,  boss.RigidBody.Position.y + AnimationBoss[boss.CurrentPhase].animPosition.y };

			if (boss.IsHit)
			{
				BlitSprite(window, AnimationBoss[boss.CurrentPhase].spriteArray[AnimationBoss[boss.CurrentPhase].actualFrameToShow], actualPosition, GetShaderList().HitEnemy);
			}
			else
			{
				BlitSprite(window, AnimationBoss[boss.CurrentPhase].spriteArray[AnimationBoss[boss.CurrentPhase].actualFrameToShow], actualPosition, NULL);
			}
		}
	}
	else
	{
		if (dungeonId == slimeDungeon)
		{
			if (DrawOnCamera(pCam, AnimationBoss[p2].spriteArray[AnimationBoss[p2].actualFrameToShow], boss.RigidBody.Position))
			{
				BlitSprite(window, AnimationBoss[p2].spriteArray[AnimationBoss[p2].actualFrameToShow], boss.RigidBody.Position, NULL);
			}
		}
		else if (dungeonId == elemDungeon)
		{
			if (DrawOnCamera(pCam, AnimationBoss[idleBoss].spriteArray[AnimationBoss[idleBoss].actualFrameToShow], boss.RigidBody.Position))
			{
				sfVector2f actualPosition = { boss.RigidBody.Position.x + AnimationBoss[boss.CurrentPhase].animPosition.x,  boss.RigidBody.Position.y + AnimationBoss[boss.CurrentPhase].animPosition.y };
				BlitSprite(window, AnimationBoss[idleBoss].spriteArray[AnimationBoss[idleBoss].actualFrameToShow], actualPosition, NULL);
			}
		}
	}
}

void DisplayBossControllerLayer1(const Boss boss, sfRenderWindow* window, const Camera pCam)
{
	if (boss.Id == razar)
	{
		if (boss.IsHit)
		{
			BlitSprite(window, AnimationBoss[boss.CurrentPhase].spriteArray[AnimationBoss[boss.CurrentPhase].actualFrameToShow], boss.RigidBody.Position, GetShaderList().HitEnemy);
		}
		else
		{
			BlitSprite(window, AnimationBoss[boss.CurrentPhase].spriteArray[AnimationBoss[boss.CurrentPhase].actualFrameToShow], boss.RigidBody.Position, NULL);
		}
	}
	else if (AnimationBossLayer1 != NULL && !(AnimationBossLayer1[boss.CurrentPhase].actualFrameToShow < 19 && boss.CurrentPhase == spawnPhase))
	{
		if (!boss.IsDead)
		{
			if (DrawOnCamera(pCam, AnimationBossLayer1[boss.CurrentPhase].spriteArray[AnimationBossLayer1[boss.CurrentPhase].actualFrameToShow], boss.RigidBody.Position))
			{
				sfVector2f actualPosition = { boss.RigidBody.Position.x + AnimationBossLayer1[boss.CurrentPhase].animPosition.x,  boss.RigidBody.Position.y + AnimationBossLayer1[boss.CurrentPhase].animPosition.y };

				if (boss.IsHit)
				{
					BlitSprite(window, AnimationBossLayer1[boss.CurrentPhase].spriteArray[AnimationBossLayer1[boss.CurrentPhase].actualFrameToShow], actualPosition, GetShaderList().HitEnemy);
				}
				else
				{
					BlitSprite(window, AnimationBossLayer1[boss.CurrentPhase].spriteArray[AnimationBossLayer1[boss.CurrentPhase].actualFrameToShow], actualPosition, NULL);
				}
				if (boss.CurrentPhase == p3)
				{
					sfVector2f vecLight;
					vecLight.x = actualPosition.x;
					vecLight.y = actualPosition.y - 190.0f;
					printf("p3 %f\n", vecLight.x);
					BlitRenderTexture(window, GetEnemiLightTexture(), light, vecLight);
				}
				if (boss.CurrentPhase == p4)
				{
					sfVector2f vecLight;
					vecLight.x = actualPosition.x - 100.0f;
					vecLight.y = actualPosition.y - 190.0f;
					BlitRenderTexture(window, GetEnemiLightTexture(), light, vecLight);
				}
			}
		}
		else
		{
			if (GetActualDungeonId() == elemDungeon)
			{
				if ((AnimationBossLayer1[boss.CurrentPhase].actualFrameToShow < 5 && boss.CurrentPhase == idleBoss) || (AnimationBossLayer1[boss.CurrentPhase].actualFrameToShow == 10 && boss.CurrentPhase == idleBoss)
					|| (AnimationBossLayer1[boss.CurrentPhase].actualFrameToShow > 15 && AnimationBossLayer1[boss.CurrentPhase].actualFrameToShow < 30 && boss.CurrentPhase == idleBoss))
				{
					if (DrawOnCamera(pCam, AnimationBossLayer1[idleBoss].spriteArray[AnimationBossLayer1[idleBoss].actualFrameToShow], boss.RigidBody.Position))
					{
						sfVector2f actualPosition = { boss.RigidBody.Position.x + AnimationBossLayer1[boss.CurrentPhase].animPosition.x,  boss.RigidBody.Position.y + AnimationBossLayer1[boss.CurrentPhase].animPosition.y };
						BlitSprite(window, AnimationBossLayer1[idleBoss].spriteArray[AnimationBossLayer1[idleBoss].actualFrameToShow], actualPosition, NULL);
					}
				}
			}
		}
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
		sfCircleShape_setPosition(boss.Shape, boss.Circle.center);
		sfRenderWindow_drawCircleShape(window, boss.Shape, NULL);
	}
#endif // _DEBUG
}