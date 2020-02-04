#include "ProceduralMain.h"
#include "ProceduralGeneration.h"
#include "ProceduralConversion.h"
#include "..\Room\Room.h"
#include "..\..\DevTools\NodeDisplayer.h"
#include "..\..\Controller\MainSceneController\LightController\SegmentLight.h"
#include "..\..\Controller\MainSceneController\LightController\PointLight.h"
#include "..\..\Controller\Serialization\Serialization.h"
#include <Windows.h>
#define SLOWCOLOR (sfColor){255, 0, 174, 255}
#define SLOWCOLOR2 (sfColor){49, 66, 20, 255}

typedef struct
{
	SegmentsLight Segments;
	Node **ArrayAdress;
	int sizeX;
	int sizeY;
}NodeCorridorBackUp;

NodeCorridorBackUp *corridorPartPtr = NULL;
int counterArray = 0;
int sizeArray = 200;
sfVertexArray* Supp = NULL;

sfBool IsEqualColor(const sfColor color1, const sfColor color2)
{
	return (color1.r == color2.r && color1.g == color2.g && color1.b == color2.b);
}

void InitPointer()
{
	if (corridorPartPtr != NULL)
	{
		corridorPartPtr = NULL;
		counterArray = 0;
		sizeArray = 200;
	}
}

void AddPointer(Node **adress, const int X, const int Y)
{
	if (corridorPartPtr == NULL)
	{
		corridorPartPtr = (NodeCorridorBackUp*)calloc(sizeArray, sizeof(NodeCorridorBackUp));
		counterArray = 0;
	}
	else if (counterArray + 5 > sizeArray)
	{
		sizeArray += 200;
		corridorPartPtr = (NodeCorridorBackUp*)realloc(corridorPartPtr, sizeof(NodeCorridorBackUp) * sizeArray);
	}

	corridorPartPtr[counterArray].ArrayAdress = adress;
	corridorPartPtr[counterArray].sizeX = X;
	corridorPartPtr[counterArray].sizeY = Y;

	counterArray++;
}

void CleanCorridorPointers()
{
	for (int i = 0; i < counterArray; i++)
	{
		for (int x = 0; x < corridorPartPtr[i].sizeX; x++)
		{
			for (int y = 0; y < corridorPartPtr[i].sizeY; y++)
			{
				ClearAllSegmentsLight(&corridorPartPtr[i].ArrayAdress[x][y].Segments);
			}
			free(corridorPartPtr[i].ArrayAdress[x]);
		}
		free(corridorPartPtr[i].ArrayAdress);
	}
	counterArray = 0;
	ResetCounterPointer();
}

void ClearDungeon(DungeonGameData* ToClean)
{
	/*if (Supp)
	{
		sfVertexArray_clear(Supp);
		sfVertexArray_destroy(Supp);
		printf("clear\n");
	}*/

	if (ToClean->room->Adress == NULL)
	{
		return;
	}

	for (int n = 0; n < ToClean->numberOfRoom; n++)
	{
		for (int j = 0; j < ToClean->room[n].Width; j++)
		{
			if (ToClean->room[n].Adress[j][0].Segments.numberSegment != 0)
			{
				for (int i = 0; i < ToClean->room[n].Height; i++)
				{
					ClearAllSegmentsLight(&ToClean->room[n].Adress[j][i].Segments);
				}
			}
			free(ToClean->room[n].Adress[j]);
		}
		free(ToClean->room[n].Adress);
	}

	CleanCorridorPointers();

	free(ToClean->room);
	ToClean->room = NULL;
}

//Connect Line or Column of Node they nered to be next to each to other to connect properly;
void ConnectSliceOfNodes(Node** SliceA, Node** SliceB)
{
	//The slice need to be next to each other(only 1 axis as a difference of one between the two slice else we cancel the try)
	if (fabsf((*SliceA)->posX - (*SliceB)->posX) > 1 && fabsf((*SliceA)->posY - (*SliceB)->posY) > 1)
	{
		printf("Can't connect Slice of Nodes, they aren't next to each others\n");
		return;
	}

	Node *PreviousSlice = (*SliceA);
	Node *NewSlice = (*SliceB);

	if (fabsf((*SliceA)->posX - (*SliceB)->posX) == 1)
	{
		while (PreviousSlice != NULL && NewSlice != NULL)
		{
			int valueFirst = GetDirectionBetWeenTwoNodes(PreviousSlice->posX, PreviousSlice->posY, NewSlice->posX, NewSlice->posY);
			int valueSecond = (valueFirst + 4) % 8;
			PreviousSlice->edges[valueFirst] = NewSlice;
			NewSlice->edges[valueSecond] = PreviousSlice;
			PreviousSlice->edges[valueFirst == 0 ? 1 : 3] = NewSlice->edges[2];
			NewSlice->edges[valueFirst == 0 ? 3 : 1] = PreviousSlice->edges[2];
			PreviousSlice->edges[valueFirst == 0 ? 7 : 5] = NewSlice->edges[6];
			NewSlice->edges[valueFirst == 0 ? 5 : 7] = PreviousSlice->edges[6];

			PreviousSlice = PreviousSlice->edges[6];
			NewSlice = NewSlice->edges[6];
		}
	}
	else
	{
		while (PreviousSlice != NULL && NewSlice != NULL)
		{
			int valueFirst = GetDirectionBetWeenTwoNodes(PreviousSlice->posX, PreviousSlice->posY, NewSlice->posX, NewSlice->posY);
			int valueSecond = (valueFirst + 4) % 8;
			PreviousSlice->edges[valueFirst] = NewSlice;
			NewSlice->edges[valueSecond] = PreviousSlice;
			PreviousSlice->edges[valueFirst == 6 ? 5 : 3] = NewSlice->edges[4];
			NewSlice->edges[valueFirst == 6 ? 3 : 5] = PreviousSlice->edges[4];
			PreviousSlice->edges[valueFirst == 6 ? 7 : 1] = NewSlice->edges[0];
			NewSlice->edges[valueFirst == 6 ? 1 : 7] = PreviousSlice->edges[0];

			PreviousSlice = PreviousSlice->edges[0];
			NewSlice = NewSlice->edges[0];
		}
	}
}

void ConnectNodes(Node **Prefab, Node **firstElement, const int Width, const int Height)
{
	Node* tempToClose = *firstElement;
	Node **connectingNode = firstElement;

	for (int c = 0; c < Width; c++)
	{
		*connectingNode = &Prefab[c][0];
		for (int j = 0; j < Height; j++)
		{
			int index = 3;
			for (int tempY = j - 1; tempY <= j + 1; tempY++)
			{
				for (int tempX = c - 1; tempX <= c + 1; tempX++)
				{
					if (tempX >= 0 && tempX < Width && tempY >= 0 && tempY < Height)
					{
						(*connectingNode)->edges[index] = &Prefab[tempX][tempY];
					}
					else
					{
						(*connectingNode)->edges[index] = NULL;
					}

					if (tempY == j && tempX == c - 1)
					{
						tempX++;
						index = 0;
					}
					else if (tempY == j + 1)
					{
						index++;
					}
					else
					{
						index--;
					}
				}
				if (tempY == j - 1)
				{
					index = 4;
				}
				else if (tempY == j)
				{
					index = 5;
				}
			}
			*connectingNode = (*connectingNode)->edges[6];
		}
	}

	*firstElement = tempToClose;
}

void GenerateMapLight(DungeonGameData* dungeonGameData, const DungeonGraphicData dungeonGraphicData, Node** tempRoomNode, const int i, const sfImage** MapLight)
{
	int a = 0;
	int b = 0;
	float offset = 32;
	for (float k = (int)(dungeonGameData->room[i].Position.x - dungeonGameData->room[i].Width / 2.0f) + 0.5f; k < dungeonGameData->room[i].Position.x + dungeonGameData->room[i].Width / 2.0f + 0.5f; k++)
	{
		for (float l = (int)(dungeonGameData->room[i].Position.y - dungeonGameData->room[i].Height / 2.0f) + 0.5f; l < dungeonGameData->room[i].Position.y + dungeonGameData->room[i].Height / 2.0f + 0.5f; l++)
		{
			sfBool createSegmentLight = sfFalse;
			sfColor colLight = sfImage_getPixel(MapLight[dungeonGraphicData.room[i].uniqueID], a, b);

			InitSegmentsLight(&tempRoomNode[a][b].Segments);
			for (unsigned short s = 0; s < dungeonGameData->room[i].nbDoors; s++)
			{


				if ((dungeonGameData->room[i].doors[s].Pos.x >= k - 1 && dungeonGameData->room[i].doors[s].Pos.x <= k + 1)
					&& dungeonGameData->room[i].doors[s].Pos.y == l
					&& (dungeonGameData->room[i].doors[s].Dir == N || dungeonGameData->room[i].doors[s].Dir == S))
				{
					if (dungeonGameData->room[i].doors[s].Pos.x == k
						&& dungeonGameData->room[i].doors[s].Pos.y == l
						&& dungeonGameData->room[i].doors[s].Dir == N)
					{
						AddSegmentsLight(&tempRoomNode[a][b].Segments, (sfVector2f) { k*SIZE_CASE - 2 * SIZE_CASE + offset, l*SIZE_CASE - offset }, (sfVector2f) { k*SIZE_CASE - 2 * SIZE_CASE + offset, l*SIZE_CASE + 3 * SIZE_CASE + offset });
						sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P1, colLight, (sfVector2f) { 0 } });
						sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P2, colLight, (sfVector2f) { 0 } });

						AddSegmentsLight(&tempRoomNode[a][b].Segments, (sfVector2f) { k*SIZE_CASE + 1 * SIZE_CASE + offset, l*SIZE_CASE - offset }, (sfVector2f) { k*SIZE_CASE + 1 * SIZE_CASE + offset, l*SIZE_CASE + 3 * SIZE_CASE + offset });
						sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P1, colLight, (sfVector2f) { 0 } });
						sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P2, colLight, (sfVector2f) { 0 } });

					}

					if (dungeonGameData->room[i].doors[s].Pos.x == k
						&& dungeonGameData->room[i].doors[s].Pos.y == l
						&& dungeonGameData->room[i].doors[s].Dir == S)
					{
						AddSegmentsLight(&tempRoomNode[a][b].Segments, (sfVector2f) { k*SIZE_CASE - 2 * SIZE_CASE + offset, l*SIZE_CASE - offset }, (sfVector2f) { k*SIZE_CASE - 2 * SIZE_CASE + offset, l * SIZE_CASE + SIZE_CASE - offset });
						sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P1, colLight, (sfVector2f) { 0 } });
						sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P2, colLight, (sfVector2f) { 0 } });

						AddSegmentsLight(&tempRoomNode[a][b].Segments, (sfVector2f) { k*SIZE_CASE + 1 * SIZE_CASE + offset, l*SIZE_CASE - offset }, (sfVector2f) { k*SIZE_CASE + 1 * SIZE_CASE + offset, l * SIZE_CASE + SIZE_CASE - offset });
						sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P1, colLight, (sfVector2f) { 0 } });
						sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P2, colLight, (sfVector2f) { 0 } });

					}
					createSegmentLight = sfTrue;
				}
				else if ((dungeonGameData->room[i].doors[s].Pos.y >= l - 3 && dungeonGameData->room[i].doors[s].Pos.y <= l)
					&& dungeonGameData->room[i].doors[s].Pos.x == k)
				{
					createSegmentLight = sfTrue;
					if (dungeonGameData->room[i].doors[s].Pos.x == k && dungeonGameData->room[i].doors[s].Pos.y == l)
					{
						AddSegmentsLight(&tempRoomNode[a][b].Segments, (sfVector2f) { k*SIZE_CASE - offset, l*SIZE_CASE - 3 * SIZE_CASE - offset }, (sfVector2f) { k*SIZE_CASE + offset, l*SIZE_CASE - 3 * SIZE_CASE - offset });
						sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P1, colLight, (sfVector2f) { 0 } });
						sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P2, colLight, (sfVector2f) { 0 } });

						AddSegmentsLight(&tempRoomNode[a][b].Segments, (sfVector2f) { k*SIZE_CASE - offset, l*SIZE_CASE + 3 * SIZE_CASE + offset }, (sfVector2f) { k*SIZE_CASE + offset, l*SIZE_CASE + 3 * SIZE_CASE + offset });
						sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P1, colLight, (sfVector2f) { 0 } });
						sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P2, colLight, (sfVector2f) { 0 } });
					}
				}

			}
			if (!createSegmentLight)
			{
				if (IsEqualColor(lSegment, colLight))
				{

					AddSegmentsLight(&tempRoomNode[a][b].Segments, (sfVector2f) { k*SIZE_CASE - offset, l*SIZE_CASE - offset }, (sfVector2f) { k*SIZE_CASE - offset, l*SIZE_CASE + offset });
					sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P1, colLight, (sfVector2f) { 0 } });
					sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P2, colLight, (sfVector2f) { 0 } });
				}
				if (IsEqualColor(rSegment, colLight))
				{
					AddSegmentsLight(&tempRoomNode[a][b].Segments, (sfVector2f) { k*SIZE_CASE + offset, l*SIZE_CASE - offset }, (sfVector2f) { k*SIZE_CASE + offset, l*SIZE_CASE + offset });
					sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P1, colLight, (sfVector2f) { 0 } });
					sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P2, colLight, (sfVector2f) { 0 } });
				}
				if (IsEqualColor(dSegment, colLight))
				{
					AddSegmentsLight(&tempRoomNode[a][b].Segments, (sfVector2f) { k*SIZE_CASE + offset, l*SIZE_CASE + offset }, (sfVector2f) { k*SIZE_CASE - offset, l*SIZE_CASE + offset });
					sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P1, colLight, (sfVector2f) { 0 } });
					sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P2, colLight, (sfVector2f) { 0 } });
				}
				if (IsEqualColor(uSegment, colLight))
				{
					AddSegmentsLight(&tempRoomNode[a][b].Segments, (sfVector2f) { k*SIZE_CASE - offset, l*SIZE_CASE - offset }, (sfVector2f) { k*SIZE_CASE + offset, l*SIZE_CASE - offset });
					sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P1, colLight, (sfVector2f) { 0 } });
					sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P2, colLight, (sfVector2f) { 0 } });
				}
				if (IsEqualColor(ldSegment, colLight))
				{
					AddSegmentsLight(&tempRoomNode[a][b].Segments, (sfVector2f) { k*SIZE_CASE - offset, l*SIZE_CASE - offset }, (sfVector2f) { k*SIZE_CASE - offset, l*SIZE_CASE + offset });
					sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P1, colLight, (sfVector2f) { 0 } });
					sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P2, colLight, (sfVector2f) { 0 } });

					AddSegmentsLight(&tempRoomNode[a][b].Segments, (sfVector2f) { k*SIZE_CASE + offset, l*SIZE_CASE + offset }, (sfVector2f) { k*SIZE_CASE - offset, l*SIZE_CASE + offset });
					sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P1, colLight, (sfVector2f) { 0 } });
					sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P2, colLight, (sfVector2f) { 0 } });
				}
				if (IsEqualColor(ldrSegment, colLight))
				{
					AddSegmentsLight(&tempRoomNode[a][b].Segments, (sfVector2f) { k*SIZE_CASE - offset, l*SIZE_CASE - offset }, (sfVector2f) { k*SIZE_CASE - offset, l*SIZE_CASE + offset });
					sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P1, colLight, (sfVector2f) { 0 } });
					sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P2, colLight, (sfVector2f) { 0 } });

					AddSegmentsLight(&tempRoomNode[a][b].Segments, (sfVector2f) { k*SIZE_CASE + offset, l*SIZE_CASE + offset }, (sfVector2f) { k*SIZE_CASE - offset, l*SIZE_CASE + offset });
					sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P1, colLight, (sfVector2f) { 0 } });
					sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P2, colLight, (sfVector2f) { 0 } });

					AddSegmentsLight(&tempRoomNode[a][b].Segments, (sfVector2f) { k*SIZE_CASE + offset, l*SIZE_CASE - offset }, (sfVector2f) { k*SIZE_CASE + offset, l*SIZE_CASE + offset });
					sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P1, colLight, (sfVector2f) { 0 } });
					sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P2, colLight, (sfVector2f) { 0 } });

				}
				if (IsEqualColor(ldruSegment, colLight))
				{
					AddSegmentsLight(&tempRoomNode[a][b].Segments, (sfVector2f) { k*SIZE_CASE - offset, l*SIZE_CASE - offset }, (sfVector2f) { k*SIZE_CASE - offset, l*SIZE_CASE + offset });
					sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P1, colLight, (sfVector2f) { 0 } });
					sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P2, colLight, (sfVector2f) { 0 } });

					AddSegmentsLight(&tempRoomNode[a][b].Segments, (sfVector2f) { k*SIZE_CASE - offset, l*SIZE_CASE - offset }, (sfVector2f) { k*SIZE_CASE + offset, l*SIZE_CASE - offset });
					sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P1, colLight, (sfVector2f) { 0 } });
					sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P2, colLight, (sfVector2f) { 0 } });

					AddSegmentsLight(&tempRoomNode[a][b].Segments, (sfVector2f) { k*SIZE_CASE + offset, l*SIZE_CASE - offset }, (sfVector2f) { k*SIZE_CASE + offset, l*SIZE_CASE + offset });
					sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P1, colLight, (sfVector2f) { 0 } });
					sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P2, colLight, (sfVector2f) { 0 } });

					AddSegmentsLight(&tempRoomNode[a][b].Segments, (sfVector2f) { k*SIZE_CASE + offset, l*SIZE_CASE + offset }, (sfVector2f) { k*SIZE_CASE - offset, l*SIZE_CASE + offset });
					sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P1, colLight, (sfVector2f) { 0 } });
					sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P2, colLight, (sfVector2f) { 0 } });

				}
				if (IsEqualColor(drSegment, colLight))
				{
					AddSegmentsLight(&tempRoomNode[a][b].Segments, (sfVector2f) { k*SIZE_CASE + offset, l*SIZE_CASE - offset }, (sfVector2f) { k*SIZE_CASE + offset, l*SIZE_CASE + offset });
					sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P1, colLight, (sfVector2f) { 0 } });
					sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P2, colLight, (sfVector2f) { 0 } });

					AddSegmentsLight(&tempRoomNode[a][b].Segments, (sfVector2f) { k*SIZE_CASE + offset, l*SIZE_CASE + offset }, (sfVector2f) { k*SIZE_CASE - offset, l*SIZE_CASE + offset });
					sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P1, colLight, (sfVector2f) { 0 } });
					sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P2, colLight, (sfVector2f) { 0 } });


				}
				if (IsEqualColor(druSegment, colLight))
				{
					AddSegmentsLight(&tempRoomNode[a][b].Segments, (sfVector2f) { k*SIZE_CASE - offset, l*SIZE_CASE - offset }, (sfVector2f) { k*SIZE_CASE + offset, l*SIZE_CASE - offset });
					sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P1, colLight, (sfVector2f) { 0 } });
					sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P2, colLight, (sfVector2f) { 0 } });

					AddSegmentsLight(&tempRoomNode[a][b].Segments, (sfVector2f) { k*SIZE_CASE + offset, l*SIZE_CASE - offset }, (sfVector2f) { k*SIZE_CASE + offset, l*SIZE_CASE + offset });
					sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P1, colLight, (sfVector2f) { 0 } });
					sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P2, colLight, (sfVector2f) { 0 } });

					AddSegmentsLight(&tempRoomNode[a][b].Segments, (sfVector2f) { k*SIZE_CASE + offset, l*SIZE_CASE + offset }, (sfVector2f) { k*SIZE_CASE - offset, l*SIZE_CASE + offset });
					sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P1, colLight, (sfVector2f) { 0 } });
					sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P2, colLight, (sfVector2f) { 0 } });
				}
				if (IsEqualColor(ruSegment, colLight))
				{
					AddSegmentsLight(&tempRoomNode[a][b].Segments, (sfVector2f) { k*SIZE_CASE - offset, l*SIZE_CASE - offset }, (sfVector2f) { k*SIZE_CASE + offset, l*SIZE_CASE - offset });
					sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P1, colLight, (sfVector2f) { 0 } });
					sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P2, colLight, (sfVector2f) { 0 } });

					AddSegmentsLight(&tempRoomNode[a][b].Segments, (sfVector2f) { k*SIZE_CASE + offset, l*SIZE_CASE - offset }, (sfVector2f) { k*SIZE_CASE + offset, l*SIZE_CASE + offset });
					sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P1, colLight, (sfVector2f) { 0 } });
					sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P2, colLight, (sfVector2f) { 0 } });

				}
				if (IsEqualColor(rulSegment, colLight))
				{
					AddSegmentsLight(&tempRoomNode[a][b].Segments, (sfVector2f) { k*SIZE_CASE - offset, l*SIZE_CASE - offset }, (sfVector2f) { k*SIZE_CASE - offset, l*SIZE_CASE + offset });
					sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P1, colLight, (sfVector2f) { 0 } });
					sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P2, colLight, (sfVector2f) { 0 } });

					AddSegmentsLight(&tempRoomNode[a][b].Segments, (sfVector2f) { k*SIZE_CASE - offset, l*SIZE_CASE - offset }, (sfVector2f) { k*SIZE_CASE + offset, l*SIZE_CASE - offset });
					sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P1, colLight, (sfVector2f) { 0 } });
					sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P2, colLight, (sfVector2f) { 0 } });

					AddSegmentsLight(&tempRoomNode[a][b].Segments, (sfVector2f) { k*SIZE_CASE + offset, l*SIZE_CASE - offset }, (sfVector2f) { k*SIZE_CASE + offset, l*SIZE_CASE + offset });
					sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P1, colLight, (sfVector2f) { 0 } });
					sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P2, colLight, (sfVector2f) { 0 } });

				}
				if (IsEqualColor(ulSegment, colLight))
				{
					AddSegmentsLight(&tempRoomNode[a][b].Segments, (sfVector2f) { k*SIZE_CASE - offset, l*SIZE_CASE - offset }, (sfVector2f) { k*SIZE_CASE - offset, l*SIZE_CASE + offset });
					sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P1, colLight, (sfVector2f) { 0 } });
					sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P2, colLight, (sfVector2f) { 0 } });

					AddSegmentsLight(&tempRoomNode[a][b].Segments, (sfVector2f) { k*SIZE_CASE - offset, l*SIZE_CASE - offset }, (sfVector2f) { k*SIZE_CASE + offset, l*SIZE_CASE - offset });
					sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P1, colLight, (sfVector2f) { 0 } });
					sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P2, colLight, (sfVector2f) { 0 } });
				}
				if (IsEqualColor(uldSegment, colLight))
				{
					AddSegmentsLight(&tempRoomNode[a][b].Segments, (sfVector2f) { k*SIZE_CASE - offset, l*SIZE_CASE - offset }, (sfVector2f) { k*SIZE_CASE - offset, l*SIZE_CASE + offset });
					sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P1, colLight, (sfVector2f) { 0 } });
					sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P2, colLight, (sfVector2f) { 0 } });

					AddSegmentsLight(&tempRoomNode[a][b].Segments, (sfVector2f) { k*SIZE_CASE - offset, l*SIZE_CASE - offset }, (sfVector2f) { k*SIZE_CASE + offset, l*SIZE_CASE - offset });
					sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P1, colLight, (sfVector2f) { 0 } });
					sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P2, colLight, (sfVector2f) { 0 } });

					AddSegmentsLight(&tempRoomNode[a][b].Segments, (sfVector2f) { k*SIZE_CASE + offset, l*SIZE_CASE + offset }, (sfVector2f) { k*SIZE_CASE - offset, l*SIZE_CASE + offset });
					sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P1, colLight, (sfVector2f) { 0 } });
					sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P2, colLight, (sfVector2f) { 0 } });
				}
				if (IsEqualColor(lrSegment, colLight))
				{
					AddSegmentsLight(&tempRoomNode[a][b].Segments, (sfVector2f) { k*SIZE_CASE - offset, l*SIZE_CASE - offset }, (sfVector2f) { k*SIZE_CASE - offset, l*SIZE_CASE + offset });
					sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P1, colLight, (sfVector2f) { 0 } });
					sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P2, colLight, (sfVector2f) { 0 } });

					AddSegmentsLight(&tempRoomNode[a][b].Segments, (sfVector2f) { k*SIZE_CASE + offset, l*SIZE_CASE - offset }, (sfVector2f) { k*SIZE_CASE + offset, l*SIZE_CASE + offset });
					sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P1, colLight, (sfVector2f) { 0 } });
					sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P2, colLight, (sfVector2f) { 0 } });
				}
				if (IsEqualColor(udSegment, colLight))
				{
					AddSegmentsLight(&tempRoomNode[a][b].Segments, (sfVector2f) { k*SIZE_CASE - offset, l*SIZE_CASE - offset }, (sfVector2f) { k*SIZE_CASE + offset, l*SIZE_CASE - offset });
					sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P1, colLight, (sfVector2f) { 0 } });
					sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P2, colLight, (sfVector2f) { 0 } });

					AddSegmentsLight(&tempRoomNode[a][b].Segments, (sfVector2f) { k*SIZE_CASE + offset, l*SIZE_CASE + offset }, (sfVector2f) { k*SIZE_CASE - offset, l*SIZE_CASE + offset });
					sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P1, colLight, (sfVector2f) { 0 } });
					sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P2, colLight, (sfVector2f) { 0 } });
				}


			}
			b++;
		}
		a++;
		b = 0;
	}
}

void GenerateMapLightCorridor(Node** tempRoomNode, const int index, const int a, const int b, const float k, const float l, const sfImage** MapLight)
{
	float offset = 32;
	//printf(" %d a %d b %d id\n", a, b, dungeonGraphicData.room[i].uniqueID);
	sfColor colLight = sfImage_getPixel(MapLight[index], a, b);
	InitSegmentsLight(&tempRoomNode[a][b].Segments);

	if (IsEqualColor(lSegment, colLight))
	{

		AddSegmentsLight(&tempRoomNode[a][b].Segments, (sfVector2f) { k*SIZE_CASE - offset, l*SIZE_CASE - offset }, (sfVector2f) { k*SIZE_CASE - offset, l*SIZE_CASE + offset });
		sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P1, colLight, (sfVector2f) { 0 } });
		sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P2, colLight, (sfVector2f) { 0 } });
	}
	if (IsEqualColor(rSegment, colLight))
	{
		AddSegmentsLight(&tempRoomNode[a][b].Segments, (sfVector2f) { k*SIZE_CASE + offset, l*SIZE_CASE - offset }, (sfVector2f) { k*SIZE_CASE + offset, l*SIZE_CASE + offset });
		sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P1, colLight, (sfVector2f) { 0 } });
		sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P2, colLight, (sfVector2f) { 0 } });
	}
	if (IsEqualColor(dSegment, colLight))
	{
		AddSegmentsLight(&tempRoomNode[a][b].Segments, (sfVector2f) { k*SIZE_CASE + offset, l*SIZE_CASE + offset }, (sfVector2f) { k*SIZE_CASE - offset, l*SIZE_CASE + offset });
		sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P1, colLight, (sfVector2f) { 0 } });
		sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P2, colLight, (sfVector2f) { 0 } });
	}
	if (IsEqualColor(uSegment, colLight))
	{
		AddSegmentsLight(&tempRoomNode[a][b].Segments, (sfVector2f) { k*SIZE_CASE - offset, l*SIZE_CASE - offset }, (sfVector2f) { k*SIZE_CASE + offset, l*SIZE_CASE - offset });
		sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P1, colLight, (sfVector2f) { 0 } });
		sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P2, colLight, (sfVector2f) { 0 } });
	}
	if (IsEqualColor(ldSegment, colLight))
	{
		AddSegmentsLight(&tempRoomNode[a][b].Segments, (sfVector2f) { k*SIZE_CASE - offset, l*SIZE_CASE - offset }, (sfVector2f) { k*SIZE_CASE - offset, l*SIZE_CASE + offset });
		sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P1, colLight, (sfVector2f) { 0 } });
		sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P2, colLight, (sfVector2f) { 0 } });

		AddSegmentsLight(&tempRoomNode[a][b].Segments, (sfVector2f) { k*SIZE_CASE + offset, l*SIZE_CASE + offset }, (sfVector2f) { k*SIZE_CASE - offset, l*SIZE_CASE + offset });
		sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P1, colLight, (sfVector2f) { 0 } });
		sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P2, colLight, (sfVector2f) { 0 } });
	}
	if (IsEqualColor(ldrSegment, colLight))
	{
		AddSegmentsLight(&tempRoomNode[a][b].Segments, (sfVector2f) { k*SIZE_CASE - offset, l*SIZE_CASE - offset }, (sfVector2f) { k*SIZE_CASE - offset, l*SIZE_CASE + offset });
		sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P1, colLight, (sfVector2f) { 0 } });
		sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P2, colLight, (sfVector2f) { 0 } });

		AddSegmentsLight(&tempRoomNode[a][b].Segments, (sfVector2f) { k*SIZE_CASE + offset, l*SIZE_CASE + offset }, (sfVector2f) { k*SIZE_CASE - offset, l*SIZE_CASE + offset });
		sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P1, colLight, (sfVector2f) { 0 } });
		sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P2, colLight, (sfVector2f) { 0 } });

		AddSegmentsLight(&tempRoomNode[a][b].Segments, (sfVector2f) { k*SIZE_CASE + offset, l*SIZE_CASE - offset }, (sfVector2f) { k*SIZE_CASE + offset, l*SIZE_CASE + offset });
		sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P1, colLight, (sfVector2f) { 0 } });
		sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P2, colLight, (sfVector2f) { 0 } });
	}
	if (IsEqualColor(ldruSegment, colLight))
	{
		AddSegmentsLight(&tempRoomNode[a][b].Segments, (sfVector2f) { k*SIZE_CASE - offset, l*SIZE_CASE - offset }, (sfVector2f) { k*SIZE_CASE - offset, l*SIZE_CASE + offset });
		sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P1, colLight, (sfVector2f) { 0 } });
		sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P2, colLight, (sfVector2f) { 0 } });

		AddSegmentsLight(&tempRoomNode[a][b].Segments, (sfVector2f) { k*SIZE_CASE - offset, l*SIZE_CASE - offset }, (sfVector2f) { k*SIZE_CASE + offset, l*SIZE_CASE - offset });
		sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P1, colLight, (sfVector2f) { 0 } });
		sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P2, colLight, (sfVector2f) { 0 } });

		AddSegmentsLight(&tempRoomNode[a][b].Segments, (sfVector2f) { k*SIZE_CASE + offset, l*SIZE_CASE - offset }, (sfVector2f) { k*SIZE_CASE + offset, l*SIZE_CASE + offset });
		sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P1, colLight, (sfVector2f) { 0 } });
		sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P2, colLight, (sfVector2f) { 0 } });

		AddSegmentsLight(&tempRoomNode[a][b].Segments, (sfVector2f) { k*SIZE_CASE + offset, l*SIZE_CASE + offset }, (sfVector2f) { k*SIZE_CASE - offset, l*SIZE_CASE + offset });
		sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P1, colLight, (sfVector2f) { 0 } });
		sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P2, colLight, (sfVector2f) { 0 } });

	}
	if (IsEqualColor(drSegment, colLight))
	{
		AddSegmentsLight(&tempRoomNode[a][b].Segments, (sfVector2f) { k*SIZE_CASE + offset, l*SIZE_CASE - offset }, (sfVector2f) { k*SIZE_CASE + offset, l*SIZE_CASE + offset });
		sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P1, colLight, (sfVector2f) { 0 } });
		sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P2, colLight, (sfVector2f) { 0 } });

		AddSegmentsLight(&tempRoomNode[a][b].Segments, (sfVector2f) { k*SIZE_CASE + offset, l*SIZE_CASE + offset }, (sfVector2f) { k*SIZE_CASE - offset, l*SIZE_CASE + offset });
		sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P1, colLight, (sfVector2f) { 0 } });
		sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P2, colLight, (sfVector2f) { 0 } });


	}
	if (IsEqualColor(druSegment, colLight))
	{
		AddSegmentsLight(&tempRoomNode[a][b].Segments, (sfVector2f) { k*SIZE_CASE - offset, l*SIZE_CASE - offset }, (sfVector2f) { k*SIZE_CASE + offset, l*SIZE_CASE - offset });
		sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P1, colLight, (sfVector2f) { 0 } });
		sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P2, colLight, (sfVector2f) { 0 } });

		AddSegmentsLight(&tempRoomNode[a][b].Segments, (sfVector2f) { k*SIZE_CASE + offset, l*SIZE_CASE - offset }, (sfVector2f) { k*SIZE_CASE + offset, l*SIZE_CASE + offset });
		sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P1, colLight, (sfVector2f) { 0 } });
		sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P2, colLight, (sfVector2f) { 0 } });

		AddSegmentsLight(&tempRoomNode[a][b].Segments, (sfVector2f) { k*SIZE_CASE + offset, l*SIZE_CASE + offset }, (sfVector2f) { k*SIZE_CASE - offset, l*SIZE_CASE + offset });
		sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P1, colLight, (sfVector2f) { 0 } });
		sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P2, colLight, (sfVector2f) { 0 } });
	}
	if (IsEqualColor(ruSegment, colLight))
	{
		AddSegmentsLight(&tempRoomNode[a][b].Segments, (sfVector2f) { k*SIZE_CASE - offset, l*SIZE_CASE - offset }, (sfVector2f) { k*SIZE_CASE + offset, l*SIZE_CASE - offset });
		sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P1, colLight, (sfVector2f) { 0 } });
		sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P2, colLight, (sfVector2f) { 0 } });

		AddSegmentsLight(&tempRoomNode[a][b].Segments, (sfVector2f) { k*SIZE_CASE + offset, l*SIZE_CASE - offset }, (sfVector2f) { k*SIZE_CASE + offset, l*SIZE_CASE + offset });
		sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P1, colLight, (sfVector2f) { 0 } });
		sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P2, colLight, (sfVector2f) { 0 } });

	}
	if (IsEqualColor(rulSegment, colLight))
	{
		AddSegmentsLight(&tempRoomNode[a][b].Segments, (sfVector2f) { k*SIZE_CASE - offset, l*SIZE_CASE - offset }, (sfVector2f) { k*SIZE_CASE - offset, l*SIZE_CASE + offset });
		sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P1, colLight, (sfVector2f) { 0 } });
		sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P2, colLight, (sfVector2f) { 0 } });

		AddSegmentsLight(&tempRoomNode[a][b].Segments, (sfVector2f) { k*SIZE_CASE - offset, l*SIZE_CASE - offset }, (sfVector2f) { k*SIZE_CASE + offset, l*SIZE_CASE - offset });
		sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P1, colLight, (sfVector2f) { 0 } });
		sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P2, colLight, (sfVector2f) { 0 } });

		AddSegmentsLight(&tempRoomNode[a][b].Segments, (sfVector2f) { k*SIZE_CASE + offset, l*SIZE_CASE - offset }, (sfVector2f) { k*SIZE_CASE + offset, l*SIZE_CASE + offset });
		sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P1, colLight, (sfVector2f) { 0 } });
		sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P2, colLight, (sfVector2f) { 0 } });

	}
	if (IsEqualColor(ulSegment, colLight))
	{
		AddSegmentsLight(&tempRoomNode[a][b].Segments, (sfVector2f) { k*SIZE_CASE - offset, l*SIZE_CASE - offset }, (sfVector2f) { k*SIZE_CASE - offset, l*SIZE_CASE + offset });
		sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P1, colLight, (sfVector2f) { 0 } });
		sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P2, colLight, (sfVector2f) { 0 } });

		AddSegmentsLight(&tempRoomNode[a][b].Segments, (sfVector2f) { k*SIZE_CASE - offset, l*SIZE_CASE - offset }, (sfVector2f) { k*SIZE_CASE + offset, l*SIZE_CASE - offset });
		sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P1, colLight, (sfVector2f) { 0 } });
		sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P2, colLight, (sfVector2f) { 0 } });
	}
	if (IsEqualColor(uldSegment, colLight))
	{
		AddSegmentsLight(&tempRoomNode[a][b].Segments, (sfVector2f) { k*SIZE_CASE - offset, l*SIZE_CASE - offset }, (sfVector2f) { k*SIZE_CASE - offset, l*SIZE_CASE + offset });
		sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P1, colLight, (sfVector2f) { 0 } });
		sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P2, colLight, (sfVector2f) { 0 } });

		AddSegmentsLight(&tempRoomNode[a][b].Segments, (sfVector2f) { k*SIZE_CASE - offset, l*SIZE_CASE - offset }, (sfVector2f) { k*SIZE_CASE + offset, l*SIZE_CASE - offset });
		sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P1, colLight, (sfVector2f) { 0 } });
		sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P2, colLight, (sfVector2f) { 0 } });

		AddSegmentsLight(&tempRoomNode[a][b].Segments, (sfVector2f) { k*SIZE_CASE + offset, l*SIZE_CASE + offset }, (sfVector2f) { k*SIZE_CASE - offset, l*SIZE_CASE + offset });
		sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P1, colLight, (sfVector2f) { 0 } });
		sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P2, colLight, (sfVector2f) { 0 } });
	}
	if (IsEqualColor(lrSegment, colLight))
	{
		AddSegmentsLight(&tempRoomNode[a][b].Segments, (sfVector2f) { k*SIZE_CASE - offset, l*SIZE_CASE - offset }, (sfVector2f) { k*SIZE_CASE - offset, l*SIZE_CASE + offset });
		sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P1, colLight, (sfVector2f) { 0 } });
		sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P2, colLight, (sfVector2f) { 0 } });

		AddSegmentsLight(&tempRoomNode[a][b].Segments, (sfVector2f) { k*SIZE_CASE + offset, l*SIZE_CASE - offset }, (sfVector2f) { k*SIZE_CASE + offset, l*SIZE_CASE + offset });
		sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P1, colLight, (sfVector2f) { 0 } });
		sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P2, colLight, (sfVector2f) { 0 } });
	}
	if (IsEqualColor(udSegment, colLight))
	{
		AddSegmentsLight(&tempRoomNode[a][b].Segments, (sfVector2f) { k*SIZE_CASE - offset, l*SIZE_CASE - offset }, (sfVector2f) { k*SIZE_CASE + offset, l*SIZE_CASE - offset });
		sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P1, colLight, (sfVector2f) { 0 } });
		sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P2, colLight, (sfVector2f) { 0 } });

		AddSegmentsLight(&tempRoomNode[a][b].Segments, (sfVector2f) { k*SIZE_CASE + offset, l*SIZE_CASE + offset }, (sfVector2f) { k*SIZE_CASE - offset, l*SIZE_CASE + offset });
		sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P1, colLight, (sfVector2f) { 0 } });
		sfVertexArray_append(Supp, (sfVertex) { tempRoomNode[a][b].Segments.Array[tempRoomNode[a][b].Segments.numberSegment - 1].P2, colLight, (sfVector2f) { 0 } });
	}
}

void PlaceLightRoom(DungeonGameData* dungeonGameData, const int i, DungeonGraphicData *dungeonGraphicData)
{
	//all rooms lights
	float width = (dungeonGameData->room[i].Width / 2)* SIZE_CASE;
	float height = (dungeonGameData->room[i].Height / 2) * SIZE_CASE;
	if (dungeonGameData->room[i].RoomId != 0)
		AddPointLight(STRONGREDLIGHT, (sfVector2f) { dungeonGameData->room[i].Position.x * SIZE_CASE - width + SIZE_CASE * 1.1f, dungeonGameData->room[i].Position.y* SIZE_CASE - height + SIZE_CASE * 1.1f }, dungeonGameData->room[i].RoomNode, 1);

	AddPointLight(STRONGREDLIGHT, (sfVector2f) { dungeonGameData->room[i].Position.x * SIZE_CASE + width - SIZE_CASE * 1.1f, dungeonGameData->room[i].Position.y* SIZE_CASE - height + SIZE_CASE * 1.1f }, dungeonGameData->room[i].RoomNode, 1);
	AddPointLight(STRONGREDLIGHT, (sfVector2f) { dungeonGameData->room[i].Position.x * SIZE_CASE - width + SIZE_CASE * 1.1f, dungeonGameData->room[i].Position.y* SIZE_CASE + height - SIZE_CASE * 1.1f }, dungeonGameData->room[i].RoomNode, 1);
	AddPointLight(STRONGREDLIGHT, (sfVector2f) { dungeonGameData->room[i].Position.x * SIZE_CASE + width - SIZE_CASE * 1.1f, dungeonGameData->room[i].Position.y* SIZE_CASE + height - SIZE_CASE * 1.1f }, dungeonGameData->room[i].RoomNode, 1);



	if (dungeonGameData->room[i].RoomId == 1)
	{
		AddPointLight(STRONGREDLIGHT, (sfVector2f) { dungeonGameData->room[i].Position.x * SIZE_CASE - 4 * SIZE_CASE, dungeonGameData->room[i].Position.y* SIZE_CASE - SIZE_CASE * 6 }, dungeonGameData->room[i].RoomNode, 1);
		AddPointLight(STRONGREDLIGHT, (sfVector2f) { dungeonGameData->room[i].Position.x * SIZE_CASE + 4 * SIZE_CASE, dungeonGameData->room[i].Position.y* SIZE_CASE - SIZE_CASE * 6 }, dungeonGameData->room[i].RoomNode, 1);

		AddPointLight(STRONGREDLIGHT, (sfVector2f) { dungeonGameData->room[i].Position.x * SIZE_CASE - 4 * SIZE_CASE, dungeonGameData->room[i].Position.y* SIZE_CASE + SIZE_CASE * 9 }, dungeonGameData->room[i].RoomNode, 1);
		AddPointLight(STRONGREDLIGHT, (sfVector2f) { dungeonGameData->room[i].Position.x * SIZE_CASE + 4 * SIZE_CASE, dungeonGameData->room[i].Position.y* SIZE_CASE + SIZE_CASE * 9 }, dungeonGameData->room[i].RoomNode, 1);

	}
	if (dungeonGameData->room[i].RoomId == 2)
	{
		AddPointLight(STRONGREDLIGHT, (sfVector2f) { dungeonGameData->room[i].Position.x * SIZE_CASE - 9 * SIZE_CASE, dungeonGameData->room[i].Position.y* SIZE_CASE - SIZE_CASE * 8 }, dungeonGameData->room[i].RoomNode, 1);
		AddPointLight(STRONGREDLIGHT, (sfVector2f) { dungeonGameData->room[i].Position.x * SIZE_CASE + 1 * SIZE_CASE, dungeonGameData->room[i].Position.y* SIZE_CASE - SIZE_CASE * 8 }, dungeonGameData->room[i].RoomNode, 1);
		AddPointLight(STRONGREDLIGHT, (sfVector2f) { dungeonGameData->room[i].Position.x * SIZE_CASE + 11 * SIZE_CASE, dungeonGameData->room[i].Position.y* SIZE_CASE - SIZE_CASE * 8 }, dungeonGameData->room[i].RoomNode, 1);
		AddPointLight(STRONGREDLIGHT, (sfVector2f) { dungeonGameData->room[i].Position.x * SIZE_CASE - 9 * SIZE_CASE, dungeonGameData->room[i].Position.y* SIZE_CASE + SIZE_CASE * 7 }, dungeonGameData->room[i].RoomNode, 1);
		AddPointLight(STRONGREDLIGHT, (sfVector2f) { dungeonGameData->room[i].Position.x * SIZE_CASE + 1 * SIZE_CASE, dungeonGameData->room[i].Position.y* SIZE_CASE + SIZE_CASE * 7 }, dungeonGameData->room[i].RoomNode, 1);
		AddPointLight(STRONGREDLIGHT, (sfVector2f) { dungeonGameData->room[i].Position.x * SIZE_CASE + 11 * SIZE_CASE, dungeonGameData->room[i].Position.y* SIZE_CASE + SIZE_CASE * 7 }, dungeonGameData->room[i].RoomNode, 1);
	}
	if (dungeonGameData->room[i].RoomId == 3)
	{
		AddPointLight(STRONGREDLIGHT, (sfVector2f) { dungeonGameData->room[i].Position.x * SIZE_CASE - 1 * SIZE_CASE, dungeonGameData->room[i].Position.y* SIZE_CASE - SIZE_CASE * 10 }, dungeonGameData->room[i].RoomNode, 1);
		AddPointLight(STRONGREDLIGHT, (sfVector2f) { dungeonGameData->room[i].Position.x * SIZE_CASE + 10 * SIZE_CASE, dungeonGameData->room[i].Position.y* SIZE_CASE - SIZE_CASE * 10 }, dungeonGameData->room[i].RoomNode, 1);
		AddPointLight(STRONGREDLIGHT, (sfVector2f) { dungeonGameData->room[i].Position.x * SIZE_CASE + 10 * SIZE_CASE, dungeonGameData->room[i].Position.y* SIZE_CASE - SIZE_CASE * 3 }, dungeonGameData->room[i].RoomNode, 1);
		AddPointLight(STRONGREDLIGHT, (sfVector2f) { dungeonGameData->room[i].Position.x * SIZE_CASE + 10 * SIZE_CASE, dungeonGameData->room[i].Position.y* SIZE_CASE + SIZE_CASE * 4 }, dungeonGameData->room[i].RoomNode, 1);
		AddPointLight(STRONGREDLIGHT, (sfVector2f) { dungeonGameData->room[i].Position.x * SIZE_CASE + 10 * SIZE_CASE, dungeonGameData->room[i].Position.y* SIZE_CASE + SIZE_CASE * 11 }, dungeonGameData->room[i].RoomNode, 1);

		AddPointLight(STRONGREDLIGHT, (sfVector2f) { dungeonGameData->room[i].Position.x * SIZE_CASE - 8.5f * SIZE_CASE, dungeonGameData->room[i].Position.y* SIZE_CASE - SIZE_CASE * 1 }, dungeonGameData->room[i].RoomNode, 1);
		AddPointLight(STRONGREDLIGHT, (sfVector2f) { dungeonGameData->room[i].Position.x * SIZE_CASE, dungeonGameData->room[i].Position.y* SIZE_CASE + SIZE_CASE * 4 }, dungeonGameData->room[i].RoomNode, 1);
		AddPointLight(STRONGREDLIGHT, (sfVector2f) { dungeonGameData->room[i].Position.x * SIZE_CASE - 5.5f*SIZE_CASE, dungeonGameData->room[i].Position.y* SIZE_CASE + SIZE_CASE * 13 }, dungeonGameData->room[i].RoomNode, 1);
		AddPointLight(STRONGREDLIGHT, (sfVector2f) { dungeonGameData->room[i].Position.x * SIZE_CASE - 12.5f*SIZE_CASE, dungeonGameData->room[i].Position.y* SIZE_CASE + SIZE_CASE * 13 }, dungeonGameData->room[i].RoomNode, 1);
	}

	if (dungeonGameData->room[i].RoomId == 4)
	{
		AddPointLight(STRONGREDLIGHT, (sfVector2f) { dungeonGameData->room[i].Position.x * SIZE_CASE - 12.5f * SIZE_CASE, dungeonGameData->room[i].Position.y* SIZE_CASE - SIZE_CASE * 1 }, dungeonGameData->room[i].RoomNode, 1);
		AddPointLight(STRONGREDLIGHT, (sfVector2f) { dungeonGameData->room[i].Position.x * SIZE_CASE - 3.5f * SIZE_CASE, dungeonGameData->room[i].Position.y* SIZE_CASE - SIZE_CASE * 1 }, dungeonGameData->room[i].RoomNode, 1);
		AddPointLight(STRONGREDLIGHT, (sfVector2f) { dungeonGameData->room[i].Position.x * SIZE_CASE + 3.5f * SIZE_CASE, dungeonGameData->room[i].Position.y* SIZE_CASE - SIZE_CASE * 1 }, dungeonGameData->room[i].RoomNode, 1);
		AddPointLight(STRONGREDLIGHT, (sfVector2f) { dungeonGameData->room[i].Position.x * SIZE_CASE + 12.5f * SIZE_CASE, dungeonGameData->room[i].Position.y* SIZE_CASE - SIZE_CASE * 1 }, dungeonGameData->room[i].RoomNode, 1);

		AddPointLight(STRONGREDLIGHT, (sfVector2f) { dungeonGameData->room[i].Position.x * SIZE_CASE - 12.5f * SIZE_CASE, dungeonGameData->room[i].Position.y* SIZE_CASE + SIZE_CASE * 13 }, dungeonGameData->room[i].RoomNode, 1);
		AddPointLight(STRONGREDLIGHT, (sfVector2f) { dungeonGameData->room[i].Position.x * SIZE_CASE - 3.5f * SIZE_CASE, dungeonGameData->room[i].Position.y* SIZE_CASE + SIZE_CASE * 13 }, dungeonGameData->room[i].RoomNode, 1);
		AddPointLight(STRONGREDLIGHT, (sfVector2f) { dungeonGameData->room[i].Position.x * SIZE_CASE + 3.5f * SIZE_CASE, dungeonGameData->room[i].Position.y* SIZE_CASE + SIZE_CASE * 13 }, dungeonGameData->room[i].RoomNode, 1);
		AddPointLight(STRONGREDLIGHT, (sfVector2f) { dungeonGameData->room[i].Position.x * SIZE_CASE + 12.5f * SIZE_CASE, dungeonGameData->room[i].Position.y* SIZE_CASE + SIZE_CASE * 13 }, dungeonGameData->room[i].RoomNode, 1);
	}

	if (dungeonGameData->room[i].RoomId == 5)
	{
		AddPointLight(CANDLELIGHT, (sfVector2f) { dungeonGameData->room[i].Position.x * SIZE_CASE - 244, dungeonGameData->room[i].Position.y* SIZE_CASE - 546 }, dungeonGameData->room[i].RoomNode, 2);
		AddPointLight(CANDLELIGHT, (sfVector2f) { dungeonGameData->room[i].Position.x * SIZE_CASE - 236, dungeonGameData->room[i].Position.y* SIZE_CASE + 722 }, dungeonGameData->room[i].RoomNode, 2);
		AddPointLight(CANDLELIGHT, (sfVector2f) { dungeonGameData->room[i].Position.x * SIZE_CASE + 616, dungeonGameData->room[i].Position.y* SIZE_CASE + 160 }, dungeonGameData->room[i].RoomNode, 2);
		AddPointLight(CANDLELIGHT, (sfVector2f) { dungeonGameData->room[i].Position.x * SIZE_CASE + 712, dungeonGameData->room[i].Position.y* SIZE_CASE + 790 }, dungeonGameData->room[i].RoomNode, 2);
	}

	if (dungeonGameData->room[i].RoomId == 6)
	{
		AddPointLight(CANDLELIGHT, (sfVector2f) { dungeonGameData->room[i].Position.x * SIZE_CASE - 268, dungeonGameData->room[i].Position.y* SIZE_CASE - 582 }, dungeonGameData->room[i].RoomNode, 2);
		AddPointLight(CANDLELIGHT, (sfVector2f) { dungeonGameData->room[i].Position.x * SIZE_CASE + 317, dungeonGameData->room[i].Position.y* SIZE_CASE - 486 }, dungeonGameData->room[i].RoomNode, 2);

		AddPointLight(STRONGREDLIGHT, (sfVector2f) { dungeonGameData->room[i].Position.x * SIZE_CASE - 11.5f* SIZE_CASE, dungeonGameData->room[i].Position.y* SIZE_CASE + 13 * SIZE_CASE }, dungeonGameData->room[i].RoomNode, 1);
		AddPointLight(STRONGREDLIGHT, (sfVector2f) { dungeonGameData->room[i].Position.x * SIZE_CASE + 11.5f* SIZE_CASE, dungeonGameData->room[i].Position.y* SIZE_CASE + 13 * SIZE_CASE }, dungeonGameData->room[i].RoomNode, 1);
	}

	if (dungeonGameData->room[i].RoomId == 7)
	{
		AddPointLight(CANDLELIGHT, (sfVector2f) { dungeonGameData->room[i].Position.x * SIZE_CASE + 305, dungeonGameData->room[i].Position.y* SIZE_CASE - 638 }, dungeonGameData->room[i].RoomNode, 2);
	}

	if (dungeonGameData->room[i].RoomId == 8)
	{
		AddPointLight(CANDLELIGHT, (sfVector2f) { dungeonGameData->room[i].Position.x * SIZE_CASE + 126, dungeonGameData->room[i].Position.y* SIZE_CASE - 405 }, dungeonGameData->room[i].RoomNode, 2);
		AddPointLight(CANDLELIGHT, (sfVector2f) { dungeonGameData->room[i].Position.x * SIZE_CASE - 303, dungeonGameData->room[i].Position.y* SIZE_CASE + 188 }, dungeonGameData->room[i].RoomNode, 2);
		AddPointLight(CANDLELIGHT, (sfVector2f) { dungeonGameData->room[i].Position.x * SIZE_CASE - 220, dungeonGameData->room[i].Position.y* SIZE_CASE + 239 }, dungeonGameData->room[i].RoomNode, 2);
		AddPointLight(CANDLELIGHT, (sfVector2f) { dungeonGameData->room[i].Position.x * SIZE_CASE + 124, dungeonGameData->room[i].Position.y* SIZE_CASE + 544 }, dungeonGameData->room[i].RoomNode, 2);
		AddPointLight(CANDLELIGHT, (sfVector2f) { dungeonGameData->room[i].Position.x * SIZE_CASE + 199, dungeonGameData->room[i].Position.y* SIZE_CASE + 545 }, dungeonGameData->room[i].RoomNode, 2);
	}
}

void GetGameDataFromGraphicData(DungeonGameData *dungeonGameData, DungeonGraphicData* dungeonGraphicData, const DungeonRawData dungeonRawData, const DungeonId ID)
{
	int countNumberOfdoors = 0;
	int typeOfDungeon = GetTypeOfDungeon();

	for (RoomList *tempRoom = dungeonRawData.FirstRoom; tempRoom != NULL; tempRoom = tempRoom->Next)
	{
		countNumberOfdoors += tempRoom->rm.nbDoors;
	}

	countNumberOfdoors /= 2;

	int indexToAdd = 0;

	Supp = sfVertexArray_create();
	sfVertexArray_setPrimitiveType(Supp, sfLines);
	Node** SaveForNDdoors = (Node**)calloc(countNumberOfdoors, sizeof(Node*));

	//Load MapCollision of Corridor;

	sfImage** CorridorColl = NULL;

	if (ID == slimeDungeon || ID == beginningDungeon)
	{
		CorridorColl = (sfImage**)calloc(8, sizeof(sfImage*));
		CorridorColl[0] = sfImage_createFromFile("Ressources/Sprites/Environment/Dungeon1/Corridor/CorridorCollision/horizontal_corridor.png");
		CorridorColl[1] = sfImage_createFromFile("Ressources/Sprites/Environment/Dungeon1/Corridor/CorridorCollision/vertical_corridor.png");
		CorridorColl[2] = sfImage_createFromFile("Ressources/Sprites/Environment/Dungeon1/Corridor/CorridorCollision/EST-NORD_corri.png");
		CorridorColl[3] = sfImage_createFromFile("Ressources/Sprites/Environment/Dungeon1/Corridor/CorridorCollision/NORD-OUEST_corri.png");
		CorridorColl[4] = sfImage_createFromFile("Ressources/Sprites/Environment/Dungeon1/Corridor/CorridorCollision/OUEST-SUD_corri.png");
		CorridorColl[5] = sfImage_createFromFile("Ressources/Sprites/Environment/Dungeon1/Corridor/CorridorCollision/EST-SUD_corri.png");
		CorridorColl[6] = sfImage_createFromFile("Ressources/Sprites/Environment/Dungeon1/Corridor/CorridorCollision/horizontalSlice_corridor.png");
		CorridorColl[7] = sfImage_createFromFile("Ressources/Sprites/Environment/Dungeon1/Corridor/CorridorCollision/verticalSlice_corridor.png");
	}

	sfImage** CorridorLight = NULL;
	if (ID == slimeDungeon || ID == beginningDungeon)
	{
		CorridorLight = (sfImage**)calloc(8, sizeof(sfImage*));
		CorridorLight[0] = sfImage_createFromFile("Ressources/Sprites/Environment/Dungeon1/Corridor/CorridorCollision/horizontal_corridor_light.png");
		CorridorLight[1] = sfImage_createFromFile("Ressources/Sprites/Environment/Dungeon1/Corridor/CorridorCollision/vertical_corridor_light.png");
		CorridorLight[2] = sfImage_createFromFile("Ressources/Sprites/Environment/Dungeon1/Corridor/CorridorCollision/EST-NORD_corri_light.png");
		CorridorLight[3] = sfImage_createFromFile("Ressources/Sprites/Environment/Dungeon1/Corridor/CorridorCollision/NORD-OUEST_corri_light.png");
		CorridorLight[4] = sfImage_createFromFile("Ressources/Sprites/Environment/Dungeon1/Corridor/CorridorCollision/OUEST-SUD_corri_light.png");
		CorridorLight[5] = sfImage_createFromFile("Ressources/Sprites/Environment/Dungeon1/Corridor/CorridorCollision/EST-SUD_corri_light.png");
		CorridorLight[6] = sfImage_createFromFile("Ressources/Sprites/Environment/Dungeon1/Corridor/CorridorCollision/horizontalSlice_corridor_light.png");
		CorridorLight[7] = sfImage_createFromFile("Ressources/Sprites/Environment/Dungeon1/Corridor/CorridorCollision/verticalSlice_corridor_light.png");
	}

	sfImage** MapColl = NULL;

	if (ID == slimeDungeon || ID == beginningDungeon)
	{
		MapColl = (sfImage**)calloc(11, sizeof(sfImage*));
		MapColl[0] = sfImage_createFromFile("Ressources/Sprites/Environment/Dungeon1/Rooms/ID_0/spawn_collision.png");
		MapColl[1] = sfImage_createFromFile("Ressources/Sprites/Environment/Dungeon1/Rooms/ID_1/Dortoir_Collision.png");
		MapColl[2] = sfImage_createFromFile("Ressources/Sprites/Environment/Dungeon1/Rooms/ID_2/Dortoir_Collision.png");
		MapColl[3] = sfImage_createFromFile("Ressources/Sprites/Environment/Dungeon1/Rooms/ID_3/Entrepot_collision.png");
		MapColl[4] = sfImage_createFromFile("Ressources/Sprites/Environment/Dungeon1/Rooms/ID_4/Entrepot_Collision.png");
		MapColl[5] = sfImage_createFromFile("Ressources/Sprites/Environment/Dungeon1/Rooms/ID_5/Cantine_Collision.png");
		MapColl[6] = sfImage_createFromFile("Ressources/Sprites/Environment/Dungeon1/Rooms/ID_6/Cantine_02_Collision.png");
		MapColl[7] = sfImage_createFromFile("Ressources/Sprites/Environment/Dungeon1/Rooms/ID_7/Bibliotheque_collision.png");
		MapColl[8] = sfImage_createFromFile("Ressources/Sprites/Environment/Dungeon1/Rooms/ID_8/bibliotheque_collision.png");
		MapColl[9] = sfImage_createFromFile("Ressources/Sprites/Environment/Dungeon1/Rooms/ID_9/MiniBoss_Collision.png");
		MapColl[10] = sfImage_createFromFile("Ressources/Sprites/Environment/Dungeon1/Rooms/ID_10/Boss_Collision.png");
	}
	else if (ID == elemDungeon)
	{
		MapColl = (sfImage**)calloc(11, sizeof(sfImage*));
		MapColl[0] = sfImage_createFromFile("Ressources/Sprites/Environment/Dungeon2/Rooms/ID_0/Spawn_Collision.png");
		MapColl[1] = sfImage_createFromFile("Ressources/Sprites/Environment/Dungeon2/Rooms/ID_1/Room_01_collision.png");
		MapColl[2] = sfImage_createFromFile("Ressources/Sprites/Environment/Dungeon2/Rooms/ID_2/Room_02_collision.png");
		MapColl[3] = sfImage_createFromFile("Ressources/Sprites/Environment/Dungeon2/Rooms/ID_3/Room_03_collision.png");
		MapColl[4] = sfImage_createFromFile("Ressources/Sprites/Environment/Dungeon2/Rooms/ID_4/Room_04_collision.png");
		MapColl[5] = sfImage_createFromFile("Ressources/Sprites/Environment/Dungeon2/Rooms/ID_5/Room_05_collision.png");
		MapColl[6] = sfImage_createFromFile("Ressources/Sprites/Environment/Dungeon2/Rooms/ID_6/Room_06_collision.png");
		MapColl[7] = sfImage_createFromFile("Ressources/Sprites/Environment/Dungeon2/Rooms/ID_7/Room_07_collision.png");
		MapColl[8] = sfImage_createFromFile("Ressources/Sprites/Environment/Dungeon2/Rooms/ID_8/Room_08_collision.png");
		MapColl[9] = sfImage_createFromFile("Ressources/Sprites/Environment/Dungeon2/Rooms/ID_9/Room_09_collision.png");
		MapColl[10] = sfImage_createFromFile("Ressources/Sprites/Environment/Dungeon2/Rooms/ID_10/Map_Boss_Elementaire_Collision.png");
	}
	else if (ID == razarDungeon)
	{
		MapColl = (sfImage**)calloc(1, sizeof(sfImage*));
		MapColl[0] = sfImage_createFromFile("Ressources/Sprites/Environment/Dungeon3/ID_0/RazarRoom_CollisionMap.png");
	}

	sfImage** MapLight = NULL;

	if (ID == slimeDungeon || ID == beginningDungeon)
	{
		MapLight = (sfImage**)calloc(11, sizeof(sfImage*));
		MapLight[0] = sfImage_createFromFile("Ressources/Sprites/Environment/Dungeon1/Rooms/ID_0/spawn_Light.png");
		MapLight[1] = sfImage_createFromFile("Ressources/Sprites/Environment/Dungeon1/Rooms/ID_1/Dortoir_Light.png");
		MapLight[2] = sfImage_createFromFile("Ressources/Sprites/Environment/Dungeon1/Rooms/ID_2/Dortoir_Light.png");
		MapLight[3] = sfImage_createFromFile("Ressources/Sprites/Environment/Dungeon1/Rooms/ID_3/Entrepot_Light.png");
		MapLight[4] = sfImage_createFromFile("Ressources/Sprites/Environment/Dungeon1/Rooms/ID_4/Entrepot_Light.png");
		MapLight[5] = sfImage_createFromFile("Ressources/Sprites/Environment/Dungeon1/Rooms/ID_5/Cantine_Light.png");
		MapLight[6] = sfImage_createFromFile("Ressources/Sprites/Environment/Dungeon1/Rooms/ID_6/Cantine_02_Light.png");
		MapLight[7] = sfImage_createFromFile("Ressources/Sprites/Environment/Dungeon1/Rooms/ID_7/Bibliotheque_Light.png");
		MapLight[8] = sfImage_createFromFile("Ressources/Sprites/Environment/Dungeon1/Rooms/ID_8/bibliotheque_Light.png");
		MapLight[9] = sfImage_createFromFile("Ressources/Sprites/Environment/Dungeon1/Rooms/ID_9/MiniBoss_Light.png");
		MapLight[10] = sfImage_createFromFile("Ressources/Sprites/Environment/Dungeon1/Rooms/ID_10/Boss_Light.png");
		InitPointer();
	}

	RoomList *tempRoom = dungeonRawData.FirstRoom;
	for (int i = 0; i < dungeonGameData->numberOfRoom; i++)
	{
		dungeonGameData->room[i].IsClear = sfFalse;
		dungeonGameData->room[i].doors = (DoorGameData*)calloc(tempRoom->rm.nbDoors, sizeof(DoorGameData));

		dungeonGameData->room[i].Position = tempRoom->rm.Position;
		dungeonGameData->room[i].Height = tempRoom->rm.Height;
		dungeonGameData->room[i].Width = tempRoom->rm.Width;

		dungeonGameData->room[i].nbDoors = (unsigned short)tempRoom->rm.nbDoors;

		for (int doorDir = 0; doorDir < dungeonGameData->room[i].nbDoors; doorDir++)
		{
			dungeonGameData->room[i].doors[doorDir].Dir = tempRoom->rm.doors[doorDir].cardDir;
		}

		Node **tempRoomNode = (Node**)calloc((int)dungeonGameData->room[i].Width, sizeof(Node*));
		for (int j = 0; j < dungeonGameData->room[i].Width; j++)
		{
			tempRoomNode[j] = (Node*)calloc((int)dungeonGameData->room[i].Height, sizeof(Node));
		}

		dungeonGameData->room[i].Adress = tempRoomNode;

		int a, b;
		a = 0;
		b = 0;

		for (float k = (int)(dungeonGameData->room[i].Position.x - dungeonGameData->room[i].Width / 2.0f) + 0.5f; k < dungeonGameData->room[i].Position.x + dungeonGameData->room[i].Width / 2.0f + 0.5f; k++)
		{
			for (float l = (int)(dungeonGameData->room[i].Position.y - dungeonGameData->room[i].Height / 2.0f) + 0.5f; l < dungeonGameData->room[i].Position.y + dungeonGameData->room[i].Height / 2.0f + 0.5f; l++)
			{
				sfColor tempColor = sfImage_getPixel(MapColl[dungeonGraphicData->room[i].uniqueID], a, b);
				tempRoomNode[a][b].posX = k;
				tempRoomNode[a][b].posY = l;
				tempRoomNode[a][b].Ice = IsEqualColor(tempColor, sfBlue);
				tempRoomNode[a][b].lava = IsEqualColor(tempColor, sfRed);
				tempRoomNode[a][b].Water = IsEqualColor(tempColor, SLOWCOLOR);
				tempRoomNode[a][b].Mud = IsEqualColor(tempColor, SLOWCOLOR2);
				tempRoomNode[a][b].isFalling = IsEqualColor(tempColor, sfColor_fromRGB(250, 150, 50));
				tempRoomNode[a][b].isWalkable = (!IsEqualColor(tempColor, sfBlack) && !IsEqualColor(tempColor, sfColor_fromRGB(50, 50, 50)));
				tempRoomNode[a][b].isWalkableForMob = (IsEqualColor(tempColor, sfWhite) || IsEqualColor(tempColor, SLOWCOLOR) || IsEqualColor(tempColor, SLOWCOLOR2) || IsEqualColor(tempColor, sfRed) || IsEqualColor(tempColor, sfBlue) || IsEqualColor(tempColor, sfColor_fromRGB(250, 150, 50)));
				tempRoomNode[a][b].isShootable = IsEqualColor(tempColor, sfColor_fromRGB(50, 50, 50));

				if (IsEqualColor(tempColor, SLOWCOLOR) || IsEqualColor(tempColor, SLOWCOLOR2))
				{
					tempRoomNode[a][b].speedOfWalk = 0.5f;
					tempRoomNode[a][b].isWalkableForMob = sfTrue;
				}
				else tempRoomNode[a][b].speedOfWalk = 1.0f;
				b++;
			}
			a++;
			b = 0;
		}

		dungeonGameData->room[i].RoomNode = &tempRoomNode[0][0];

		ConnectNodes(tempRoomNode, &dungeonGameData->room[i].RoomNode, (int)dungeonGameData->room[i].Width, (int)dungeonGameData->room[i].Height);

		dungeonGameData->room[i].RoomNode = &tempRoomNode[0][0];

		if (ID == elemDungeon)
		{
			dungeonGameData->room[i].nbPortals = dungeonGameData->room[i].nbDoors;
			dungeonGameData->room[i].portals = (Portal*)calloc(dungeonGameData->room[i].nbPortals, sizeof(Portal));
		}
		for (unsigned short s = 0; s < dungeonGameData->room[i].nbDoors; s++)
		{
			dungeonGameData->room[i].doors[s].Pos = tempRoom->rm.doors[s].Position;

			dungeonGameData->room[i].doors[s].DoorsNode = GetClosestNodeInArray((floorf(dungeonGameData->room[i].doors[s].Pos.x) + 0.5f) * 64, (floorf(dungeonGameData->room[i].doors[s].Pos.y) + 0.5f) * 64, dungeonGameData->room[i].RoomNode);
			dungeonGameData->room[i].doors[s].Pos.x = dungeonGameData->room[i].doors[s].DoorsNode->posX;
			dungeonGameData->room[i].doors[s].Pos.y = dungeonGameData->room[i].doors[s].DoorsNode->posY;

			Node* previous = NULL;

			if (typeOfDungeon == dungeonCorridor)
			{
				//Set walkable wall node where the door is installed
				if (dungeonGameData->room[i].doors[s].Dir == W || dungeonGameData->room[i].doors[s].Dir == E)
				{
					Node *temp = dungeonGameData->room[i].doors[s].DoorsNode;
					temp->isWalkable = sfTrue;
					for (int count = 0; count < 3; count++)
					{
						temp = temp->edges[6];
						temp->isWalkable = sfTrue;
					}
					previous = dungeonGameData->room[i].doors[s].DoorsNode;
					for (int move = 0; move < (int)(SIZE_HORIZONTAL_Y_x64 / 2); move++)
					{
						previous = previous->edges[2];
					}
				}
				else if (dungeonGameData->room[i].doors[s].Dir == N)
				{
					Node *temp = dungeonGameData->room[i].doors[s].DoorsNode;
					temp = temp->edges[4];
					Node *tempForY = temp;
					for (int y = 0; y < 4; y++)
					{
						temp = tempForY;
						for (int count = 0; count < 3; count++)
						{
							temp->isWalkable = sfTrue;
							temp = temp->edges[0];
						}
						tempForY = tempForY->edges[6];
					}
					previous = dungeonGameData->room[i].doors[s].DoorsNode;
					for (int move = 0; move < (int)(SIZE_VERTICAL_X_x64 / 2); move++)
					{
						previous = previous->edges[4];
					}
				}
				else if (dungeonGameData->room[i].doors[s].Dir == S)
				{
					Node *temp = dungeonGameData->room[i].doors[s].DoorsNode;

					temp = temp->edges[4];
					for (int count = 0; count < 3; count++)
					{
						temp->isWalkable = sfTrue;
						temp = temp->edges[0];
					}
					previous = dungeonGameData->room[i].doors[s].DoorsNode;
					for (int move = 0; move < (int)(SIZE_VERTICAL_X_x64 / 2); move++)
					{
						previous = previous->edges[4];
					}
				}

				//Connect parts of nodes
				if (dungeonGraphicData->room[i].doors[s].isDebutOfCorridor)
				{
					Vector2fList *PreviousV2fList = NULL;
					for (Vector2fList *v = dungeonGraphicData->room[i].doors[s].corridorRef->firstElementPos; v != NULL; v = v->next)
					{
						int x = 0, y = 0;
						sfVector2u imageSize = sfImage_getSize(CorridorColl[v->id]);
						float PosXNewNode = ((v->Position.x / SIZE_CASE) - imageSize.x / 2.0f) + 0.5f;
						float PosYNewNode = ((v->Position.y / SIZE_CASE) - imageSize.y / 2.0f) + 0.5f;

						Node **elementsCorridor = (Node**)calloc(imageSize.x, sizeof(Node*));

						for (int ndDim = 0; ndDim < (int)imageSize.x; ndDim++)
						{
							elementsCorridor[ndDim] = (Node*)calloc(imageSize.y, sizeof(Node));
						}

						for (int CollX = 0; CollX < (int)imageSize.x; CollX++)
						{
							for (int CollY = 0; CollY < (int)imageSize.y; CollY++)
							{
								//TODO Make a function that compare sfColors
								sfColor tempColor = sfImage_getPixel(CorridorColl[v->id], x, y);
								elementsCorridor[CollX][CollY].Ice = IsEqualColor(tempColor, sfBlue);
								elementsCorridor[CollX][CollY].lava = IsEqualColor(tempColor, sfRed);
								elementsCorridor[CollX][CollY].isWalkable = (tempColor.r == 255) && !IsEqualColor(tempColor, sfColor_fromRGB(50, 50, 50));
								elementsCorridor[CollX][CollY].isShootable = IsEqualColor(tempColor, sfColor_fromRGB(50, 50, 50));
								elementsCorridor[CollX][CollY].isFalling = IsEqualColor(tempColor, sfColor_fromRGB(250, 150, 50));
								elementsCorridor[CollX][CollY].posX = PosXNewNode + x;
								elementsCorridor[CollX][CollY].posY = PosYNewNode + y;
								elementsCorridor[CollX][CollY].speedOfWalk = 1.0f;
								GenerateMapLightCorridor(elementsCorridor, v->id, CollX, CollY, elementsCorridor[CollX][CollY].posX, elementsCorridor[CollX][CollY].posY, CorridorLight);
								y++;
							}
							x++;
							y = 0;
						}

						Node *temp = &elementsCorridor[0][0];

						ConnectNodes(elementsCorridor, &temp, imageSize.x, imageSize.y);

						Node *tempToConnectSlice;
						if (v == dungeonGraphicData->room[i].doors[s].corridorRef->firstElementPos)
						{
							if ((v->Position.x - (tempRoom->rm.doors[s].Position.x * SIZE_CASE)) == 0)
							{
								tempToConnectSlice = v->Position.y < tempRoom->rm.doors[s].Position.y  * SIZE_CASE ? &elementsCorridor[0][(int)imageSize.y - 1] : &elementsCorridor[0][0];
							}
							else
							{
								tempToConnectSlice = v->Position.x < tempRoom->rm.doors[s].Position.x  * SIZE_CASE ? &elementsCorridor[(int)imageSize.x - 1][0] : &elementsCorridor[0][0];
							}
						}
						else
						{
							if ((v->Position.x - PreviousV2fList->Position.x) == 0)
							{
								tempToConnectSlice = v->Position.y < PreviousV2fList->Position.y ? &elementsCorridor[0][(int)imageSize.y - 1] : &elementsCorridor[0][0];
							}
							else
							{
								tempToConnectSlice = v->Position.x < PreviousV2fList->Position.x ? &elementsCorridor[(int)imageSize.x - 1][0] : &elementsCorridor[0][0];
							}
						}

						//Connect part with each others
						ConnectSliceOfNodes(&previous, &tempToConnectSlice);

						//DevTools PurPoses
						AddPointerDevTools(&elementsCorridor[0][0], imageSize.x, imageSize.y);
						AddPointer(elementsCorridor, imageSize.x, imageSize.y);
						//For the next Iteration
						if (v->next != NULL)
						{
							if ((v->Position.x - (v->next->Position.x)) == 0)
							{
								previous = v->Position.y < v->next->Position.y ? &elementsCorridor[0][(int)imageSize.y - 1] : &elementsCorridor[0][0];
							}
							else
							{
								previous = v->Position.x < v->next->Position.x ? &elementsCorridor[(int)imageSize.x - 1][0] : &elementsCorridor[0][0];
							}
							PreviousV2fList = v;
						}
						else
						{
							if ((v->Position.x - PreviousV2fList->Position.x) == 0)
							{
								SaveForNDdoors[indexToAdd] = v->Position.y < PreviousV2fList->Position.y ? &elementsCorridor[0][0] : &elementsCorridor[0][(int)imageSize.y - 1];
								indexToAdd++;
							}
							else
							{
								SaveForNDdoors[indexToAdd] = v->Position.x < PreviousV2fList->Position.x ? &elementsCorridor[0][0] : &elementsCorridor[(int)imageSize.x - 1][0];
								indexToAdd++;
							}
						}
					}
				}
			}
		}
		//

		if (typeOfDungeon == dungeonCorridor)
		{
			GenerateMapLight(dungeonGameData, *dungeonGraphicData, tempRoomNode, i, MapLight);
			PlaceLightRoom(dungeonGameData, i, dungeonGraphicData);
		}

		if (tempRoom->Next != NULL)
		{
			tempRoom = tempRoom->Next;
		}
	}


	if (ID == elemDungeon)
	{
		tempRoom = dungeonRawData.FirstRoom;
		for (int i = 0; i < dungeonGameData->numberOfRoom; i++)
		{
			sfVector2f Dim = { 1.0f, 1.0f };
			for (unsigned short s = 0; s < dungeonGameData->room[i].nbPortals; s++)
			{
				if (dungeonGameData->room[i].portals[s].LinkPortal == NULL)
				{
					InitPortal(&dungeonGameData->room[i].portals[s], dungeonGameData->room[i].doors[s].Pos, Dim, 1.75f, 0.75f);

					int indexRoom = 0;
					int indexDoor = 0;
					for (RoomList *r = dungeonRawData.FirstRoom; r != NULL; r = r->Next)
					{
						if (r->rm.Index == tempRoom->rm.doors[s].PortalRef->roomBack->Index)
						{
							for (int i = 0; i < r->rm.nbDoors; i++)
							{
								if (r->rm.doors[i].Index == tempRoom->rm.doors[s].PortalRef->Index)
								{
									i = r->rm.nbDoors;
								}
								else
								{
									indexDoor++;
								}
							}
							break;
						}
						else
						{
							indexRoom++;
						}
					}

					InitPortal(&dungeonGameData->room[indexRoom].portals[indexDoor], dungeonGameData->room[indexRoom].doors[indexDoor].Pos, Dim, 1.75f, 0.75f);
					dungeonGameData->room[indexRoom].portals[indexDoor].LinkPortal = &dungeonGameData->room[i].portals[s];
					dungeonGameData->room[i].portals[s].LinkPortal = &dungeonGameData->room[indexRoom].portals[indexDoor];
				}
			}

			if (tempRoom->Next != NULL)
			{
				tempRoom = tempRoom->Next;
			}
		}
	}

	if (typeOfDungeon == dungeonCorridor)
	{
		//Rego Through to finish Connection with last doors
		for (int i = 0; i < dungeonGameData->numberOfRoom; i++)
		{
			for (unsigned short s = 0; s < dungeonGameData->room[i].nbDoors; s++)
			{
				if (!dungeonGraphicData->room[i].doors[s].isDebutOfCorridor)
				{
					Node* previous2nd = NULL;

					//Set walkable wall node where the door is installed
					if (dungeonGameData->room[i].doors[s].Dir == W || dungeonGameData->room[i].doors[s].Dir == E)
					{
						Node *temp = dungeonGameData->room[i].doors[s].DoorsNode;
						temp->isWalkable = sfTrue;
						for (int count = 0; count < 3; count++)
						{
							temp = temp->edges[6];
							temp->isWalkable = sfTrue;
						}
						previous2nd = dungeonGameData->room[i].doors[s].DoorsNode;
						for (int move = 0; move < (int)(SIZE_HORIZONTAL_Y_x64 / 2); move++)
						{
							previous2nd = previous2nd->edges[2];
						}
					}
					else if (dungeonGameData->room[i].doors[s].Dir == N)
					{
						Node *temp = dungeonGameData->room[i].doors[s].DoorsNode;
						temp = temp->edges[4];
						Node *tempForY = temp;
						for (int y = 0; y < 4; y++)
						{
							temp = tempForY;
							for (int count = 0; count < 3; count++)
							{
								temp->isWalkable = sfTrue;
								temp = temp->edges[0];
							}
							tempForY = tempForY->edges[6];
						}
						previous2nd = dungeonGameData->room[i].doors[s].DoorsNode;
						for (int move = 0; move < (int)(SIZE_VERTICAL_X_x64 / 2); move++)
						{
							previous2nd = previous2nd->edges[4];
						}
					}
					else
					{
						Node *temp = dungeonGameData->room[i].doors[s].DoorsNode;

						temp = temp->edges[4];
						for (int count = 0; count < 3; count++)
						{
							temp->isWalkable = sfTrue;
							temp = temp->edges[0];
						}
						previous2nd = dungeonGameData->room[i].doors[s].DoorsNode;
						for (int move = 0; move < (int)(SIZE_VERTICAL_X_x64 / 2); move++)
						{
							previous2nd = previous2nd->edges[4];
						}
					}

					for (int index = 0; index < indexToAdd; index++)
					{
						if (SaveForNDdoors[index] != NULL)
						{
							if (fabsf(previous2nd->posX - SaveForNDdoors[index]->posX) == 1 && (previous2nd->posY - SaveForNDdoors[index]->posY == 0)
								|| fabsf(previous2nd->posY - SaveForNDdoors[index]->posY) == 1 && (previous2nd->posX - SaveForNDdoors[index]->posX == 0))
							{
								ConnectSliceOfNodes(&previous2nd, &SaveForNDdoors[index]);
								index = indexToAdd;
							}
						}
					}
				}
			}
		}
	}

	if (ID == slimeDungeon || ID == beginningDungeon)
	{
		for (int i = 0; i < 8; i++)
		{
			sfImage_destroy(CorridorLight[i]);
			sfImage_destroy(CorridorColl[i]);
		}
		for (int i = 0; i < 11; i++)
		{
			sfImage_destroy(MapLight[i]);
			sfImage_destroy(MapColl[i]);
		}
	}
	else if (ID == elemDungeon)
	{
		for (int i = 0; i < 11; i++)
		{
			sfImage_destroy(MapColl[i]);
		}
	}
	else if (ID == razarDungeon)
	{
		sfImage_destroy(MapColl[0]);
	}

	free(CorridorLight);
	free(CorridorColl);
	free(MapLight);
	free(MapColl);
}

//Modifie some details
void Correction(Dungeon *dungeon)
{
	for (int i = 0; i < dungeon->DGameData.numberOfRoom; i++)
	{
		for (unsigned short s = 0; s < dungeon->DGameData.room[i].nbDoors; s++)
		{
			dungeon->DGraphicData.room[i].doors[s].LocalPos.x = dungeon->DGameData.room[i].doors[s].Pos.x - dungeon->DGameData.room[i].Position.x;
			dungeon->DGraphicData.room[i].doors[s].LocalPos.y = dungeon->DGameData.room[i].doors[s].Pos.y - dungeon->DGameData.room[i].Position.y;

			if (dungeon->ID != elemDungeon && dungeon->DGraphicData.room[i].doors[s].Dir == (CardinalDirection)N)
			{
				dungeon->DGraphicData.room[i].doors[s].LocalPos.y += 1.5f;
			}
		}
	}


	if (dungeon->ID == elemDungeon)
	{
		for (int i = 0; i < dungeon->DGameData.numberOfRoom; i++)
		{
			if (dungeon->DGameData.room[i].RoomId == 10)
			{
				for (int j = 0; j < dungeon->DGameData.room[i].nbPortals; j++)
				{
					dungeon->DGameData.room[i].portals[j].PosCenter.x = dungeon->DGameData.room[i].Position.x + j * 4.f;
					dungeon->DGameData.room[i].portals[j].PosCenter.y = dungeon->DGameData.room[i].Position.y + (dungeon->DGameData.room[i].Height / 2.0f) - 3.0f;
				}
			}
		}
	}
}

RoomPrefab* LoadPrefabNoCorridor(const DungeonId ID)
{
	RoomPrefab *LoadPrefab = (RoomPrefab*)calloc(11, sizeof(RoomPrefab));

	for (int i = 0; i < 11; i++)
	{
		LoadPrefab[i].indexPrefab = i;
		LoadPrefab[i].hasContrain = sfTrue;
	}

	LoadPrefab[10].indexPrefab = 99;
	sfImage* Prefab[11];

	Prefab[0] = sfImage_createFromFile("Ressources/Sprites/Environment/Dungeon2/Rooms/ID_0/Spawn_Contrains.png");
	Prefab[1] = sfImage_createFromFile("Ressources/Sprites/Environment/Dungeon2/Rooms/ID_1/Room_01_Contrains.png");
	Prefab[2] = sfImage_createFromFile("Ressources/Sprites/Environment/Dungeon2/Rooms/ID_2/Room_02_Contrains.png");
	Prefab[3] = sfImage_createFromFile("Ressources/Sprites/Environment/Dungeon2/Rooms/ID_3/Room_03_Contrains.png");
	Prefab[4] = sfImage_createFromFile("Ressources/Sprites/Environment/Dungeon2/Rooms/ID_4/Wind_01_Contrains.png");
	Prefab[5] = sfImage_createFromFile("Ressources/Sprites/Environment/Dungeon2/Rooms/ID_1/Room_01_Contrains.png");
	Prefab[6] = sfImage_createFromFile("Ressources/Sprites/Environment/Dungeon2/Rooms/ID_1/Room_01_Contrains.png");
	Prefab[7] = sfImage_createFromFile("Ressources/Sprites/Environment/Dungeon2/Rooms/ID_7/Room_07_Contrains.png");
	Prefab[8] = sfImage_createFromFile("Ressources/Sprites/Environment/Dungeon2/Rooms/ID_8/Room_08_Contrains.png");
	Prefab[9] = sfImage_createFromFile("Ressources/Sprites/Environment/Dungeon2/Rooms/ID_9/Wind_02_Contrains.png");
	Prefab[10] = sfImage_createFromFile("Ressources/Sprites/Environment/Dungeon2/Rooms/ID_1/Room_01_Contrains.png");




	//Yellow is North
	//Red is East
	//Green is South
	//Blue is West

	for (int k = 0; k < 11; k++)
	{
		if (LoadPrefab[k].hasContrain)
		{
			sfVector2u sizePrefab = sfImage_getSize(Prefab[k]);
			for (int y = 0; y < (int)sizePrefab.y; y++)
			{
				for (int x = 0; x < (int)sizePrefab.x; x++)
				{
					sfColor temp = sfImage_getPixel(Prefab[k], x, y);
					if (!IsEqualColor(temp, sfWhite))
					{
						int index;
						if (IsEqualColor(temp, sfYellow))
						{
							index = (CardinalDirection)N;
						}
						else if (IsEqualColor(temp, sfRed))
						{
							index = (CardinalDirection)E;
						}
						else if (IsEqualColor(temp, sfGreen))
						{
							index = (CardinalDirection)S;
						}
						else if (IsEqualColor(temp, sfBlue))
						{
							index = (CardinalDirection)W;
						}
						else
						{
							printf("Error color invalid prefab loading\n");
							continue;
						}
						sf_Vector2fList *newVect = (sf_Vector2fList*)malloc(sizeof(sf_Vector2fList));
						newVect->Vector.x = (float)x;
						newVect->Vector.y = (float)y;
						newVect->Next = LoadPrefab[k].AvailablePos[index];
						LoadPrefab[k].AvailablePos[index] = newVect;
						LoadPrefab[k].ListLengths[index]++;
					}
				}
			}
		}
	}

	for (int i = 0; i < 11; i++)
	{
		sfImage_destroy(Prefab[i]);
	}

	return (LoadPrefab);
}

RoomPrefab* LoadPrefab()
{
	RoomPrefab *LoadPrefab = (RoomPrefab*)calloc(AMOUNT_PREFAB_1ST_DUNGEON, sizeof(RoomPrefab));

	for (int i = 0; i < AMOUNT_PREFAB_1ST_DUNGEON; i++)
	{
		LoadPrefab[i].indexPrefab = i;
		LoadPrefab[i].hasContrain = sfTrue;
	}

	LoadPrefab[10].indexPrefab = 99;

	sfImage* Prefab[AMOUNT_PREFAB_1ST_DUNGEON];

	Prefab[0] = sfImage_createFromFile("Ressources/Sprites/Environment/Dungeon1/Rooms/ID_0/spawn_doorcontrain.png");
	Prefab[1] = sfImage_createFromFile("Ressources/Sprites/Environment/Dungeon1/Rooms/ID_1/Dortoir_DoorContrains.png");
	Prefab[2] = sfImage_createFromFile("Ressources/Sprites/Environment/Dungeon1/Rooms/ID_2/Dortoir_DoorContrains.png");
	Prefab[3] = sfImage_createFromFile("Ressources/Sprites/Environment/Dungeon1/Rooms/ID_3/Entrepot_DoorContrains.png");
	Prefab[4] = sfImage_createFromFile("Ressources/Sprites/Environment/Dungeon1/Rooms/ID_4/Entrepot_02_DoorContrains.png");
	Prefab[5] = sfImage_createFromFile("Ressources/Sprites/Environment/Dungeon1/Rooms/ID_5/Cantine_DoorContrains.png");
	Prefab[6] = sfImage_createFromFile("Ressources/Sprites/Environment/Dungeon1/Rooms/ID_6/Cantine_02_DoorContrains.png");
	Prefab[7] = sfImage_createFromFile("Ressources/Sprites/Environment/Dungeon1/Rooms/ID_7/Bibliotheque_DoorContrains.png");
	Prefab[8] = sfImage_createFromFile("Ressources/Sprites/Environment/Dungeon1/Rooms/ID_8/Bibliotheque_02_DoorContrains.png");
	Prefab[9] = sfImage_createFromFile("Ressources/Sprites/Environment/Dungeon1/Rooms/ID_9/MiniBoss_DoorContrains.png");
	Prefab[10] = sfImage_createFromFile("Ressources/Sprites/Environment/Dungeon1/Rooms/ID_10/BossRoom_DoorContrains.png");


	int index;

	for (int k = 0; k < AMOUNT_PREFAB_1ST_DUNGEON; k++)
	{
		if (LoadPrefab[k].hasContrain)
		{
			index = (CardinalDirection)N;
			sfVector2u sizePrefab = sfImage_getSize(Prefab[k]);
			for (int y = 0; y < (int)sizePrefab.y; y += (sizePrefab.y - 1))
			{
				for (int x = 0; x < (int)sizePrefab.x; x++)
				{
					if (sfImage_getPixel(Prefab[k], x, y).g >= 200)
					{
						sf_Vector2fList *newVect = (sf_Vector2fList*)malloc(sizeof(sf_Vector2fList));
						newVect->Vector.x = (float)x;
						newVect->Vector.y = (float)y;
						newVect->Next = LoadPrefab[k].AvailablePos[index];
						LoadPrefab[k].AvailablePos[index] = newVect;
						LoadPrefab[k].ListLengths[index]++;
					}
				}
				index = (CardinalDirection)S;
			}

			index = (CardinalDirection)W;

			for (int x = 0; x < (int)sizePrefab.x; x += (sizePrefab.x - 1))
			{
				for (int y = 0; y < (int)sizePrefab.y; y++)
				{
					if (sfImage_getPixel(Prefab[k], x, y).g > 200)
					{
						sf_Vector2fList *newVect = (sf_Vector2fList*)malloc(sizeof(sf_Vector2fList));
						newVect->Vector.x = (float)x;
						newVect->Vector.y = (float)y;
						newVect->Next = LoadPrefab[k].AvailablePos[index];
						LoadPrefab[k].AvailablePos[index] = newVect;
						LoadPrefab[k].ListLengths[index]++;
					}
				}
				index = (CardinalDirection)E;
			}
		}
	}

	for (int i = 0; i < AMOUNT_PREFAB_1ST_DUNGEON; i++)
	{
		sfImage_destroy(Prefab[i]);
	}

	return (LoadPrefab);
}

//pArea : Size of the zone when all room appears before being pushed from one another
//pMargin : max Marge distance authorized between rooms
//pmaxWidth : max room size
void GenerateDungeon(Dungeon* newDungeon, const int numberRoom, const float pArea, const float pMargin, const float pmaxWidth, const DungeonId ID)
{
	static int previousDungeonID = 0;
	int typeOfDungeon = GetTypeOfDungeon();
	SaveData saveData = GetSaveData();
	if (newDungeon->NeedToClean)
	{
		newDungeon->NeedToClean = sfFalse;
		ClearGraphicData(&newDungeon->DGraphicData);


		//NEED TO BE FIXED
		if (previousDungeonID == dungeonCorridor)
		{
			ClearDungeon(&newDungeon->DGameData);
		}
	}

	previousDungeonID = typeOfDungeon;

	newDungeon->DGameData.numberOfRoom = 0;
	newDungeon->DGameData.seed = 0;
	newDungeon->DGraphicData.room = NULL;
	newDungeon->DGraphicData.numberOfRoom = 0;
	newDungeon->DGraphicData.seed = 0;

	//TODO Clean RawData Function
	DungeonRawData dungeonRD;
	dungeonRD.seed = 0;
	dungeonRD.FirstEdge = NULL;
	dungeonRD.FirstRoom = NULL;

	sfVector2f OriginOfGeneration = { 1024.f / 2.0f, 768.f / 2.0f };

	RoomPrefab *arrayOfPrebab = typeOfDungeon == dungeonCorridor ? LoadPrefab() : LoadPrefabNoCorridor(ID);

	DWORD threadID = GetCurrentThreadId();

	if (ID == beginningDungeon)
	{
		GenerateDungeonRD(&dungeonRD, numberRoom, OriginOfGeneration, pArea, pMargin, pmaxWidth, arrayOfPrebab, 0, 2624649824);
	}
	else if (saveData.IsLoaded)
	{
		if (typeOfDungeon == dungeonCorridor)
		{
			GenerateDungeonRD(&dungeonRD, saveData.NumberOfRoom, OriginOfGeneration, pArea, pMargin, pmaxWidth, arrayOfPrebab, threadID, saveData.Seed);
		}
		else
		{
			GenerateDungeonNoCorri(&dungeonRD, saveData.NumberOfRoom, OriginOfGeneration, pArea, pMargin, pmaxWidth, arrayOfPrebab, threadID, saveData.Seed);
		}
	}
	else if (!MultiProceduralGeneration(&dungeonRD, numberRoom, OriginOfGeneration, pArea, pMargin, pmaxWidth, arrayOfPrebab, 5, ID))
	{
		printf("Massive error break out\n");
		return;
	}

	free(arrayOfPrebab);

	newDungeon->ID = ID;

	ConvertToGraphicData(&newDungeon->DGraphicData, &dungeonRD);

	newDungeon->DGameData.numberOfRoom = newDungeon->DGraphicData.numberOfRoom;
	newDungeon->DGameData.seed = newDungeon->DGraphicData.seed;
	newDungeon->DGameData.room = (RoomGameData*)calloc(newDungeon->DGameData.numberOfRoom, sizeof(RoomGameData));

	for (int i = 0; i < newDungeon->DGameData.numberOfRoom; i++)
	{
		newDungeon->DGameData.room[i].Position = newDungeon->DGraphicData.room[i].Pos;
		newDungeon->DGameData.room[i].RoomId = (unsigned char)newDungeon->DGraphicData.room[i].IdEnemyRoom;
	}

	GetGameDataFromGraphicData(&newDungeon->DGameData, &newDungeon->DGraphicData, dungeonRD, newDungeon->ID);

	Correction(newDungeon);

}

void DrawSupp(sfRenderWindow* wind)
{
	sfRenderWindow_drawVertexArray(wind, Supp, NULL);
}