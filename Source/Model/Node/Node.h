#ifndef _NODE_H
#define _NODE_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <SFML\Graphics.h>
#include "..\..\Controller\MainSceneController\LightController\SegmentLight.h"
#include "..\Physics\RigidBody\RigidBody.h"
typedef struct NODE NODE;

typedef struct NODE
{
	NODE *edges[8];
	float posX;
	float posY;
	float speedOfWalk;
	bool isWalkable;
	bool isWalkableForMob;
	bool isEndOfWorld;
	bool isShootable;
	bool isFalling;
	bool Ice;
	bool lava;
	bool Water;
	bool Mud;
	SegmentsLight Segments;
}Node;

typedef struct NODELIST NODELIST;

typedef struct NODELIST
{
	Node *Node;
	NODELIST *Next;
}NodeList;

Node* GetNeighboursWithCoord(const float coordX, const float coordY, const Node actualNode);
int GetDirectionBetWeenTwoNodes(const float coordXA, const float coordYA, const float coordXB, const float coordYB);
Node* GetClosestNodeInArray(float coordX, float coordY, Node* nodeArray);
Node* GetNodeInArray(float coordX, float coordY, Node* nodeArray);
sfBool CheckNextNode(Node* ActualNode, sfVector2f* Direction, const sfVector2f Position, const sfBool AlterateDirection);
sfBool CheckNextNodeE(Node* ActualNode, RigidBody* rb, const sfVector2f Position, const sfBool AlterateDirection);
sfBool CheckNextNodeP(Node* ActualNode, RigidBody* rb, const sfVector2f Position, const sfBool AlterateDirection);
sfBool HaveObstacleBetween(const Node* firstNode, const Node* secondNode, const sfBool forPlayer, const sfBool forEnemy, const sfBool forShot);
void ChangeNodePlayer(Node** ActualNode, const sfVector2f Position);
void ChangeNode(Node** ActualNode, const sfVector2f Position);
void CleanNodeListNode(NodeList **ToClean);
Node* SearchWalkableNodeAround(Node* actualNode);
Node* SearchWalkableNodeForMobAround(Node* actualNode);


#endif