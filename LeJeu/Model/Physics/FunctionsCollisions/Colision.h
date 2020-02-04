#ifndef _COLISION_H
#define _COLISION_H

#include <stdio.h>
#include <stdlib.h>
#include "..\..\Math\MathUtilities.h"

typedef struct
{
	sfBool IsCollided;
	sfVector2f Normal;
	sfVector2f Point;
}Hit;

typedef struct
{
	sfVector2f center;
	float radius;
}Circle;

typedef struct
{
	sfVector2f point1;
	sfVector2f point2;
	sfVector2f direction;
}Segment;

typedef struct
{
	sfVector2f point;
	sfVector2f direction;
}StraightLine;


Segment CreateSegment(const sfVector2f point1, const sfVector2f point2);

Hit IsCollidingPointCircle(const sfVector2f point, const Circle circle);

Hit IsCollidingCircleCircle(const Circle circle1, const Circle circle2);

Hit IsCollindingCircleSegment(const Circle circle, const Segment segment);

Hit IsCollindingCircleLine(const Circle circle, const StraightLine line);

Hit IsCollidingCircleSquare(const Circle circle, const sfVector2f posSquare, const sfVector2f sizeSquare);

sfVector2f NearestPointOfVector(const Circle circle, const sfVector2f point1, sfVector2f direction);

#endif
