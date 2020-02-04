#ifndef _POINTLIGHT_H
#define _POINTLIGHT_H

#include "LightController.h"

typedef struct POINTLIGHT
{
	int Index;
	Light Data;
}PointLight;

void AddPointLight(const sfGlslVec4 color, const sfVector2f pos, const Node* node, const int index);

void ActivatePointLight(PointLight* ptLight, const char enable);

Light* GetPointLight(const int index);

PointLight* GetPointLights();

int GetNumberPointLights();

void ClearPointLights();
#endif