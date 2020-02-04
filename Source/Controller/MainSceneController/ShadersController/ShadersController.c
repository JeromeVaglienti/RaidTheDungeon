#include "ShadersController.h"
#include "..\..\GraphicsUtilities\Utilities.h"
#include "..\..\LeJeu\Model\Config\Config.h"

ShadersContainer mainS;
sfTexture* screenTexture = 0;
sfSprite* toDraw = { 0 };
//sfRenderTexture* normalMap = 0;
sfRenderTexture* deferredLight = 0;
sfRenderTexture* lavaLight = 0;
sfRenderTexture* enemiLight = 0;
sfRenderStates states = { 0 };
sfBool enableNm = sfTrue;



//Initialize each fragment, sfTexture and sfRenderTexture link to the normal map
void InitShadersContainer(const sfVideoMode mode)
{
	//Two sfRenderTexture with one which we draw the normal map and the othe we draw the lights 
	//normalMap = sfRenderTexture_create(mode.width, mode.height, 0);
	deferredLight = sfRenderTexture_create(mode.width, mode.height, 0);
	lavaLight = sfRenderTexture_create(mode.width, mode.height, 0);
	enemiLight = sfRenderTexture_create(mode.width, mode.height, 0);
	enableNm = GetConfigDynamicLight();
	states.blendMode = sfBlendAlpha;
	states.transform = sfTransform_Identity;
	states.texture = NULL;
	states.shader = NULL;
	//sfRenderTexture_setSmooth(normalMap, sfTrue);
	//Sprite corresponding to the screen
	screenTexture = sfTexture_create(mode.width, mode.height);
	toDraw= sfSprite_create();
	//Every shaders
	mainS.Flash = sfShader_createFromFile("Ressources/Shaders/vertex.vert", NULL, "Ressources/Shaders/Flash/flash.frag");
	mainS.Greyscale = sfShader_createFromFile("Ressources/Shaders/vertex.vert", NULL, "Ressources/Shaders/Greyscale/greyscale.frag");
	mainS.Purple = sfShader_createFromFile("Ressources/Shaders/vertex.vert", NULL, "Ressources/Shaders/Purple/purple.frag");
	mainS.HitEnemy = sfShader_createFromFile("Ressources/Shaders/vertex.vert", NULL, "Ressources/Shaders/HitEnemy/hitEnemy.frag");
	mainS.NormalMapShd = sfShader_createFromFile("Ressources/Shaders/vertex.vert", NULL, "Ressources/Shaders/NormalMap/normalmap.frag");
	mainS.TransparenceMap = sfShader_createFromFile("Ressources/Shaders/vertex.vert", NULL, "Ressources/Shaders/TransparenceMap/transparenceMap.frag");
}
//Draw the normal map in function of the current lights positions 
void DisplayNormalMap(const sfRenderWindow* window, const sfView* view)
{
	///////////////Sort light///////////////

	//Set two arrays to send to the normal map shader
	sfGlslVec4 tmpArrayColor[LIGHTNUMBER];
	sfGlslVec4 tmpArrayPos[LIGHTNUMBER];
	float tmpArrayRadius[LIGHTNUMBER];

	sfVector2u tempSizeWindow = sfRenderWindow_getSize(window);


	states.shader = NULL;

	/*int ct = 0;
	int ctt = 0;*/
	Light* tempLights = GetLights();
	int tempLightCount = GetLightCount();
	sfSprite** spriteLights = GetSpriteLight();

	for (int i = 0; i < tempLightCount; i++)
	{
		for (int j = 0; j < LIGHTNUMBER; j++)
		{
			Light temp;
			if (tempLights[j].enabled)
			{
				Light* tempPointLight = GetPointLight(j);
				//Compute the Dynamic light 
				if (tempPointLight)
				{
					//ctt++;
					if (tempPointLight->enabled)
					{
						//ct++;
						temp = *tempPointLight;
					}
					else
					{
						temp = tempLights[j];
					}
				}
				else
				{
					temp = tempLights[j];
				}
				states.texture = sfSprite_getTexture(spriteLights[temp.IndexSprite]);
				UpdateVertexLight(&temp);
				tmpArrayColor[i] = temp.ColorGl;
				tmpArrayRadius[i] = temp.Radius;
				sfVector2f inGamePos = (sfVector2f) { temp.Pos.x, temp.Pos.y };
				sfVector2i tempMapCoords = sfRenderWindow_mapCoordsToPixel(window, inGamePos, view);
				sfVector2f newPos = (sfVector2f) { tempMapCoords.x / (float)tempSizeWindow.x, 1.0f - tempMapCoords.y / (float)tempSizeWindow.y };

				tmpArrayPos[i].x = newPos.x;
				tmpArrayPos[i].y = newPos.y;
				tmpArrayPos[i].z = 0.075f;
				tmpArrayPos[i].w = 1.0f;

				//sfRenderTexture_setView(deferredLight, view);

				sfRenderTexture_drawVertexArray(deferredLight, temp.array, &states);

				i++;
			}
		}
	}
	/*printf("CTt %d\n", ctt);
	printf("CT %d\n", ct);*/
	/////////////////////////////////////

	sfTexture_updateFromRenderWindow(screenTexture, window, 0, 0);
	sfShader_setTextureUniform(mainS.NormalMapShd, "u_texture", (const sfTexture*)screenTexture);
	sfShader_setTextureUniform(mainS.NormalMapShd, "u_deferred", (const sfTexture*)sfRenderTexture_getTexture(deferredLight));
	sfShader_setTextureUniform(mainS.NormalMapShd, "u_lava", (const sfTexture*)sfRenderTexture_getTexture(lavaLight));
	sfShader_setTextureUniform(mainS.NormalMapShd, "u_enemi", (const sfTexture*)sfRenderTexture_getTexture(enemiLight));
	sfShader_setVec2Uniform(mainS.NormalMapShd, "Resolution", (sfGlslVec2) { (float)tempSizeWindow.x, (float)tempSizeWindow.y });
	sfShader_setVec4UniformArray(mainS.NormalMapShd, "LightPos", tmpArrayPos, LIGHTNUMBER);
	sfShader_setVec4UniformArray(mainS.NormalMapShd, "LightColor", tmpArrayColor, LIGHTNUMBER);
	sfShader_setVec4Uniform(mainS.NormalMapShd, "AmbientColor", GetAmbient());
	sfShader_setFloatUniformArray(mainS.NormalMapShd, "radius", tmpArrayRadius, LIGHTNUMBER);
	sfShader_setIntUniform(mainS.NormalMapShd, "LightNumber", tempLightCount);

	//Copy the current screen texture to the Draw Sprite 
	sfSprite_setTexture(toDraw, screenTexture, 1);
	if (toDraw)
	{
		BlitSprite((sfRenderWindow*)window, toDraw, (sfVector2f) { 0, 0 }, mainS.NormalMapShd);
	}
}
//Return the variable which contains shaders 
ShadersContainer GetShaderList()
{
	return(mainS);
}

void ResetRenderTexture(const sfVideoMode mode, sfRenderTexture** texture)
{
	if ((*texture))
	{
		sfRenderTexture_destroy((sfRenderTexture*)(*texture));
		(*texture) = sfRenderTexture_create(mode.width, mode.height, 0);
	}
}

void ResetTexture(const sfVideoMode mode, sfTexture** texture)
{
	sfTexture_destroy((*texture));
	(*texture) = sfTexture_create(mode.width, mode.height);
}
//Reset all the textures use for normal map shader
void ResetShaderLight(const sfVideoMode mode)
{
	/*if (normalMap)
	{
		ResetRenderTexture(mode, &normalMap);
	}*/
	if (deferredLight)
	{
		ResetRenderTexture(mode, &deferredLight);
	}
	if (lavaLight)
	{
		ResetRenderTexture(mode, &lavaLight);
	}
	if (enemiLight)
	{
		ResetRenderTexture(mode, &enemiLight);
	}
	if (screenTexture)
	{
		ResetTexture(mode, &screenTexture);
	}
}
//Return the screen texture
sfTexture* GetScreenTexture()
{
	return(screenTexture);
}
//Return the screen sprite ie all the sprites in one
sfSprite* GetScreenSprite()
{
	return(toDraw);
}
//Return the renderTexture for the normal map
sfRenderTexture* GetNormalMap()
{
	return NULL;
	//return(normalMap);
}
//Return the current RenderState
sfRenderStates* GetRenderState()
{
	return (&states);
}
//to know if the normal map is enabled  or not
sfBool IsNmEnabled()
{
	return enableNm;
}
//Enable or disable the normal map 
void SetNmEnabled(const sfBool b)
{
	enableNm = b;
}

sfRenderTexture* GetLightTexture()
{
	return(deferredLight);
}

sfRenderTexture* GetLavaLightTexture()
{
	return(lavaLight);
}

sfRenderTexture* GetEnemiLightTexture()
{
	return(enemiLight);
}

/*void DrawNormalMap(sfSprite* sprite, sfVector2f pos)
{
	sfVector2f lightSize = (sfVector2f) { sfSprite_getGlobalBounds(pointLight.Sprite).width, sfSprite_getGlobalBounds(pointLight.Sprite).height };
	sfVector2f spriteSize = (sfVector2f) { sfSprite_getGlobalBounds(sprite).width, sfSprite_getGlobalBounds(sprite).height };
	for (int i = 0; i < LIGHTNUMBER; i++)
	{
		if (lightsPoints.enabled[i])
		{
			if (lightsPoints.Pos[i].x + (lightSize.x + spriteSize.x) / 2.0f > pos.x &&  lightsPoints.Pos[i].x - (lightSize.x + spriteSize.x) / 2.0f < pos.x &&
				lightsPoints.Pos[i].y + (lightSize.y + spriteSize.y) / 2.0f > pos.y &&  lightsPoints.Pos[i].y - (lightSize.y + spriteSize.y) / 2.0f < pos.y)
			{
				float X = lightsPoints.Pos[i].x - (pos.x - spriteSize.x / 2.0f);
				float Y = lightsPoints.Pos[i].y - (pos.y - spriteSize.y / 2.0f);
				printf("i %d X %f Y %f x %f y %f \n", i, X, Y, pos.x, pos.y);
				sfSprite_setTextureRect(sprite, (sfIntRect) { X, Y, X + lightSize.x, Y + lightSize.y });
				sfSprite_setOrigin(sprite, (sfVector2f) { lightSize.x / 2.0f, lightSize.y / 2.0f });
				sfSprite_setPosition(sprite, (sfVector2f) { lightsPoints.Pos[i].x, lightsPoints.Pos[i].y });
				sfRenderTexture_drawSprite(normalMap, sprite, NULL);
			}
		}
	}
}*/