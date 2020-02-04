#ifndef _LOOTSCONTROLLER_H
#define _LOOTSCONTROLLER_H
#include "..\..\..\Model\Loot\Loot.h"
#include "..\..\Camera\Camera.h"

typedef struct LootsController
{
	sfSprite** Loots;
}LootsController;

void InitLootsGraphics();

void DisplayLoots(const LootData lootData, sfRenderWindow* window, const Camera pCam);

#endif // !_SHOTSCONTROLLER_H