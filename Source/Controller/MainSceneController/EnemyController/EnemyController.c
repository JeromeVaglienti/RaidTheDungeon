#include "EnemyController.h"
#include <Windows.h>
#include "..\BossController\BossController.h"
#include "..\ShadersController\ShadersController.h"

EnemySpriteContainer *enemySpriteContainer = NULL;
EnemySpriteElemContainer *enemySpriteElemContainer = NULL;

void InitEnemySlimeController(const sfVideoMode mode)
{

#if _DEBUG
	printf("Load Enemy Slime\n");
#endif

	enemySpriteContainer = (EnemySpriteContainer*)calloc(1, sizeof(EnemySpriteContainer));
	for (int i = 0; i < 6; i++)
	{
		enemySpriteContainer->enemyVertexArraySlime[i] = sfVertexArray_create();
		sfVertexArray_setPrimitiveType(enemySpriteContainer->enemyVertexArraySlime[i], sfQuads);
		enemySpriteContainer->transitionVertexArraySlime[i] = sfVertexArray_create();
		sfVertexArray_setPrimitiveType(enemySpriteContainer->transitionVertexArraySlime[i], sfQuads);
	}

	for (int i = 0; i < 7; i++)
	{
		enemySpriteContainer->spawnVertexArraySlime[i] = sfVertexArray_create();
		sfVertexArray_setPrimitiveType(enemySpriteContainer->spawnVertexArraySlime[i], sfQuads);
	}

	enemySpriteContainer->Ratio.x = mode.width / 1920.0f;
	enemySpriteContainer->Ratio.y = mode.height / 1080.0f;

	enemySpriteContainer->ParticlesEnnemies = (Particles**)calloc(3, sizeof(Particles*));
	enemySpriteContainer->ParticlesEnnemies[0] = (Particles *)calloc(1, sizeof(Particles));
	enemySpriteContainer->ParticlesEnnemies[1] = (Particles *)calloc(1, sizeof(Particles));
	enemySpriteContainer->ParticlesEnnemies[2] = (Particles *)calloc(1, sizeof(Particles));

	enemySpriteContainer->Smudge[0] = LoadSprite("Ressources/Sprites/Enemy/Dungeon1/Smudge/Smudge0.png", 1);
	enemySpriteContainer->Smudge[1] = LoadSprite("Ressources/Sprites/Enemy/Dungeon1/Smudge/Smudge1.png", 1);
	enemySpriteContainer->Smudge[2] = LoadSprite("Ressources/Sprites/Enemy/Dungeon1/Smudge/Smudge2.png", 1);

	enemySpriteContainer->PartKamikaze = (sfSprite**)calloc(6, sizeof(sfSprite*));
	enemySpriteContainer->PartKamikaze[0] = LoadSprite("Ressources/Sprites/Enemy/Dungeon1/SlimeKamikaze/particle/kamikaze_Particle0.png", 1);
	enemySpriteContainer->PartKamikaze[1] = LoadSprite("Ressources/Sprites/Enemy/Dungeon1/SlimeKamikaze/particle/kamikaze_Particle1.png", 1);
	enemySpriteContainer->PartKamikaze[2] = LoadSprite("Ressources/Sprites/Enemy/Dungeon1/SlimeKamikaze/particle/kamikaze_Particle2.png", 1);
	enemySpriteContainer->PartKamikaze[3] = LoadSprite("Ressources/Sprites/Enemy/Dungeon1/SlimeKamikaze/particle/kamikaze_Particle3.png", 1);
	enemySpriteContainer->PartKamikaze[4] = LoadSprite("Ressources/Sprites/Enemy/Dungeon1/SlimeKamikaze/particle/kamikaze_Particle4.png", 1);
	enemySpriteContainer->PartKamikaze[5] = LoadSprite("Ressources/Sprites/Enemy/Dungeon1/SlimeKamikaze/particle/kamikaze_Particle5.png", 1);

	enemySpriteContainer->Slime = (sfSprite**)calloc(6, sizeof(sfSprite*));
	enemySpriteContainer->Scale = (sfVector2f*)calloc(6, sizeof(sfVector2f));
	enemySpriteContainer->Scale[cac] = (sfVector2f) { 0.50f, 0.50f };
	enemySpriteContainer->Slime[cac] = LoadSprite("Ressources/Sprites/Enemy/Dungeon1/SlimeCac/spritesheet.png", 1);
	enemySpriteContainer->Scale[distance] = (sfVector2f) { 0.45f, 0.45f };
	enemySpriteContainer->Slime[distance] = LoadSprite("Ressources/Sprites/Enemy/Dungeon1/SlimeDistance/spritesheet.png", 1);
	enemySpriteContainer->Scale[kamikaze] = (sfVector2f) { 0.65f, 0.65f };
	enemySpriteContainer->Slime[kamikaze] = LoadSprite("Ressources/Sprites/Enemy/Dungeon1/SlimeKamikaze/spritesheet.png", 1);
	enemySpriteContainer->Scale[cacUpgrade] = (sfVector2f) { 0.8f, 0.8f };
	enemySpriteContainer->Slime[cacUpgrade] = LoadSprite("Ressources/Sprites/Enemy/Dungeon1/SlimeCacElite/spritesheet.png", 1);
	enemySpriteContainer->Scale[distanceUpgrade] = (sfVector2f) { 0.7f, 0.7f };
	enemySpriteContainer->Slime[distanceUpgrade] = LoadSprite("Ressources/Sprites/Enemy/Dungeon1/SlimeDistanceElite/spritesheet.png", 1);
	enemySpriteContainer->Scale[kamikazeUpgrade] = (sfVector2f) { 0.8f, 0.8f };
	enemySpriteContainer->Slime[kamikazeUpgrade] = LoadSprite("Ressources/Sprites/Enemy/Dungeon1/SlimeKamikazeElite/spritesheet.png", 1);

	enemySpriteContainer->SlimeTransition = (sfSprite**)calloc(6, sizeof(sfSprite*));
	enemySpriteContainer->SlimeTransition[cac] = LoadSprite("Ressources/Sprites/Enemy/Dungeon1/SlimeCac/Transition/spritesheet.png", 1);
	enemySpriteContainer->SlimeTransition[distance] = LoadSprite("Ressources/Sprites/Enemy/Dungeon1/SlimeDistance/Transition/spritesheet.png", 1);
	enemySpriteContainer->SlimeTransition[kamikaze] = LoadSprite("Ressources/Sprites/Enemy/Dungeon1/SlimeKamikaze/Transition/spritesheet.png", 1);
	enemySpriteContainer->SlimeTransition[cacUpgrade] = LoadSprite("Ressources/Sprites/Enemy/Dungeon1/SlimeCacElite/Transition/spritesheet.png", 1);
	enemySpriteContainer->SlimeTransition[distanceUpgrade] = LoadSprite("Ressources/Sprites/Enemy/Dungeon1/SlimeDistanceElite/Transition/spritesheet.png", 1);
	enemySpriteContainer->SlimeTransition[kamikazeUpgrade] = LoadSprite("Ressources/Sprites/Enemy/Dungeon1/SlimeKamikazeElite/Transition/spritesheet.png", 1);

	enemySpriteContainer->Death = (Animation*)calloc(1, sizeof(Animation));
	Animation temp = LoadAnimation(10, 0.3f, "Ressources/Sprites/Enemy/Dungeon1/Explosion anim/explosion_", "png", (sfVector2f) { 0.f }, sfFalse, 1);
	*(enemySpriteContainer->Death) = temp;

	enemySpriteContainer->SheetSpawnSlime = (sfSprite**)calloc(7, sizeof(sfSprite*));
	enemySpriteContainer->SheetSpawnSlime[cac] = LoadSprite("Ressources/Sprites/Enemy/Dungeon1/SpawnCac/spritesheet.png", 1);
	enemySpriteContainer->SheetSpawnSlime[cacUpgrade] = LoadSprite("Ressources/Sprites/Enemy/Dungeon1/SpawnCacElite/spritesheet.png", 1);
	enemySpriteContainer->SheetSpawnSlime[distance] = LoadSprite("Ressources/Sprites/Enemy/Dungeon1/SpawnDistance/spritesheet.png", 1);
	enemySpriteContainer->SheetSpawnSlime[distanceUpgrade] = LoadSprite("Ressources/Sprites/Enemy/Dungeon1/SpawnDistanceElite/spritesheet.png", 1);
	enemySpriteContainer->SheetSpawnSlime[miniBoss] = LoadSprite("Ressources/Sprites/Enemy/Dungeon1/SpawnMiniBoss/spritesheet.png", 1);
	enemySpriteContainer->SheetSpawnSlime[kamikaze] = LoadSprite("Ressources/Sprites/Enemy/Dungeon1/SpawnKamikaze/spritesheet.png", 1);
	enemySpriteContainer->SheetSpawnSlime[kamikazeUpgrade] = LoadSprite("Ressources/Sprites/Enemy/Dungeon1/SpawnKamikazeElite/spritesheet.png", 1);

	enemySpriteContainer->SlimeAnim = (Animation*)calloc(1, sizeof(Animation));
	//enemySpriteContainer->SlimeAnim =
	enemySpriteContainer->SlimeAnim[miniBoss - 6] = LoadAnimation(12, 1.0f, "Ressources/Sprites/Enemy/Dungeon1/SlimeMiniBoss/Body/Body_Miniboss_", "png", (sfVector2f) { 0.0f, 0.0f }, sfTrue, 1);
	enemySpriteContainer->MouthMiniBoss = LoadAnimation(12, 1.0f, "Ressources/Sprites/Enemy/Dungeon1/SlimeMiniBoss/Bouche/Bouche_MiniBoss_", "png", (sfVector2f) { 0.0f, 0.0f }, sfTrue, 1);

	//ReScaleAnimation(&enemySpriteContainer->Spawn[miniBoss], 300.0f, 300.0f, ratioX, ratioY);
	ReScaleAnimation(&enemySpriteContainer->SlimeAnim[miniBoss - 6], 300.0f, 300.0f, 1.0f, 1.0f);
	ReScaleAnimation(&enemySpriteContainer->MouthMiniBoss, 300.0f, 300.0f, 1.0f, 1.0f);


	enemySpriteContainer->UpgradeSlime[cac][0] = LoadAnimation(12, 1.0f, "Ressources/Sprites/Enemy/Dungeon1/MoveCac/east/", "png", (sfVector2f) { 0.0f, 0.0f }, sfTrue, 1);
	enemySpriteContainer->UpgradeSlime[cac][1] = LoadAnimation(12, 1.0f, "Ressources/Sprites/Enemy/Dungeon1/MoveCac/south east/",  "png", (sfVector2f) { 0.0f, 0.0f }, sfTrue, 1);
	enemySpriteContainer->UpgradeSlime[cac][2] = LoadAnimation(12, 1.0f, "Ressources/Sprites/Enemy/Dungeon1/MoveCac/south/", "png", (sfVector2f) { 0.0f, 0.0f }, sfTrue, 1);
	enemySpriteContainer->UpgradeSlime[cac][3] = LoadAnimation(12, 1.0f, "Ressources/Sprites/Enemy/Dungeon1/MoveCac/south west/", "png", (sfVector2f) { 0.0f, 0.0f }, sfTrue, 1);
	enemySpriteContainer->UpgradeSlime[cac][4] = LoadAnimation(12, 1.0f, "Ressources/Sprites/Enemy/Dungeon1/MoveCac/west/", "png", (sfVector2f) { 0.0f, 0.0f }, sfTrue, 1);
	enemySpriteContainer->UpgradeSlime[cac][5] = LoadAnimation(12, 1.0f, "Ressources/Sprites/Enemy/Dungeon1/MoveCac/north west/", "png", (sfVector2f) { 0.0f, 0.0f }, sfTrue, 1);
	enemySpriteContainer->UpgradeSlime[cac][6] = LoadAnimation(12, 1.0f, "Ressources/Sprites/Enemy/Dungeon1/MoveCac/north/", "png", (sfVector2f) { 0.0f, 0.0f }, sfTrue, 1);
	enemySpriteContainer->UpgradeSlime[cac][7] = LoadAnimation(12, 1.0f, "Ressources/Sprites/Enemy/Dungeon1/MoveCac/north east/", "png", (sfVector2f) { 0.0f, 0.0f }, sfTrue, 1);

	enemySpriteContainer->UpgradeSlime[cacUpgrade][0] = LoadAnimation(12, 1.0f, "Ressources/Sprites/Enemy/Dungeon1/MoveCacElite/east/", "png", (sfVector2f) { 0.0f, 0.0f }, sfTrue, 1);
	enemySpriteContainer->UpgradeSlime[cacUpgrade][1] = LoadAnimation(12, 1.0f, "Ressources/Sprites/Enemy/Dungeon1/MoveCacElite/south east/", "png", (sfVector2f) { 0.0f, 0.0f }, sfTrue, 1);
	enemySpriteContainer->UpgradeSlime[cacUpgrade][2] = LoadAnimation(12, 1.0f, "Ressources/Sprites/Enemy/Dungeon1/MoveCacElite/south/", "png", (sfVector2f) { 0.0f, 0.0f }, sfTrue, 1);
	enemySpriteContainer->UpgradeSlime[cacUpgrade][3] = LoadAnimation(12, 1.0f, "Ressources/Sprites/Enemy/Dungeon1/MoveCacElite/south west/", "png", (sfVector2f) { 0.0f, 0.0f }, sfTrue, 1);
	enemySpriteContainer->UpgradeSlime[cacUpgrade][4] = LoadAnimation(12, 1.0f, "Ressources/Sprites/Enemy/Dungeon1/MoveCacElite/west/", "png", (sfVector2f) { 0.0f, 0.0f }, sfTrue, 1);
	enemySpriteContainer->UpgradeSlime[cacUpgrade][5] = LoadAnimation(12, 1.0f, "Ressources/Sprites/Enemy/Dungeon1/MoveCacElite/north west/", "png", (sfVector2f) { 0.0f, 0.0f }, sfTrue, 1);
	enemySpriteContainer->UpgradeSlime[cacUpgrade][6] = LoadAnimation(12, 1.0f, "Ressources/Sprites/Enemy/Dungeon1/MoveCacElite/north/", "png", (sfVector2f) { 0.0f, 0.0f }, sfTrue, 1);
	enemySpriteContainer->UpgradeSlime[cacUpgrade][7] = LoadAnimation(12, 1.0f, "Ressources/Sprites/Enemy/Dungeon1/MoveCacElite/north east/", "png", (sfVector2f) { 0.0f, 0.0f }, sfTrue, 1);


	for (int i = 0; i < 8; i++)
	{
		ReScaleAnimation(&enemySpriteContainer->UpgradeSlime[cac][i], 122.5f, 77.0f, 1.0f, 1.0f);
		ReScaleAnimation(&enemySpriteContainer->UpgradeSlime[cacUpgrade][i], 204.f, 204.f, 1.0f, 1.0f);
	}
}

void UnloadEnemySlimeController()
{
#if _DEBUG
	printf("UnLoad Enemy slime\n");
#endif
	if (enemySpriteContainer == NULL)
	{
		return;
	}

	for (int i = 0; i < 3; i++)
	{
		DestroySfSprite(&enemySpriteContainer->Smudge[i]);
	}

	for (int i = 0; i < 6; i++)
	{
		enemySpriteContainer->Scale[i] = (sfVector2f) { 0.0f, 0.0f };
		DestroySfSprite(&enemySpriteContainer->Slime[i]);
		DestroySfSprite(&enemySpriteContainer->SlimeTransition[i]);
		DestroySfSprite(&enemySpriteContainer->PartKamikaze[i]);
	}

	for (int i = 0; i < 7; i++)
	{
		DestroySfSprite(&enemySpriteContainer->SheetSpawnSlime[i]);
	}

	UnLoadAnimation(enemySpriteContainer->Death);
	UnLoadAnimation(enemySpriteContainer->SlimeAnim);
	UnLoadAnimation(&enemySpriteContainer->MouthMiniBoss);

	ClearParticle(&enemySpriteContainer->ParticlesEnnemies[0]);
	ClearParticle(&enemySpriteContainer->ParticlesEnnemies[1]);

	for (int i = 0; i < 9; i++)
	{
		if (i < 7)
		{
			sfVertexArray_destroy(enemySpriteContainer->spawnVertexArraySlime[i]);
		}
		if (i < 6)
		{
			sfVertexArray_destroy(enemySpriteContainer->enemyVertexArraySlime[i]);
			sfVertexArray_destroy(enemySpriteContainer->transitionVertexArraySlime[i]);
		}
	}

	for (int i = 0; i < 8; i++)
	{
		UnLoadAnimation(&enemySpriteContainer->UpgradeSlime[cac][i]);
		UnLoadAnimation(&enemySpriteContainer->UpgradeSlime[cacUpgrade][i]);
	}


	free(enemySpriteContainer->ParticlesEnnemies);
	free(enemySpriteContainer->Slime);
	free(enemySpriteContainer->Scale);
	free(enemySpriteContainer->SlimeTransition);
	free(enemySpriteContainer->Death);
	free(enemySpriteContainer->SheetSpawnSlime);
	free(enemySpriteContainer->SlimeAnim);

	free(enemySpriteContainer);
	enemySpriteContainer = NULL;
}

void InitEnemyElemController(const sfVideoMode mode)
{

#if _DEBUG
	printf("Load Enemy Elem\n");
#endif

	enemySpriteElemContainer = (EnemySpriteElemContainer*)calloc(1, sizeof(EnemySpriteElemContainer));

	for (int i = 0; i < 9; i++)
	{
		enemySpriteElemContainer->spawnVertexArrayElem[i] = sfVertexArray_create();
		sfVertexArray_setPrimitiveType(enemySpriteElemContainer->spawnVertexArrayElem[i], sfQuads);
		for (int j = 0; j < 4; j++)
		{
			enemySpriteElemContainer->enemyVertexArrayElem[i][j] = sfVertexArray_create();
			sfVertexArray_setPrimitiveType(enemySpriteElemContainer->enemyVertexArrayElem[i][j], sfQuads);
		}
	}

	enemySpriteElemContainer->Scale = (sfVector2f*)calloc(9, sizeof(sfVector2f));

	enemySpriteElemContainer->Scale[fireElem] = (sfVector2f) { 1.f, 1.f };
	enemySpriteElemContainer->Scale[waterElem] = (sfVector2f) { 1.f, 1.f };
	enemySpriteElemContainer->Scale[windElem] = (sfVector2f) { 1.f, 1.f };
	enemySpriteElemContainer->Scale[dirtElem] = (sfVector2f) { 1.f, 1.f };
	enemySpriteElemContainer->Scale[lavaElem] = (sfVector2f) { 1.f, 1.f };
	enemySpriteElemContainer->Scale[iceElem] = (sfVector2f) { 1.f, 1.f };
	enemySpriteElemContainer->Scale[stormElem] = (sfVector2f) { 1.f, 1.f };
	enemySpriteElemContainer->Scale[rockElem] = (sfVector2f) { 1.f, 1.f };
	enemySpriteElemContainer->Scale[littleElemOfLava] = (sfVector2f) { 0.5f, 0.5f };

	enemySpriteElemContainer->Elem = (sfSprite***)calloc(9, sizeof(sfSprite**));

	for (int i = 0; i < 9; i++)
	{
		enemySpriteElemContainer->Elem[i] = (sfSprite**)calloc(4, sizeof(sfSprite*));
	}


	enemySpriteElemContainer->Elem[fireElem][0] = LoadSprite("Ressources/Sprites/Enemy/Dungeon2/Moving/Right/Fire/spritesheet.png", 1);
	enemySpriteElemContainer->Elem[waterElem][0] = LoadSprite("Ressources/Sprites/Enemy/Dungeon2/Moving/Right/Water/spritesheet.png", 1);
	enemySpriteElemContainer->Elem[windElem][0] = LoadSprite("Ressources/Sprites/Enemy/Dungeon2/Moving/Right/Wind/spritesheet.png", 1);
	enemySpriteElemContainer->Elem[dirtElem][0] = LoadSprite("Ressources/Sprites/Enemy/Dungeon2/Moving/Right/Dirt/spritesheet.png", 1);
	enemySpriteElemContainer->Elem[lavaElem][0] = LoadSprite("Ressources/Sprites/Enemy/Dungeon2/Moving/Right/Lava/spritesheet.png", 1);
	enemySpriteElemContainer->Elem[iceElem][0] = LoadSprite("Ressources/Sprites/Enemy/Dungeon2/Moving/Right/Ice/spritesheet.png", 1);
	enemySpriteElemContainer->Elem[stormElem][0] = LoadSprite("Ressources/Sprites/Enemy/Dungeon2/Moving/Right/Storm/spritesheet.png", 1);
	enemySpriteElemContainer->Elem[rockElem][0] = LoadSprite("Ressources/Sprites/Enemy/Dungeon2/Moving/Right/Rock/spritesheet.png", 1);
	enemySpriteElemContainer->Elem[littleElemOfLava][0] = LoadSprite("Ressources/Sprites/Enemy/Dungeon2/Moving/Right/Fire/spritesheet.png", 1);

	enemySpriteElemContainer->Elem[fireElem][1] = LoadSprite("Ressources/Sprites/Enemy/Dungeon2/Moving/Front/Fire/spritesheet.png", 1);
	enemySpriteElemContainer->Elem[waterElem][1] = LoadSprite("Ressources/Sprites/Enemy/Dungeon2/Moving/Front/Water/spritesheet.png", 1);
	enemySpriteElemContainer->Elem[windElem][1] = LoadSprite("Ressources/Sprites/Enemy/Dungeon2/Moving/Front/Wind/spritesheet.png", 1);
	enemySpriteElemContainer->Elem[dirtElem][1] = LoadSprite("Ressources/Sprites/Enemy/Dungeon2/Moving/Front/Dirt/spritesheet.png", 1);
	enemySpriteElemContainer->Elem[lavaElem][1] = LoadSprite("Ressources/Sprites/Enemy/Dungeon2/Moving/Front/Lava/spritesheet.png", 1);
	enemySpriteElemContainer->Elem[iceElem][1] = LoadSprite("Ressources/Sprites/Enemy/Dungeon2/Moving/Front/Ice/spritesheet.png", 1);
	enemySpriteElemContainer->Elem[stormElem][1] = LoadSprite("Ressources/Sprites/Enemy/Dungeon2/Moving/Front/Storm/spritesheet.png", 1);
	enemySpriteElemContainer->Elem[rockElem][1] = LoadSprite("Ressources/Sprites/Enemy/Dungeon2/Moving/Front/Rock/spritesheet.png", 1);
	enemySpriteElemContainer->Elem[littleElemOfLava][1] = LoadSprite("Ressources/Sprites/Enemy/Dungeon2/Moving/Front/Fire/spritesheet.png", 1);

	enemySpriteElemContainer->Elem[fireElem][2] = LoadSprite("Ressources/Sprites/Enemy/Dungeon2/Moving/Left/Fire/spritesheet.png", 1);
	enemySpriteElemContainer->Elem[waterElem][2] = LoadSprite("Ressources/Sprites/Enemy/Dungeon2/Moving/Left/Water/spritesheet.png", 1);
	enemySpriteElemContainer->Elem[windElem][2] = LoadSprite("Ressources/Sprites/Enemy/Dungeon2/Moving/Left/Wind/spritesheet.png", 1);
	enemySpriteElemContainer->Elem[dirtElem][2] = LoadSprite("Ressources/Sprites/Enemy/Dungeon2/Moving/Left/Dirt/spritesheet.png", 1);
	enemySpriteElemContainer->Elem[lavaElem][2] = LoadSprite("Ressources/Sprites/Enemy/Dungeon2/Moving/Left/Lava/spritesheet.png", 1);
	enemySpriteElemContainer->Elem[iceElem][2] = LoadSprite("Ressources/Sprites/Enemy/Dungeon2/Moving/Left/Ice/spritesheet.png", 1);
	enemySpriteElemContainer->Elem[stormElem][2] = LoadSprite("Ressources/Sprites/Enemy/Dungeon2/Moving/Left/Storm/spritesheet.png", 1);
	enemySpriteElemContainer->Elem[rockElem][2] = LoadSprite("Ressources/Sprites/Enemy/Dungeon2/Moving/Left/Rock/spritesheet.png", 1);
	enemySpriteElemContainer->Elem[littleElemOfLava][2] = LoadSprite("Ressources/Sprites/Enemy/Dungeon2/Moving/Left/Fire/spritesheet.png", 1);

	enemySpriteElemContainer->Elem[fireElem][3] = LoadSprite("Ressources/Sprites/Enemy/Dungeon2/Moving/Back/Fire/spritesheet.png", 1);
	enemySpriteElemContainer->Elem[waterElem][3] = LoadSprite("Ressources/Sprites/Enemy/Dungeon2/Moving/Back/Water/spritesheet.png", 1);
	enemySpriteElemContainer->Elem[windElem][3] = LoadSprite("Ressources/Sprites/Enemy/Dungeon2/Moving/Back/Wind/spritesheet.png", 1);
	enemySpriteElemContainer->Elem[dirtElem][3] = LoadSprite("Ressources/Sprites/Enemy/Dungeon2/Moving/Back/Dirt/spritesheet.png", 1);
	enemySpriteElemContainer->Elem[lavaElem][3] = LoadSprite("Ressources/Sprites/Enemy/Dungeon2/Moving/Back/Lava/spritesheet.png", 1);
	enemySpriteElemContainer->Elem[iceElem][3] = LoadSprite("Ressources/Sprites/Enemy/Dungeon2/Moving/Back/Ice/spritesheet.png", 1);
	enemySpriteElemContainer->Elem[stormElem][3] = LoadSprite("Ressources/Sprites/Enemy/Dungeon2/Moving/Back/Storm/spritesheet.png", 1);
	enemySpriteElemContainer->Elem[rockElem][3] = LoadSprite("Ressources/Sprites/Enemy/Dungeon2/Moving/Back/Rock/spritesheet.png", 1);
	enemySpriteElemContainer->Elem[littleElemOfLava][3] = LoadSprite("Ressources/Sprites/Enemy/Dungeon2/Moving/Back/Fire/spritesheet.png", 1);

	enemySpriteElemContainer->SheetSpawnElem = (sfSprite**)calloc(9, sizeof(sfSprite*));

	enemySpriteElemContainer->SheetSpawnElem[fireElem] = LoadSprite("Ressources/Sprites/Enemy/Dungeon2/SpawnElem/Fire/spritesheet.png", 1);
	enemySpriteElemContainer->SheetSpawnElem[waterElem] = LoadSprite("Ressources/Sprites/Enemy/Dungeon2/SpawnElem/Water/spritesheet.png", 1);
	enemySpriteElemContainer->SheetSpawnElem[windElem] = LoadSprite("Ressources/Sprites/Enemy/Dungeon2/SpawnElem/Wind/spritesheet.png", 1);
	enemySpriteElemContainer->SheetSpawnElem[dirtElem] = LoadSprite("Ressources/Sprites/Enemy/Dungeon2/SpawnElem/Dirt/spritesheet.png", 1);
	enemySpriteElemContainer->SheetSpawnElem[lavaElem] = LoadSprite("Ressources/Sprites/Enemy/Dungeon2/SpawnElem/Lava/spritesheet.png", 1);
	enemySpriteElemContainer->SheetSpawnElem[iceElem] = LoadSprite("Ressources/Sprites/Enemy/Dungeon2/SpawnElem/Ice/spritesheet.png", 1);
	enemySpriteElemContainer->SheetSpawnElem[stormElem] = LoadSprite("Ressources/Sprites/Enemy/Dungeon2/SpawnElem/Storm/spritesheet.png", 1);
	enemySpriteElemContainer->SheetSpawnElem[rockElem] = LoadSprite("Ressources/Sprites/Enemy/Dungeon2/SpawnElem/Rock/spritesheet.png", 1);
	enemySpriteElemContainer->SheetSpawnElem[littleElemOfLava] = LoadSprite("Ressources/Sprites/Enemy/Dungeon2/SpawnElem/Fire/spritesheet.png", 1);
}

void UnloadEnemyElemController()
{
#if _DEBUG
	printf("UnLoad Enemy Elem\n");
#endif

	for (int i = 0; i < 9; i++)
	{
		enemySpriteElemContainer->Scale[i] = (sfVector2f) { 0.0f, 0.0f };

		DestroySfSprite(&enemySpriteElemContainer->SheetSpawnElem[i]);
		sfVertexArray_destroy(enemySpriteElemContainer->spawnVertexArrayElem[i]);

		for (int j = 0; j < 4; j++)
		{
			DestroySfSprite(&enemySpriteElemContainer->Elem[i][j]);

			sfVertexArray_destroy(enemySpriteElemContainer->enemyVertexArrayElem[i][j]);
		}
		free(enemySpriteElemContainer->Elem[i]);
	}

	free(enemySpriteElemContainer->Elem);
	free(enemySpriteElemContainer->SheetSpawnElem);
	free(enemySpriteElemContainer->Scale);

	free(enemySpriteElemContainer);
	enemySpriteContainer = NULL;
}

void DisplayEnemiesElem(const Enemy* firstElement, sfRenderWindow* window, const Camera pCam)
{
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
#endif
	Enemy* tempEnemy = (Enemy*)firstElement;

	sfBool canRepeat = (tempEnemy != NULL);
	while (canRepeat)
	{
		sfColor col = sfWhite;
		sfVector2f tempPos = { tempEnemy->RigidBody.Position.x, tempEnemy->RigidBody.Position.y };
		/*sfShader* temp = NULL;

		if (tempEnemy->IsHit)
		{
			temp = GetShaderList().HitEnemy;
		}*/
		if (tempEnemy->IsHit)
		{
			col = sfColor_fromRGB(255, 150, 150);
		}

		if (tempEnemy->StateAnim == spawn)
		{
			AppendSprite(enemySpriteElemContainer->spawnVertexArrayElem[tempEnemy->Id], enemySpriteElemContainer->SheetSpawnElem[tempEnemy->Id], tempPos, enemySpriteElemContainer->Scale[tempEnemy->Id], col, 0.0f, tempEnemy->SpawnAnimKey.actualFrameToShow, 6, 4);
		}
		else if (tempEnemy->StateAnim == move)
		{
			AppendSprite(enemySpriteElemContainer->enemyVertexArrayElem[tempEnemy->Id][tempEnemy->DirectionAnimation], enemySpriteElemContainer->Elem[tempEnemy->Id][tempEnemy->DirectionAnimation], tempPos, enemySpriteElemContainer->Scale[tempEnemy->Id], col, 0.0f, tempEnemy->MoveAnimKey.actualFrameToShow, 3, 4);
		}
		else
		{
			AppendSprite(enemySpriteElemContainer->spawnVertexArrayElem[tempEnemy->Id], enemySpriteElemContainer->SheetSpawnElem[tempEnemy->Id], tempPos, enemySpriteElemContainer->Scale[tempEnemy->Id], col, 0.0f, tempEnemy->DeathAnimKey.numberOfFrame - tempEnemy->DeathAnimKey.actualFrameToShow - 1, 6, 4);
		}


#if _DEBUG

		if (toShow)
		{
			sfCircleShape_setPosition(tempEnemy->Shape, tempEnemy->Circle.center);
			sfRenderWindow_drawCircleShape(window, tempEnemy->Shape, NULL);
		}
#endif // _DEBUG

		if (tempEnemy->Next != NULL)
		{
			tempEnemy = tempEnemy->Next;
		}
		else
		{
			canRepeat = sfFalse;
		}
	}
}

void DisplayEnemyVertexArrayElem(sfRenderWindow* window)
{
	for (int i = 0; i < 9; i++)
	{
		BlitVertexArray(window, enemySpriteElemContainer->spawnVertexArrayElem[i], enemySpriteElemContainer->SheetSpawnElem[i]);
		for (int j = 0; j < 4; j++)
		{
			BlitVertexArray(window, enemySpriteElemContainer->enemyVertexArrayElem[i][j], enemySpriteElemContainer->Elem[i][j]);
		}
	}
}

void DisplayEnemiesSlime(const Enemy* firstElement, sfRenderWindow* window, const Camera pCam)
{
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
#endif
	Enemy* tempEnemy = (Enemy*)firstElement;

	sfBool canRepeat = (tempEnemy != NULL);
	while (canRepeat)
	{
		sfVector2f tempPos = { tempEnemy->RigidBody.Position.x, tempEnemy->RigidBody.Position.y };
		//sfShader* temp = NULL;
		sfColor col = sfWhite;
		if (tempEnemy->IsHit)
		{
			col = sfColor_fromRGB(255, 150, 150);
			//temp = GetShaderList().HitEnemy;
		}
		if (tempEnemy->StateAnim == spawn || tempEnemy->StateAnim == deathAnim || tempEnemy->Id != miniBoss)
		{
			if (tempEnemy->StateAnim == spawn)
			{
				if (tempEnemy->Id == miniBoss)
				{
					AppendSprite(enemySpriteContainer->spawnVertexArraySlime[tempEnemy->Id], enemySpriteContainer->SheetSpawnSlime[tempEnemy->Id], tempPos, (sfVector2f) { enemySpriteContainer->Ratio.x / 1.5f, enemySpriteContainer->Ratio.y / 1.5f }, col, 0.0f, tempEnemy->SpawnAnimKey.actualFrameToShow, 3, 4);
				}
				else
				{
					AppendSprite(enemySpriteContainer->spawnVertexArraySlime[tempEnemy->Id], enemySpriteContainer->SheetSpawnSlime[tempEnemy->Id], tempPos, enemySpriteContainer->Scale[tempEnemy->Id], col, 0.0f, tempEnemy->SpawnAnimKey.actualFrameToShow, 3, 4);
				}
			}
			else if (tempEnemy->StateAnim == move)
			{
				if (tempEnemy->Id == cac || tempEnemy->Id == cacUpgrade)
				{
					BlitSprite(window, enemySpriteContainer->UpgradeSlime[tempEnemy->Id][tempEnemy->DirectionAnimation].spriteArray[tempEnemy->MoveAnimKey.actualFrameToShow], tempEnemy->RigidBody.Position, NULL);
				}
				else if (tempEnemy->IsTransitioning)
				{
					AppendSprite(enemySpriteContainer->transitionVertexArraySlime[tempEnemy->Id], enemySpriteContainer->SlimeTransition[tempEnemy->Id], tempPos, enemySpriteContainer->Scale[tempEnemy->Id], col, 0.0f, tempEnemy->DirectionAnimation, 4, 2);
				}
				else
				{
					AppendSprite(enemySpriteContainer->enemyVertexArraySlime[tempEnemy->Id], enemySpriteContainer->Slime[tempEnemy->Id], tempPos, enemySpriteContainer->Scale[tempEnemy->Id], col, 0.0f, tempEnemy->DirectionAnimation, 3, 3);
				}

			}
			else if (tempEnemy->Id == kamikaze || tempEnemy->Id == kamikazeUpgrade)
			{
				tempPos.y -= 70.0f;
				BlitSprite(window, enemySpriteContainer->Death->spriteArray[tempEnemy->DeathAnimKey.actualFrameToShow], tempPos, NULL);
			}
			else
			{
				AppendSprite(enemySpriteContainer->spawnVertexArraySlime[tempEnemy->Id], enemySpriteContainer->SheetSpawnSlime[tempEnemy->Id], tempPos, enemySpriteContainer->Scale[tempEnemy->Id], col, 0.0f, tempEnemy->DeathAnimKey.numberOfFrame - tempEnemy->DeathAnimKey.actualFrameToShow - 1, 3, 4);
			}
		}
		else if (tempEnemy->StateAnim == move)
		{
			if (DrawOnCamera(pCam, enemySpriteContainer->SlimeAnim[0].spriteArray[tempEnemy->MoveAnimKey.actualFrameToShow], tempPos))
			{
				sfSprite_setColor(enemySpriteContainer->SlimeAnim[0].spriteArray[tempEnemy->MoveAnimKey.actualFrameToShow], col);
				sfSprite_setColor(enemySpriteContainer->MouthMiniBoss.spriteArray[tempEnemy->AttackAnimKey.actualFrameToShow], col);
				BlitSprite(window, enemySpriteContainer->SlimeAnim[0].spriteArray[tempEnemy->MoveAnimKey.actualFrameToShow], tempPos, NULL);
				BlitSprite(window, enemySpriteContainer->MouthMiniBoss.spriteArray[tempEnemy->AttackAnimKey.actualFrameToShow], tempPos, NULL);
			}
		}

#if _DEBUG

		if (toShow)
		{
			sfCircleShape_setPosition(tempEnemy->Shape, tempEnemy->Circle.center);
			sfRenderWindow_drawCircleShape(window, tempEnemy->Shape, NULL);
		}
#endif // _DEBUG

		if (tempEnemy->Next != NULL)
		{
			tempEnemy = tempEnemy->Next;
		}
		else
		{
			canRepeat = sfFalse;
		}
	}
}

void DisplayEnemyVertexArraySlime(sfRenderWindow* window)
{
	for (int i = 0; i < 7; i++)
	{
		BlitVertexArray(window, enemySpriteContainer->spawnVertexArraySlime[i], enemySpriteContainer->SheetSpawnSlime[i]);
	}

	for (int i = 0; i < 6; i++)
	{
		BlitVertexArray(window, enemySpriteContainer->transitionVertexArraySlime[i], enemySpriteContainer->SlimeTransition[i]);
		BlitVertexArray(window, enemySpriteContainer->enemyVertexArraySlime[i], enemySpriteContainer->Slime[i]);
	}
}

Particles** GetParticlesEnnemies()
{
	return (enemySpriteContainer == NULL ? NULL : (enemySpriteContainer->ParticlesEnnemies));
}

void DisplayEnemyParticles(sfRenderWindow* window)
{
	DisplayParticles(enemySpriteContainer->ParticlesEnnemies[0], enemySpriteContainer->Smudge[0], window, NULL, enemySpriteContainer->Smudge);
	DisplayParticles(enemySpriteContainer->ParticlesEnnemies[1], enemySpriteContainer->PartKamikaze[0], window, NULL, enemySpriteContainer->PartKamikaze);

	DisplayParticles(enemySpriteContainer->ParticlesEnnemies[2], NULL, window, NULL, NULL);
}
