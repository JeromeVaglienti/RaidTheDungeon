#include "Utilities.h"
#include "..\MainSceneController\ShadersController\ShadersController.h"
#include "..\..\Model\Config\Config.h"


sfSprite* LoadSprite(const char* Path, const char isCentered)
{
	sfTexture* tempTex;
	sfSprite* tempSprite;

	tempSprite = sfSprite_create();

	tempTex = sfTexture_createFromFile(Path, NULL);

	sfSprite_setTexture(tempSprite, tempTex, 0);
	sfVector2u size;

	if (isCentered)
	{
		size = sfTexture_getSize(tempTex);
		sfVector2f pos = { size.x / 2.0f , size.y / 2.0f };
		sfSprite_setOrigin(tempSprite, pos);
	}
	return tempSprite;
}

void DestroySfSprite(sfSprite **toDestroy)
{
	if (toDestroy != NULL)
	{
		sfTexture_destroy((sfTexture*)sfSprite_getTexture(*toDestroy));
		sfSprite_destroy(*toDestroy);
		toDestroy = NULL;
	}
}

sfSprite** LoadAnimDirect(const char* path, const char isCentered, const int numberSprite)
{
	if (numberSprite < 9)
	{
		printf("ERROR LoadAnimDirect\n");
		return NULL;
	}
	sfSprite** sprite = (sfSprite * *)calloc(numberSprite, sizeof(sfSprite * ));
	char str[100] = { 0 };
	char strNm[100] = { 0 };
	sprintf(str, "%sEast.png", path);
	sprite[0] = LoadSprite(str, isCentered);
	sprintf(str, "%sSouthEast.png", path);
	sprite[1] = LoadSprite(str, isCentered);
	sprintf(str, "%sSouth.png", path);
	sprite[2] = LoadSprite(str, isCentered);
	sprintf(str, "%sSouthWest.png", path);
	sprite[3] = LoadSprite(str, isCentered);
	sprintf(str, "%sWest.png", path);
	sprite[4] = LoadSprite(str, isCentered);
	sprintf(str, "%sNorthWest.png", path);
	sprite[5] = LoadSprite(str, isCentered);
	sprintf(str, "%sNorth.png", path);
	sprite[6] = LoadSprite(str, isCentered);
	sprintf(str, "%sNorthEast.png", path);
	sprite[7] = LoadSprite(str, isCentered);
	sprintf(str, "%sIdle.png", path);
	sprite[8] = LoadSprite(str, isCentered);
	return sprite;
}

sfSprite** LoadAnimDirect4Dir(const char* path, const char isCentered, const int numberSprite)
{
	if (numberSprite < 4)
	{
		printf("ERROR LoadAnimeDirect4Dir\n");
		return NULL;
	}
	sfSprite** sprite = (sfSprite**)calloc(numberSprite, sizeof(sfSprite*));
	char str[100] = { 0 };
	char strNm[100] = { 0 };
	sprintf(str, "%sEast.png", path);
	sprite[0] = LoadSprite(str, isCentered);
	sprintf(str, "%sSouth.png", path);
	sprite[1] = LoadSprite(str, isCentered);
	sprintf(str, "%sWest.png", path);
	sprite[2] = LoadSprite(str, isCentered);
	sprintf(str, "%sNorth.png", path);
	sprite[3] = LoadSprite(str, isCentered);
	return sprite;
}

sfSprite** LoadAnimDirectTransition(const char* path, const char isCentered, const int numberSprite)
{
	if (numberSprite < 8)
	{
		printf("ERROR LoadAnimDirectTransition\n");
		return NULL;
	}
	sfSprite** sprite = (sfSprite**)calloc(numberSprite, sizeof(sfSprite * ));
	char str[100] = { 0 };
	char strNm[100] = { 0 };
	sprintf(str, "%s0.png", path);
	sprite[0] = LoadSprite(str, isCentered);
	sprintf(str, "%s1.png", path);
	sprite[1] = LoadSprite(str, isCentered);
	sprintf(str, "%s2.png", path);
	sprite[2] = LoadSprite(str, isCentered);
	sprintf(str, "%s3.png", path);
	sprite[3] = LoadSprite(str, isCentered);
	sprintf(str, "%s4.png", path);
	sprite[4] = LoadSprite(str, isCentered);
	sprintf(str, "%s5.png", path);
	sprite[5] = LoadSprite(str, isCentered);
	sprintf(str, "%s6.png", path);
	sprite[6] = LoadSprite(str, isCentered);
	sprintf(str, "%s7.png", path);
	sprite[7] = LoadSprite(str, isCentered);
	return sprite;
}

void BlitSpriteNoRS(sfRenderWindow* window, sfSprite* spriteToPrint, const sfVector2f pos)
{
	sfSprite_setPosition(spriteToPrint, pos);
	sfRenderWindow_drawSprite(window, spriteToPrint, NULL);
}

void BlitSprite(sfRenderWindow* window, sfSprite* spriteToPrint, const sfVector2f pos, sfShader* shd)
{
	sfRenderStates* tempRenderStates = GetRenderState();
	if (tempRenderStates)
	{
		tempRenderStates->texture = NULL;
		tempRenderStates->shader = shd;
		sfSprite_setPosition(spriteToPrint, pos);
		sfRenderWindow_drawSprite(window, spriteToPrint, tempRenderStates);
	}
}

void BlitVertexArray(sfRenderWindow* window, sfVertexArray* array, const sfSprite* spriteToPrnt)
{
	if (sfVertexArray_getVertexCount(array) > 0)
	{
		sfRenderStates* tempRenderState = GetRenderState();
		if (tempRenderState)
		{
			if (spriteToPrnt)
			{
				tempRenderState->texture = sfSprite_getTexture(spriteToPrnt);
				tempRenderState->shader = NULL;
				sfRenderWindow_drawVertexArray(window, array, tempRenderState);
			}
			sfVertexArray_clear(array);
		}
	}
}

void BlitRenderTexture(sfRenderWindow* window, sfRenderTexture* rd, sfSprite* spriteToPrint, const sfVector2f pos)
{
	sfSprite_setPosition(spriteToPrint, pos);
	sfRenderTexture_drawSprite(rd, spriteToPrint, NULL);
}

void BlitVertexArrayLight(sfVertexArray* array, sfRenderTexture* rd, const sfSprite* spriteToPrnt)
{
	if (sfVertexArray_getVertexCount(array) > 0)
	{
		sfRenderStates* tempRenderState = GetRenderState();
		if (tempRenderState)
		{
			if (spriteToPrnt)
			{
				tempRenderState->texture = sfSprite_getTexture(spriteToPrnt);
				tempRenderState->shader = NULL;
				sfRenderTexture_drawVertexArray(rd, array, tempRenderState);
			}
			sfVertexArray_clear(array);
		}
	}
}
//insert Custom Sprite into a vertex array with this position, scale, angle, index Image (corresponds to the image positon in the sprite's texture), image number line(default 0), image number column(default 0)
void AppendSprite(sfVertexArray* array, const sfSprite* spriteToPrnt, const sfVector2f pos, const sfVector2f scale, const sfColor color, const float angle, const int indexImage, const int wImages, const int hImages)
{
	sfVector2f size;
	sfVector2f leftCorner = (sfVector2f) { 0.0f, 0.0f };

	sfFloatRect tempLocalBounds = sfSprite_getLocalBounds(spriteToPrnt);
	if (wImages > 0 || hImages > 0)
	{
		//Get the size of one image in the sprite's texture
		size = (sfVector2f) { (tempLocalBounds.width / 2.0f) / wImages, (tempLocalBounds.height / 2.0f) / hImages };
		leftCorner.x = (indexImage % wImages)*size.x *2.0f;
		leftCorner.y = (indexImage / wImages)*size.y *2.0f;
	}
	else
	{
		size = (sfVector2f) { tempLocalBounds.width / 2.0f, tempLocalBounds.height / 2.0f };
	}
	//Set the transform and apply to the new vertexes
	sfTransform tr = sfTransform_Identity;
	sfTransform_rotateWithCenter(&tr, angle, pos.x, pos.y);
	sfTransform_scaleWithCenter(&tr, scale.x, scale.y, pos.x, pos.y);
	//Add a new sfQuad to the vertex array
	sfVertex leftTopCorner = (sfVertex) { sfTransform_transformPoint(&tr, (sfVector2f) { pos.x - size.x, pos.y - size.y }), color, (sfVector2f) { leftCorner.x + 0.0f, leftCorner.y + 0.0f } };
	sfVertex rightTopCorner = (sfVertex) { sfTransform_transformPoint(&tr, (sfVector2f) { pos.x + size.x, pos.y - size.y }), color, (sfVector2f) { leftCorner.x + size.x *2.0f, leftCorner.y + 0.0f } };
	sfVertex rightDownCorner = (sfVertex) { sfTransform_transformPoint(&tr, (sfVector2f) { pos.x + size.x, pos.y + size.y }), color, (sfVector2f) { leftCorner.x + size.x *2.0f, leftCorner.y + size.y *2.0f, } };
	sfVertex leftDownCorner = (sfVertex) { sfTransform_transformPoint(&tr, (sfVector2f) { pos.x - size.x, pos.y + size.y }), color, (sfVector2f) { leftCorner.x + 0.0f, leftCorner.y + size.y *2.0f, } };

	sfVertexArray_append(array, leftTopCorner);
	sfVertexArray_append(array, rightTopCorner);
	sfVertexArray_append(array, rightDownCorner);
	sfVertexArray_append(array, leftDownCorner);
}

void DrawPixel(sfImage* image, const int posX, const int posY, const sfColor color)
{
	sfVector2u tempV = sfImage_getSize(image);
	if (posX < (int)tempV.x && posX >= 0 && posY < (int)tempV.y && posY >= 0)
	{
		sfImage_setPixel(image, posX, posY, color);
	}
}
