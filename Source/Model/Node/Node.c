#include "Node.h"
#include "..\Math\MathUtilities.h"
#include "..\..\Model\Player\Player.h"


Node* GetNeighboursWithCoord(const float coordX, const  float coordY, const Node actualNode)
{
	for (int i = 0; i < 8; i++)
	{
		if (actualNode.edges[i]->posX == coordX && actualNode.edges[i]->posY == coordY)
		{
			return(actualNode.edges[i]);
		}
	}
	return NULL;
}

//Find the index of the direction in order to connect two nodes with each others
int GetDirectionBetWeenTwoNodes(const float coordXA, const  float coordYA, const  float coordXB, const float coordYB)
{
	sfVector2f Diff = { coordXB - coordXA, -(coordYB - coordYA) };
	//Diff = Normalize(Diff);
	float angle = RadianToDegree(atan2f(Diff.y, Diff.x));

	if (angle < 0)
	{
		angle += 360.f;
	}

	int value = (int)(roundf(angle / 45.f));
	value %= 8;

	return value;
}


Node *GetClosestNodeInArray(float coordX, float coordY, Node* nodeArray)
{
	if (nodeArray == NULL)
	{
		return NULL;
	}

	coordX /= 64;
	coordY /= 64;

	coordX = floorf(coordX);
	coordY = floorf(coordY);

	coordX += 0.5f;
	coordY += 0.5f;

	float tempX = nodeArray->posX;
	float tempY = nodeArray->posY;

	Node* temp = nodeArray;
	if (nodeArray->posX == coordX && nodeArray->posY == coordY)
	{
		return(nodeArray);
	}
	do
	{
		sfVector2f Diff = { coordX - temp->posX, -(coordY - temp->posY) };
		//Diff = Normalize(Diff);
		float angle = RadianToDegree(atan2f(Diff.y, Diff.x));

		if (angle < 0)
		{
			angle += 360.f;
		}

		int value = (int)(roundf(angle / 45.f));
		value %= 8;

		if (temp->edges[value] == NULL)
		{
			return (temp);
		}
		temp = temp->edges[value];
	} while (temp->posX != coordX || temp->posY != coordY);

	return(temp);
}

Node* GetNodeInArray(float coordX, float coordY, Node* nodeArray)
{
	if (nodeArray == NULL)
	{
		return NULL;
	}

	coordX /= 64;
	coordY /= 64;

	coordX = floorf(coordX);
	coordY = floorf(coordY);

	coordX += 0.5f;
	coordY += 0.5f;

	float tempX = nodeArray->posX;
	float tempY = nodeArray->posY;

	Node* temp = nodeArray;
	if (nodeArray->posX == coordX && nodeArray->posY == coordY)
	{
		return(nodeArray);
	}
	do
	{
		sfVector2f Diff = { coordX - temp->posX, -(coordY - temp->posY) };
		//Diff = Normalize(Diff);
		float angle = RadianToDegree(atan2f(Diff.y, Diff.x));

		if (angle < 0)
		{
			angle += 360.f;
		}

		int value = (int)(roundf(angle / 45.f));
		value %= 8;

		if (temp->edges[value] == NULL)
		{
			return NULL;
		}
		temp = temp->edges[value];
	} while (temp->posX != coordX && temp->posY != coordY);

	return(temp);
}

sfBool CheckNextNode(Node* ActualNode, sfVector2f* Direction, const sfVector2f Position, const sfBool AlterateDirection)
{
	if (ActualNode == NULL)
	{
		return sfTrue;
	}
	if (Direction->x == 0 && Direction->y == 0)
	{
		return(sfFalse);
	}

	float tempX = fabsf((Position.x / 64.0f) - ActualNode->posX);
	float tempY = fabsf(((Position.y + (1 / 3.0f)* PLAYERHEIGHT) / 64.0f) - ActualNode->posY);

	if (AlterateDirection)
	{
		if (tempX < 0.15f && tempY < 0.15f)
		{
			return(sfTrue);
		}
		else if (Sign(Direction->x) != Sign((Position.x / 64.0f) - ActualNode->posX) && Sign(Direction->y) != Sign(((Position.y + (1 / 3.0f)* PLAYERHEIGHT) / 64.0f) - ActualNode->posY))
		{
			return(sfTrue);
		}
	}

	float angle = RadianToDegree(atan2f(-Direction->y, Direction->x));

	if (angle < 0)
	{
		angle += 360.f;
	}

	int value = (int)(roundf(angle / 45.f));
	value %= 8;

	if (ActualNode->edges[value] == NULL)
	{
		return(sfFalse);
	}

	if (!AlterateDirection)
	{
		return(ActualNode->edges[value]->isWalkable);
	}

	if (ActualNode->edges[value]->isWalkable)
	{
		if (Direction->x != 0.0f && Direction->y != 0.0f)
		{
			sfVector2f xDir = { 1.0f * (Direction->x / fabsf(Direction->x)), 0.0f };
			sfVector2f yDir = { 0.0f, 1.0f * (Direction->y / fabsf(Direction->y)) };
			sfBool checkX = CheckNextNode(ActualNode, &xDir, Position, sfFalse);
			sfBool checkY = CheckNextNode(ActualNode, &yDir, Position, sfFalse);

			if (checkX && checkY)
			{
				return(sfTrue);
			}
			else if (checkX && !checkY)
			{
				Direction->x = xDir.x;
				Direction->y = xDir.y;
				return(sfTrue);
			}
			else if (!checkX && checkY)
			{
				Direction->x = yDir.x;
				Direction->y = yDir.y;
				return(sfTrue);
			}
			else
			{
				return(sfFalse);
			}
		}
		else
		{
			return(sfTrue);
		}
	}
	else
	{
		if (Direction->x != 0.0f && Direction->y != 0.0f)
		{
			sfVector2f xDir = { 1.0f * (Direction->x / fabsf(Direction->x)), 0.0f };
			sfVector2f yDir = { 0.0f, 1.0f * (Direction->y / fabsf(Direction->y)) };

			sfBool checkX = CheckNextNode(ActualNode, &xDir, Position, sfFalse);
			sfBool checkY = CheckNextNode(ActualNode, &yDir, Position, sfFalse);

			if (checkX)
			{
				Direction->x = xDir.x;
				Direction->y = xDir.y;
				return(sfTrue);
			}
			else if (checkY)
			{
				Direction->x = yDir.x;
				Direction->y = yDir.y;
				return(sfTrue);
			}
			else
			{
				return(sfFalse);
			}
		}
		else
		{
			return(sfFalse);
		}
	}
}

sfBool CheckNextNodeP(Node* ActualNode, RigidBody* rb, const sfVector2f Position, const sfBool AlterateDirection)
{
	sfVector2f Direction;

	sfVector2f vec = (sfVector2f) { rb->Velocity.x + rb->KnockBackForce.x + rb->Friction.x, rb->Velocity.y + rb->KnockBackForce.y + rb->Friction.y };
	Direction = Normalize(vec);

	if (ActualNode == NULL)
	{
		return sfTrue;
	}
	if (Direction.x == 0 && Direction.y == 0)
	{
		rb->Velocity.x = 0.0f;
		rb->Velocity.y = 0.0f;
		return(sfFalse);
	}

	float tempX = fabsf((Position.x / 64.0f) - ActualNode->posX);
	float tempY = fabsf(((Position.y + (1 / 3.0f)* PLAYERHEIGHT) / 64.0f) - ActualNode->posY);

	if (AlterateDirection)
	{
		if (tempX < 0.15f && tempY < 0.15f)
		{
			return(sfTrue);
		}
		else if (Sign(Direction.x) != Sign((Position.x / 64.0f) - ActualNode->posX) && Sign(Direction.y) != Sign(((Position.y + (1 / 3.0f)* PLAYERHEIGHT) / 64.0f) - ActualNode->posY))
		{
			return(sfTrue);
		}
	}

	float angle = RadianToDegree(atan2f(-Direction.y, Direction.x));

	if (angle < 0)
	{
		angle += 360.f;
	}

	int value = (int)(roundf(angle / 45.f));
	value %= 8;

	if (ActualNode->edges[value] == NULL)
	{
		rb->KnockBackForce.x = 0.0f;
		rb->KnockBackForce.y = 0.0f;
		rb->Friction.x = 0.0f;
		rb->Friction.y = 0.0f;
		rb->Velocity.x = 0.0f;
		rb->Velocity.y = 0.0f;
		return(sfFalse);
	}

	if (!AlterateDirection)
	{
		return(ActualNode->edges[value]->isWalkable);
	}

	if (ActualNode->edges[value]->isWalkable)
	{
		if (Direction.x != 0.0f && Direction.y != 0.0f)
		{
			sfVector2f xDir = { 1.0f * (Direction.x / fabsf(Direction.x)), 0.0f };
			sfVector2f yDir = { 0.0f, 1.0f * (Direction.y / fabsf(Direction.y)) };
			sfBool checkX = CheckNextNode(ActualNode, &xDir, Position, sfFalse);
			sfBool checkY = CheckNextNode(ActualNode, &yDir, Position, sfFalse);

			if (checkX && checkY)
			{
				return(sfTrue);
			}
			else if (checkX && !checkY)
			{
				rb->KnockBackForce.y = 0.0f;
				rb->Friction.y = 0.0f;
				rb->Velocity.y = 0.0f;
				return(sfTrue);
			}
			else if (!checkX && checkY)
			{
				rb->KnockBackForce.x = 0.0f;
				rb->Friction.x = 0.0f;
				rb->Velocity.x = 0.0f;
				return(sfTrue);
			}
			else
			{
				rb->KnockBackForce.x = 0.0f;
				rb->KnockBackForce.y = 0.0f;
				rb->Friction.x = 0.0f;
				rb->Friction.y = 0.0f;
				rb->Velocity.x = 0.0f;
				rb->Velocity.y = 0.0f;
				return(sfFalse);
			}
		}
		else
		{
			return(sfTrue);
		}
	}
	else
	{

		if (Direction.x != 0.0f && Direction.y != 0.0f)
		{
			sfVector2f xDir = { 1.0f * (Direction.x / fabsf(Direction.x)), 0.0f };
			sfVector2f yDir = { 0.0f, 1.0f * (Direction.y / fabsf(Direction.y)) };

			sfBool checkX = CheckNextNode(ActualNode, &xDir, Position, sfFalse);
			sfBool checkY = CheckNextNode(ActualNode, &yDir, Position, sfFalse);

			if (checkX)
			{
				rb->KnockBackForce.y = 0.0f;
				rb->Friction.y = 0.0f;
				rb->Velocity.y = 0.0f;
				return(sfTrue);
			}
			else if (checkY)
			{
				rb->KnockBackForce.x = 0.0f;
				rb->Friction.x = 0.0f;
				rb->Velocity.x = 0.0f;
				return(sfTrue);
			}
			else
			{
				rb->KnockBackForce.x = 0.0f;
				rb->KnockBackForce.y = 0.0f;
				rb->Friction.x = 0.0f;
				rb->Friction.y = 0.0f;
				rb->Velocity.x = 0.0f;
				rb->Velocity.y = 0.0f;
				return(sfFalse);
			}
		}
		else
		{
			rb->KnockBackForce.x = 0.0f;
			rb->KnockBackForce.y = 0.0f;
			rb->Friction.x = 0.0f;
			rb->Friction.y = 0.0f;
			rb->Velocity.x = 0.0f;
			rb->Velocity.y = 0.0f;
			return(sfFalse);
		}
	}
}

sfBool CheckNextNodeE(Node* ActualNode, RigidBody* rb, const sfVector2f Position, const sfBool AlterateDirection)
{
	sfVector2f Direction;

	sfVector2f vec = (sfVector2f) { rb->Velocity.x + rb->KnockBackForce.x + rb->Friction.x, rb->Velocity.y + rb->KnockBackForce.y + rb->Friction.y };
	Direction = Normalize(vec);

	if (ActualNode == NULL)
	{
		return sfTrue;
	}
	if (Direction.x == 0 && Direction.y == 0)
	{
		rb->Velocity.x = 0.0f;
		rb->Velocity.y = 0.0f;
		return(sfFalse);
	}

	float tempX = fabsf((Position.x / 64.0f) - ActualNode->posX);
	float tempY = fabsf(((Position.y + (1 / 3.0f)* PLAYERHEIGHT) / 64.0f) - ActualNode->posY);

	if (AlterateDirection)
	{
		if (tempX < 0.15f && tempY < 0.15f)
		{
			return(sfTrue);
		}
		else if (Sign(Direction.x) != Sign((Position.x / 64.0f) - ActualNode->posX) && Sign(Direction.y) != Sign(((Position.y + (1 / 3.0f)* PLAYERHEIGHT) / 64.0f) - ActualNode->posY))
		{
			return(sfTrue);
		}
	}

	float angle = RadianToDegree(atan2f(-Direction.y, Direction.x));

	if (angle < 0)
	{
		angle += 360.f;
	}

	int value = (int)(roundf(angle / 45.f));
	value %= 8;

	if (ActualNode->edges[value] == NULL)
	{
		rb->KnockBackForce.x = 0.0f;
		rb->KnockBackForce.y = 0.0f;
		rb->Friction.x = 0.0f;
		rb->Friction.y = 0.0f;
		rb->Velocity.x = 0.0f;
		rb->Velocity.y = 0.0f;
		return(sfFalse);
	}

	if (!AlterateDirection)
	{
		return(ActualNode->edges[value]->isWalkable);
	}

	if (ActualNode->edges[value]->isWalkable)
	{
		if (Direction.x != 0.0f && Direction.y != 0.0f)
		{
			sfVector2f xDir = { 1.0f * (Direction.x / fabsf(Direction.x)), 0.0f };
			sfVector2f yDir = { 0.0f, 1.0f * (Direction.y / fabsf(Direction.y)) };
			sfBool checkX = CheckNextNode(ActualNode, &xDir, Position, sfFalse);
			sfBool checkY = CheckNextNode(ActualNode, &yDir, Position, sfFalse);

			if (checkX && checkY)
			{
				return(sfTrue);
			}
			else if (checkX && !checkY)
			{
				rb->KnockBackForce.y = 0.0f;
				rb->Friction.y = 0.0f;
				rb->Velocity.y = 0.0f;
				return(sfTrue);
			}
			else if (!checkX && checkY)
			{
				rb->KnockBackForce.x = 0.0f;
				rb->Friction.x = 0.0f;
				rb->Velocity.x = 0.0f;
				return(sfTrue);
			}
			else
			{
				rb->KnockBackForce.x = 0.0f;
				rb->KnockBackForce.y = 0.0f;
				rb->Friction.x = 0.0f;
				rb->Friction.y = 0.0f;
				rb->Velocity.x = 0.0f;
				rb->Velocity.y = 0.0f;
				return(sfFalse);
			}
		}
		else
		{
			return(sfTrue);
		}
	}
	else
	{

		if (Direction.x != 0.0f && Direction.y != 0.0f)
		{
			sfVector2f xDir = { 1.0f * (Direction.x / fabsf(Direction.x)), 0.0f };
			sfVector2f yDir = { 0.0f, 1.0f * (Direction.y / fabsf(Direction.y)) };

			sfBool checkX = CheckNextNode(ActualNode, &xDir, Position, sfFalse);
			sfBool checkY = CheckNextNode(ActualNode, &yDir, Position, sfFalse);

			if (checkX)
			{
				rb->KnockBackForce.y = 0.0f;
				rb->Friction.y = 0.0f;
				rb->Velocity.y = 0.0f;
				return(sfTrue);
			}
			else if (checkY)
			{
				rb->KnockBackForce.x = 0.0f;
				rb->Friction.x = 0.0f;
				rb->Velocity.x = 0.0f;
				return(sfTrue);
			}
			else
			{
				rb->KnockBackForce.x = 0.0f;
				rb->KnockBackForce.y = 0.0f;
				rb->Friction.x = 0.0f;
				rb->Friction.y = 0.0f;
				rb->Velocity.x = 0.0f;
				rb->Velocity.y = 0.0f;
				return(sfFalse);
			}
		}
		else
		{
			rb->KnockBackForce.x = 0.0f;
			rb->KnockBackForce.y = 0.0f;
			rb->Friction.x = 0.0f;
			rb->Friction.y = 0.0f;
			rb->Velocity.x = 0.0f;
			rb->Velocity.y = 0.0f;
			return(sfFalse);
		}
	}
}
//PlayerSpecific
void ChangeNodePlayer(Node** ActualNode, const sfVector2f Position)
{
	float tempX = (Position.x / 64.0f);
	float tempY = ((Position.y + ((1 / 3.0f) * PLAYERHEIGHT)) / 64.0f);

	if (fabsf(tempX - (*ActualNode)->posX) > .5f || fabsf(tempY - (*ActualNode)->posY) > .5f)
	{
		*ActualNode = GetNodeInArray(Position.x, Position.y + ((1 / 3.0f) * PLAYERHEIGHT), *ActualNode);
	}
}

void ChangeNode(Node** ActualNode, const sfVector2f Position)
{
	float tempX = (Position.x / 64.0f);
	float tempY = (Position.y / 64.0f);

	if (fabsf(tempX - (*ActualNode)->posX) > .5f || fabsf(tempY - (*ActualNode)->posY) > .5f)
	{
		*ActualNode = GetNodeInArray(Position.x, Position.y, *ActualNode);
	}
}

void CleanNodeListNode(NodeList **ToClean)
{
	NodeList *tempNode = *ToClean;
	while (tempNode != NULL)
	{
		NodeList* nodeToDelete = tempNode;
		tempNode = tempNode->Next;
		free(nodeToDelete);
		nodeToDelete = NULL;
	}
	*ToClean = NULL;
}

sfBool HaveObstacleBetween(const Node* firstNode, const Node* secondNode, const sfBool forPlayer, const sfBool forEnemy, const sfBool forShot)
{
	if (firstNode == NULL || secondNode == NULL)
	{
		return sfTrue;
	}

	sfVector2f direction = Normalize(AddTwoVectors((sfVector2f) { firstNode->posX, firstNode->posY }, (sfVector2f) { secondNode->posX, secondNode->posY }));
	sfVector2f tempPos = { firstNode->posX * 64.f, firstNode->posY * 64.f };
	Node* tempNode = (Node*)firstNode;
	while (tempNode != secondNode)
	{
		if (forPlayer && !tempNode->isWalkable)
		{
			return sfTrue;
		}
		if (forEnemy && !tempNode->isWalkableForMob)
		{
			return sfTrue;
		}
		if (forShot && !tempNode->isShootable && !tempNode->isWalkable)
		{
			return sfTrue;
		}
		tempNode = GetNodeInArray(tempPos.x, tempPos.y, tempNode);
		tempPos.x += 32.f * direction.x;
		tempPos.y += 32.f * direction.y;
	}

	return sfFalse;
}

Node* SearchWalkableNodeAround(Node* actualNode)
{
	if (actualNode->isWalkable)
	{
		return actualNode;
	}
	for (int i = 0; i < 8; i++)
	{
		if (actualNode->edges[i] != NULL && actualNode->edges[i]->isWalkable)
		{
			return actualNode->edges[i];
		}
	}
	int random;
	do
	{
		random = rand() % 8;
	} while (actualNode->edges[random] == NULL || actualNode->edges[random]->isEndOfWorld);
	return SearchWalkableNodeAround(actualNode->edges[random]);
}

Node* SearchWalkableNodeForMobAround(Node* actualNode)
{
	if (actualNode->isWalkableForMob)
	{
		return actualNode;
	}
	for (int i = 0; i < 8; i++)
	{
		if (actualNode->edges[i] != NULL && actualNode->edges[i]->isWalkableForMob)
		{
			return actualNode->edges[i];
		}
	}
	int random;
	do
	{
		random = rand() % 8;
	} while (actualNode->edges[random] == NULL || actualNode->edges[random]->isEndOfWorld);
	return SearchWalkableNodeAround(actualNode->edges[random]);
}