#ifndef _NODEDISPLAYER
#define _NODEDISPLAYER


#include <stdio.h>
#include <stdlib.h>
#include <SFML\Graphics.h>
#include "..\Model\Node\Node.h"
#include "..\Controller\GraphicsUtilities\Utilities.h"
#include "..\Controller\Camera\Camera.h"
#include "..\Model\Room\Room.h"

void ResetCounterPointer();
void InitNodeSpritesDisplay(void);
void AddPointerDevTools(Node *adress, const int X, const int Y);
void DisplayOneRoomNodes(sfRenderWindow *window, const void* firstRoomNode, const Camera cam, const RoomGameData *room, const Node *playerNode);
void DisplayCorridorNodes(sfRenderWindow *window, const void* firstCorridorNode);
void TakeScreenShot(sfRenderWindow* window, const char* pathDirectory);
void SaveNodeList(NodeList **ToSave);
void UpdateNodeList(NodeList **ToUpdate);

#endif