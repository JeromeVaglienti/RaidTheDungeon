#ifndef _WORLDMAPCONTROLLER
#define _WORLDMAPCONTROLLER

#include <stdio.h>
#include <stdlib.h>
#include <SFML\Graphics.h>
#include "..\GraphicsUtilities\Utilities.h"

typedef struct
{
	sfSprite* WorldMap;
}WordlMapRessources;

void InitWorldMapController(const sfVideoMode mode);
void UpdateWorldMapController(const float deltaTime);
void DisplayWorldMapController(sfRenderWindow* mainWindow);

#endif