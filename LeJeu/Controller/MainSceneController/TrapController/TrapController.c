#include "TrapController.h"
#include "..\ShadersController\ShadersController.h"


TrapContainer* trapContainter = NULL;


void LoadSlimeTraps()
{
	trapContainter = (TrapContainer*)calloc(1, sizeof(TrapContainer));

	trapContainter->SheetMiniBossFlask = (sfSprite**)calloc(1, sizeof(sfSprite*));
	
	*trapContainter->SheetMiniBossFlask = LoadSprite("Ressources/Sprites/Trap/Dungeon1/MiniBossFlask/spritesheet.png", 1);

	trapContainter->flaskVertexArray = sfVertexArray_create();
	sfVertexArray_setPrimitiveType(trapContainter->flaskVertexArray, sfQuads);

}

void UnLoadSlimeTraps()
{
	DestroySfSprite(trapContainter->SheetMiniBossFlask);

	free(trapContainter->SheetMiniBossFlask);
	
	sfVertexArray_clear(trapContainter->flaskVertexArray);

	free(trapContainter);
	trapContainter = NULL;
}

void LoadElemTraps()
{
	trapContainter = (TrapContainer*)calloc(1, sizeof(TrapContainer));
	trapContainter->RockSpike = (sfSprite**)calloc(1, sizeof(sfSprite*));
	trapContainter->SheetThunderFirstLayer = (sfSprite**)calloc(1, sizeof(sfSprite*));
	trapContainter->SheetThunderSecondLayer = (sfSprite**)calloc(1, sizeof(sfSprite*));
	trapContainter->SheetMapThunderFirstLayer = (sfSprite**)calloc(1, sizeof(sfSprite*));
	trapContainter->SheetMapThunderSecondLayer = (sfSprite**)calloc(1, sizeof(sfSprite*));

	*trapContainter->RockSpike = LoadSprite("Ressources/Sprites/Trap/Dungeon2/AttaqueRock/spritesheet.png", 1);
	*trapContainter->SheetThunderFirstLayer = LoadSprite("Ressources/Sprites/Trap/Dungeon2/Tonnerre/spritesheet.png", 1);
	*trapContainter->SheetThunderSecondLayer = LoadSprite("Ressources/Sprites/Trap/Dungeon2/Tonnerre/spritesheet2.png", 1);
	*trapContainter->SheetMapThunderFirstLayer = LoadSprite("Ressources/Sprites/Trap/Dungeon2/Tonnerre/LightMap_spritesheet.png", 1);
	*trapContainter->SheetMapThunderSecondLayer = LoadSprite("Ressources/Sprites/Trap/Dungeon2/Tonnerre/LightMap_spritesheet2.png", 1);

	trapContainter->rockSpikeVertexArray = sfVertexArray_create();
	sfVertexArray_setPrimitiveType(trapContainter->rockSpikeVertexArray, sfQuads);
	trapContainter->lightningVertexArray = sfVertexArray_create();
	sfVertexArray_setPrimitiveType(trapContainter->lightningVertexArray, sfQuads);
	trapContainter->lightningVertexArray2 = sfVertexArray_create();
	sfVertexArray_setPrimitiveType(trapContainter->lightningVertexArray2, sfQuads);

	trapContainter->lightningMapVertexArray = sfVertexArray_create();
	sfVertexArray_setPrimitiveType(trapContainter->lightningMapVertexArray, sfQuads);
	trapContainter->lightningMapVertexArray2 = sfVertexArray_create();
	sfVertexArray_setPrimitiveType(trapContainter->lightningMapVertexArray2, sfQuads);
}

void UnLoadElemTraps()
{
	DestroySfSprite(trapContainter->RockSpike);
	DestroySfSprite(trapContainter->SheetThunderFirstLayer);
	DestroySfSprite(trapContainter->SheetThunderSecondLayer);
	DestroySfSprite(trapContainter->SheetMapThunderFirstLayer);
	DestroySfSprite(trapContainter->SheetMapThunderSecondLayer);
	
	free(trapContainter->RockSpike);
	free(trapContainter->SheetThunderFirstLayer);
	free(trapContainter->SheetThunderSecondLayer);
	free(trapContainter->SheetMapThunderFirstLayer);
	free(trapContainter->SheetMapThunderSecondLayer);
	
	sfVertexArray_clear(trapContainter->rockSpikeVertexArray);
	sfVertexArray_clear(trapContainter->lightningVertexArray);
	sfVertexArray_clear(trapContainter->lightningVertexArray2);
	sfVertexArray_clear(trapContainter->lightningMapVertexArray);
	sfVertexArray_clear(trapContainter->lightningMapVertexArray2);
	
	free(trapContainter);
	trapContainter = NULL;
}

void DrawTrapFirstLayer(sfRenderWindow* window)
{
	TrapData* trapData = GetPointerToTrapData();
	DungeonId dungeonId = GetActualDungeonId();
	for (int i = 0; i < (int)trapData->NbrTrap; i++)
	{
		if (trapData->Traps[i].Id == flask)
		{
			AppendSprite(trapContainter->flaskVertexArray, *trapContainter->SheetMiniBossFlask, trapData->Traps[i].Pos, (sfVector2f) { 1.0f, 1.0f}, sfWhite, 0.0f, trapData->Traps[i].KeyAnimation.actualFrameToShow, 4, 4);
		}
		else if (trapData->Traps[i].Id == lightning)
		{
			sfVector2f tempPos = { trapData->Traps[i].Pos.x, trapData->Traps[i].Pos.y - 15.f };
			AppendSprite(trapContainter->lightningVertexArray, *trapContainter->SheetThunderFirstLayer, tempPos, (sfVector2f) { 1.0f, 1.0f }, sfWhite, 0.0f, trapData->Traps[i].KeyAnimation.actualFrameToShow, 8, 3);
			AppendSprite(trapContainter->lightningMapVertexArray, *trapContainter->SheetMapThunderFirstLayer, tempPos, (sfVector2f) { 1.0f, 1.0f }, sfWhite, 0.0f, trapData->Traps[i].KeyAnimation.actualFrameToShow, 8, 3);
		}
		else if (trapData->Traps[i].Id == rockSpike)
		{
			AppendSprite(trapContainter->rockSpikeVertexArray, *trapContainter->RockSpike, trapData->Traps[i].Pos, (sfVector2f) { 1.0f, 1.0f }, sfWhite, 0.0f, trapData->Traps[i].KeyAnimation.actualFrameToShow, 5, 5);
		}
		else if (trapData->Traps[i].Id == rockSpikeBoss)
		{
			AppendSprite(trapContainter->rockSpikeVertexArray, *trapContainter->RockSpike, trapData->Traps[i].Pos, (sfVector2f) { 1.5f, 1.5f }, sfWhite, 0.0f, trapData->Traps[i].KeyAnimation.actualFrameToShow, 5, 5);
		}
	}
	if (dungeonId == slimeDungeon || dungeonId == beginningDungeon)
	{
		BlitVertexArray(window, trapContainter->flaskVertexArray, *trapContainter->SheetMiniBossFlask);
	}
	else if (dungeonId == elemDungeon)
	{
		BlitVertexArray(window, trapContainter->lightningVertexArray, *trapContainter->SheetThunderFirstLayer);
		BlitVertexArray(window, trapContainter->rockSpikeVertexArray, *trapContainter->RockSpike);
		BlitVertexArrayLight(trapContainter->lightningMapVertexArray, GetEnemiLightTexture(), *trapContainter->SheetMapThunderFirstLayer);
	}
}

void DrawTrapSecondLayer(sfRenderWindow* window)
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
	TrapData* trapData = GetPointerToTrapData();
	for (int i = 0; i < (int)trapData->NbrTrap; i++)
	{
		if (trapData->Traps[i].Id == lightning && trapData->Traps[i].KeyAnimation.actualFrameToShow > 3 && trapData->Traps[i].KeyAnimation.actualFrameToShow < 21)
		{
			sfVector2f tempPos = { trapData->Traps[i].Pos.x, trapData->Traps[i].Pos.y - 368.5f };
			AppendSprite(trapContainter->lightningVertexArray2, *trapContainter->SheetThunderSecondLayer, tempPos, (sfVector2f) {1.0f, 1.0f }, sfWhite, 0.0f, trapData->Traps[i].KeyAnimation.actualFrameToShow - 4, 8, 2);
			AppendSprite(trapContainter->lightningMapVertexArray2, *trapContainter->SheetMapThunderSecondLayer, tempPos, (sfVector2f) { 1.0f, 1.0f }, sfWhite, 0.0f, trapData->Traps[i].KeyAnimation.actualFrameToShow - 4, 8, 2);
		}
	}
	BlitVertexArray(window, trapContainter->lightningVertexArray2, *trapContainter->SheetThunderSecondLayer);
	BlitVertexArrayLight(trapContainter->lightningMapVertexArray2, GetEnemiLightTexture(), *trapContainter->SheetMapThunderSecondLayer);
#if _DEBUG	
	if (toShow)
	{
		for (int i = 0; i < (int)trapData->NbrTrap; i++)
		{
			sfCircleShape_setPosition(trapData->Traps[i].Shape, trapData->Traps[i].Collider.center);
			sfRenderWindow_drawCircleShape(window, trapData->Traps[i].Shape, NULL);
		}
	}
#endif // _DEBUG
}

