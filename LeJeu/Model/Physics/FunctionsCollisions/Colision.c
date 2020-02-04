#include "Colision.h"
#include <SFML/Window.h>
#include <SFML/Graphics.h>

Segment CreateSegment(const sfVector2f point1, const sfVector2f point2)
{
	Segment segment;
	segment.point1 = point1;
	segment.point2 = point2;
	segment.direction = Normalize((sfVector2f) { point2.x - point1.x, point2.y - point1.y });
	return segment;
}

Hit IsCollidingPointCircle(const sfVector2f point, const Circle circle)
{
	Hit hit;
	hit.Normal.x = circle.center.x - point.x;
	hit.Normal.y = circle.center.y - point.y;
	hit.Normal = Normalize(hit.Normal);
	hit.Point = point;
	hit.IsCollided = Distance(point, circle.center) < circle.radius;
	return hit;
}

Hit IsCollidingCircleCircle(const Circle circle1, const Circle circle2)
{
	Hit hit;
	float sumRadius = circle1.radius + circle2.radius;
	hit.IsCollided = Distance(circle1.center, circle2.center) < sumRadius;
	hit.Normal = Normalize((sfVector2f) { circle1.center.x - circle2.center.x, circle1.center.y - circle2.center.y });
	hit.Point.x = circle2.center.x + hit.Normal.x * circle2.radius;
	hit.Point.y = circle2.center.y + hit.Normal.y * circle2.radius;
	return hit;
}

Hit IsCollindingCircleSegment(const Circle circle, const Segment segment)
{
	sfVector2f v = { circle.center.x - segment.point1.x, circle.center.y - segment.point1.y };
	sfVector2f perpendicular = { segment.direction.y, -segment.direction.x };
	float dotP = Dot(v, perpendicular);
	Hit hit = { 0 };
	Hit hitLine;
	hitLine.IsCollided = dotP <= circle.radius;
	if (hitLine.IsCollided)
	{
		float dot = Dot(segment.direction, v);
		float norme = Distance(segment.point1, segment.point2);
		if (dot <= 0)
		{
			hit.IsCollided = IsCollidingPointCircle(segment.point1, circle).IsCollided;
			hit.Normal = Normalize(v);
			hit.Point = segment.point1;
		}
		else if (dot > 0 && dot < norme)
		{
			hit.IsCollided = hitLine.IsCollided;
			hit.Normal = perpendicular;
			hit.Point.x = circle.center.x - perpendicular.x * dotP;
			hit.Point.y = circle.center.y - perpendicular.y * dotP;
		}
		else if (dot >= norme)
		{
			hit.IsCollided = IsCollidingPointCircle(segment.point2, circle).IsCollided;
			hit.Normal = Normalize((sfVector2f) { circle.center.x - segment.point2.x, circle.center.y - segment.point2.y });
			hit.Point = segment.point2;
		}
	}
	return hit;
}

sfVector2f NearestPointOfVector(const Circle circle, const sfVector2f point1, sfVector2f direction)
{
	sfVector2f v = { circle.center.x - point1.x, circle.center.y - point1.y };
	direction = Normalize(direction);
	if (Distance(circle.center, point1) <= 3)
	{
		sfVector2f tempPos = circle.center;
		tempPos.x += direction.x * 10;
		tempPos.y += direction.y * 10;
		return tempPos;
	}
	sfVector2f perpendicular = { direction.y, -direction.x };
	float dot = Dot(v, perpendicular);
	if (fabs(dot) <= circle.radius)
	{
		return circle.center;
	}
	sfVector2f point;
	point.x = circle.center.x - perpendicular.x * (dot / fabsf(dot)) * circle.radius;
	point.y = circle.center.y - perpendicular.y * (dot / fabsf(dot)) * circle.radius;
	return point;
}

Hit IsCollindingCircleLine(const Circle circle, const StraightLine line)
{
	sfVector2f v = { circle.center.x - line.point.x, circle.center.y - line.point.y };
	sfVector2f perpendicular = { line.direction.y, -line.direction.x };

	Hit hit;
	hit.Normal = perpendicular;
	float dot = fabsf(Dot(v, perpendicular));
	hit.Point.x = circle.center.x - perpendicular.x * dot;
	hit.Point.y = circle.center.y - perpendicular.y * dot;
	hit.IsCollided = dot <= circle.radius;
	return hit;
}

Hit IsCollidingCircleSquare(const Circle circle, const sfVector2f posSquare, const sfVector2f sizeSquare)
{
	Hit hit = { 0 };


	sfVector2f distance = { (circle.center.x - posSquare.x, (circle.center.y - posSquare.y)) };
	if (fabsf(distance.x) <= sizeSquare.x / 2.f && fabsf(distance.x) <= sizeSquare.x / 2.f)
	{
		Circle tempCircle;
		tempCircle.center = posSquare;
		tempCircle.radius = sizeSquare.x;
		hit = IsCollidingCircleCircle(circle, tempCircle);
	}
	else
	{
		// Find the closest point to the circle within the rectangle
		float closestX = Clamp(circle.center.x, posSquare.x - sizeSquare.x / 2.f, posSquare.x + sizeSquare.x / 2.f);
		float closestY = Clamp(circle.center.y, posSquare.y - sizeSquare.y / 2.f, posSquare.y + sizeSquare.y / 2.f);

		// Calculate the distance between the circle's center and this closest point
		hit = IsCollidingPointCircle((sfVector2f) { closestX, closestY }, circle);

	}

	return hit;
}