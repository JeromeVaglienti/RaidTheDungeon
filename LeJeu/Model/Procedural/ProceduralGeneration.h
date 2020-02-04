#ifndef _PROCEDURALGENERATION_H
#define _PROCEDURALGENERATION_H
#include <Windows.h>
#include "ProceduralType.h"
#include "..\..\Controller\State\State.h"

sfBool GenerateDungeonRD(DungeonRawData* DungeonData, const int pNumberOfRoom, const sfVector2f pCenter, const float pArea, const float pMargin, const float pmaxWidth, RoomPrefab *arrayOfPrebab, const DWORD ThreadID, const long int previousSeed);
sfBool GenerateDungeonNoCorri(DungeonRawData* DungeonData, const int pNumberOfRoom, const sfVector2f pCenter, const float pArea, const float pMargin, const float pmaxWidth, RoomPrefab *arrayOfPrebab, const DWORD ThreadID, const long int previousSeed);
sfBool MultiProceduralGeneration(DungeonRawData* DungeonData, const int pNumberOfRoom, const sfVector2f pCenter, const float pArea, const float pMargin, const float pmaxWidth, RoomPrefab *arrayOfPrebab, const int amountParallalThread, const DungeonId dungeonId);


#endif // !_PROCEDURALGENERATION_H
