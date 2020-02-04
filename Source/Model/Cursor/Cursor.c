#include "Cursor.h"

Sight  InitializeCursor()
{
	Sight  newCursor;
	newCursor.Position = (sfVector2f) { 0.0f, 0.0f };
	newCursor.Scale = (sfVector2f) { 0.5f, 0.5f };
	return newCursor;
}

void UpdateScaleCursor(Sight * pCursor, const float pReScale, const float pDeltaTime)
{
	pCursor->Scale.x = Lerp(pCursor->Scale.x, pReScale, pDeltaTime);
	pCursor->Scale.y = pCursor->Scale.x;
}

