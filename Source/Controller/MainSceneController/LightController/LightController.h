#ifndef _LIGHTCONTROLLER_H
#define _LIGHTCONTROLLER_H

#include <stdio.h>
#include <stdlib.h>
#include <SFML\Graphics.h>
#include "..\..\..\Model\Math\MathUtilities.h"
#include ".\..\..\GraphicsUtilities\Utilities.h"
#include "..\..\..\Model\Node\Node.h"
#include "..\..\GraphicsUtilities\Animation\Animation.h"

#define LIGHTNUMBER 50//Warnings change into shader too
//default player's light
#define DEFAULTLIGHT (sfGlslVec4) { 3.f, 1.f, 1.f, 1.0f }
//player red
#define REDLIGHT (sfGlslVec4) { 7.f, 0.0f, 0.0f, 3.0f }
#define CANDLELIGHTRED (sfGlslVec4) { 7.0f, 0.0f, 0.0f, 1.0f }
#define FIREBALLLIGHT (sfGlslVec4) { 5.0f, 1.0f, 1.0f, 1.0f }
#define CONELIGHT (sfGlslVec4) { 7.f, 0.0f, 0.0f, 1.0f }
//player black

#define REDLIGHTBLACK (sfGlslVec4) { 0.0f, 0.0f, 7.0f, 3.0f}
#define CANDLELIGHTBLACK (sfGlslVec4) { 0.0f, 0.0f, 7.0f, 1.0f }
#define FIREBALLLIGHTBLACK (sfGlslVec4) { 1.f, 1.f, 5.0f, 1.0f }
#define CONELIGHTBLACK (sfGlslVec4) { 0.f, 0.0f, 7.0f, 1.0f }
//
//player white

#define REDLIGHTWHITE (sfGlslVec4) { 3.0f, 3.0f, 3.0f, 3.0f }
#define CANDLELIGHTWHITE (sfGlslVec4) { 5.0f, 5.0f, 5.0f, 1.0f}
#define FIREBALLLIGHTWHITE (sfGlslVec4) { 2.5f, 2.5f, 2.5f, 1.0f}
#define CONELIGHTWHITE (sfGlslVec4) { 5.f, 5.0f, 5.0f, 1.0f }
//

//player green

#define REDLIGHTGREEN (sfGlslVec4) { 0.0f, 7.0f, 0.0f, 3.0f }
#define CANDLELIGHTGREEN (sfGlslVec4) { 0.0f, 7.0f, 0.0f, 1.0f }
#define FIREBALLLIGHTGREEN (sfGlslVec4) { 1.f, 5.0f, 1.0f, 1.0f }
#define CONELIGHTGREEN (sfGlslVec4) { 0.f, 7.0f, 0.0f, 1.0f }
//
#define STRONGREDLIGHT (sfGlslVec4) { 7.0f, 3.0f, 3.0f, 1.0f }
#define CANDLELIGHT (sfGlslVec4) { 10.f, 2.5f, 0.0f, 5.0f }
//main ambient
#define DEFAULTABIENT (sfGlslVec4) { 0.9f, 0.9f, 1.6f, 0.42f }
#define LAVAABIENT (sfGlslVec4) { 0.9f, 0.9f, 0.9f, 0.20f }
#define ICEABIENT (sfGlslVec4) {0.5f, 0.5f, 0.65f, 1.8f}
#define WINDABIENT (sfGlslVec4) {0.5f, 0.81f, 0.8f, 1.2f}
#define DIRTABIENT (sfGlslVec4) {0.8f, 0.8f, 1.0f, 1.8f}
#define SPAWNABIENT (sfGlslVec4) {0.8f, 0.8f, 0.8f, 1.2f}
#define BOSSELEMABIENT (sfGlslVec4) {1.0f, 1.0f, 1.0f, 0.8f}


typedef struct LIGHT
{
	sfBool enabled;
	sfVector2f Pos;
	sfVector2f LastPos;
	sfGlslVec4 ColorGl;
	sfGlslVec4 PosGl;
	sfVector2f Size;
	float Radius;
	int nbNodes;
	int IndexSprite;
	Node* Node;
	sfVertexArray* array;
}Light;
// Initializes the 50 lights
void InitLights(const int index);

void SetIndexSprite(const int index, const int indexSprite);

//Set color light to pass into the shader
void SetLigthColor(const int index, const sfGlslVec4 color);
//Set position of the light in the world
void SetLigthPosition(const int index, const sfVector2f pos, const Node* node);
//Enable or disable the light (reduce the number light pass to into the shader)
sfBool ActivateLigth(const int index, const char enable);

int GetLightCount();

Light* GetLights();
//Return the index of a light which isn't enabled
int GetAvaibleLight();
//Get the Default sprite for the light
sfSprite** GetSpriteLight();
//Find the intersection point with  two sfVector2f, the first parameter corresponds to the intersection point
void lineIntersect(sfVector2f** interPt, const sfVector2f a, const sfVector2f b, const sfVector2f c, const sfVector2f d);
//Set the vertex Array's light compared to the current node
void UpdateVertexLight(Light* light);

float GetDefaultLigth();

sfGlslVec4 GetAmbient();

void SetAmbient(const sfGlslVec4 Ambient);

void ClearLights();
#endif