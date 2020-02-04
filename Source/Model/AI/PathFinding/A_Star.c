#include "A_Star.h"

typedef struct NODEASTAR NODEASTAR;

typedef struct NODEASTAR
{
	Node* Node;
	int Heuristic;
	int Gcost;
	int Fcost;
	NODEASTAR* Next;
	NODEASTAR* Parent;
	sfBool IsThePath;
}NodeAStar;

typedef struct OpenList
{
	NodeAStar* FirstNode;
}OpenList;

typedef struct CloseList
{
	NodeAStar* FirstNode;
}CloseList;

typedef struct PATHFINDINGNODELIST
{
	OpenList openList;
	CloseList closeList;
}PathFindingNodeList;


void CleanNodeList(PathFindingNodeList* nodeList);

void SetFCost(NodeAStar* node)
{
	node->Fcost = node->Gcost + node->Heuristic;
}

void SetHeuristic(NodeAStar* node, const Node* destinationNode)
{
	int distanceX = abs((int)node->Node->posX - (int)destinationNode->posX);
	int distanceY = abs((int)node->Node->posY - (int)destinationNode->posY);
	if (distanceX <= distanceY)
	{
		node->Heuristic = 14 * distanceX + (distanceY - distanceX) * 10;
	}
	else
	{
		node->Heuristic = 14 * distanceY + (distanceX - distanceY) * 10;
	}
}

void SetGCost(NodeAStar* node)
{
	int distanceX = abs((int)node->Node->posX - (int)node->Parent->Node->posX);
	int distanceY = abs((int)node->Node->posY - (int)node->Parent->Node->posY);
	if (distanceX <= distanceY)
	{
		node->Gcost = 14 * distanceX + (distanceY - distanceX) * 10 + node->Parent->Gcost;
	}
	else
	{
		node->Gcost = 14 * distanceY + (distanceX - distanceY) * 10 + node->Parent->Gcost;
	}
}

void AddNodeInOpenList(const Node* node, const NodeAStar* parent, OpenList* openList, const Node* destinationNode)
{
	NodeAStar* newElement = (NodeAStar*)calloc(1, sizeof(NodeAStar));
	newElement->Node = (Node*)node;
	newElement->Parent = (NodeAStar*)parent;
	if (parent != NULL)
	{
		SetHeuristic(newElement, destinationNode);
		SetGCost(newElement);
		SetFCost(newElement);
	}
	newElement->Next = openList->FirstNode;
	openList->FirstNode = newElement;
}

void AddNodeInCloseList(NodeAStar* node, OpenList* openList, CloseList* closeList)
{
	NodeAStar* tempNode = openList->FirstNode;

	if (tempNode == node)
	{
		openList->FirstNode = tempNode->Next;
		node->Next = closeList->FirstNode;
		closeList->FirstNode = node;
	}
	else
	{
		while (tempNode != NULL)
		{
			if (tempNode->Next == node)
			{
				tempNode->Next = tempNode->Next->Next;
				tempNode = NULL;
			}
			else
			{
				tempNode = tempNode->Next;
			}
		}
		node->Next = closeList->FirstNode;
		closeList->FirstNode = node;
	}
}

void ChangeNodeInOpenList(const NodeAStar* node, const OpenList* openList, const Node* destinationNode, const Node* nodeToChange)
{
	int distanceX = abs((int)nodeToChange->posX - (int)destinationNode->posX);
	int distanceY = abs((int)nodeToChange->posY - (int)destinationNode->posY);

	int tempHeuristic;
	int tempGCost;
	int tempFCost;
	if (distanceX <= distanceY)
	{
		tempHeuristic = 14 * distanceX + (distanceY - distanceX) * 10;
	}
	else
	{
		tempHeuristic = 14 * distanceY + (distanceX - distanceY) * 10;
	}
	distanceX = abs((int)node->Node->posX - (int)nodeToChange->posX);
	distanceY = abs((int)node->Node->posY - (int)nodeToChange->posY);
	if (distanceX <= distanceY)
	{
		tempGCost = 14 * distanceX + (distanceY - distanceX) * 10 + node->Gcost;
	}
	else
	{
		tempGCost = 14 * distanceY + (distanceX - distanceY) * 10 + node->Gcost;
	}
	tempFCost = tempHeuristic + tempGCost;


	NodeAStar* tempNode = openList->FirstNode;
	while (tempNode != NULL)
	{
		if (tempNode->Node == nodeToChange && tempFCost < tempNode->Fcost)
		{
			tempNode->Fcost = tempFCost;
			tempNode->Gcost = tempGCost;
			tempNode->Heuristic = tempHeuristic;
			tempNode->Parent = (NodeAStar*)node;
		}
		tempNode = tempNode->Next;
	}
}

sfBool IsInCloseList(const Node* node, const CloseList* closeList)
{
	NodeAStar* temp = closeList->FirstNode;
	while (temp != NULL)
	{
		if (temp->Node == node)
		{
			return sfTrue;
		}
		temp = temp->Next;
	}
	return sfFalse;
}

sfBool IsInOpenList(const Node* node, const OpenList* openList)
{
	NodeAStar* temp = openList->FirstNode;
	while (temp != NULL)
	{
		if (temp->Node == node)
		{
			return sfTrue;
		}
		temp = temp->Next;
	}
	return sfFalse;
}

void AddNeighbourToOpenList(const NodeAStar* node, OpenList* openList, const CloseList* closeList, const Node* destinationNode)
{
	for (int i = 0; i < 8; i++)
	{
		if (node->Node->edges[i] != NULL && node->Node->edges[i]->isWalkableForMob && !IsInCloseList(node->Node->edges[i], closeList))
		{
			if ((i % 2 && node->Node->edges[i - 1] != NULL && node->Node->edges[i - 1]->isWalkableForMob && node->Node->edges[(i + 1) % 8] != NULL && node->Node->edges[(i + 1) % 8]->isWalkableForMob) || !(i % 2))
			{

				if (!IsInOpenList(node->Node->edges[i], openList))
				{
					AddNodeInOpenList(node->Node->edges[i], node, openList, destinationNode);
				}
				else
				{
					ChangeNodeInOpenList(node, openList, destinationNode, node->Node->edges[i]);
				}
			}
		}
	}
}

sfBool CheckIfFindThePath(const OpenList openList, const Node* destinationNode)
{
	NodeAStar* tempNode = openList.FirstNode;
	while (tempNode != NULL)
	{
		if (tempNode->Node == destinationNode)
		{
			tempNode->Parent->IsThePath = sfTrue;
			return sfTrue;
		}
		tempNode = tempNode->Next;
	}
	return sfFalse;
}

NodeList* SetPath(const CloseList* closeList)
{
	NodeList* firstPath = (NodeList*)calloc(1, sizeof(NodeList));
	NodeList* tempPath = firstPath;
	NodeAStar* tempNode = closeList->FirstNode;
	while (tempNode != NULL)
	{
		if (tempNode->IsThePath)
		{
			while (tempNode->Parent != NULL)
			{
				//Pas sur de ce que je fais
				tempPath->Node = tempNode->Node;
				NodeList* tempPath2 = (NodeList*)calloc(1, sizeof(NodeList));
				tempPath->Next = tempPath2;
				tempPath = tempPath2;
				tempNode->IsThePath = sfTrue;
				tempNode = tempNode->Parent;
			}
		}
		if (tempNode != NULL)
		{
			tempNode = tempNode->Next;
		}
	}
	return firstPath;
}

void AddStartNodeToCloseList(const Node* startNode, CloseList* closeList)
{
	NodeAStar* newElement = (NodeAStar*)calloc(1, sizeof(NodeAStar));
	newElement->Node = (Node*)startNode;
	closeList->FirstNode = newElement;
}

NodeList* PathAStar(const Node* startNode, const Node* destinationNode)
{
	if (destinationNode == startNode || destinationNode == NULL || startNode == NULL)
	{
		NodeList* temp = NULL;
		return temp;
	}
	PathFindingNodeList nodeList = { 0 };
	AddStartNodeToCloseList(destinationNode, &nodeList.closeList);
	AddNeighbourToOpenList(nodeList.closeList.FirstNode, &nodeList.openList, &nodeList.closeList, startNode);
	sfBool haveFindThePath = CheckIfFindThePath(nodeList.openList, startNode);
	while (!haveFindThePath && nodeList.openList.FirstNode != NULL)
	{
		NodeAStar* tempNode = nodeList.openList.FirstNode;
		NodeAStar* nodeToCheck = nodeList.openList.FirstNode;
		while (tempNode != NULL)
		{
			if ((tempNode->Fcost < nodeToCheck->Fcost) || (tempNode->Fcost == nodeToCheck->Fcost && tempNode->Heuristic < nodeToCheck->Heuristic))
			{
				nodeToCheck = tempNode;
			}
			tempNode = tempNode->Next;
		}
		AddNeighbourToOpenList(nodeToCheck, &nodeList.openList, &nodeList.closeList, startNode);
		AddNodeInCloseList(nodeToCheck, &nodeList.openList, &nodeList.closeList);
		haveFindThePath = CheckIfFindThePath(nodeList.openList, startNode);
	}
	NodeList* path = SetPath(&nodeList.closeList);
	CleanNodeList(&nodeList);

	return path;
}


void CleanNodeList(PathFindingNodeList* nodeList)
{
	NodeAStar* tempNode = nodeList->openList.FirstNode;
	while (tempNode != NULL)
	{
		NodeAStar* nodeToDelete = tempNode;
		tempNode = tempNode->Next;
		free(nodeToDelete);
		nodeToDelete = NULL;
	}
	tempNode = nodeList->closeList.FirstNode;
	while (tempNode != NULL)
	{
		NodeAStar* nodeToDelete = tempNode;
		tempNode = tempNode->Next;
		free(nodeToDelete);
		nodeToDelete = NULL;
	}
}