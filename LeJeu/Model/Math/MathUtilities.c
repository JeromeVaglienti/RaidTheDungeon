#include "MathUtilities.h"

float Lerp(const float pStartValue, const float pEndValue, const float pChangeValue)
{
	return  pStartValue + pChangeValue * (pEndValue - pStartValue);
}

sfVector2f LerpVector2f(const sfVector2f pStartValue, const sfVector2f pEndValue, const float pChangeValue)
{

	sfVector2f pos = pStartValue;
	pos.x += (pEndValue.x - pStartValue.x) * pChangeValue;
	pos.y += (pEndValue.y - pStartValue.y) * pChangeValue;
	return pos;
}

float DegreeToRadian(const float pRadian)
{
	return (pRadian* M_PI) / 180.0f;
}

float RadianToDegree(const float pAngle)
{
	return (180.0f*pAngle) / M_PI;
}

int Sign(const float pValue)
{
	int sign = 0;

	if (pValue > 0.0f)
	{
		sign = 1;
	}
	else if (pValue < 0.0f)
	{
		sign = -1;
	}

	return sign;
}

float Clamp(float pX, const float pMin, const float pMax)
{
	if (pX < pMin)
	{
		pX = pMin;
	}
	else if (pX > pMax)
	{
		pX = pMax;
	}

	return pX;
}

int CheckAngle(const sfVector2f a, const sfVector2f b, const float angle, const float coneAngle)
{
	float angleAB = RadianToDegree(atan2f(b.y - a.y, b.x - a.x)) + 90;
	if (fabsf(angle - angleAB) <= coneAngle / 2)
	{
		return 1;
	}
	else if (fabsf((float)(((int)angle + 50) % 360) - (float)(((int)angleAB + 50) % 360)) <= coneAngle / 2)
	{
		return 1;
	}
	return 0;
}

int Comparefloat(const float f1, const float f2)
{
	float precision = 0.00001f;
	if (((f1 - precision) < f2) &&
		((f1 + precision) > f2))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

sfVector2f EllipticalTrajectory(const float deltaTime, const sfVector2f startPos, const sfVector2f endPos, const int directionRotate)
{
	sfVector2f pos = startPos;
	sfVector2f tempV = AddTwoVectors(startPos, endPos);
	float tempAngle = -atan2f(tempV.y, tempV.x) + 3.1415f;
	float distance = Magnitude(tempV) / 2.f;
	pos.x = startPos.x + distance * (cosf(deltaTime * directionRotate - tempAngle));
	pos.y = startPos.y + distance * (sinf(deltaTime * directionRotate - tempAngle));
	pos.x += tempV.x / 2.f;
	pos.y += tempV.y / 2.f;

	return pos;
}

//sfVector2f ElypticalTrajectory(const float deltaTime, float A, float B)
//{
//	sfVector2f pos;
//	pos.x = A * cosf(deltaTime);
//	pos.y = B * sinf(deltaTime);
//
//	return pos;
//}