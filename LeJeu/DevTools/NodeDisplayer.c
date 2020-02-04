#include "NodeDisplayer.h"
#include <time.h>
#include <Windows.h>

sfSprite* NodeInfo[3];
NodeList* test;

typedef struct
{
	Node *pointer;
	int sizeX;
	int sizeY;
}NodeCorridorDisplayer;

NodeCorridorDisplayer corridorDisplayer[1000];
int counterPointer = 0;

void ResetCounterPointer()
{
	counterPointer = 0;
}

void InitNodeSpritesDisplay(void)
{
	NodeInfo[0] = LoadSprite("Ressources/Sprites/Environment/coliBlack.png", 1);
	NodeInfo[1] = LoadSprite("Ressources/Sprites/Environment/coliWhite.png", 1);
	NodeInfo[2] = LoadSprite("Ressources/Sprites/Environment/coliDoor.png", 1);
	test = NULL;
}

void AddPointerDevTools(Node *adress, const int X, const int Y)
{
	corridorDisplayer[counterPointer].pointer = adress;
	corridorDisplayer[counterPointer].sizeX = X;
	corridorDisplayer[counterPointer].sizeY = Y;
	counterPointer++;
}

void DisplayOneRoomNodes(sfRenderWindow *window, const void* firstRoomNode, const Camera cam, const RoomGameData *room, const Node *playerNode)
{
	Node* first = (Node*)firstRoomNode;

	while (first != NULL)
	{
		Node* second = first;
		while (second != NULL)
		{
			BlitSprite(window, NodeInfo[second->isWalkable], (sfVector2f) { second->posX * 64.0f, second->posY * 64.0f }, NULL);
			second = second->edges[0];
		}
		first = first->edges[6];
	}

	for (int i = 0; i < room->nbDoors; i++)
	{
		BlitSprite(window, NodeInfo[2], (sfVector2f) { room->doors[i].Pos.x*64.0f, room->doors[i].Pos.y*64.0f }, NULL);
	}
	
	for (int i = 0; i < counterPointer; i++)
	{
		Node* Corri = corridorDisplayer[i].pointer;

		int cptX = 0;
		int cptY = 0;
		while (Corri != NULL && cptY < corridorDisplayer[i].sizeY)
		{
			Node* secondCorri = Corri;
			while (secondCorri != NULL && cptX < corridorDisplayer[i].sizeX)
			{
				sfVector2f temp = {(float)secondCorri->posX, (float)secondCorri->posY};
				if (fabsf(temp.x - ((Node*)firstRoomNode)->posX) < 100 && fabsf(temp.y - ((Node*)firstRoomNode)->posY) < 100)
				{
					temp.x *= 64;
					temp.y *= 64;
					BlitSprite(window, NodeInfo[secondCorri->isWalkable], temp, NULL);
				}
				
				secondCorri = secondCorri->edges[0];
				cptX++;
			}
			Corri = Corri->edges[6];
			cptY++;
			cptX = 0;
		}
	}

	BlitSprite(window, NodeInfo[2], (sfVector2f) { playerNode->posX*64.0f, playerNode->posY *64.0f }, NULL);
}

void DisplayCorridorNodes(sfRenderWindow *window, const void* firstCorridorNode)
{
	Node* first = (Node*)firstCorridorNode;

	while (first != NULL)
	{
		Node* second = first;
		while (second != NULL)
		{
			BlitSprite(window, NodeInfo[second->isWalkable], (sfVector2f) { second->posX * 64.0f, second->posY * 64.0f }, NULL);
			second = second->edges[0];
		}
		first = first->edges[6];
	}
}

void TakeScreenShot(sfRenderWindow* window, const char* pathDirectory)
{
	sfVector2u windowSize = sfRenderWindow_getSize(window);
	sfTexture* texture = sfTexture_create(windowSize.x, windowSize.y);
	sfTexture_updateFromRenderWindow(texture, window, 0, 0);
	sfImage* screenshot = sfTexture_copyToImage(texture);
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	char path[200];
	if (tm.tm_mon < 10)
	{
		sprintf(path, "%s/%d_0%d_%d %d-%d-%d.png", pathDirectory, tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
	}
	else
	{
		sprintf(path, "%s/%d_%d_%d %d-%d-%d.png", pathDirectory, tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
	}
	sfImage_saveToFile(screenshot, path);
	sfTexture_destroy(texture);
	sfImage_destroy(screenshot);
}


void SaveNodeList(NodeList **ToSave)
{
	test = *ToSave;
}

void UpdateNodeList(NodeList **ToUpdate)
{
	test = *ToUpdate;
}