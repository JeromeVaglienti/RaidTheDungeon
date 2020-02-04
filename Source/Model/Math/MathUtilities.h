#ifndef MATH_H
#define MATH_H
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <SFML\System.h>
#include "Vector\VectorUtilities.h"

#define M_PI 3.14159265358979323846f
#define M_PI_2 M_PI/2

float Lerp(const float pStartValue, const float pEndValue, const float pChangeValue);

sfVector2f LerpVector2f(const sfVector2f pStartValue, const sfVector2f pEndValue, const float pChangeValue);

float DegreeToRadian(const float pAngle);

float RadianToDegree(const float pRadian);
//Return -1 if value is shorter than 0, return 1 if value is greater than 0 and return 0 if value is equal to 0 
int Sign(const float pValue);
//Set and return pX between a min value and a max value
float Clamp(float pX, const float pMin, const float pMax);
// check if an angle between two point is included in a cone with a predefined angle 
// angle is the angle of the cone and coneAngle is the cone width
int CheckAngle(const sfVector2f a, const sfVector2f b, const float angle, const float coneAngle);
//Compare two float if f1 == f2 return 1 else 0
int Comparefloat(const float f1, const float f2);

sfVector2f EllipticalTrajectory(const float deltaTime, const sfVector2f startPos, const sfVector2f endPos, const int directionRotate);

#endif