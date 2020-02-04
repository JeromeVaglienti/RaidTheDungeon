#ifndef _PROCEDURALCONVERSION_H
#define _PROCEDURALCONVERSION_H

#include "ProceduralType.h"

void ClearSfVector2fList(Vector2fList **firstElement);
void ClearGraphicData(DungeonGraphicData *ToClean);
void ConvertToGraphicData(DungeonGraphicData *DungeonGD, DungeonRawData* DungeonRD);

#endif // !_PROCEDURALCONVERSION_H
