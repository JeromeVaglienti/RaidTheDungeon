#include "ProceduralGeneration.h"
#include "..\Math\Vector\VectorUtilities.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

void SortEdge(EdgeList** pFirstEdge)
{
	sfBool permutation;
	EdgeList* currentEdge = *pFirstEdge;
	EdgeList* nEdge = NULL;

	if (currentEdge == NULL)
	{
		return;
	}

	//printf("////////////////////////\n");
	do
	{
		currentEdge = *pFirstEdge;
		permutation = sfFalse;

		while (currentEdge->Next != nEdge)
		{
			if (currentEdge->Weight > currentEdge->Next->Weight)
			{
				unsigned char tmp = currentEdge->Weight;
				currentEdge->Weight = currentEdge->Next->Weight;
				currentEdge->Next->Weight = tmp;

				tmp = currentEdge->Index;
				currentEdge->Index = currentEdge->Next->Index;
				currentEdge->Next->Index = tmp;

				Room* tmpRm[2];
				tmpRm[0] = currentEdge->Rooms[0];
				currentEdge->Rooms[0] = currentEdge->Next->Rooms[0];
				currentEdge->Next->Rooms[0] = tmpRm[0];

				tmpRm[1] = currentEdge->Rooms[1];
				currentEdge->Rooms[1] = currentEdge->Next->Rooms[1];
				currentEdge->Next->Rooms[1] = tmpRm[1];

				permutation = sfTrue;
			}
			currentEdge = currentEdge->Next;
		}

		nEdge = currentEdge;

	} while (permutation);
}

sfBool IsAlreadyExist(EdgeList** pFirstEdge, Room* pRm1, Room* pRm2)
{
	sfBool b = sfFalse;
	EdgeList* currentEdge = *pFirstEdge;

	while (currentEdge != NULL)
	{
		if ((currentEdge->Rooms[0] == pRm1 && currentEdge->Rooms[1] == pRm2) ||
			(currentEdge->Rooms[1] == pRm1 && currentEdge->Rooms[0] == pRm2))
		{
			b = sfTrue;
			currentEdge = NULL;
			return b;
		}
		currentEdge = currentEdge->Next;
	}

	return b;
}

//return group
short IsRoomAlreadyExist(EdgeList** pFirstEdge, Room* pRm)
{
	EdgeList* currentEdge = *pFirstEdge;
	short index = -1;
	while (currentEdge != NULL)
	{
		if ((currentEdge->Rooms[0] == pRm || currentEdge->Rooms[1] == pRm))
		{
			if (index == -1 || index > currentEdge->GroupIndex)
				index = currentEdge->GroupIndex;
		}

		currentEdge = currentEdge->Next;
	}

	return index;
}

void ChangingIndexGroup(EdgeList** pFirstEdge)
{
	EdgeList* currentEdge = *pFirstEdge;
	int index1 = -1;
	int index2 = -1;
	while (currentEdge != NULL)
	{
		index1 = IsRoomAlreadyExist(pFirstEdge, currentEdge->Rooms[0]);
		index2 = IsRoomAlreadyExist(pFirstEdge, currentEdge->Rooms[1]);
		if (index1 != -1 && index2 != -1)
		{
			if (index1 < index2)
			{
				currentEdge->GroupIndex = index1;
			}
			else
			{
				currentEdge->GroupIndex = index2;
			}
		}

		currentEdge = currentEdge->Next;
	}
}

void AddEdge(EdgeList** pFirstEdge, const short pIndex, Room* pRm1, Room* pRm2)
{
	EdgeList* newEdge = (EdgeList*)malloc(sizeof(EdgeList));

	newEdge->Rooms[0] = pRm1;
	newEdge->Rooms[1] = pRm2;
	newEdge->Weight = rand() % 5;
	newEdge->Index = 0;
	newEdge->Prev = NULL;
	newEdge->Next = NULL;
	newEdge->Tail = NULL;
	newEdge->GroupIndex = pIndex;

	if (*pFirstEdge != NULL)
	{
		newEdge->Index = (*pFirstEdge)->Tail->Index + 1;
		newEdge->Prev = (*pFirstEdge)->Tail;
		(*pFirstEdge)->Tail->Next = newEdge;
		(*pFirstEdge)->Tail = (*pFirstEdge)->Tail->Next;

	}
	else
	{
		*pFirstEdge = newEdge;
		(*pFirstEdge)->Tail = *pFirstEdge;
	}

	newEdge->door[0] = newEdge->door[1] = NULL;
}

void DeleteEdge(EdgeList** pFirstEdge, const short pIndex)
{
	EdgeList* currentEdge = *pFirstEdge;

	if (*pFirstEdge == NULL)
		return;

	while (currentEdge != NULL)
	{

		//to do
		if (currentEdge->Index == pIndex)
		{

			currentEdge->Prev->Next = currentEdge->Next;

			currentEdge->Next->Prev = currentEdge->Prev;

			if (currentEdge->Next == NULL)
			{
				currentEdge->Tail = currentEdge->Prev;
			}

			free(currentEdge);

			return;
		}
		currentEdge = currentEdge->Next;
	}
}

void InitalizeRoom(Room* pRoom, const sfVector2f pCenter, const float pArea)
{
	int radiusMax = 5;
	pRoom->nbDoors = 0;
	pRoom->Index = 0;
	pRoom->Position = (sfVector2f) { (float)(pCenter.x + (rand() % radiusMax)), (float)(pCenter.y + (rand() % radiusMax)) };
	pRoom->Rect = sfRectangleShape_create();
	sfRectangleShape_setSize(pRoom->Rect, (sfVector2f) { pRoom->Width, pRoom->Height });
	sfRectangleShape_setFillColor(pRoom->Rect, sfRed);
	sfRectangleShape_setPosition(pRoom->Rect, pRoom->Position);
	sfRectangleShape_setOrigin(pRoom->Rect, (sfVector2f) { pRoom->Width / 2.0f, pRoom->Height / 2.0f });
}

void InitalizeDoor(Door* pDoor, const sfVector2f pPosition, const int dir)
{
	static unsigned char index = 0;

	float radius = 5.0f;
	pDoor->Position.x = pPosition.x;
	pDoor->Position.y = pPosition.y;
	pDoor->Circle = sfCircleShape_create();
	pDoor->Index = index;
	pDoor->Corridor = NULL;
	index++;
	pDoor->cardDir = dir;

	sfCircleShape_setRadius(pDoor->Circle, radius);
	sfCircleShape_setFillColor(pDoor->Circle, sfGreen);
	sfCircleShape_setPosition(pDoor->Circle, pPosition);
	sfCircleShape_setOrigin(pDoor->Circle, (sfVector2f) { radius, radius });
}

void ClearRooms(RoomList** pFirstRoom)
{

	RoomList* prevRoom, *deleteRoom;

	if (*pFirstRoom != NULL)
	{
		deleteRoom = (*pFirstRoom)->Tail;
		while (deleteRoom != NULL)
		{
			prevRoom = deleteRoom->Prev;
			sfRectangleShape_destroy(deleteRoom->rm.Rect);
			free(deleteRoom);
			deleteRoom = prevRoom;
		}
		*pFirstRoom = NULL;
	}
}

void ClearEdges(EdgeList** pFirstEdge)
{
	EdgeList* prevEdge, *deleteEdge;

	if (*pFirstEdge != NULL)
	{
		deleteEdge = (*pFirstEdge)->Tail;
		while (deleteEdge != NULL)
		{
			if (deleteEdge->door[0] != NULL)
			{
				if (deleteEdge->door[0]->Corridor != NULL)
				{
					if (deleteEdge->door[0]->Corridor->firstCheckPoint->CP.pos.x >= 0)
					{
						int cpt = 0;
						CorridorCheckPointsList *delCP = deleteEdge->door[0]->Corridor->firstCheckPoint;
						CorridorCheckPointsList *nextCP = NULL;
						while (delCP != NULL)
						{
							nextCP = delCP->Next;
							if (delCP->CP.CircleA != NULL && !delCP->CP.isIntersect)
							{
								sfCircleShape_destroy(delCP->CP.CircleA);
							}
							free(delCP);
							delCP = nextCP;
						}
					}
					deleteEdge->door[0]->Corridor->firstCheckPoint = NULL;
					if (deleteEdge->door[1]->Corridor->firstCheckPoint != NULL)
					{
						deleteEdge->door[1]->Corridor->firstCheckPoint = NULL;
					}
					free(deleteEdge->door[0]->Corridor);
					deleteEdge->door[0]->Corridor = NULL;
					deleteEdge->door[1]->Corridor = NULL;
				}
				sfCircleShape_destroy(deleteEdge->door[0]->Circle);
				sfCircleShape_destroy(deleteEdge->door[1]->Circle);
				deleteEdge->door[0] = NULL;
				deleteEdge->door[1] = NULL;
				deleteEdge->Rooms[0] = NULL;
				deleteEdge->Rooms[1] = NULL;
			}
			prevEdge = deleteEdge->Prev;
			free(deleteEdge);
			deleteEdge = prevEdge;
		}
		*pFirstEdge = NULL;
	}
}

void AddRoom(RoomList** pFirstRoom, const sfVector2f pCenter, const float pArea)
{
	RoomList* newRoom = (RoomList*)malloc(sizeof(RoomList));
	InitalizeRoom(&newRoom->rm, pCenter, pArea);
	newRoom->Prev = NULL;
	newRoom->Next = NULL;
	newRoom->Tail = NULL;

	if (*pFirstRoom != NULL)
	{
		newRoom->rm.Index = (*pFirstRoom)->Tail->rm.Index + 1;
		newRoom->Prev = (*pFirstRoom)->Tail;
		(*pFirstRoom)->Tail->Next = newRoom;
		(*pFirstRoom)->Tail = (*pFirstRoom)->Tail->Next;
	}
	else
	{
		sfRectangleShape_setFillColor(newRoom->rm.Rect, sfBlue);
		*pFirstRoom = newRoom;
		(*pFirstRoom)->Tail = *pFirstRoom;
	}
	DungeonId dungeonId = GetActualDungeonId();

	if ((dungeonId == slimeDungeon || dungeonId == beginningDungeon) && newRoom->rm.Index == 0)
	{
		newRoom->rm.Width = 20;
		newRoom->rm.Height = 18;
	}
	else if (dungeonId == elemDungeon && newRoom->rm.Index == 10)
	{
		newRoom->rm.Width = 40;
		newRoom->rm.Height = 45;
	}
	else
	{
		newRoom->rm.Width = 40;
		newRoom->rm.Height = 36;
	}

	if (newRoom->rm.Index >= AMOUNT_PREFAB_1ST_DUNGEON && dungeonId == slimeDungeon)
	{
		newRoom->rm.GraphicsIndex = 10;
	}
	else
	{
		newRoom->rm.GraphicsIndex = newRoom->rm.Index;
	}

}

void DeleteRoom(RoomList** pFirstRoom, const short pIndex)
{
	RoomList* currentRoom = *pFirstRoom;

	while (currentRoom != NULL)
	{
		//to do
		if (currentRoom->rm.Index == pIndex && currentRoom->Prev != NULL)
		{
			//printf("Find Index %d h  %d   w %d\n", currentRoom->rm.Index, currentRoom->rm.Height, currentRoom->rm.Width);

			currentRoom->Prev->Next = currentRoom->Next;

			currentRoom->Next->Prev = currentRoom->Prev;

			if (currentRoom->Next == NULL)
			{
				currentRoom->Tail = currentRoom;
			}

			free(currentRoom);

			return;
		}
		currentRoom = currentRoom->Next;
	}
}

//Simple swap of two Doors
void SwapDoorPos(Door *swapA, Door *swapB)
{
	sfVector2f temp = { swapA->Position.x, swapA->Position.y };

	swapA->Position.x = swapB->Position.x;
	swapA->Position.y = swapB->Position.y;

	swapB->Position.x = temp.x;
	swapB->Position.y = temp.y;

	sfCircleShape_setPosition(swapA->Circle, swapA->Position);
	sfCircleShape_setPosition(swapB->Circle, swapB->Position);
}

//Check Door distance, if they are too close space them out from what separate them to be at a minDistance from each others
void CheckDoorDist(Door *doorA, Door *doorB)
{
	sfVector2f twoDoors = AddTwoVectors(doorA->Position, doorB->Position);
	float temp;
	float minDistance = 0.0f;
	switch (doorA->cardDir)
	{
	case (W):
	case (E):
		minDistance = (SIZE_HORIZONTAL_Y_x64 * 2.0f) + 1.0f;
		temp = fabsf(twoDoors.y);
		if (temp < minDistance)
		{
			if (doorA->Position.y < doorB->Position.y)
			{
				doorA->Position.y -= (((minDistance - temp) / 2.0f) + 1.0f);
				doorB->Position.y += (((minDistance - temp) / 2.0f) + 1.0f);
			}
			else
			{
				doorB->Position.y -= (((minDistance - temp) / 2.0f) + 1.0f);
				doorA->Position.y += (((minDistance - temp) / 2.0f) + 1.0f);
			}

			sfCircleShape_setPosition(doorA->Circle, doorA->Position);
			sfCircleShape_setPosition(doorB->Circle, doorB->Position);
		}
		break;
	case(N):
	case(S):
		minDistance = (SIZE_VERTICAL_X_x64*2.0f) + 1.0f;
		temp = fabsf(twoDoors.x);
		if (temp < minDistance)
		{
			if (doorA->Position.x < doorB->Position.x)
			{
				doorA->Position.x -= (((minDistance - temp) / 2.0f) + 1.0f);
				doorB->Position.x += (((minDistance - temp) / 2.0f) + 1.0f);
			}
			else
			{
				doorB->Position.x -= (((minDistance - temp) / 2.0f) + 1.0f);
				doorA->Position.x += (((minDistance - temp) / 2.0f) + 1.0f);
			}
			sfCircleShape_setPosition(doorA->Circle, doorA->Position);
			sfCircleShape_setPosition(doorB->Circle, doorB->Position);
		}
		break;
	default:
		printf("Error\n");
		break;
	}
}

//Generate a Corridor From doorA to doorB
void GenerateCorridor(Door *doorA, Door *doorB)
{
	static unsigned char index = 0;

	doorA->Corridor = (Corridor*)calloc(1, sizeof(Corridor));
	doorA->Corridor->Index = index;
	index++;
	doorA->Corridor->corridorSize = 5;

	//0 is vertical 1 is horizontal
	unsigned short lastDir = 0;
	sfVector2f container[50];

	sfVector2f positionOfEnvil = { doorA->Position.x, doorA->Position.y };
	sfVector2f vectorEnvilEndPoint = AddTwoVectors(positionOfEnvil, doorB->Position);
	vectorEnvilEndPoint.x = fabsf(vectorEnvilEndPoint.x);
	vectorEnvilEndPoint.y = fabsf(vectorEnvilEndPoint.y);

	switch (doorA->cardDir)
	{
	case(N):
		lastDir = 0;
		break;
	case(S):
		lastDir = 0;
		break;
	case(W):
		lastDir = 1;
		break;
	case(E):
		lastDir = 1;
		break;
	default:
		break;
	}

	unsigned int checkPointsCount = 0;
	container[checkPointsCount].x = positionOfEnvil.x;
	container[checkPointsCount].y = positionOfEnvil.y;
	checkPointsCount++;

	vectorEnvilEndPoint = AddTwoVectors(positionOfEnvil, doorB->Position);
	vectorEnvilEndPoint.x = fabsf(vectorEnvilEndPoint.x);
	vectorEnvilEndPoint.y = fabsf(vectorEnvilEndPoint.y);

	//Envil movement in Manhattan specifications
	do
	{
		if (doorA->cardDir == N || doorA->cardDir == S)
		{
			if (lastDir == 0)
			{
				positionOfEnvil.y += ((positionOfEnvil.y <= doorB->Position.y) ? 1 : -1) * vectorEnvilEndPoint.y / 2.0f;
				lastDir = 1;
			}
			else
			{
				//Had Either all the distance of half so that the corridor is a stairs' step
				positionOfEnvil.x += ((positionOfEnvil.x <= doorB->Position.x) ? 1 : -1) * ((doorB->cardDir == S || doorB->cardDir == N) ? vectorEnvilEndPoint.x : vectorEnvilEndPoint.x / 2.0f);
				lastDir = 0;
			}
		}
		else
		{
			if (lastDir == 1)
			{
				positionOfEnvil.x += ((positionOfEnvil.x <= doorB->Position.x) ? 1 : -1) * vectorEnvilEndPoint.x / 2.0f;
				lastDir = 0;
			}
			else
			{
				positionOfEnvil.y += ((positionOfEnvil.y <= doorB->Position.y) ? 1 : -1) * ((doorB->cardDir == W || doorB->cardDir == E) ? vectorEnvilEndPoint.y : vectorEnvilEndPoint.y / 2.0f);
				lastDir = 1;
			}
		}

		//Check if envil has moved since last iteration
		if (container[checkPointsCount - 1].x != positionOfEnvil.x || container[checkPointsCount - 1].y != positionOfEnvil.y)
		{
			container[checkPointsCount].x = positionOfEnvil.x;
			container[checkPointsCount].y = positionOfEnvil.y;
			checkPointsCount++;
		}
		//Continue until Envil has reach the B Door
	} while (fabsf(positionOfEnvil.x - doorB->Position.x) > 1.0f || fabsf(positionOfEnvil.y - doorB->Position.y) > 1.0f);

	doorA->Corridor->firstCheckPoint = NULL;

	for (unsigned int i = 0; i < checkPointsCount; i++)
	{
		CorridorCheckPointsList *newCP = (CorridorCheckPointsList*)malloc(sizeof(CorridorCheckPointsList));
		newCP->CP.pos.x = container[i].x;
		newCP->CP.pos.y = container[i].y;

		newCP->CP.isIntersect = sfFalse;

		sfVector2f temp = { (float)newCP->CP.pos.x,  (float)newCP->CP.pos.y };

		newCP->CP.CircleA = sfCircleShape_create();
		sfCircleShape_setRadius(newCP->CP.CircleA, 2.f);
		sfCircleShape_setFillColor(newCP->CP.CircleA, sfCyan);
		sfCircleShape_setPosition(newCP->CP.CircleA, temp);
		sfCircleShape_setOrigin(newCP->CP.CircleA, (sfVector2f) { 2.f, 2.f });
		newCP->Next = doorA->Corridor->firstCheckPoint;
		doorA->Corridor->firstCheckPoint = newCP;
	}

	doorA->Corridor->doorsBack[0] = doorA;
	doorA->Corridor->doorsBack[1] = doorB;

	doorB->Corridor = doorA->Corridor;
}

//If Two Corridor are intersecting we had a IntersectCheckPoint
void AddIntersectCheckPoints(CorridorCheckPointsList** Aprevious, CorridorCheckPointsList** Bprevious, sfVector2f IntersectCoord)
{
	//Add An Intersect Checkpoint for Both Chained List 
	CorridorCheckPointsList *newCP = (CorridorCheckPointsList*)malloc(sizeof(CorridorCheckPointsList));
	newCP->CP.pos.x = IntersectCoord.x;
	newCP->CP.pos.y = IntersectCoord.y;
	newCP->CP.isIntersect = sfTrue;

	newCP->CP.CircleA = sfCircleShape_create();
	sfCircleShape_setRadius(newCP->CP.CircleA, 2.f);
	sfCircleShape_setFillColor(newCP->CP.CircleA, sfRed);
	sfCircleShape_setPosition(newCP->CP.CircleA, IntersectCoord);
	sfCircleShape_setOrigin(newCP->CP.CircleA, (sfVector2f) { 2.f, 2.f });

	//Link to firstCorridor
	CorridorCheckPointsList *newCPB = newCP;
	newCP->Next = (*Aprevious)->Next;
	(*Aprevious)->Next = newCP;

	//Link to secondCorridor
	newCPB = (*Bprevious)->Next;
	(*Bprevious)->Next = newCPB;
}

//Add two IntersectPoint in case there is a complex intersection
void AddTwoIntersectPoint(CorridorCheckPointsList** Aprevious, CorridorCheckPointsList** Bprevious)
{
	CorridorCheckPointsList *newCP[2] =
	{
		(CorridorCheckPointsList*)malloc(sizeof(CorridorCheckPointsList)),
		(CorridorCheckPointsList*)malloc(sizeof(CorridorCheckPointsList))
	};

	//Determine on wich axis the next Checkpoints have to be modified (reminder: we're using Manhattan principle)

	//Determine if it's on the X or Y axis
	sfBool isY = ((*Aprevious)->CP.pos.x - (*Bprevious)->CP.pos.x == 0);

	float tempAA2 = isY ? (float)((*Bprevious)->CP.pos.y - (*Aprevious)->CP.pos.y) : (float)((*Bprevious)->CP.pos.x - (*Aprevious)->CP.pos.x);
	float tempBA2 = isY ? (float)((*Bprevious)->CP.pos.y - (*Aprevious)->Next->CP.pos.y) : (float)((*Bprevious)->CP.pos.x - (*Aprevious)->Next->CP.pos.x);

	//If Sign of Vector are opposite the point is between the two points making the main vector that represent the actual segment
	//Mainly determine if the corridor is going from Aprevious or B Previous or the opposite in order to properly add thoses two Checkpoint to the current chained list
	sfBool A2In = sfFalse;
	if (Sign(tempAA2) != Sign(tempBA2))
	{
		A2In = sfTrue;
		(*Bprevious)->CP.isIntersect = sfTrue;
		sfCircleShape_setFillColor((*Bprevious)->CP.CircleA, sfRed);
		newCP[0]->CP = (*Bprevious)->CP;
		newCP[0]->Next = (*Aprevious)->Next;
		(*Aprevious)->Next = newCP[0];
	}

	float tempAB2 = isY ? (float)((*Bprevious)->Next->CP.pos.y - (*Aprevious)->CP.pos.y) : (float)((*Bprevious)->Next->CP.pos.x - (*Aprevious)->CP.pos.x);
	float tempBB2 = isY ? (float)((*Bprevious)->Next->CP.pos.y - (*Aprevious)->Next->CP.pos.y) : (float)((*Bprevious)->Next->CP.pos.x - (*Aprevious)->Next->CP.pos.x);

	//If Sign of Vector are opposite the point is between the two points making the main vector that represent the actual segment
	//Mainly determine if the corridor is going from Aprevious or B Previous or the opposite in order to properly add thoses two Checkpoint to the current chained list
	sfBool B2In = sfFalse;
	if (Sign(tempAB2) != Sign(tempBB2))
	{
		B2In = sfTrue;
		(*Bprevious)->Next->CP.isIntersect = sfTrue;
		sfCircleShape_setFillColor((*Bprevious)->Next->CP.CircleA, sfRed);
		newCP[A2In]->CP = (*Bprevious)->Next->CP;
		newCP[A2In]->Next = (*Aprevious)->Next;
		(*Aprevious)->Next = newCP[A2In];
	}

	//If Both aren't in the two previous segment we take the last two remaining else if only one has been selected we check both lasting 
	if (!A2In && !B2In)
	{
		(*Aprevious)->CP.isIntersect = sfTrue;
		sfCircleShape_setFillColor((*Aprevious)->CP.CircleA, sfRed);
		newCP[0]->CP = (*Aprevious)->CP;
		newCP[0]->Next = (*Bprevious)->Next;
		(*Bprevious)->Next = newCP[0];

		(*Aprevious)->Next->CP.isIntersect = sfTrue;
		sfCircleShape_setFillColor((*Aprevious)->Next->CP.CircleA, sfRed);
		newCP[1]->CP = (*Aprevious)->Next->CP;
		newCP[1]->Next = (*Bprevious)->Next;
		(*Bprevious)->Next = newCP[1];
	}
	else if (!A2In || !B2In)
	{
		float tempA2A = isY ? (float)((*Aprevious)->CP.pos.y - (*Bprevious)->CP.pos.y) : (float)((*Aprevious)->CP.pos.x - (*Bprevious)->CP.pos.x);
		float tempB2A = isY ? (float)((*Aprevious)->CP.pos.y - (*Bprevious)->Next->CP.pos.y) : (float)((*Aprevious)->CP.pos.x - (*Bprevious)->Next->CP.pos.x);
		if (Sign(tempA2A) != Sign(tempB2A))
		{
			//A is in
			(*Aprevious)->CP.isIntersect = sfTrue;
			sfCircleShape_setFillColor((*Aprevious)->CP.CircleA, sfRed);
			newCP[1]->CP = (*Aprevious)->CP;
			newCP[1]->Next = (*Bprevious)->Next;
			(*Bprevious)->Next = newCP[1];
		}
		else
		{
			//B is in
			(*Aprevious)->Next->CP.isIntersect = sfTrue;
			sfCircleShape_setFillColor((*Aprevious)->Next->CP.CircleA, sfRed);
			newCP[1]->CP = (*Aprevious)->Next->CP;
			newCP[1]->Next = (*Bprevious)->Next;
			(*Bprevious)->Next = newCP[1];
		}
	}
}

//Align Door if they are very close to each other in order to avoid small corner in the corridor
sfBool RepositionateDoor(Door* doorA, Door* doorB)
{
	float tempX = fabsf(doorA->Position.x - doorB->Position.x);
	float tempY = fabsf(doorA->Position.y - doorB->Position.y);

	if (doorA->cardDir == N || doorA->cardDir == S)
	{
		if (tempX < SIZE_VERTICAL_X_x64 * 2.0f)
		{
			return sfTrue;
			if (tempX < SIZE_VERTICAL_X_x64)
			{
				if (doorA->Position.x < doorB->Position.x)
				{
					doorA->Position.x += (tempX / 2.0f);
					doorB->Position.x -= (tempX / 2.0f);
				}
				else if (doorA->Position.x > doorB->Position.x)
				{
					doorA->Position.x -= (tempX / 2.0f);
					doorB->Position.x += (tempX / 2.0f);
				}
				else
				{
					return sfFalse;
				}
			}
			else
			{
				if (doorA->Position.x < doorB->Position.x)
				{
					doorA->Position.x -= (tempX / 2.0f);
					doorB->Position.x += (tempX / 2.0f);
				}
				else if (doorA->Position.x > doorB->Position.x)
				{
					doorA->Position.x += (tempX / 2.0f);
					doorB->Position.x -= (tempX / 2.0f);
				}
				else
				{
					return sfFalse;
				}
			}

			sfCircleShape_setPosition(doorA->Circle, doorA->Position);
			sfCircleShape_setPosition(doorB->Circle, doorB->Position);
		}

		return sfFalse;
	}
	else
	{
		if (tempY < SIZE_HORIZONTAL_Y_x64 * 2.0f)
		{
			return sfTrue;
			if (tempY < SIZE_HORIZONTAL_Y_x64)
			{
				if (doorA->Position.y < doorB->Position.y)
				{
					doorA->Position.y += (tempY / 2.0f);
					doorB->Position.y -= (tempY / 2.0f);
				}
				else if (doorA->Position.y > doorB->Position.y)
				{
					doorA->Position.y -= (tempY / 2.0f);
					doorB->Position.y += (tempY / 2.0f);
				}
				else
				{
					return sfFalse;
				}
			}
			else
			{
				if (doorA->Position.y < doorB->Position.y)
				{
					doorA->Position.y -= (tempY / 2.0f);
					doorB->Position.y += (tempY / 2.0f);
				}
				else if (doorA->Position.y > doorB->Position.y)
				{
					doorA->Position.y += (tempY / 2.0f);
					doorB->Position.y -= (tempY / 2.0f);
				}
				else
				{
					return sfFalse;
				}
			}

			sfCircleShape_setPosition(doorA->Circle, doorA->Position);
			sfCircleShape_setPosition(doorB->Circle, doorB->Position);

		}

		return sfFalse;
	}
}

//Check is a point is Overlapping over a room
sfBool CheckPointIsOverlapping(CorridorCheckPointsList** CPtoCheck, const Room rm)
{
	if ((*CPtoCheck)->Next == NULL)
	{
		return sfFalse;
	}

	float left = roundf(rm.Position.x - (rm.Width / 2.0f));
	float top = roundf(rm.Position.y - (rm.Height / 2.0f));

	sfVector2f cpPos = { (float)(*CPtoCheck)->CP.pos.x , (float)(*CPtoCheck)->CP.pos.y };
	if (cpPos.x > left - 3.f && cpPos.x < (left + rm.Width) + 3.f && cpPos.y > top - 3.0f && cpPos.y < (top + rm.Height) + 3.0f)
	{
		sfCircleShape_setFillColor((*CPtoCheck)->CP.CircleA, sfYellow);
		return sfTrue;
	}
	return sfFalse;
}

//Reposition a CheckPoint in case he's overlapping a room
void RepositionateCheckPoint(CorridorCheckPointsList** CPPrevious, CorridorCheckPointsList** CPActual, Room rm, CorridorCheckPointsList *FirstCP)
{
	float left = roundf(rm.Position.x - (rm.Width / 2.0f));
	float top = roundf(rm.Position.y - (rm.Height / 2.0f));
	float distanceX;
	float distanceY;
	sfVector2f temp;

	if ((*CPActual)->CP.pos.x < rm.Position.x)
	{
		distanceX = left - (*CPActual)->CP.pos.x - 4.0f;
	}
	else
	{
		distanceX = (left + rm.Width) - (*CPActual)->CP.pos.x + 4.0f;
	}

	if ((*CPActual)->CP.pos.y < rm.Position.y)
	{
		distanceY = top - (*CPActual)->CP.pos.y - 4.0f;
	}
	else
	{
		distanceY = (top + rm.Height) - (*CPActual)->CP.pos.y + 4.0f;
	}

	sfBool isX;

	if ((*CPPrevious)->CP.pos.x == FirstCP->CP.pos.x && (*CPPrevious)->CP.pos.y == FirstCP->CP.pos.y)
	{
		isX = ((*CPActual)->CP.pos.y != (*CPActual)->Next->CP.pos.y);
	}
	else if ((*CPActual)->Next->Next == NULL)
	{
		isX = ((*CPActual)->CP.pos.y == (*CPActual)->Next->CP.pos.y);
	}
	else
	{
		isX = (fabsf(distanceX) <= fabsf(distanceY));
	}

	if (isX)
	{
		(*CPActual)->CP.pos.x += (int)distanceX;
		if ((*CPActual)->CP.pos.y != (*CPActual)->Next->CP.pos.y)
		{
			(*CPActual)->Next->CP.pos.x = (*CPActual)->CP.pos.x;
			temp.x = (float)(*CPActual)->Next->CP.pos.x;
			temp.y = (float)(*CPActual)->Next->CP.pos.y;
			sfCircleShape_setPosition((*CPActual)->Next->CP.CircleA, temp);
		}
		else
		{
			(*CPPrevious)->CP.pos.x = (*CPActual)->CP.pos.x;
			temp.x = (float)(*CPPrevious)->CP.pos.x;
			temp.y = (float)(*CPPrevious)->CP.pos.y;
			sfCircleShape_setPosition((*CPPrevious)->CP.CircleA, temp);
		}
	}
	else
	{
		(*CPActual)->CP.pos.y += (int)distanceY;
		if ((*CPActual)->CP.pos.x != (*CPActual)->Next->CP.pos.x)
		{
			(*CPActual)->Next->CP.pos.y = (*CPActual)->CP.pos.y;
			temp.x = (float)(*CPActual)->Next->CP.pos.x;
			temp.y = (float)(*CPActual)->Next->CP.pos.y;
			sfCircleShape_setPosition((*CPActual)->Next->CP.CircleA, temp);
		}
		else
		{
			(*CPPrevious)->CP.pos.y = (*CPActual)->CP.pos.y;
			temp.x = (float)(*CPPrevious)->CP.pos.x;
			temp.y = (float)(*CPPrevious)->CP.pos.y;
			sfCircleShape_setPosition((*CPPrevious)->CP.CircleA, temp);
		}
	}

	sfVector2f tempI = { (float)(*CPActual)->CP.pos.x , (float)(*CPActual)->CP.pos.y };
	sfCircleShape_setPosition((*CPActual)->CP.CircleA, tempI);
}

//Add a checkpoints when there is an OverLap
void AddCheckpointsForOverlap(CorridorCheckPointsList** CPPrevious, CorridorCheckPointsList** CPActual, Room rm, CorridorCheckPointsList *FirstCP)
{
	CorridorCheckPointsList *newCP[2] =
	{
		(CorridorCheckPointsList*)malloc(sizeof(CorridorCheckPointsList)),
		(CorridorCheckPointsList*)malloc(sizeof(CorridorCheckPointsList))
	};

	sfVector2f MoveOut;

	newCP[0]->CP = newCP[1]->CP = (*CPActual)->CP;
	for (int i = 0; i < 2; i++)
	{
		sfVector2f temp = { (float)newCP[i]->CP.pos.x, (float)newCP[i]->CP.pos.y };
		newCP[i]->CP.CircleA = sfCircleShape_create();
		sfCircleShape_setRadius(newCP[i]->CP.CircleA, 2.f);
		sfCircleShape_setFillColor(newCP[i]->CP.CircleA, sfMagenta);
		sfCircleShape_setPosition(newCP[i]->CP.CircleA, temp);
		sfCircleShape_setOrigin(newCP[i]->CP.CircleA, (sfVector2f) { 2.f, 2.f });
	}
	if ((*CPActual)->Next->Next == NULL)
	{
		newCP[1]->Next = (*CPActual);
		newCP[0]->Next = newCP[1];
		(*CPPrevious)->Next = newCP[0];

		float x = (float)((*CPPrevious)->CP.pos.x - (*CPActual)->CP.pos.x);
		float y = (float)((*CPPrevious)->CP.pos.y - (*CPActual)->CP.pos.y);

		MoveOut.x = x == 0 ? 0 : x / fabsf(x) * ((rm.Width / 2.0f) - fabsf((*CPActual)->CP.pos.x - rm.Position.x) + 6.0f);
		MoveOut.y = y == 0 ? 0 : y / fabsf(y) * ((rm.Height / 2.0f) - fabsf((*CPActual)->CP.pos.y - rm.Position.y) + 6.0f);
	}
	else
	{
		newCP[1]->Next = (*CPActual)->Next;
		newCP[0]->Next = newCP[1];
		(*CPActual)->Next = newCP[0];

		float x = (float)(newCP[1]->Next->CP.pos.x - (*CPActual)->CP.pos.x);
		float y = (float)(newCP[1]->Next->CP.pos.y - (*CPActual)->CP.pos.y);

		MoveOut.x = x == 0 ? 0 : x / fabsf(x) * ((rm.Width / 2.0f) - fabsf((*CPActual)->CP.pos.x - rm.Position.x) + 6.0f);
		MoveOut.y = y == 0 ? 0 : y / fabsf(y) * ((rm.Height / 2.0f) - fabsf((*CPActual)->CP.pos.y - rm.Position.y) + 6.0f);
	}

	newCP[0]->CP.pos.x = newCP[1]->CP.pos.x = (MoveOut.x + (*CPActual)->CP.pos.x);
	newCP[0]->CP.pos.y = newCP[1]->CP.pos.y = (MoveOut.y + (*CPActual)->CP.pos.y);
	for (int i = 0; i < 2; i++)
	{
		sfVector2f temp = { (float)newCP[i]->CP.pos.x, (float)newCP[i]->CP.pos.y };
		sfCircleShape_setPosition(newCP[i]->CP.CircleA, temp);
	}
}

//Last Check the generation 
sfBool CheckValidityOfGeneration(RoomList *pFirstRoom, EdgeList *pFirstEdge)
{
	for (RoomList * i = pFirstRoom; i != NULL; i = i->Next)
	{
		if (i->rm.nbDoors == 0)
		{
			return sfFalse;
		}

	}

	for (EdgeList * i = pFirstEdge; i != NULL; i = i->Next)
	{
		for (CorridorCheckPointsList *j = i->door[0]->Corridor->firstCheckPoint->Next; j != NULL; j = j->Next)
		{
			for (RoomList * k = pFirstRoom; k != NULL; k = k->Next)
			{
				if (CheckPointIsOverlapping(&j, k->rm))
				{
					return sfFalse;
				}
			}

		}
	}

	return sfTrue;
}

//Connect Room to the rest of the dungeon if they are not well connected
void ManualConnectRoom(DungeonRawData *Data, RoomList **i)
{
	float minDistance = 5000.0f;
	for (RoomList * j = Data->FirstRoom; j != NULL; j = j->Next)
	{
		if ((*i)->rm.Index != j->rm.Index)
		{
			float temp = Distance(j->rm.Position, (*i)->rm.Position);
			if (temp < minDistance)
			{
				if ((*i)->rm.nbDoors == 0)
				{
					minDistance = temp;
				}
				else
				{
					sfBool isValidToConnect = sfTrue;

					for (EdgeList *e = Data->FirstEdge; e != NULL; e = e->Next)
					{
						if (e->Rooms[0]->Index == (*i)->rm.Index && e->Rooms[1]->Index == j->rm.Index || e->Rooms[1]->Index == (*i)->rm.Index && e->Rooms[0]->Index == j->rm.Index)
						{
							isValidToConnect = sfFalse;
							break;
						}
					}

					if (isValidToConnect)
					{
						minDistance = temp;
					}
				}
			}
		}
	}

	for (RoomList * j = Data->FirstRoom; j != NULL; j = j->Next)
	{
		if ((*i)->rm.Index != j->rm.Index)
		{
			if (Distance(j->rm.Position, (*i)->rm.Position) == minDistance)
			{
				AddEdge(&Data->FirstEdge, -1, &j->rm, &(*i)->rm);
				(Data->FirstEdge)->Tail->Rooms[0]->nbDoors++;
				(Data->FirstEdge)->Tail->Rooms[1]->nbDoors++;
				if (j->rm.nbDoors == 1)
				{
					ManualConnectRoom(Data, &j);
				}
				break;
			}
		}
		Data->iterationsCount++;
	}

}

//Nagivate through Dungeon To Check that no rooms are isolated //ID of room that has been already checked in this instance
void NavigateDungeon(Room* room, int* count, int IDChecked[], const int sizeOfArray)
{
	for (int i = 0; i < sizeOfArray; i++)
	{
		if (IDChecked[i] == room->Index)
		{
			return;
		}
		else if (IDChecked[i] == -1)
		{
			IDChecked[i] = room->Index;
			(*count)++;
			i = sizeOfArray;
		}
	}

	for (int i = 0; i < room->nbDoors; i++)
	{
		int idOtherDoor = room->doors[i].Corridor->doorsBack[0]->Index == room->doors[i].Index ? 1 : 0;
		NavigateDungeon(room->doors[i].Corridor->doorsBack[idOtherDoor]->roomBack, count, IDChecked, sizeOfArray);
	}
}

void GenerateIdOfRoom(DungeonRawData* dungeon)
{
	RoomList* furthestRoom = dungeon->FirstRoom;
	RoomList* tempRoom = dungeon->FirstRoom->Next;
	RoomList* bossRoom = NULL;
	int tempIndex = 0;
	float distanceFurthestRoom = 0;
	for (int i = 0; i < dungeon->numberOfRoom - 1; i++)
	{
		float distanceRoom = Distance(dungeon->FirstRoom->rm.Position, tempRoom->rm.Position);
		if (furthestRoom != tempRoom && tempRoom->rm.nbDoors == 1 && distanceRoom > distanceFurthestRoom)
		{
			furthestRoom = tempRoom;
			distanceFurthestRoom = distanceRoom;
		}
		if (tempRoom->rm.Index == dungeon->numberOfRoom - 1)
		{
			bossRoom = tempRoom;
		}
		tempRoom = tempRoom->Next;
	}
	if (bossRoom != furthestRoom && furthestRoom != dungeon->FirstRoom)
	{
		int indexTemp = furthestRoom->rm.Index;
		int graphicIndexTemp = furthestRoom->rm.GraphicsIndex;
		int widthTemp = furthestRoom->rm.Width;
		int heightTemp = furthestRoom->rm.Height;

		furthestRoom->rm.Index = bossRoom->rm.Index;
		furthestRoom->rm.GraphicsIndex = bossRoom->rm.GraphicsIndex;
		furthestRoom->rm.Width = bossRoom->rm.Width;
		furthestRoom->rm.Height = bossRoom->rm.Height;

		bossRoom->rm.Index = indexTemp;
		bossRoom->rm.GraphicsIndex = graphicIndexTemp;
		bossRoom->rm.Width = widthTemp;
		bossRoom->rm.Height = heightTemp;
	}
}

void RoomsGeneration(DungeonRawData* DungeonData, const int pNumberOfRoom, const sfVector2f pCenter, const float pArea, const float pMargin, const float pmaxWidth)
{
	int count = 0;
	short groupIndex = 0;
	int margin = (int)pMargin;
	int maxWidth = (int)pmaxWidth;

	RoomList* currentRoom = NULL;
	RoomList* tmpRoom = NULL;

	if (pNumberOfRoom == 1)
	{
		AddRoom(&DungeonData->FirstRoom, pCenter, pArea);
		DungeonData->iterationsCount++;
		return;
	}

	while (count < pNumberOfRoom - 1)
	{
		ClearEdges(&DungeonData->FirstEdge);
		ClearRooms(&DungeonData->FirstRoom);

		EdgeList*  newEdgeList = NULL;
		sfBool b = sfTrue;
		sfBool b1 = sfFalse;
		sfBool b2 = sfFalse;

		for (int i = 0; i < pNumberOfRoom; i++)
		{
			AddRoom(&DungeonData->FirstRoom, pCenter, pArea);
			DungeonData->iterationsCount++;
		}
		////////////////////////////////////room creation/////////////////////////////////////////////
		while (b)
		{
			b = sfFalse;

			currentRoom = DungeonData->FirstRoom;

			while (currentRoom != NULL)
			{

				tmpRoom = DungeonData->FirstRoom;

				while (tmpRoom != NULL)
				{
					if (tmpRoom->rm.Index != currentRoom->rm.Index)
					{
						sfVector2f newVector = AddTwoVectors(currentRoom->rm.Position, tmpRoom->rm.Position);

						if (fabsf(newVector.x) <= (tmpRoom->rm.Width + currentRoom->rm.Width + margin) / 2.0f
							&& fabsf(newVector.y) <= (tmpRoom->rm.Height + currentRoom->rm.Height + margin) / 2.0f)
						{
							newVector = Normalize(newVector);
							currentRoom->rm.Position.x -= newVector.x;
							currentRoom->rm.Position.y -= newVector.y;
							tmpRoom->rm.Position.x += newVector.x;
							tmpRoom->rm.Position.y += newVector.y;

							b = sfTrue;
						}
					}
					tmpRoom = tmpRoom->Next;
					DungeonData->iterationsCount++;
				}
				currentRoom = currentRoom->Next;
			}
		}
		for (RoomList * i = DungeonData->FirstRoom; i != NULL; i = i->Next)
		{
			i->rm.Position.x = floorf(i->rm.Position.x);
			i->rm.Position.y = floorf(i->rm.Position.y);
			DungeonData->iterationsCount++;
		}
		/////////////////////////triangulation////////////////////////////////////////
		currentRoom = DungeonData->FirstRoom;

		while (currentRoom != NULL)
		{

			tmpRoom = DungeonData->FirstRoom;

			while (tmpRoom != NULL)
			{
				if (tmpRoom->rm.Index != currentRoom->rm.Index)
				{
					if (Distance(tmpRoom->rm.Position, currentRoom->rm.Position) < margin + maxWidth
						&& !IsAlreadyExist(&DungeonData->FirstEdge, &tmpRoom->rm, &currentRoom->rm))
					{
						AddEdge(&DungeonData->FirstEdge, -1, &tmpRoom->rm, &currentRoom->rm);
					}
				}
				tmpRoom = tmpRoom->Next;
				DungeonData->iterationsCount++;
			}
			currentRoom = currentRoom->Next;
		}
		/////////////////////////////sort///////////////////////////////////////
		SortEdge(&DungeonData->FirstEdge);
		//////////////////////////////minimal spanning tree/////////////////////////////////////
		count = 0;
		groupIndex = 0;

		for (EdgeList * i = DungeonData->FirstEdge; i->Next != NULL; i = i->Next)
		{
			if (i->Rooms[0]->nbDoors <= 2 && i->Rooms[1]->nbDoors <= 2)
			{
				short index1 = IsRoomAlreadyExist(&newEdgeList, i->Rooms[0]);
				short index2 = IsRoomAlreadyExist(&newEdgeList, i->Rooms[1]);

				if (index1 == -1 && index2 == -1)
				{
					//printf("only index edge %d index1 %d index2 %d\n", i->Index, index1, index2);
					i->Rooms[0]->nbDoors++;
					i->Rooms[1]->nbDoors++;
					AddEdge(&newEdgeList, groupIndex, i->Rooms[0], i->Rooms[1]);
					groupIndex++;
					count++;
				}
				else if (index1 != index2)
				{
					//printf("index edge %d index1 %d index2 %d\n", i->Index, index1, index2);
					int index = -1;

					if (index1 != -1 && index2 != -1)
					{
						if (index1 < index2)
						{
							index = index1;
						}
						else
						{
							index = index2;
						}
					}
					else if (index1 != -1)
					{
						index = index1;
					}
					else
					{
						index = index2;
					}
					count++;

					//printf("new index %d\n", index);
					i->Rooms[0]->nbDoors++;
					i->Rooms[1]->nbDoors++;
					AddEdge(&newEdgeList, index, i->Rooms[0], i->Rooms[1]);
				}
			}
			DungeonData->iterationsCount++;
		}

		ClearEdges(&DungeonData->FirstEdge);
		DungeonData->FirstEdge = newEdgeList;
	}

	//////Check if every room has at least one connection////////////////////////////////////////
	for (RoomList * i = DungeonData->FirstRoom; i != NULL; i = i->Next)
	{
		if (i->rm.nbDoors == 0)
		{
			ManualConnectRoom(DungeonData, &i);
		}
		else if (i->rm.nbDoors == 1)
		{
			sfBool Done = sfFalse;
			EdgeList *e = DungeonData->FirstEdge;
			while (!Done && e != NULL)
			{
				for (int n = 0; n < 2; n++)
				{
					if (e->Rooms[n]->Index == i->rm.Index)
					{
						if (e->Rooms[(n + 1) % 2]->nbDoors == 1)
						{
							Room* temp = e->Rooms[(n + 1) % 2];
							for (RoomList * search = DungeonData->FirstRoom; search != NULL; search = search->Next)
							{
								if (search->rm.Index == temp->Index)
								{
									ManualConnectRoom(DungeonData, &search);
									n = 2;
									Done = sfTrue;
									break;
								}
							}
						}
					}
				}
				if (!Done)
				{
					e = e->Next;
				}
			}
		}
	}

	free(currentRoom);
	free(tmpRoom);
	DungeonData->edgesCount = count;

	GenerateIdOfRoom(DungeonData);
}

void DoorsGeneration(DungeonRawData* DungeonData, const sfBool hasCorridor, const sfBool hasContrains, RoomPrefab* arrayOfPrebab, const DungeonId ID)
{
	if (DungeonData->FirstEdge == NULL)
	{
		return;
	}

	//////////////////////////////////////////////// Door Making & Placement /////////////////////////////////////////////////
	for (EdgeList * i = DungeonData->FirstEdge; i != NULL; i = i->Next)
	{
		sfVector2f posFinal;
		int saveFirstIDFirstDraw = -1;
		CardinalDirection  saveCardDirFirstDraw = -1;

		for (int j = 0; j < 2; j++)
		{
			int r = (j + 1) % 2;
			sfVector2f temp = { i->Rooms[r]->Position.x - i->Rooms[j]->Position.x, i->Rooms[r]->Position.y - i->Rooms[j]->Position.y };

			int choice = -1;

			sfBool done = sfFalse;
			sfVector2f tempNorm = Normalize(temp);
			sfVector2f goThrough = { i->Rooms[j]->Position.x, i->Rooms[j]->Position.y };
			sfVector2f roomBorder = { i->Rooms[j]->Position.x + i->Rooms[j]->Width / 2.0f, i->Rooms[j]->Position.y + i->Rooms[j]->Height / 2.0f };
			while (!done)
			{
				goThrough.x += fabsf(tempNorm.x);
				goThrough.y += fabsf(tempNorm.y);

				if (goThrough.x >= roomBorder.x)
				{
					choice = 1;
					done = sfTrue;
				}
				else if (goThrough.y >= roomBorder.y)
				{
					choice = 0;
					done = sfTrue;
				}
			}

			if (choice == 0)
			{
				if (!hasContrains || !arrayOfPrebab[i->Rooms[j]->GraphicsIndex].hasContrain)
				{
					posFinal.x = i->Rooms[j]->Position.x - (i->Rooms[j]->Width / 2.0f - (SIZE_VERTICAL_X_x64 / 2.0f)) + (rand() % (int)(i->Rooms[j]->Width - SIZE_VERTICAL_X_x64 + 1.0f));
					posFinal.y = i->Rooms[j]->Position.y + ((temp.y > 0 ? 1 : -1) * (i->Rooms[j]->Height / 2.0f));
				}
				else
				{
					CardinalDirection newDoorDir = (temp.y > 0 ? S : N);

					int ChoosenIndex;
					do
					{
						ChoosenIndex = rand() % arrayOfPrebab[i->Rooms[j]->GraphicsIndex].ListLengths[newDoorDir];
					} while (saveFirstIDFirstDraw == ChoosenIndex && saveCardDirFirstDraw == newDoorDir);

					saveFirstIDFirstDraw = ChoosenIndex;
					saveCardDirFirstDraw = newDoorDir;

					int incr = 0;
					sf_Vector2fList *search = arrayOfPrebab[i->Rooms[j]->GraphicsIndex].AvailablePos[newDoorDir];
					while (true)
					{
						if (incr == ChoosenIndex)
						{
							posFinal.x = i->Rooms[j]->Position.x - ((float)i->Rooms[j]->Width / 2.0f) + search->Vector.x;
							posFinal.y = i->Rooms[j]->Position.y - ((float)i->Rooms[j]->Height / 2.0f) + search->Vector.y;
							break;
						}
						else
						{
							search = search->Next;
							incr++;
						}
					}

				}
			}
			else if (choice == 1)
			{
				if (!hasContrains || !arrayOfPrebab[i->Rooms[j]->GraphicsIndex].hasContrain)
				{
					posFinal.x = i->Rooms[j]->Position.x + ((temp.x > 0 ? 1 : -1) * (i->Rooms[j]->Width / 2.0f));
					posFinal.y = i->Rooms[j]->Position.y - ((i->Rooms[j]->Height / 2.0f) - (SIZE_HORIZONTAL_Y_x64 / 2.0f)) + (rand() % (int)(i->Rooms[j]->Height - SIZE_HORIZONTAL_Y_x64 + 1.0f));

				}
				else
				{
					CardinalDirection newDoorDir = (temp.x > 0 ? E : W);

					int ChoosenIndex;
					do
					{
						ChoosenIndex = rand() % arrayOfPrebab[i->Rooms[j]->GraphicsIndex].ListLengths[newDoorDir];
					} while (saveFirstIDFirstDraw == ChoosenIndex && saveCardDirFirstDraw == newDoorDir);

					saveFirstIDFirstDraw = ChoosenIndex;
					saveCardDirFirstDraw = newDoorDir;

					int incr = 0;
					sf_Vector2fList *search = arrayOfPrebab[i->Rooms[j]->GraphicsIndex].AvailablePos[newDoorDir];
					while (true)
					{
						if (incr == ChoosenIndex)
						{
							posFinal.x = i->Rooms[j]->Position.x - ((float)i->Rooms[j]->Width / 2.0f) + search->Vector.x;
							posFinal.y = i->Rooms[j]->Position.y - ((float)i->Rooms[j]->Height / 2.0f) + search->Vector.y;
							//printf("VectorY Choosen : %f\n", search->Vector.y);
							break;
						}
						else
						{
							search = search->Next;
							incr++;
						}
					}
				}
			}

			int tempInt = -1;
			//Find empty door in room to asign it there
			for (int k = 0; k < i->Rooms[j]->nbDoors; k++)
			{
				if (i->Rooms[j]->doors[k].Circle == NULL)
				{
					int dir = -1;
					if (choice == 0)
					{
						dir = (temp.y > 0 ? S : N);
					}
					else
					{
						dir = (temp.x > 0 ? E : W);
					}
					InitalizeDoor(&i->Rooms[j]->doors[k], posFinal, dir);
					i->Rooms[j]->doors[k].roomBack = i->Rooms[j];
					tempInt = k;
					k = i->Rooms[j]->nbDoors;
				}
			}

			if (tempInt != -1)
			{
				i->door[j] = &i->Rooms[j]->doors[tempInt];
			}

			DungeonData->iterationsCount++;
		}
	}

	if (hasCorridor)
	{
		for (EdgeList * i = DungeonData->FirstEdge; i != NULL; i = i->Next)
		{
			for (EdgeList * j = DungeonData->FirstEdge; j != NULL; j = j->Next)
			{
				if (i->Index != j->Index)
				{
					if (IsIntersect(i->door[0]->Position, i->door[1]->Position, j->door[0]->Position, j->door[1]->Position, NULL))
					{
						for (int k = 0; k < 2; k++)
						{
							if (i->door[0]->roomBack->Index == j->door[k]->roomBack->Index)
							{
								if (i->door[0]->cardDir == j->door[k]->cardDir)
								{
									SwapDoorPos(i->door[0], j->door[k]);
								}
								k = 2;
							}
							else if (i->door[1]->roomBack->Index == j->door[k]->roomBack->Index)
							{
								if (i->door[1]->cardDir == j->door[k]->cardDir)
								{
									SwapDoorPos(i->door[1], j->door[k]);
								}
								k = 2;
							}

						}
					}
					DungeonData->iterationsCount++;
				}
			}
		}
	}


	//May cause issue
	/*for (RoomList * i = DungeonData->FirstRoom; i != NULL; i = i->Next)
	{
		for (int j = 0; j < (i->rm.nbDoors); j++)
		{
			for (int k = 0; k < (i->rm.nbDoors); k++)
			{
				if (i->rm.doors[j].cardDir == i->rm.doors[k].cardDir && i->rm.doors[j].Index != i->rm.doors[k].Index)
				{
					CheckDoorDist(&i->rm.doors[j], &i->rm.doors[k]);
				}
				DungeonData->iterationsCount++;
			}
		}
	}*/

	//ReCheck after Aligned
	for (EdgeList * i = DungeonData->FirstEdge; i != NULL; i = i->Next)
	{
		if (hasCorridor)
		{
			for (EdgeList * j = DungeonData->FirstEdge; j != NULL; j = j->Next)
			{
				if (i->Index != j->Index)
				{
					if (IsIntersect(i->door[0]->Position, i->door[1]->Position, j->door[0]->Position, j->door[1]->Position, NULL))
					{
						for (int k = 0; k < 2; k++)
						{
							if (i->door[0]->roomBack->Index == j->door[k]->roomBack->Index)
							{
								if (i->door[0]->cardDir == j->door[k]->cardDir)
								{
									SwapDoorPos(i->door[0], j->door[k]);
								}
								k = 2;
							}
							else if (i->door[1]->roomBack->Index == j->door[k]->roomBack->Index)
							{
								if (i->door[1]->cardDir == j->door[k]->cardDir)
								{
									SwapDoorPos(i->door[1], j->door[k]);
								}
								k = 2;
							}

						}
					}
				}
				DungeonData->iterationsCount++;
			}
		}

		if (ID == elemDungeon)
		{
			i->door[0]->PortalRef = i->door[1];
			i->door[1]->PortalRef = i->door[0];
		}
	}

	//Put Doors in .5f coordonates

	if (GetTypeOfDungeon() == dungeonCorridor)
	{
		for (RoomList * i = DungeonData->FirstRoom; i != NULL; i = i->Next)
		{
			for (int j = 0; j < (i->rm.nbDoors); j++)
			{
				if (i->rm.doors[j].cardDir == N || i->rm.doors[j].cardDir == S)
				{
					i->rm.doors[j].Position.x = floorf(i->rm.doors[j].Position.x) + 0.5f;

					i->rm.doors[j].Position.y = floorf(i->rm.doors[j].Position.y) + (i->rm.doors[j].cardDir == N ? 0.5f : -0.5f);
				}
				else
				{
					i->rm.doors[j].Position.x = floorf(i->rm.doors[j].Position.x) + (i->rm.doors[j].cardDir == W ? 0.5f : -0.5f);

					i->rm.doors[j].Position.y = floorf(i->rm.doors[j].Position.y) + 0.5f;
				}


				if (i->rm.doors[j].cardDir == S)
				{
					i->rm.doors[j].Position.y += 1.0f;
				}
				else if (i->rm.doors[j].cardDir == E)
				{
					i->rm.doors[j].Position.x += 1.0f;
				}

				DungeonData->iterationsCount++;
			}
		}
	}
}

HANDLE MutexBool;

DWORD WINAPI GenerationDungeonSoloThread(void *data)
{
	DungeonRawData dungeonRD;
	dungeonRD.seed = 0;
	dungeonRD.FirstEdge = NULL;
	dungeonRD.FirstRoom = NULL;

	int pNumberOfRoom = *(int *)((void**)data)[1];
	sfVector2f pCenter = *(sfVector2f *)(((void**)data)[2]);
	float pArea = *(float*)(((void**)data)[3]);
	float pMargin = *(float*)(((void**)data)[4]);
	float pmaxWidth = *(float*)(((void**)data)[5]);
	RoomPrefab *arrayOfPrebab = (RoomPrefab*)(((void**)data)[6]);
	bool *IsSeekingDone = (bool*)(((void**)data)[7]);
	sfBool(*GenerationPtr)(DungeonRawData*, int, sfVector2f, float, float, float, RoomPrefab*, DWORD, long int) = (((void**)data)[8]);

	DWORD threadID = GetCurrentThreadId();
	printf("ThreadID : %d\n", threadID);


	int countOfGenerationTry = 0;
	do
	{
		if (*IsSeekingDone)
		{
			break;
		}
		Sleep(1);
		countOfGenerationTry++;
	} while (!GenerationPtr(&dungeonRD, pNumberOfRoom, pCenter, pArea, pMargin, pmaxWidth, arrayOfPrebab, threadID, 0));


	if (!(*IsSeekingDone))
	{
		//Mutex Processus
		WaitForSingleObject(MutexBool, INFINITE);

		*IsSeekingDone = true;

#if _DEBUG
		printf("Generation Try (by the Thread that Succeeded) : %d\n", countOfGenerationTry);
		printf("seeds : %u\n", dungeonRD.seed);
		printf("total_Iteration : %u\n", dungeonRD.iterationsCount);
#endif
		memcpy(((void**)data)[0], (void*)&dungeonRD, sizeof(DungeonRawData));
		ReleaseMutex(MutexBool);
	}
	else
	{
#if _DEBUG
		printf("Generation Try (by other Thread ) : %d\n", countOfGenerationTry);
#endif
		ClearEdges(&dungeonRD.FirstEdge);
		ClearRooms(&dungeonRD.FirstRoom);
	}
	return 0;
}

//Proceed Dungeon Generation
sfBool GenerateDungeonRD(DungeonRawData* DungeonData, const int pNumberOfRoom, const sfVector2f pCenter, const float pArea, const float pMargin, const float pmaxWidth, RoomPrefab *arrayOfPrebab, const DWORD ThreadID, const long int previousSeed)
{
	unsigned int temp = (unsigned int)time(NULL);

	if (previousSeed != 0)
	{
		DungeonData->seed = previousSeed;
	}
	else if (ThreadID == 0)
	{
		DungeonData->seed = temp * rand();
	}
	else
	{
		DungeonData->seed = temp * rand() * ThreadID;
	}
	//DungeonData->seed = 1012501756;
	srand(DungeonData->seed);

	DungeonData->edgesCount = 0;
	DungeonData->iterationsCount = 0;
	DungeonData->numberOfRoom = pNumberOfRoom;

	RoomsGeneration(DungeonData, pNumberOfRoom, pCenter, pArea, pMargin, pmaxWidth);

	/////////////////////////////////////////////////// Edge Intersection Check ///////////////////////////////
	for (EdgeList * i = DungeonData->FirstEdge; i != NULL; i = i->Next)
	{
		for (EdgeList * j = DungeonData->FirstEdge; j != NULL; j = j->Next)
		{
			if (i->Index != j->Index)
			{
				if (IsIntersect(i->Rooms[0]->Position, i->Rooms[1]->Position, j->Rooms[0]->Position, j->Rooms[1]->Position, NULL))
				{
					printf("///////////// Intersect detected, generation restarted \\\\\\\\\\\n");
					DungeonData->iterationsCount = 0;
					ClearEdges(&DungeonData->FirstEdge);
					ClearRooms(&DungeonData->FirstRoom);
					return sfFalse;
				}
			}
			DungeonData->iterationsCount++;
		}
	}

	for (EdgeList * i = DungeonData->FirstEdge; i != NULL; i = i->Next)
	{
		if (fmodf(i->Rooms[0]->Position.x, 2.0f) != 0.0f)
		{
			i->Rooms[0]->Position.x += (rand() % 2) == 0 ? -1 : 1;
		}

		if (fmodf(i->Rooms[0]->Position.y, 2.0f) != 0.0f)
		{
			i->Rooms[0]->Position.y += (rand() % 2) == 0 ? -1 : 1;
		}

		if (fmodf(i->Rooms[1]->Position.x, 2.0f) != 0.0f)
		{
			i->Rooms[1]->Position.x += (rand() % 2) == 0 ? -1 : 1;
		}

		if (fmodf(i->Rooms[1]->Position.y, 2.0f) != 0.0f)
		{
			i->Rooms[1]->Position.y += (rand() % 2) == 0 ? -1 : 1;
		}
	}

	for (RoomList * i = DungeonData->FirstRoom; i != NULL; i = i->Next)
	{
		i->rm.doors = (Door*)calloc(i->rm.nbDoors, sizeof(Door));
		DungeonData->iterationsCount++;
	}

	DoorsGeneration(DungeonData, sfTrue, sfTrue, arrayOfPrebab, GetActualDungeonId());

	///////////////////////////////////////////////// Corridor Creation /////////////////////////////////////////////////

	for (EdgeList * i = DungeonData->FirstEdge; i != NULL; i = i->Next)
	{
		if (!arrayOfPrebab[i->door[0]->roomBack->GraphicsIndex].hasContrain && !arrayOfPrebab[i->door[1]->roomBack->GraphicsIndex].hasContrain)
		{
			RepositionateDoor(i->door[0], i->door[1]);
		}
		else if (RepositionateDoor(i->door[0], i->door[1]))
		{
			DungeonData->iterationsCount = 0;
			ClearEdges(&DungeonData->FirstEdge);
			ClearRooms(&DungeonData->FirstRoom);
			return sfFalse;
		}
		GenerateCorridor(i->door[0], i->door[1]);
		DungeonData->iterationsCount++;
	}

	//Repositionate CheckPoints for center purposes
	for (EdgeList * i = DungeonData->FirstEdge; i != NULL; i = i->Next)
	{
		for (CorridorCheckPointsList *c = i->door[0]->Corridor->firstCheckPoint; c != NULL; c = c->Next)
		{
			if ((c->CP.pos.x - (int)c->CP.pos.x) == 0.0f)
			{
				c->CP.pos.x += 0.5f;
			}

			if ((c->CP.pos.y - (int)c->CP.pos.y) == 0.0f)
			{
				c->CP.pos.y += 0.5f;
			}
			DungeonData->iterationsCount++;
		}
	}

	//Check Corridor Overlapping;
	int cpt3 = 0;
	for (EdgeList * edge = DungeonData->FirstEdge; edge != NULL; edge = edge->Next)
	{
		CorridorCheckPointsList *saveLast = edge->door[0]->Corridor->firstCheckPoint;
		for (CorridorCheckPointsList *CP = edge->door[0]->Corridor->firstCheckPoint->Next; CP != NULL; CP = CP->Next)
		{
			for (RoomList * room = DungeonData->FirstRoom; room != NULL; room = room->Next)
			{
				if (CheckPointIsOverlapping(&CP, room->rm))
				{
					RepositionateCheckPoint(&saveLast, &CP, room->rm, edge->door[0]->Corridor->firstCheckPoint);
					cpt3++;
				}
				DungeonData->iterationsCount++;
			}
			saveLast = CP;
		}
	}

	int cpt4 = 0;
	//Second Check For Complex Overlapping Issues
	for (EdgeList * edge = DungeonData->FirstEdge; edge != NULL; edge = edge->Next)
	{
		CorridorCheckPointsList *saveLast = edge->door[0]->Corridor->firstCheckPoint;
		for (CorridorCheckPointsList *CP = edge->door[0]->Corridor->firstCheckPoint->Next; CP != NULL; CP = CP->Next)
		{
			for (RoomList * room = DungeonData->FirstRoom; room != NULL; room = room->Next)
			{
				if (CheckPointIsOverlapping(&CP, room->rm))
				{
					AddCheckpointsForOverlap(&saveLast, &CP, room->rm, edge->door[0]->Corridor->firstCheckPoint);
					cpt4++;
					CorridorCheckPointsList *saveLastOverLap = edge->door[0]->Corridor->firstCheckPoint;
					for (CorridorCheckPointsList *CPoverlap = edge->door[0]->Corridor->firstCheckPoint->Next; CPoverlap != NULL; CPoverlap = CPoverlap->Next)
					{
						for (RoomList * roomOverLap = DungeonData->FirstRoom; roomOverLap != NULL; roomOverLap = roomOverLap->Next)
						{
							if (CheckPointIsOverlapping(&CPoverlap, roomOverLap->rm))
							{
								RepositionateCheckPoint(&saveLastOverLap, &CPoverlap, roomOverLap->rm, edge->door[0]->Corridor->firstCheckPoint);
							}
							DungeonData->iterationsCount++;
						}
						saveLastOverLap = CPoverlap;
					}
				}
				DungeonData->iterationsCount++;
			}
			saveLast = CP;
		}
	}

	//Check Corridor intersect
	int cpt = 0;
	int cpt2 = 0;
	for (EdgeList * i = DungeonData->FirstEdge; i != NULL; i = i->Next)
	{
		for (CorridorCheckPointsList *CP = i->door[0]->Corridor->firstCheckPoint; CP != NULL; CP = CP->Next)
		{
			if (CP->Next != NULL)
			{
				for (EdgeList * k = DungeonData->FirstEdge; k != NULL; k = k->Next)
				{
					for (CorridorCheckPointsList *CP2 = k->door[0]->Corridor->firstCheckPoint; CP2 != NULL; CP2 = CP2->Next)
					{
						sfVector2f intersectCoord;
						if (CP2->Next != NULL && (i != k))
						{
							//Is Going Vertical
							sfVector2f CpPos, CpPosNext;
							sfVector2f Cp2Pos, Cp2PosNext;
							float coefCP1, coefCP2;

							CpPos = CP->CP.pos;
							CpPosNext = CP->Next->CP.pos;

							Cp2Pos = CP2->CP.pos;
							Cp2PosNext = CP2->Next->CP.pos;

							if((CP->CP.pos.x - CP->Next->CP.pos.x) == 0) 
							{
								coefCP1 = SIZE_VERTICAL_X_x64;
								CpPos.x -= coefCP1 / 2.0f;
								CpPosNext.x -= coefCP1 / 2.0f;
							}
							else
							{
								coefCP1 = SIZE_HORIZONTAL_Y_x64;
								CpPos.y -= coefCP1 / 2.0f;
								CpPosNext.y -= coefCP1 / 2.0f;
							}

							for (int coefI = 0; coefI < 2; coefI++)
							{
								if ((CP2->CP.pos.x - CP2->Next->CP.pos.x) == 0)
								{
									coefCP2 = SIZE_VERTICAL_X_x64;
									Cp2Pos.x = CP2->CP.pos.x - (coefCP2 / 2.0f);
									Cp2PosNext.x = CP2->Next->CP.pos.x - (coefCP2 / 2.0f);
								}
								else
								{
									coefCP2 = SIZE_HORIZONTAL_Y_x64;
									Cp2Pos.y = CP2->CP.pos.y - (coefCP2 / 2.0f);
									Cp2PosNext.y = CP2->Next->CP.pos.y - (coefCP2 / 2.0f);
								}
								
								for (int coefJ = 0; coefJ < 2; coefJ++)
								{
									if ((IsIntersect(CpPos, CpPosNext, Cp2Pos, Cp2PosNext, &intersectCoord)))
									{
										ClearEdges(&DungeonData->FirstEdge);
										ClearRooms(&DungeonData->FirstRoom);
										return sfFalse;
									}
									
									if ((CP2->CP.pos.x - CP2->Next->CP.pos.x) == 0)
									{
										Cp2Pos.x += coefCP2;
										Cp2PosNext.x += coefCP2;
									}
									else
									{
										Cp2Pos.y += coefCP2;
										Cp2PosNext.y += coefCP2;
									}
									cpt2++;
								}

								if ((CP->CP.pos.x - CP->Next->CP.pos.x) == 0)
								{
									CpPos.x += coefCP1;
									CpPosNext.x += coefCP1;
								}
								else
								{
									CpPos.y += coefCP1;
									CpPosNext.y += coefCP1;
								}
							}				
						}
					}
				}
			}
		}
	}

	DungeonData->iterationsCount += cpt2;

	//Check if a door is not too close from the edge of his room Not usefull in this case
	for (RoomList * i = DungeonData->FirstRoom; i != NULL; i = i->Next)
	{
		for (int k = 0; k < i->rm.nbDoors; k++)
		{
			if (i->rm.doors[k].cardDir == W || i->rm.doors[k].cardDir == E)
			{
				float distY = fabsf(i->rm.doors[k].Position.y - i->rm.Position.y) - (((float)i->rm.Height - SIZE_HORIZONTAL_Y_x64) / 2.0f);
				if (distY > 0.0f)
				{
					DungeonData->iterationsCount = 0;
					ClearEdges(&DungeonData->FirstEdge);
					ClearRooms(&DungeonData->FirstRoom);
					return sfFalse;
				}
			}
			else
			{
				float distX = fabsf(i->rm.doors[k].Position.x - i->rm.Position.x) - (((float)i->rm.Width - SIZE_VERTICAL_X_x64) / 2.0f);
				if (distX > 0.0f)
				{
					DungeonData->iterationsCount = 0;
					ClearEdges(&DungeonData->FirstEdge);
					ClearRooms(&DungeonData->FirstRoom);
					return sfFalse;
				}
			}
			DungeonData->iterationsCount++;
		}
	}

	int cptRoomChecked = 0;
	Room *first = &DungeonData->FirstRoom->rm;
	int *IDCheck = (int*)calloc(pNumberOfRoom, sizeof(int));
	for (int i = 0; i < pNumberOfRoom; i++)
	{
		IDCheck[i] = -1;
	}

	NavigateDungeon(first, &cptRoomChecked, IDCheck, pNumberOfRoom);

	if (cptRoomChecked < pNumberOfRoom)
	{
		printf("///////////// Dungeon is not fully connected, generation restarted \\\\\\\\\\\n");
		DungeonData->iterationsCount = 0;
		ClearEdges(&DungeonData->FirstEdge);
		ClearRooms(&DungeonData->FirstRoom);
		return sfFalse;
	}

	free(IDCheck);

#if _DEBUG
	printf("Overlapping Count : %d\n", cpt3);
	printf("Complex Overlapping Count : %d\n", cpt4);
	printf("cpt : %d, cpt2 : %d, total_Iteration : %u\n", cpt, cpt2, DungeonData->iterationsCount);
#endif

	return sfTrue;
}

sfBool GenerateDungeonNoCorri(DungeonRawData* DungeonData, const int pNumberOfRoom, const sfVector2f pCenter, const float pArea, const float pMargin, const float pmaxWidth, RoomPrefab *arrayOfPrebab, const DWORD ThreadID, const long int previousSeed)
{
	unsigned int temp = (unsigned int)time(NULL);

	if (previousSeed != 0)
	{
		DungeonData->seed = previousSeed;
	}
	else if (ThreadID == 0)
	{
		DungeonData->seed = temp * rand();
	}
	else
	{
		DungeonData->seed = temp * rand() * ThreadID;
	}
	//DungeonData->seed = 1012501756;
	srand(DungeonData->seed);

	int count = 0;
	short groupIndex = 0;
	DungeonData->edgesCount = 0;
	DungeonData->iterationsCount = 0;
	DungeonData->numberOfRoom = pNumberOfRoom;

	RoomsGeneration(DungeonData, pNumberOfRoom, pCenter, pArea, pMargin, pmaxWidth);

	for (RoomList * i = DungeonData->FirstRoom; i != NULL; i = i->Next)
	{
		i->rm.doors = (Door*)calloc(i->rm.nbDoors, sizeof(Door));
		DungeonData->iterationsCount++;
	}

	DoorsGeneration(DungeonData, sfFalse, sfTrue, arrayOfPrebab, GetActualDungeonId());

	return sfTrue;
}

sfBool MultiProceduralGeneration(DungeonRawData* DungeonData, const int pNumberOfRoom, const sfVector2f pCenter, const float pArea, const float pMargin, const float pmaxWidth, RoomPrefab *arrayOfPrebab, const int amountParallalThread, const DungeonId dungeonId)
{
	bool* GenerationFounded = (bool*)malloc(sizeof(bool));
	*GenerationFounded = false;

	void* Transfert[9] = { DungeonData, (int*)&pNumberOfRoom, (sfVector2f*)&pCenter, (float*)&pArea, (float*)&pMargin, (float*)&pmaxWidth, arrayOfPrebab, GenerationFounded, (GetTypeOfDungeon() == dungeonCorridor ? GenerateDungeonRD : GenerateDungeonNoCorri) };

	//Create Mutex
	MutexBool = CreateMutexA(NULL, false, NULL);

	//Aray of Threads
	HANDLE* parallalThread = (HANDLE*)calloc(amountParallalThread, sizeof(HANDLE));

	//Inits Threads
	for (int i = 0; i < amountParallalThread; i++)
	{
		parallalThread[i] = CreateThread(NULL, 0, GenerationDungeonSoloThread, Transfert, 0, NULL);
	}

	//Wait for one of the thread to finish
	WaitForMultipleObjects(amountParallalThread, parallalThread, false, INFINITE);

	//Clean up Procedures
	for (int i = 0; i < amountParallalThread; i++)
	{
		DWORD holder;
		GetExitCodeThread(parallalThread[i], &holder);
		if (holder == STILL_ACTIVE)
		{
			Sleep(20);
			CloseHandle(parallalThread[i]);
		}
	}

	free(parallalThread);
	free(GenerationFounded);

	CloseHandle(MutexBool);
	return sfTrue;
}
