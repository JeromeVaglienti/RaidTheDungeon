#ifndef _UTILITIES_H
#define _UTILITIES_H

#include <stdio.h>
#include <stdlib.h>
#include "CustomSprite\CustomSprite.h"

sfSprite* LoadSprite(const char* Path, const char isCentered);
void DestroySfSprite(sfSprite** toDestroy);
sfSprite** LoadAnimDirect(const char* path, const char isCentered, const int numberSprite);
sfSprite** LoadAnimDirect4Dir(const char* path, const char isCentered, const int numberSprite);
sfSprite** LoadAnimDirectTransition(const char* path, const char isCentered, const int numberSprite);
void BlitSpriteNoRS(sfRenderWindow* window, sfSprite* spriteToPrint, const sfVector2f pos);
void BlitSprite(sfRenderWindow* window, sfSprite* spriteToPrint, const sfVector2f pos, sfShader* shd);
void BlitVertexArray(sfRenderWindow* window, sfVertexArray* array, const sfSprite* spriteToPrnt);
void BlitRenderTexture(sfRenderWindow* window, sfRenderTexture* rd, sfSprite* spriteToPrint, const sfVector2f pos);
void BlitVertexArrayLight(sfVertexArray* array, sfRenderTexture* rd, const sfSprite* spriteToPrnt);
//insert Custom Sprite into a vertex array with this position, scale, angle, index Image (corresponds to the image positon in the sprite's texture), image number line(default 0), image number column(default 0)
void AppendSprite(sfVertexArray* array, const sfSprite* spriteToPrnt, const sfVector2f pos, const sfVector2f scale, const sfColor color, const float angle, const int indexImage, const int wImages, const int hImages);
void DrawPixel(sfImage* image, const int posX, const int posY, const sfColor color);

#endif // !_UTILITIES_H