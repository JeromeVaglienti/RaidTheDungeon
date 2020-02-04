#include "PointLight.h"

PointLight* array = NULL;
int nbPointLight = 0;

void InitPointLight(PointLight* ptLight, const sfGlslVec4 color, const sfVector2f pos, const Node* node, const int index)
{
	sfFloatRect tempRect = sfSprite_getLocalBounds(GetSpriteLight()[index]);
	sfVector2f sizeLight = (sfVector2f) { (float)tempRect.width / 2, (float)tempRect.height / 2 };
	ptLight->Index = -1;
	ptLight->Data.Pos = pos;
	ptLight->Data.LastPos = (sfVector2f) { -1 };
	ptLight->Data.ColorGl = color;
	ptLight->Data.PosGl = (sfGlslVec4) { 0.0f, 0.0f, GetDefaultLigth(), 1.f };
	ptLight->Data.enabled = sfFalse;
	ptLight->Data.array = sfVertexArray_create();
	sfVertexArray_setPrimitiveType(ptLight->Data.array, sfTriangleFan);
	ptLight->Data.Size = sizeLight;
	ptLight->Data.Node = (Node*)node;
	ptLight->Data.nbNodes = (int)(ceilf(tempRect.width / 64.0f));
	ptLight->Data.Radius = sizeLight.x;
	ptLight->Data.IndexSprite = index;
}

void ActivatePointLight(PointLight* ptLight, const char enable)
{
	int index = -1;
	if (ptLight->Index < 0)
	{
		index = GetAvaibleLight();
		ptLight->Index = index;
	}
	else
	{
		index = ptLight->Index;
	}

	if (index >= 0)
	{
		ptLight->Data.enabled = ActivateLigth(index, enable);
	}
}

void AddPointLight(const sfGlslVec4 color, const sfVector2f pos, const Node* node, const int index)
{
	nbPointLight++;
	array = (PointLight*)realloc(array, nbPointLight * sizeof(PointLight));
	InitPointLight(&array[nbPointLight - 1], color, pos, node, index);
}

Light* GetPointLight(const int index)
{
	for (int i = 0; i < nbPointLight; i++)
	{
		if (array[i].Index == index)
		{
			return &array[i].Data;
		}
	}
	return NULL;
}

PointLight* GetPointLights()
{
	return array;
}

int GetNumberPointLights()
{
	return nbPointLight;
}

void ClearPointLight(PointLight* ptLight)
{
	sfVertexArray_destroy(ptLight->Data.array);
}

void ClearPointLights()
{
	for (int i = 0; i < nbPointLight; i++)
	{
		ClearPointLight(&array[i]);
	}
	nbPointLight = 0;
	free(array);
	array = NULL;
}