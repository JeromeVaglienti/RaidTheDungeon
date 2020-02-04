#ifndef _CURSOR_H
#define _CURSOR_H
#include <stdio.h>
#include <stdlib.h>
#include "../Math/MathUtilities.h"

typedef struct SIGHT
{
	sfVector2f Position;
	sfVector2f Scale;
}Sight;

Sight  InitializeCursor();
//Change scale sight
void UpdateScaleCursor(Sight * pCursor, const float pReScale, const float pDeltaTime);

#endif