#include "ProceduralConversion.h"
#include <math.h>


void ClearSfVector2fList(Vector2fList **firstElement)
{
	Vector2fList *toDelete = *firstElement;
	while (toDelete != NULL)
	{
		Vector2fList *temp = toDelete->next;
		free(toDelete);
		toDelete = temp;
	}
	*firstElement = NULL;
}

void GetDoorFromIndex(int *idRoom, int *idDoor, const int index, DungeonGraphicData* dungeon)
{
	for (int i = 0; i < dungeon->numberOfRoom; i++)
	{
		for (int j = 0; j < dungeon->room[i].doorCount; j++)
		{
			if (dungeon->room[i].doors[j].uniqueID == index)
			{
				*idRoom = i;
				*idDoor = j;
				return;
			}
		}
	}
}

void ClearGraphicData(DungeonGraphicData *ToClean)
{
	for (int i = 0; i < ToClean->numberOfRoom; i++)
	{
		for (int j = 0; j < ToClean->room[i].doorCount; j++)
		{
			if (ToClean->room[i].doors[j].corridorRef != NULL)
			{
				int index = ToClean->room[i].doors[j].corridorRef->doorsRef[0]->uniqueID == ToClean->room[i].doors[j].uniqueID ? 1 : 0;
				ToClean->room[i].doors[j].corridorRef->doorsRef[index]->corridorRef = NULL;

				ClearSfVector2fList(&ToClean->room[i].doors[j].corridorRef->firstElementPos);
				free(ToClean->room[i].doors[j].corridorRef);
			}
		}

		if (ToClean->room[i].TorchCount != 0)
		{
			free(ToClean->room[i].Torchs);
			ToClean->room[i].TorchCount = 0;
		}

		free(ToClean->room[i].doors);
		ToClean->room[i].doorCount = 0;
	}
	free(ToClean->room);
	ToClean->room = NULL;
	ToClean->numberOfRoom = 0;
}

void ConvertToGraphicData(DungeonGraphicData *DungeonGD, DungeonRawData* DungeonRD)
{
	DungeonGD->room = NULL;
	DungeonGD->numberOfRoom = DungeonRD->numberOfRoom;
	DungeonGD->seed = DungeonRD->seed;

	DungeonGD->room = (RoomGraphicData*)calloc(DungeonGD->numberOfRoom, sizeof(RoomGraphicData));

	int count = 0;
	for (RoomList* r = DungeonRD->FirstRoom; r != NULL; r = r->Next)
	{
		for (int i = 0; i < r->rm.nbDoors; i++)
		{
			r->rm.doors[i].Index = count;
			count++;
		}
	}

	RoomList *tempRoom = DungeonRD->FirstRoom;

	for (int i = 0; i < DungeonGD->numberOfRoom; i++)
	{
		DungeonGD->room[i].Pos.x = tempRoom->rm.Position.x;
		DungeonGD->room[i].Pos.y = tempRoom->rm.Position.y;
		DungeonGD->room[i].size = 0;
		DungeonGD->room[i].uniqueID = tempRoom->rm.GraphicsIndex;
		DungeonGD->room[i].IdEnemyRoom = tempRoom->rm.Index;
		DungeonGD->room[i].doors = (DoorGraphicData*)calloc(tempRoom->rm.nbDoors, sizeof(DoorGraphicData));
		DungeonGD->room[i].doorCount = tempRoom->rm.nbDoors;


		for (int j = 0; j < tempRoom->rm.nbDoors; j++)
		{
			DungeonGD->room[i].doors[j].LocalPos.x = tempRoom->rm.doors[j].Position.x - tempRoom->rm.Position.x;
			DungeonGD->room[i].doors[j].LocalPos.y = tempRoom->rm.doors[j].Position.y - tempRoom->rm.Position.y;
			DungeonGD->room[i].doors[j].uniqueID = tempRoom->rm.doors[j].Index;
			DungeonGD->room[i].doors[j].Dir = tempRoom->rm.doors[j].cardDir;
		}
		if (tempRoom->Next != NULL)
		{
			tempRoom = tempRoom->Next;
		}
	}
	unsigned short IDCount = 0;
	for (EdgeList* e = DungeonRD->FirstEdge; e != NULL; e = e->Next)
	{
		if (e->door[0]->Corridor != NULL)
		{
			CorridorGraphicData* corridor = (CorridorGraphicData*)malloc(sizeof(CorridorGraphicData));
			corridor->firstElementPos = NULL;
			corridor->uniqueID = IDCount;
			IDCount++;


			CorridorCheckPointsList *Previous = NULL;

			for (CorridorCheckPointsList *CPList = e->door[0]->Corridor->firstCheckPoint; CPList != NULL; CPList = CPList->Next)
			{
				if (CPList->Next != NULL)
				{
					//Going Horizontal
					if (CPList->CP.pos.x - CPList->Next->CP.pos.x != 0)
					{
						float offset = SIZE_HORIZONTAL_X_x64;
						if (CPList->Next->Next != NULL && CPList->Next->CP.pos.x - CPList->Next->Next->CP.pos.x == 0)
						{
							offset = SIZE_VERTICAL_X_x64 / 2.0f + SIZE_HORIZONTAL_X_x64 / 2.0f;
						}
						else if (CPList->Next->Next == NULL)
						{
							offset = SIZE_HORIZONTAL_X_x64 / 2.0f;
						}

						float i = (float)CPList->CP.pos.x;

						if (CPList == e->door[0]->Corridor->firstCheckPoint)
						{
							i += CPList->CP.pos.x < CPList->Next->CP.pos.x ? (SIZE_HORIZONTAL_X_x64 / 2.0f + 0.5f) : -(SIZE_HORIZONTAL_X_x64 / 2.0f + 0.5f);
						}

						if (Previous != NULL)
						{
							if (CPList->CP.pos.x - Previous->CP.pos.x == 0)
							{
								Vector2fList *newPos = (Vector2fList*)malloc(sizeof(Vector2fList));
								newPos->Position.x = i *64.f;
								newPos->Position.y = CPList->CP.pos.y *64.f;
								newPos->id = CPList->CP.pos.x < CPList->Next->CP.pos.x ? 2 : 3;
								newPos->id = CPList->CP.pos.x < CPList->Next->CP.pos.x ? (CPList->CP.pos.y < Previous->CP.pos.y ? 5 : 2) : (CPList->CP.pos.y < Previous->CP.pos.y ? 4 : 3);
								newPos->next = corridor->firstElementPos;
								corridor->firstElementPos = newPos;

								i += CPList->CP.pos.x < CPList->Next->CP.pos.x ? ((SIZE_HORIZONTAL_X_x64 / 2.0f) + (SIZE_VERTICAL_X_x64 / 2.0f)) : -((SIZE_HORIZONTAL_X_x64 / 2.0f) + (SIZE_VERTICAL_X_x64 / 2.0f));
							}
							else
							{
								i += CPList->CP.pos.x < CPList->Next->CP.pos.x ? -.5f : .5f;
							}
						}

						if (CPList->CP.pos.x < CPList->Next->CP.pos.x)
						{
							while (i <= CPList->Next->CP.pos.x - offset)
							{
								Vector2fList *newPos = (Vector2fList*)malloc(sizeof(Vector2fList));
								newPos->Position.x = i *64.f;
								newPos->Position.y = CPList->CP.pos.y *64.f;
								newPos->id = 0;
								newPos->next = corridor->firstElementPos;
								corridor->firstElementPos = newPos;
								i += SIZE_HORIZONTAL_X_x64;
							}

							//Check to Add one slice to build up the missing parts
							i -= (SIZE_HORIZONTAL_X_x64 / 2.0f - 0.5f);
							while (i <= CPList->Next->CP.pos.x - offset + 1.5f)
							{
								Vector2fList *newPos = (Vector2fList*)malloc(sizeof(Vector2fList));
								newPos->Position.x = i *64.f;
								newPos->Position.y = CPList->CP.pos.y *64.f;
								newPos->id = 6;
								newPos->next = corridor->firstElementPos;
								corridor->firstElementPos = newPos;
								i += 1.0f;
							}
						}
						else
						{
							while (i >= CPList->Next->CP.pos.x + offset)
							{
								Vector2fList *newPos = (Vector2fList*)malloc(sizeof(Vector2fList));
								newPos->Position.x = i *64.f;
								newPos->Position.y = CPList->CP.pos.y *64.f;
								newPos->id = 0;
								newPos->next = corridor->firstElementPos;
								corridor->firstElementPos = newPos;
								i -= SIZE_HORIZONTAL_X_x64;
							}

							//Check to Add one slice to build up the missing parts
							i += (SIZE_HORIZONTAL_X_x64 / 2.0f - 0.5f);
							while (i >= CPList->Next->CP.pos.x + offset - 1.5f)
							{
								Vector2fList *newPos = (Vector2fList*)malloc(sizeof(Vector2fList));
								newPos->Position.x = i *64.f;
								newPos->Position.y = CPList->CP.pos.y *64.f;
								newPos->id = 6;
								newPos->next = corridor->firstElementPos;
								corridor->firstElementPos = newPos;
								i -= 1.0f;
							}
						}
					}
					else
					{
						//Going Vertical
						float offset = SIZE_VERTICAL_Y_x64;
						if (CPList->Next->Next != NULL && CPList->Next->CP.pos.y - CPList->Next->Next->CP.pos.y == 0)
						{
							offset = SIZE_HORIZONTAL_Y_x64 / 2.0f + SIZE_VERTICAL_Y_x64 / 2.0f;
						}
						else if (CPList->Next->Next == NULL)
						{
							offset = SIZE_VERTICAL_Y_x64 / 2.0f;
						}

						float i = CPList->CP.pos.y;
						if (CPList == e->door[0]->Corridor->firstCheckPoint)
						{
							if (CPList->CP.pos.y < CPList->Next->CP.pos.y)
							{
								i += (SIZE_VERTICAL_Y_x64 / 2.0f + 0.5f);
							}
							else
							{
								i += -(SIZE_VERTICAL_Y_x64 / 2.0f + 0.5f);
							}
						}

						if (Previous != NULL)
						{
							if (CPList->CP.pos.y - Previous->CP.pos.y == 0)
							{
								Vector2fList *newPos = (Vector2fList*)malloc(sizeof(Vector2fList));
								newPos->Position.x = CPList->CP.pos.x *64.f;
								newPos->Position.y = i *64.f;
								newPos->id = CPList->CP.pos.y < CPList->Next->CP.pos.y ? (CPList->CP.pos.x < Previous->CP.pos.x ? 5 : 4) : (CPList->CP.pos.x < Previous->CP.pos.x ? 2 : 3);
								newPos->next = corridor->firstElementPos;
								corridor->firstElementPos = newPos;

								i += CPList->CP.pos.y < CPList->Next->CP.pos.y ? ((SIZE_HORIZONTAL_Y_x64 / 2.0f) + (SIZE_VERTICAL_Y_x64 / 2.0f)) : -((SIZE_HORIZONTAL_Y_x64 / 2.0f) + (SIZE_VERTICAL_Y_x64 / 2.0f));
							}
							else
							{
								i += CPList->CP.pos.y < CPList->Next->CP.pos.y ? -0.5f : 0.5f;
							}
						}

						if (CPList->CP.pos.y < CPList->Next->CP.pos.y)
						{
							while (i <= CPList->Next->CP.pos.y - offset)
							{
								Vector2fList *newPos = (Vector2fList*)malloc(sizeof(Vector2fList));
								newPos->Position.x = CPList->CP.pos.x *64.f;
								newPos->Position.y = i *64.f;
								newPos->id = 1;
								newPos->next = corridor->firstElementPos;
								corridor->firstElementPos = newPos;
								i += SIZE_VERTICAL_Y_x64;
							}
							//Check to Add one slice to build up the missing parts
							i -= (SIZE_VERTICAL_Y_x64 / 2.0f - 0.5f);
							while (i <= CPList->Next->CP.pos.y - offset + 1.5f)
							{
								Vector2fList *newPos = (Vector2fList*)malloc(sizeof(Vector2fList));
								newPos->Position.x = CPList->CP.pos.x *64.f;
								newPos->Position.y = i *64.f;
								newPos->id = 7;
								newPos->next = corridor->firstElementPos;
								corridor->firstElementPos = newPos;
								i += 1.0f;
							}

						}
						else
						{
							while (i >= CPList->Next->CP.pos.y + offset)
							{
								Vector2fList *newPos = (Vector2fList*)malloc(sizeof(Vector2fList));
								newPos->Position.x = CPList->CP.pos.x *64.f;
								newPos->Position.y = i *64.f;
								newPos->id = 1;
								newPos->next = corridor->firstElementPos;
								corridor->firstElementPos = newPos;
								i -= SIZE_VERTICAL_Y_x64;
							}

							//Check to Add one slice to build up the missing parts
							i += (SIZE_VERTICAL_Y_x64 / 2.0f - 0.5f);
							while (i >= CPList->Next->CP.pos.y + offset - 1.5f)
							{
								Vector2fList *newPos = (Vector2fList*)malloc(sizeof(Vector2fList));
								newPos->Position.x = CPList->CP.pos.x *64.f;
								newPos->Position.y = i *64.f;
								newPos->id = 7;
								newPos->next = corridor->firstElementPos;
								corridor->firstElementPos = newPos;
								i -= 1.0f;
							}
						}
					}
					Previous = CPList;
				}
			}

			int tempIdRoom = 0;
			int tempIdDoor = 0;

			GetDoorFromIndex(&tempIdRoom, &tempIdDoor, e->door[0]->Index, DungeonGD);
			DungeonGD->room[tempIdRoom].doors[tempIdDoor].corridorRef = corridor;
			DungeonGD->room[tempIdRoom].doors[tempIdDoor].isDebutOfCorridor = sfTrue;
			corridor->doorsRef[0] = &DungeonGD->room[tempIdRoom].doors[tempIdDoor];

			GetDoorFromIndex(&tempIdRoom, &tempIdDoor, e->door[1]->Index, DungeonGD);
			DungeonGD->room[tempIdRoom].doors[tempIdDoor].corridorRef = corridor;
			DungeonGD->room[tempIdRoom].doors[tempIdDoor].isDebutOfCorridor = sfFalse;
			corridor->doorsRef[1] = &DungeonGD->room[tempIdRoom].doors[tempIdDoor];
		}
		else
		{
			int tempIdRoom = 0;
			int tempIdDoor = 0;

			GetDoorFromIndex(&tempIdRoom, &tempIdDoor, e->door[0]->Index, DungeonGD);
			DungeonGD->room[tempIdRoom].doors[tempIdDoor].corridorRef = NULL;
			GetDoorFromIndex(&tempIdRoom, &tempIdDoor, e->door[1]->Index, DungeonGD);
			DungeonGD->room[tempIdRoom].doors[tempIdDoor].corridorRef = NULL;
		}
	}
}
