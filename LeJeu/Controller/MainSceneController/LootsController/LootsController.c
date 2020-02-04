#include "LootsController.h"
#include "..\..\GraphicsUtilities\Utilities.h"

LootsController lootsController = { 0 };

void InitLootsGraphics()
{
	lootsController.Loots = (sfSprite**)calloc(4, (sizeof(sfSprite*)));
	lootsController.Loots[0] = LoadSprite("Ressources/Sprites/Loot/littleBottleOfMana.png", 1);
	sfSprite_setScale(lootsController.Loots[0], (sfVector2f) { 0.5f, 0.5f });

	lootsController.Loots[1] = LoadSprite("Ressources/Sprites/Loot/bigBottleOfMana.png", 1);
	sfSprite_setScale(lootsController.Loots[1], (sfVector2f) { 0.5f, 0.5f });
	
	lootsController.Loots[2] = LoadSprite("Ressources/Sprites/Loot/littleBottleOfLife.png", 1);
	sfSprite_setScale(lootsController.Loots[2], (sfVector2f) { 0.5f, 0.5f });
	
	lootsController.Loots[3] = LoadSprite("Ressources/Sprites/Loot/bigBottleOfLife.png", 1);
	sfSprite_setScale(lootsController.Loots[3], (sfVector2f) { 0.5f, 0.5f });
}

void DisplayLoots(const LootData lootData, sfRenderWindow* window, const Camera pCam)
{
	for (int i = 0; i < lootData.NbrLoot; i++)
	{
		if (DrawOnCamera(pCam, lootsController.Loots[lootData.Loot[i].Id], lootData.Loot[i].Pos))
		{
			BlitSprite(window, lootsController.Loots[lootData.Loot[i].Id], lootData.Loot[i].Pos, NULL);
		}
	}
}