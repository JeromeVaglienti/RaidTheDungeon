#include "VectorUtilities.h"

sfVector2f dir[directionalNumber] = { 0 };
///Return a new sfVector2f AB 
sfVector2f AddTwoVectors(const sfVector2f pVectorA, const sfVector2f pVectorB)
{
	sfVector2f newVector;
	newVector.x = pVectorB.x - pVectorA.x;
	newVector.y = pVectorB.y - pVectorA.y;
	return newVector;
}

///Return the dot in float of the dot product between two vectors
float Dot(const sfVector2f pVectorA, const sfVector2f pVectorB)
{
	return pVectorA.x*pVectorB.x + pVectorA.y*pVectorB.y;
}

///Return a new sfVector2f of the cross product between two vectors
sfVector2f Cross(const sfVector2f pVectorA, const sfVector2f pVectorB)
{
	sfVector2f newVector;
	newVector.x = pVectorA.x*pVectorB.y - pVectorA.y*pVectorB.x;

	newVector.y = 0;
	return newVector;
}

///Return a Magnitude in float
float Magnitude(const sfVector2f pVector)
{
	return sqrtf(pVector.x * pVector.x + pVector.y * pVector.y);
}

///Return a new sfVector2f normalized 
sfVector2f Normalize(const sfVector2f pVector)
{
	sfVector2f newVector = { 0.0f, 0.0f };

	float norm = Magnitude(pVector);
	if (norm > 0.000001f)
	{
		newVector.x = pVector.x / norm;
		newVector.y = pVector.y / norm;
	}
	return newVector;
}

///Return the perdicular
sfVector2f Perpendicular(const sfVector2f pVector)
{
	return (sfVector2f) { pVector.y, -pVector.x };
}

float Distance(const sfVector2f point1, const sfVector2f point2)
{
	sfVector2f newVector = AddTwoVectors(point1, point2);
	return sqrtf(newVector.x * newVector.x + newVector.y * newVector.y);
}

// rotate with polar coordonate around the 0;0
// angle is in radian
sfVector2f Rotate(const sfVector2f v, const float angle)
{
	sfVector2f nextV = v;
	float cosAngle = cosf(angle);
	float sinAngle = sinf(angle);
	nextV.x = v.x * cosAngle + v.y * sinAngle;
	nextV.y = v.y * cosAngle - v.x * sinAngle;
	return nextV;
}

// rotate with polar coordonate around a point
// angle is in radian
sfVector2f RotateAround(sfVector2f v, const sfVector2f pivot, const float angle)
{
	v.x -= pivot.x;
	v.y -= pivot.y;
	v = Rotate(v, angle);
	v.x += pivot.x;
	v.y += pivot.y;

	return v;
}

///Return if [A,B] and [A2, B2] are intersecting or not
sfBool IsIntersect(const sfVector2f A, const sfVector2f B, const sfVector2f A2, const sfVector2f B2, sfVector2f* CoordInter)
{
	if (CoordInter != NULL)
	{
		//No Intersection;
		CoordInter->x = -1;
		CoordInter->y = -1;
	}
	if (A.x == A2.x && A.y == A2.y || A.x == B2.x && A.y == B2.y ||
		B.x == A2.x && B.y == A2.y || B.x == B2.x && B.y == B2.y)
	{
		return sfFalse;
	}

	sfVector2f vertex1 = { B.x - A.x, B.y - A.y };
	sfVector2f vertex2 = { B2.x - A2.x, B2.y - A2.y };

	// mx + b => equation of line
	// m = AB.y/AB.x => directive coefficient of the line 
	float m;
	float b;
	//Means that the vertex1 is parallel with the ordinate axis
	sfBool v1Vertical = sfFalse;
	if (vertex1.x == 0)
	{
		v1Vertical = sfTrue;
	}
	else
	{
		m = vertex1.y / vertex1.x;
		b = (-m)*A.x + A.y;
	}

	// mx + b => equation of line
	// m = AB.y/AB.x => directive coefficient of the line 
	float m2;
	float b2;
	//Means that the vertex2 is parallal with the ordonné axis
	sfBool v2Vertical = sfFalse;
	if (vertex2.x == 0)
	{
		v2Vertical = sfTrue;
	}
	else
	{
		m2 = vertex2.y / vertex2.x;
		b2 = (-m2)*A2.x + A2.y;
	}


	sfVector2f u, v;
	sfVector2f u2, v2;
	float x, y;

	if (v2Vertical && v1Vertical)
	{
		//Check if both are parallel to ordinate if they are on the same x
		if (A.x == A2.x)
		{
			sfVector2f tempA = { A.x - A2.x, A.y - A2.y };
			sfVector2f tempB = { A.x - B2.x, A.y - B2.y };
			sfVector2f tempA2 = { B.x - A2.x, B.y - A2.y };
			sfVector2f tempB2 = { B.x - B2.x, B.y - B2.y };

			if (Sign(tempA.y) != Sign(tempB.y) || Sign(tempA2.y) != Sign(tempB2.y))
			{
				if (CoordInter != NULL)
				{
					//-2 means there is two points of intersect and therefore the two points needs to be calculated elsewhere
					CoordInter->x = -2;
					CoordInter->y = -2;
				}
				return(sfTrue);
			}
		}
		return(sfFalse);
	}
	else if (!v2Vertical && !v1Vertical)
	{
		//Separing x and num on each sides
		float xSide = m - m2;
		float numSide = b2 - b;

		//Getting the x of the intersection
		if (xSide == 0)
		{
			x = 1;
		}
		else
		{
			x = numSide / xSide;
		}

		//Getting the y of the intersection
		y = m * x + b;

		u.x = A.x - x;
		u.y = A.y - y;
		v.x = B.x - x;
		v.y = B.y - y;

		u2.x = A2.x - x;
		u2.y = A2.y - y;
		v2.x = B2.x - x;
		v2.y = B2.y - y;
	}
	else
	{
		if (v1Vertical)
		{
			x = A.x;
			y = m2 * x + b2;
		}
		else
		{
			x = A2.x;
			y = m * x + b;
		}

		u.x = A.x - x;
		u.y = A.y - y;
		v.x = B.x - x;
		v.y = B.y - y;

		u2.x = A2.x - x;
		u2.y = A2.y - y;
		v2.x = B2.x - x;
		v2.y = B2.y - y;
	}

	//Check if sign of two vectors are oppose
	if (Sign(u.x) != 0 && Sign(u.x) == Sign(v.x) || Sign(u.y) != 0 && Sign(u.y) == Sign(v.y))
	{
		return sfFalse;
	}
	else if (Sign(u2.x) != 0 && Sign(u2.x) == Sign(v2.x) || Sign(u2.y) != 0 && Sign(u2.y) == Sign(v2.y))
	{
		return sfFalse;
	}

	if (CoordInter != NULL)
	{
		CoordInter->x = x;
		CoordInter->y = y;
	}
	return sfTrue;
}

sfBool IsIntersectIntToFloat(const sfVector2i A, const sfVector2i B, const sfVector2i A2, const sfVector2i B2, sfVector2f* CoordInter)
{
	sfVector2f Af = { (float)A.x, (float)A.y };
	sfVector2f Bf = { (float)B.x, (float)B.y };
	sfVector2f A2f = { (float)A2.x, (float)A2.y };
	sfVector2f B2f = { (float)B2.x, (float)B2.y };
	sfBool temp = IsIntersect(Af, Bf, A2f, B2f, CoordInter);
	return(temp);
}

void InitDirectionalArray()
{
	sfVector2f temp = { cosf(0.f), sinf(0.f) };
	for (int i = 0; i < directionalNumber; i++)
	{
		float angle = DegreeToRadian((float)i);
		dir[i] = (sfVector2f) { cosf(angle), sinf(angle) };
	}
	dir[directionalNumber - 1] = dir[0];
}

sfVector2f* GetDirectionalArray()
{
	return dir;
}