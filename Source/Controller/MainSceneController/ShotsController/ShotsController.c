#include "ShotsController.h"
#include "..\SpellsController\SpellsController.h"
#include "..\ShadersController\ShadersController.h"
#define NBR_VERTEX_ARRAY 26
ShotsController shotsController = { 0 };
sfVertexArray* shotsVertexArray[NBR_VERTEX_ARRAY] = { 0 };


void InitShotsGraphicsPlayer(const sfVideoMode mode)
{
	shotsController.SheetBasicAttack = LoadSprite("Ressources/Sprites/Players/AnimBasicFire/spritesheet.png", 1);
}

void InitShotsGraphicsSlime(const sfVideoMode mode)
{
	shotsController.SheetRangeBall = LoadSprite("Ressources/Sprites/Enemy/Dungeon1/SlimeRangeAttack/spritesheet.png", 1);

	shotsController.BossAtack = LoadSprite("Ressources/Sprites/Boss/Dungeon1/BossAttack/tir.png", 1);

	shotsController.MiniBossAtack = LoadSprite("Ressources/Sprites/Enemy/Dungeon1/tirMiniBoss.png", 1);

	shotsController.SheetKamikazeProjectile = LoadSprite("Ressources/Sprites/Enemy/Dungeon1/SlimeKamikazeProjectile/spritesheet.png", 1);

}

void InitShotsGraphicsRazar(const sfVideoMode mode)
{
	shotsController.SheetRazarShot = LoadSprite("Ressources/Sprites/Boss/Razar/Basic_Attack/spritesheet.png", 1);
	shotsController.SheetVoidBalls = LoadSprite("Ressources/Sprites/Boss/Razar/Void_Ball/spritesheet.png", 1);
	shotsController.SheetVoidGround = LoadSprite("Ressources/Sprites/Boss/Razar/Void_Ground/spritesheet.png", 1);
	shotsController.SheetRazarBigFrag = LoadSprite("Ressources/Sprites/Boss/Razar/Frag/spritesheetBig.png", 1);
	shotsController.SheetRazarMediumFrag = LoadSprite("Ressources/Sprites/Boss/Razar/Frag/spritesheetMedium.png", 1);
	shotsController.SheetRazarLittleFrag = LoadSprite("Ressources/Sprites/Boss/Razar/Frag/spritesheetMedium.png", 1);

	shotsController.SheetLightRazarBigFrag = LoadSprite("Ressources/Sprites/Boss/Razar/Frag/spritesheetLightBig.png", 1);
	shotsController.SheetLightRazarMediumFrag = LoadSprite("Ressources/Sprites/Boss/Razar/Frag/spritesheetLightMedium.png", 1);
	shotsController.SheetLightRazarLittleFrag = LoadSprite("Ressources/Sprites/Boss/Razar/Frag/spritesheetLightMedium.png", 1);
}

void InitShotsGraphicsElem(const sfVideoMode mode)
{
	shotsController.BossAtack = LoadSprite("Ressources/Sprites/Boss/Dungeon1/BossAttack/tir.png", 1);
	shotsController.SheetDirtElemProjectile = LoadSprite("Ressources/Sprites/Enemy/Dungeon2/sheetMudElemAttack.png", 1);
	shotsController.SheetFireElemProjectile = LoadSprite("Ressources/Sprites/Enemy/Dungeon2/sheetFireElemAttack.png", 1);
	shotsController.SheetLightFireElemProjectile = LoadSprite("Ressources/Sprites/Enemy/Dungeon2/sheetLightFireElemAttack.png", 1);

	shotsController.SheetIceElemProjectile = LoadSprite("Ressources/Sprites/Enemy/Dungeon2/sheetIceElemAttack.png", 1);
	shotsController.SheetIceShardProjectile[0] = LoadSprite("Ressources/Sprites/Enemy/Dungeon2/sheetIceShard1.png", 1);
	shotsController.SheetIceShardProjectile[1] = LoadSprite("Ressources/Sprites/Enemy/Dungeon2/sheetIceShard2.png", 1);
	shotsController.SheetIceShardProjectile[2] = LoadSprite("Ressources/Sprites/Enemy/Dungeon2/sheetIceShard3.png", 1);

	shotsController.SheetLightIceElemProjectile = LoadSprite("Ressources/Sprites/Enemy/Dungeon2/sheetIceElemAttack.png", 1);
	shotsController.SheetLightIceShardProjectile[0] = LoadSprite("Ressources/Sprites/Enemy/Dungeon2/sheetLightIceShard1.png", 1);
	shotsController.SheetLightIceShardProjectile[1] = LoadSprite("Ressources/Sprites/Enemy/Dungeon2/sheetLightIceShard2.png", 1);
	shotsController.SheetLightIceShardProjectile[2] = LoadSprite("Ressources/Sprites/Enemy/Dungeon2/sheetLightIceShard3.png", 1);

	shotsController.SheetWindElemProjectile = LoadSprite("Ressources/Sprites/Enemy/Dungeon2/sheetWindElemAttack.png", 1);
	shotsController.SheetLightWindElemProjectile = LoadSprite("Ressources/Sprites/Enemy/Dungeon2/sheetLightWindElemAttack.png", 1);
	shotsController.SheetWaterElemProjectile = LoadSprite("Ressources/Sprites/Enemy/Dungeon2/sheetWaterElemAttack.png", 1);
	shotsController.SheetLightWaterElemProjectile = LoadSprite("Ressources/Sprites/Enemy/Dungeon2/sheetLightWaterElemAttack.png", 1);
}

void InitShotsGraphics(const sfVideoMode mode)
{
	DungeonId tempId = GetActualDungeonId();
	for (int i = 0; i < NBR_VERTEX_ARRAY; i++)
	{
		shotsVertexArray[i] = sfVertexArray_create();
		sfVertexArray_setPrimitiveType(shotsVertexArray[i], sfQuads);
	}
	shotsController.Ratio.x = mode.width / 1920.0f;
	shotsController.Ratio.y = mode.height / 1080.0f;
	InitShotsGraphicsPlayer(mode);
	if (tempId == slimeDungeon || tempId == beginningDungeon)
	{
		InitShotsGraphicsSlime(mode);
	}
	else if (tempId == elemDungeon)
	{
		InitShotsGraphicsElem(mode);
	}
	else if (tempId == razarDungeon)
	{
		InitShotsGraphicsRazar(mode);
	}

}

void SetShots(Shot* firstElement, sfRenderWindow* window, const Camera pCam)
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
#endif // _DEBUG

	Shot* tempShot = firstElement;
	sfBool canRepeat = (tempShot != NULL);
	while (canRepeat)
	{
		sfVector2f tempPos = { tempShot->RigidBody.Position.x, tempShot->RigidBody.Position.y };
		sfColor col = sfWhite;
		if (tempShot->IdShot == matches || tempShot->IdShot == matchesB || tempShot->IdShot == matchesW || tempShot->IdShot == matchesG)
		{
			sfVector2f tempScale = { 0.5f, 0.5f };
			if (tempShot->IdShot == matches)
				col = sfColor_fromRGB(100, 0, 0);
			if (tempShot->IdShot == matchesB)
				col = sfColor_fromRGB(0, 0, 100);
			if (tempShot->IdShot == matchesG)
				col = sfColor_fromRGB(0, 100, 0);

			float angleRotation = RadianToDegree(atan2f(tempShot->Direction.y, tempShot->Direction.x));
			AppendSprite(shotsVertexArray[0], shotsController.SheetBasicAttack, tempPos, tempScale, col, angleRotation, tempShot->shotAnimKey->actualFrameToShow, 4, 3);
		}
		else if (tempShot->IdShot == fireBall || tempShot->IdShot == fireBallB || tempShot->IdShot == fireBallW || tempShot->IdShot == fireBallG)
		{
			sfVector2f tempScale = { 0.75f, 0.75f };
			tempPos.x += -48.f * tempShot->Direction.x * tempScale.x;
			tempPos.y += -48.f * tempShot->Direction.y * tempScale.y;
			if (tempShot->IdShot == fireBall)
				col = sfColor_fromRGB(100, 0, 0);
			if (tempShot->IdShot == fireBallB)
				col = sfColor_fromRGB(0, 0, 100);
			if (tempShot->IdShot == fireBallG)
				col = sfColor_fromRGB(0, 100, 0);

			float angleRotation = RadianToDegree(atan2f(tempShot->Direction.y, tempShot->Direction.x));
			AppendSprite(shotsVertexArray[1], GetSpells().SheetFireBall, tempPos, (sfVector2f) { 0.75f, 0.75f }, col, angleRotation, tempShot->shotAnimKey->actualFrameToShow, 4, 4);
		}
		else if (tempShot->IdShot == enemiesShot)
		{
			sfVector2f tempScale = shotsController.Ratio;
			tempPos.x += -29.f * tempShot->Direction.x * tempScale.x;
			tempPos.y += -29.f * tempShot->Direction.y * tempScale.y;
			float angleRotation = RadianToDegree(atan2f(tempShot->Direction.y, tempShot->Direction.x));
			AppendSprite(shotsVertexArray[2], shotsController.SheetRangeBall, tempPos, tempScale, sfWhite, angleRotation, tempShot->shotAnimKey->actualFrameToShow, 4, 3);
		}
		else if (tempShot->IdShot == kamikazeShot)
		{
			sfVector2f tempScale = shotsController.Ratio;
			tempPos.x += -29.f * tempShot->Direction.x * tempScale.x;
			tempPos.y += -29.f * tempShot->Direction.y * tempScale.y;
			float angleRotation = RadianToDegree(atan2f(tempShot->Direction.y, tempShot->Direction.x));
			AppendSprite(shotsVertexArray[5], shotsController.SheetKamikazeProjectile, tempPos, tempScale, sfWhite, angleRotation, tempShot->shotAnimKey->actualFrameToShow, 4, 3);
		}
		else if (tempShot->IdShot == miniBossShot)
		{
			AppendSprite(shotsVertexArray[4], shotsController.MiniBossAtack, tempPos, (sfVector2f) { 0.6f, 0.6f }, sfWhite, 0.0f, tempShot->shotAnimKey->actualFrameToShow, 0, 0);
		}
		else if (tempShot->IdShot == bossShot)
		{
			AppendSprite(shotsVertexArray[3], shotsController.BossAtack, tempPos, (sfVector2f) { 0.6f, 0.6f }, sfWhite, 0.0f, 0, 0, 0);
		}
		else if (tempShot->IdShot == fireElemShot)
		{
			float angleRotation = RadianToDegree(atan2f(tempShot->Direction.y, tempShot->Direction.x));
			AppendSprite(shotsVertexArray[6], shotsController.SheetFireElemProjectile, tempPos, (sfVector2f) { 0.4f, 0.4f }, sfWhite, angleRotation, tempShot->shotAnimKey->actualFrameToShow, 4, 3);
			AppendSprite(shotsVertexArray[12], shotsController.SheetLightFireElemProjectile, tempPos, (sfVector2f) { 0.4f, 0.4f }, sfWhite, angleRotation, tempShot->shotAnimKey->actualFrameToShow, 4, 3);
		}
		else if (tempShot->IdShot == waterElemShot)
		{
			float angleRotation = RadianToDegree(atan2f(tempShot->Direction.y, tempShot->Direction.x));
			AppendSprite(shotsVertexArray[7], shotsController.SheetWaterElemProjectile, tempPos, (sfVector2f) { 0.4f, 0.4f }, sfWhite, angleRotation, tempShot->shotAnimKey->actualFrameToShow, 4, 4);
			AppendSprite(shotsVertexArray[13], shotsController.SheetLightWaterElemProjectile, tempPos, (sfVector2f) { 0.4f, 0.4f }, sfWhite, angleRotation, tempShot->shotAnimKey->actualFrameToShow, 4, 4);
		}
		else if (tempShot->IdShot == iceElemShot)
		{
			float angleRotation = RadianToDegree(atan2f(tempShot->Direction.y, tempShot->Direction.x));
			AppendSprite(shotsVertexArray[8], shotsController.SheetIceElemProjectile, tempPos, (sfVector2f) { 0.6f, 0.6f }, sfWhite, angleRotation, tempShot->shotAnimKey->actualFrameToShow, 0, 0);
			AppendSprite(shotsVertexArray[15], shotsController.SheetLightIceElemProjectile, tempPos, (sfVector2f) { 0.6f, 0.6f }, sfWhite, angleRotation, tempShot->shotAnimKey->actualFrameToShow, 0, 0);
		}
		else if (tempShot->IdShot == iceBossElemShot)
		{
			float angleRotation = RadianToDegree(atan2f(tempShot->Direction.y, tempShot->Direction.x));
			AppendSprite(shotsVertexArray[8], shotsController.SheetIceElemProjectile, tempPos, (sfVector2f) { 0.8f, 0.8f }, sfWhite, angleRotation, tempShot->shotAnimKey->actualFrameToShow, 0, 0);
			AppendSprite(shotsVertexArray[15], shotsController.SheetLightIceElemProjectile, tempPos, (sfVector2f) { 0.8f, 0.8f }, sfWhite, angleRotation, tempShot->shotAnimKey->actualFrameToShow, 0, 0);
		}
		else if (tempShot->IdShot == fireBallBossElemShot)
		{
			float angleRotation = RadianToDegree(atan2f(tempShot->Direction.y, tempShot->Direction.x));
			AppendSprite(shotsVertexArray[6], shotsController.SheetFireElemProjectile, tempPos, (sfVector2f) { 1.2f, 1.2f }, sfWhite, angleRotation, tempShot->shotAnimKey->actualFrameToShow, 4, 3);
			AppendSprite(shotsVertexArray[12], shotsController.SheetLightFireElemProjectile, tempPos, (sfVector2f) { 1.2f, 1.2f }, sfWhite, angleRotation, tempShot->shotAnimKey->actualFrameToShow, 4, 3);
		}
		else if (tempShot->IdShot == iceShard)
		{
			float angleRotation = RadianToDegree(atan2f(tempShot->Direction.y, tempShot->Direction.x));
			AppendSprite(shotsVertexArray[9], shotsController.SheetIceShardProjectile[0], tempPos, (sfVector2f) { 0.6f, 0.6f }, sfWhite, angleRotation, tempShot->shotAnimKey->actualFrameToShow, 0, 0);
			AppendSprite(shotsVertexArray[16], shotsController.SheetLightIceShardProjectile[0], tempPos, (sfVector2f) { 0.6f, 0.6f }, sfWhite, angleRotation, tempShot->shotAnimKey->actualFrameToShow, 0, 0);
		}
		else if (tempShot->IdShot == windElemShot)
		{
			float angleRotation = RadianToDegree(atan2f(tempShot->Direction.y, tempShot->Direction.x));
			AppendSprite(shotsVertexArray[10], shotsController.SheetWindElemProjectile, tempPos, (sfVector2f) { 0.4f, 0.4f }, sfWhite, angleRotation, tempShot->shotAnimKey->actualFrameToShow, 4, 3);
			AppendSprite(shotsVertexArray[14], shotsController.SheetLightWindElemProjectile, tempPos, (sfVector2f) { 0.4f, 0.4f }, sfWhite, angleRotation, tempShot->shotAnimKey->actualFrameToShow, 4, 3);
		}
		else if (tempShot->IdShot == dirtElemShot)
		{
			sfVector2f tempScale = shotsController.Ratio;
			tempPos.x += -72.f * tempShot->Direction.x * tempScale.x;
			tempPos.y += -72.f * tempShot->Direction.y * tempScale.y;
			float angleRotation = RadianToDegree(atan2f(tempShot->Direction.y, tempShot->Direction.x));
			AppendSprite(shotsVertexArray[11], shotsController.SheetDirtElemProjectile, tempPos, (sfVector2f) { 0.6f, 0.6f }, sfWhite, angleRotation, tempShot->shotAnimKey->actualFrameToShow, 4, 4);
		}
		else if (tempShot->IdShot == razarShot)
		{
			sfVector2f tempScale = { 1.f / 2.f, 1.f / 2.f };
			float angleRotation = RadianToDegree(atan2f(tempShot->Direction.y, tempShot->Direction.x));
			tempPos.x += -50.f * tempShot->Direction.x * tempScale.x;
			tempPos.y += -50.f * tempShot->Direction.y * tempScale.y;
			AppendSprite(shotsVertexArray[18], shotsController.SheetRazarShot, tempPos, tempScale, sfWhite, angleRotation, tempShot->shotAnimKey->actualFrameToShow, 3, 4);
		}
		else if (tempShot->IdShot == razarBlockShot)
		{
			AppendSprite(shotsVertexArray[19], shotsController.SheetVoidGround, tempPos, (sfVector2f) { 1.2f, 1.2f }, sfWhite, 0.0f, tempShot->shotAnimKey->actualFrameToShow, 4, 6);
		}
		else if (tempShot->IdShot == razarWindShot)
		{
			float angleRotation = RadianToDegree(atan2f(tempShot->Direction.y, tempShot->Direction.x));
			AppendSprite(shotsVertexArray[17], shotsController.SheetVoidBalls, tempPos, (sfVector2f) { 1.f / 3.f, 1.f / 3.f }, sfWhite, angleRotation, tempShot->shotAnimKey->actualFrameToShow, 3, 4);
		}
		else if (tempShot->IdShot == razarBoomerangShot)
		{
			float angleRotation = RadianToDegree(atan2f(tempShot->Direction.y, tempShot->Direction.x));
			AppendSprite(shotsVertexArray[17], shotsController.SheetVoidBalls, tempPos, (sfVector2f) { 0.5f, 0.5f }, sfWhite, angleRotation, tempShot->shotAnimKey->actualFrameToShow, 3, 4);
		}
		else if (tempShot->IdShot == razarLittleFragShot)
		{
			printf("Little = %d\n", tempShot->shotAnimKey->actualFrameToShow);
			AppendSprite(shotsVertexArray[20], shotsController.SheetRazarLittleFrag, tempPos, (sfVector2f) { 15.f / 115.f, 15.f / 115.f }, sfWhite, 0.0f, tempShot->shotAnimKey->actualFrameToShow, 3, 3);
			AppendSprite(shotsVertexArray[23], shotsController.SheetLightRazarLittleFrag, tempPos, (sfVector2f) { 15.f / 115.f, 15.f / 115.f }, sfWhite, 0.0f, tempShot->shotAnimKey->actualFrameToShow, 3, 3);
		}
		else if (tempShot->IdShot == razarMediumFragShot)
		{
			printf("Medium = %d\n", tempShot->shotAnimKey->actualFrameToShow);
			AppendSprite(shotsVertexArray[21], shotsController.SheetRazarMediumFrag, tempPos, (sfVector2f) { 30.f / 115.f, 30.f / 115.f }, sfWhite, 0.0f, tempShot->shotAnimKey->actualFrameToShow, 3, 3);
			AppendSprite(shotsVertexArray[24], shotsController.SheetLightRazarMediumFrag, tempPos, (sfVector2f) { 30.f / 115.f, 30.f / 115.f }, sfWhite, 0.0f, tempShot->shotAnimKey->actualFrameToShow, 0, 0);
		}
		else if (tempShot->IdShot == razarBigFragShot)
		{
			printf("Big = %d\n", tempShot->shotAnimKey->actualFrameToShow);
			AppendSprite(shotsVertexArray[22], shotsController.SheetRazarBigFrag, tempPos, (sfVector2f) { 45.f / 115.f, 45.f / 115.f }, sfWhite, 0.0f, tempShot->shotAnimKey->actualFrameToShow, 3, 3);
			AppendSprite(shotsVertexArray[25], shotsController.SheetLightRazarBigFrag, tempPos, (sfVector2f) { 45.f / 115.f, 45.f / 115.f }, sfWhite, 0.0f, tempShot->shotAnimKey->actualFrameToShow, 0, 0);
		}
		/*else
		{
			BlitSprite(window, shotsController.BossAtack, tempPos, NULL);
		}*/
#if _DEBUG	
		if (toShow)
		{
			sfCircleShape_setPosition(tempShot->Shape, tempShot->Circle.center);
			sfRenderWindow_drawCircleShape(window, tempShot->Shape, NULL);
		}
#endif // _DEBUG

		if (tempShot->Next != NULL)
		{
			tempShot = tempShot->Next;
		}
		else
		{
			canRepeat = sfFalse;
		}
	}
}

void DisplayShotVertexArray(sfRenderWindow* window)
{
	sfRenderTexture* enemyLightTexture = GetEnemiLightTexture();
	BlitVertexArray(window, shotsVertexArray[0], shotsController.SheetBasicAttack);
	BlitVertexArray(window, shotsVertexArray[1], GetSpells().SheetFireBall);
	BlitVertexArray(window, shotsVertexArray[2], shotsController.SheetRangeBall);
	BlitVertexArray(window, shotsVertexArray[3], shotsController.BossAtack);
	BlitVertexArray(window, shotsVertexArray[4], shotsController.MiniBossAtack);
	BlitVertexArray(window, shotsVertexArray[5], shotsController.SheetKamikazeProjectile);
	BlitVertexArray(window, shotsVertexArray[6], shotsController.SheetFireElemProjectile);
	BlitVertexArray(window, shotsVertexArray[7], shotsController.SheetWaterElemProjectile);
	BlitVertexArray(window, shotsVertexArray[8], shotsController.SheetIceElemProjectile);
	BlitVertexArray(window, shotsVertexArray[9], shotsController.SheetIceShardProjectile[0]);
	BlitVertexArray(window, shotsVertexArray[10], shotsController.SheetWindElemProjectile);
	BlitVertexArray(window, shotsVertexArray[11], shotsController.SheetDirtElemProjectile);
	BlitVertexArray(window, shotsVertexArray[17], shotsController.SheetVoidBalls);
	BlitVertexArray(window, shotsVertexArray[18], shotsController.SheetRazarShot);
	BlitVertexArray(window, shotsVertexArray[19], shotsController.SheetVoidGround);
	BlitVertexArray(window, shotsVertexArray[20], shotsController.SheetRazarLittleFrag);
	BlitVertexArray(window, shotsVertexArray[21], shotsController.SheetRazarMediumFrag);
	BlitVertexArray(window, shotsVertexArray[22], shotsController.SheetRazarBigFrag);

	BlitVertexArrayLight(shotsVertexArray[12], enemyLightTexture, shotsController.SheetLightFireElemProjectile);
	BlitVertexArrayLight(shotsVertexArray[13], enemyLightTexture, shotsController.SheetLightWaterElemProjectile);
	BlitVertexArrayLight(shotsVertexArray[14], enemyLightTexture, shotsController.SheetLightWindElemProjectile);
	BlitVertexArrayLight(shotsVertexArray[15], enemyLightTexture, shotsController.SheetLightIceElemProjectile);
	BlitVertexArrayLight(shotsVertexArray[16], enemyLightTexture, shotsController.SheetLightIceShardProjectile[0]);

	BlitVertexArrayLight(shotsVertexArray[23], enemyLightTexture, shotsController.SheetLightRazarLittleFrag);
	BlitVertexArrayLight(shotsVertexArray[24], enemyLightTexture, shotsController.SheetLightRazarMediumFrag);
	BlitVertexArrayLight(shotsVertexArray[25], enemyLightTexture, shotsController.SheetLightRazarBigFrag);
}

void ClearShotVertexArray()
{
	for (int i = 0; i < NBR_VERTEX_ARRAY; i++)
	{
		sfVertexArray_clear(shotsVertexArray[i]);
	}
}

void UnloadShotsGraphicsSlime()
{
	DestroySfSprite(&shotsController.SheetBasicAttack);
	DestroySfSprite(&shotsController.SheetRangeBall);
	DestroySfSprite(&shotsController.BossAtack);
	DestroySfSprite(&shotsController.MiniBossAtack);
	DestroySfSprite(&shotsController.SheetKamikazeProjectile);
	for (int i = 0; i < NBR_VERTEX_ARRAY; i++)
	{
		sfVertexArray_destroy(shotsVertexArray[i]);
	}
}

void UnloadShotsGraphicsElem()
{
	DestroySfSprite(&shotsController.BossAtack);
	DestroySfSprite(&shotsController.SheetDirtElemProjectile);
	DestroySfSprite(&shotsController.SheetFireElemProjectile);
	DestroySfSprite(&shotsController.SheetLightFireElemProjectile);
	DestroySfSprite(&shotsController.SheetIceElemProjectile);
	DestroySfSprite(&shotsController.SheetIceShardProjectile[0]);
	DestroySfSprite(&shotsController.SheetIceShardProjectile[1]);
	DestroySfSprite(&shotsController.SheetIceShardProjectile[2]);

	DestroySfSprite(&shotsController.SheetLightIceElemProjectile);
	DestroySfSprite(&shotsController.SheetLightIceShardProjectile[0]);
	DestroySfSprite(&shotsController.SheetLightIceShardProjectile[1]);
	DestroySfSprite(&shotsController.SheetLightIceShardProjectile[2]);
	DestroySfSprite(&shotsController.SheetWindElemProjectile);
	DestroySfSprite(&shotsController.SheetLightWindElemProjectile);
	DestroySfSprite(&shotsController.SheetWaterElemProjectile);
	DestroySfSprite(&shotsController.SheetLightWaterElemProjectile);

	for (int i = 0; i < NBR_VERTEX_ARRAY; i++)
	{
		sfVertexArray_destroy(shotsVertexArray[i]);
	}
}