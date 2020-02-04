#ifndef _PROCEDURALMAIN_H
#define _PROCEDURALMAIN_H

#include "ProceduralType.h"

void GenerateDungeon(Dungeon* newDungeon, const int numberRoom, const float pArea, const float pMargin, const float pmaxWidth, const DungeonId ID);

void DrawSupp(sfRenderWindow* wind);
#endif // !_PROCEDURALMAIN_H