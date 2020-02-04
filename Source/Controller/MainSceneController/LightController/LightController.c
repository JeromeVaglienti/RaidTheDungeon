#include "LightController.h"

Light lights[LIGHTNUMBER] = { 0 };
int nbLights = 0;
float defaultLight = 0.075f;
sfGlslVec4 ambient = { 0 };
sfSprite* LightController_pointLight[3] = { 0 };
// Initializes the 50 lights
void InitLights(const int index)
{
	nbLights = 0;
	ambient = DEFAULTABIENT;
	LightController_pointLight[0] = LoadSprite("Ressources/Sprites/Light/light.png", 1);
	LightController_pointLight[1] = LoadSprite("Ressources/Sprites/Light/wall_light.png", 1);
	LightController_pointLight[2] = LoadSprite("Ressources/Sprites/Light/candle_light.png", 1);
	sfFloatRect tempRect = sfSprite_getLocalBounds(LightController_pointLight[index]);
	sfVector2f sizeLight = (sfVector2f) { (float)tempRect.width / 2, (float)tempRect.height / 2 };
	InitDirectionalArray(/*sizeLight*/);

	for (int i = 0; i < LIGHTNUMBER; i++)
	{
		lights[i].Pos = (sfVector2f) { 0.0f, 0.0f };
		lights[i].LastPos = (sfVector2f) { 0.0f, 0.0f };
		lights[i].ColorGl = DEFAULTLIGHT;
		lights[i].PosGl = (sfGlslVec4) { 0.0f, 0.0f, defaultLight, 1.f };
		lights[i].enabled = sfFalse;
		lights[i].array = sfVertexArray_create();
		lights[i].Size = sizeLight;
		sfVertexArray_setPrimitiveType(lights[i].array, sfTriangleFan);
		lights[i].Node = NULL;
		lights[i].Radius = sizeLight.x;
		lights[i].nbNodes = (int)(ceilf(tempRect.width / 64.0f));
		lights[i].IndexSprite = index;
	}
}

void SetIndexSprite(const int index, const  int indexSprite)
{
	sfFloatRect tempRect = sfSprite_getLocalBounds(LightController_pointLight[indexSprite]);
	lights[index].IndexSprite = indexSprite;
	lights[index].Size = (sfVector2f) { (float)tempRect.width / 2, (float)tempRect.height / 2 };
	lights[index].nbNodes = (int)(ceilf(tempRect.width / 64.0f));
	lights[index].Radius = lights[index].Size.x;
}

//Set color light to pass into the shader
void SetLigthColor(const int index, const sfGlslVec4 color)
{
	lights[index].ColorGl = color;
}
//Set position of the light in the world
void SetLigthPosition(const int index, const sfVector2f pos, const Node* node)
{
	lights[index].Pos.x = pos.x;
	lights[index].Pos.y = pos.y;
	lights[index].Node = (Node*)node;
}
//Enable or disable the light (reduce the number light pass to into the shader)
sfBool ActivateLigth(const int index, const char enable)
{
	if (enable && !lights[index].enabled)
	{
		nbLights++;
		lights[index].enabled = sfTrue;
	}
	if (!enable && lights[index].enabled)
	{
		nbLights--;
		lights[index].enabled = sfFalse;
	}
	return lights[index].enabled;
}

int GetLightCount()
{
	return nbLights;
}

Light* GetLights()
{
	return(lights);
}
//Return the index of a light which isn't enabled
int GetAvaibleLight()
{
	int returnValue = -1;
	for (int i = 0; i < LIGHTNUMBER; i++)
	{

		if (i != 0 && i != 1 && !lights[i].enabled)
		{
			return i;
		}
	}
	return returnValue;
}
//Get the Default sprite for the light
sfSprite** GetSpriteLight()
{
	return(LightController_pointLight);
}
//Find the intersection point with  two sfVector2f, the first parameter corresponds to the intersection point
void lineIntersect(sfVector2f** interPt, const sfVector2f a, const sfVector2f b, const sfVector2f c, const sfVector2f d)
{

	sfVector2f r = { b.x - a.x, b.y - a.y };
	sfVector2f s = { d.x - c.x, d.y - c.y };
	float det = r.x * s.y - r.y * s.x;
	float u = ((c.x - a.x) * r.y - (c.y - a.y) * r.x) / det;
	float t = ((c.x - a.x) * s.y - (c.y - a.y) * s.x) / det;

	if (0 <= u && u <= 1 && 0 <= t && t <= 1)
	{
		float X = a.x + t * r.x;
		float Y = a.y + t * r.y;
		if (!*interPt)
		{
			*interPt = (sfVector2f*)malloc(sizeof(sfVector2f));
			(*interPt)->x = X;
			(*interPt)->y = Y;
		}
		else
		{
			float lastInterDist = fabsf(a.x - (*interPt)->x) + fabsf(a.y - (*interPt)->y);
			float newInterDist = fabsf(a.x - X) + fabsf(a.y - Y);
			if (lastInterDist < newInterDist)
			{
				(*interPt)->x = X;
				(*interPt)->y = Y;
			}

		}
	}
}
//Set the vertex Array's light compared to the current node
void UpdateVertexLight(Light* light)
{
	if (light->Pos.x != light->LastPos.x ||
		light->Pos.y != light->LastPos.y)
	{
		//int size = light->Size.x * 2.0f / 64.0f;
		//size *= size;
		//Node** wallCheck = (Node*) malloc(sizeof(Node)* size);

		//Node* wallCheck[64] = {NULL};
		int size = light->nbNodes*light->nbNodes;
		Node** wallCheck;

		wallCheck = (Node**)calloc(size, sizeof(Node*));
		for (int i = 0; i < size; i++)
		{
			wallCheck[i] = NULL;
		}

		int ct = 0;

		for (int i = (int)(light->Pos.x - light->Size.x); i < light->Pos.x + light->Size.x; i += 64)
		{
			for (int j = (int)(light->Pos.y - light->Size.y); j < light->Pos.y + light->Size.y; j += 64)
			{

				wallCheck[ct] = GetClosestNodeInArray((float)i, (float)j, light->Node);
				/*if (wallCheck[ct])
				{
					if (wallCheck[ct]->Segments.numberSegment == 0)
					{
						wallCheck[ct] = NULL;
					}
				}*/
				ct++;
			}
		}

		sfVector2f* inter = NULL;
		sfVertexArray_clear(light->array);

		sfVertexArray_append(light->array, (sfVertex) { light->Pos, sfWhite, light->Size });
		sfVector2f* tempDirectionArray = GetDirectionalArray();
		for (int i = 0; i <= directionalNumber; i++)
		{
			sfVector2f pA = { light->Pos.x + tempDirectionArray[i].x*light->Size.x,  light->Pos.y + tempDirectionArray[i].y*light->Size.y };
			sfVector2f pB = { pA.x - (light->Pos.x - light->Size.x) , pA.y - (light->Pos.y - light->Size.y) };

			float offset = 32;


			for (int k = 0; k < size; k++)
			{
				if (wallCheck[k] == NULL)
				{
					free(wallCheck);
					return;
				}
				float X = wallCheck[k]->posX*64.0f;
				float Y = wallCheck[k]->posY*64.0f;

				if (X > light->Pos.x - light->Size.x - offset && X < light->Pos.x + light->Size.x + offset &&
					Y > light->Pos.y - light->Size.y - offset && Y < light->Pos.y + light->Size.y + offset)
				{
					for (int j = 0; j < wallCheck[k]->Segments.numberSegment; j++)
					{
						lineIntersect(&inter, pA, light->Pos, wallCheck[k]->Segments.Array[j].P1, wallCheck[k]->Segments.Array[j].P2);
					}
				}
			}


			if (inter)
			{

				pB.x = (*inter).x - (light->Pos.x - light->Size.x);
				pB.y = (*inter).y - (light->Pos.y - light->Size.y);
				sfVertexArray_append(light->array, (sfVertex) { *inter, sfWhite, pB });
				free(inter);
				inter = NULL;

			}
			else
			{
				sfVertexArray_append(light->array, (sfVertex) { pA, sfWhite, pB });
			}
		}
		light->LastPos = light->Pos;
		free(wallCheck);
	}
}

float GetDefaultLigth()
{
	return defaultLight;
}

sfGlslVec4 GetAmbient()
{
	return ambient;
}

void SetAmbient(const sfGlslVec4 Ambient)
{
	ambient = Ambient;
}

void ClearLights()
{
	nbLights = 0;
	for (int i = 0; i < 3; i++)
	{
		sfTexture_destroy((sfTexture*)sfSprite_getTexture(LightController_pointLight[i]));
		sfSprite_destroy(LightController_pointLight[i]);
	}


	for (int i = 0; i < LIGHTNUMBER; i++)
	{
		sfVertexArray_clear(lights[i].array);
		sfVertexArray_destroy(lights[i].array);
	}
}