#ifndef VECTOR_H
#define VECTOR_H
#include "..\MathUtilities.h"

#define directionalNumber 360

///Return a new sfVector2f AB 
sfVector2f AddTwoVectors(const sfVector2f pVectorA, const sfVector2f pVectorB);
///Return the dot in float of the dot product between two vectors
float Dot(const sfVector2f pVectorA, const sfVector2f pVectorB);
///Return a new sfVector2f of the cross product between two vectors
sfVector2f Cross(const sfVector2f pVectorA, const sfVector2f pVectorB);
///Return a Magnitude in float
float Magnitude(const sfVector2f pVector);
///Return a new sfVector2f normalized 
sfVector2f Normalize(const sfVector2f pVector);

float Distance(const sfVector2f point1, const sfVector2f point2);

sfVector2f Rotate(const sfVector2f v, const float angle);

sfVector2f RotateAround(sfVector2f v, const sfVector2f pivot, const float angle);

///Return if [A,B] and [A2, B2] are intersecting or not
sfBool IsIntersect(const sfVector2f A, const sfVector2f B, const sfVector2f A2, const sfVector2f B2, sfVector2f* CoordInter);

sfBool IsIntersectIntToFloat(const sfVector2i A, const sfVector2i B, const sfVector2i A2, const sfVector2i B2, sfVector2f* CoordInter);

void InitDirectionalArray();

sfVector2f* GetDirectionalArray();

#endif