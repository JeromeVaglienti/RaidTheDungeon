#ifndef _TRAPCONTROLLER
#define _TRAPCONTROLLER

#include <stdio.h>
#include <stdlib.h>
#include <SFML\Graphics.h>
#include "..\..\GraphicsUtilities\Utilities.h"
#include "..\..\..\Model\Trap\Trap.h"

typedef struct
{
	sfVertexArray* flaskVertexArray;
	sfVertexArray* lightningVertexArray;
	sfVertexArray* lightningVertexArray2;

	sfVertexArray* lightningMapVertexArray;
	sfVertexArray* lightningMapVertexArray2;

	sfVertexArray* rockSpikeVertexArray;

	sfSprite** RockSpike;
	sfSprite** SheetMiniBossFlask;
	sfSprite** SheetThunderFirstLayer;
	sfSprite** SheetThunderSecondLayer;
	sfSprite** SheetMapThunderFirstLayer;
	sfSprite** SheetMapThunderSecondLayer;
}TrapContainer;


void LoadSlimeTraps();
void UnLoadSlimeTraps();
void LoadElemTraps();
void UnLoadElemTraps();
void DrawTrapFirstLayer(sfRenderWindow* window);
void DrawTrapSecondLayer(sfRenderWindow* window);


#endif // !_TRAPCONTROLLER
