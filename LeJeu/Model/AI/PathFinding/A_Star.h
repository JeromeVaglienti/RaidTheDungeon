#ifndef ASTAR_H
#define ASTAR_H
#include <stdio.h>
#include <stdlib.h>
#include "..\..\Node\Node.h"

NodeList* PathAStar(const Node* StartNode, const Node* destinationNode);

#endif // !ASTAR_H
