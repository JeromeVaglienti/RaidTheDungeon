#ifndef _SHADERSCONTROLLER
#define _SHADERSCONTROLLER

#include <stdio.h>
#include <stdlib.h>
#include "..\LightController\PointLight.h"

typedef struct
{
	sfShader* Flash;
	sfShader* Greyscale;
	sfShader* Purple;
	sfShader* HitEnemy;
	sfShader* TransparenceMap;
	sfShader* NormalMapShd;
}ShadersContainer;

//Initialize each fragment, sfTexture and sfRenderTexture link to the normal map
void InitShadersContainer(const sfVideoMode mode);
//Draw the normal map in function of the current lights positions 
void DisplayNormalMap(const sfRenderWindow* window, const sfView* view);
//Return the variable which contains shaders 
ShadersContainer GetShaderList();
//Reset all the textures use for normal map shader
void ResetShaderLight(const sfVideoMode mode);
//Return the screen texture
sfTexture* GetScreenTexture();
//Return the screen sprite ie all the sprites in one
sfSprite* GetScreenSprite();
//Return the renderTexture for the normal map
sfRenderTexture* GetNormalMap();
sfRenderTexture* GetLightTexture();
sfRenderTexture* GetLavaLightTexture();
sfRenderTexture* GetEnemiLightTexture();

//Return the current RenderState
sfRenderStates* GetRenderState();
//to know if the normal map is enabled  or not
sfBool IsNmEnabled();
//Enable or disable the normal map 
void SetNmEnabled(const sfBool b);

#endif // ! _SHADERSCONTROLLERS
