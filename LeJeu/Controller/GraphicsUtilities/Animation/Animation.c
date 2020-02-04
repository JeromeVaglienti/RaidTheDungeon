#include "Animation.h"
#include <string.h>

void UpdateAnimation(Animation *animToPlay, const float timeElapsed)
{
	animToPlay->actualDuration += timeElapsed;
	float temp = animToPlay->actualDuration / animToPlay->totalDuration;

	if (temp >= 1.0f)
	{
		if (animToPlay->loop)
		{
			animToPlay->actualDuration -= animToPlay->totalDuration * (int)temp;
			temp = animToPlay->actualDuration / animToPlay->totalDuration;
		}
		else
		{
			animToPlay->actualFrameToShow = animToPlay->numberOfFrame - 1;
			return;
		}
	}
	animToPlay->actualFrameToShow = (int)(animToPlay->numberOfFrame * temp);
}

void UpdateAnimationReversed(Animation *animToPlay, const float timeElapsed)
{
	animToPlay->actualDuration += timeElapsed;
	float temp = animToPlay->actualDuration / animToPlay->totalDuration;

	if (temp >= 1.0f)
	{
		if (animToPlay->loop)
		{
			animToPlay->actualDuration -= animToPlay->totalDuration * (int)temp;
			temp = animToPlay->actualDuration / animToPlay->totalDuration;
		}
		else
		{
			animToPlay->actualFrameToShow = animToPlay->numberOfFrame - 1;
			return;
		}
	}

	animToPlay->actualFrameToShow = (int)((animToPlay->numberOfFrame - 1) - animToPlay->numberOfFrame * temp);
}

void UnLoadAnimation(Animation *toUnLoad)
{
	toUnLoad->isLoaded = false;
	printf("ANIMATION UNLOAD\n");

	for (int i = 0; i < toUnLoad->numberOfFrame; i++)
	{
		sfTexture_destroy((sfTexture*)sfSprite_getTexture(toUnLoad->spriteArray[i]));
		sfSprite_destroy(toUnLoad->spriteArray[i]);
		toUnLoad->spriteArray[i] = NULL;
	}

}

void ReScaleAnimation(Animation* toRescale, const float newSizeX, const float newSizeY, const float ratioX, const float ratioY)
{
	sfVector2f scale;
	const sfTexture* tempTexture = sfSprite_getTexture(toRescale->spriteArray[0]);
	sfVector2u tempSize = sfTexture_getSize(tempTexture);
	scale.x = newSizeX * ratioX / (float)tempSize.x;
	scale.y = newSizeY * ratioY / (float)tempSize.y;

	for (int i = 0; i < toRescale->numberOfFrame; i++)
	{
		sfSprite_setScale(toRescale->spriteArray[i], scale);
	}
}

void UnLoadFrameOfAnim(Animation *toUnLoad, const int index)
{
	sfTexture_destroy((sfTexture*)sfSprite_getTexture(toUnLoad->spriteArray[index]));
	sfSprite_destroy(toUnLoad->spriteArray[index]);
}

void ReloadAnimationOrigin(Animation *toReload, const char* path, const char* extension)
{
	printf("RELOAD OF : %s\n", path);
	toReload->actualDuration = 0.0f;
	toReload->actualFrameToShow = 0;
	char directoryOfimage[250];
	for (int i = 0; i < toReload->numberOfFrame; i++)
	{
		char* toAdd;
		if (i < 10)
		{
			toAdd = "0000";
		}
		else if (i < 100)
		{
			toAdd = "000";
		}
		else if (i < 1000)
		{
			toAdd = "00";
		}
		else
		{
			toAdd = "";
		}

		sprintf(directoryOfimage, "%s%s%d.%s", path, toAdd, i, extension);
		toReload->spriteArray[i] = LoadSprite(directoryOfimage, 0);
	}

	toReload->isLoaded = true;
}

void ReloadAnimation(Animation *toReload, const char* path, const char* extension, const sfVector2f pos)
{
	printf("RELOAD OF : %s\n", path);
	toReload->actualDuration = 0.0f;
	toReload->actualFrameToShow = 0;
	char directoryOfimage[250];
	for (int i = 0; i < toReload->numberOfFrame; i++)
	{
		char* toAdd;
		if (i < 10)
		{
			toAdd = "000";
		}
		else if (i < 100)
		{
			toAdd = "00";
		}
		else if (i < 1000)
		{
			toAdd = "0";
		}
		else
		{
			toAdd = "";
		}
	
		sprintf(directoryOfimage, "%s%s%d.%s", path, toAdd, i, extension);
		toReload->spriteArray[i] = LoadSprite(directoryOfimage, 0);
		sfSprite_setPosition(toReload->spriteArray[i], pos);
	}

	toReload->isLoaded = true;

}

Animation LoadAnimationSpecifiedIndexStart(const int numberOfFrame, const float totalDuration, const char* path, const char* extension, const sfVector2f pos, const bool isLoop, const int index, const char isCentered)
{
	Animation test;

	test.numberOfFrame = numberOfFrame;
	test.totalDuration = totalDuration;
	test.actualDuration = 0.0f;
	test.spriteArray = NULL;
	test.actualFrameToShow = 0;
	test.animPosition = pos;
	test.loop = isLoop;
	test.isLoaded = true;
	test.spriteArray = (sfSprite**)malloc(numberOfFrame * sizeof(sfSprite*));
	if (test.spriteArray == NULL)
	{
		printf("Error can't allocate dynamacily the array of sprite of the animation");
		test.actualDuration = -1.0f;
		return test;
	}

	printf("LOAD OF : %s\n", path);

	char directoryOfimage[250];
	for (int i = index; i < numberOfFrame + index; i++)
	{
		char* toAdd;
		if (i < 10)
		{
			toAdd = "000";
		}
		else if (i < 100)
		{
			toAdd = "00";
		}
		else if (i < 1000)
		{
			toAdd = "0";
		}
		else
		{
			toAdd = "";
		}

		sprintf(directoryOfimage, "%s%s%d.%s", path, toAdd, i, extension);
		test.spriteArray[i - index] = LoadSprite(directoryOfimage, isCentered);
		if (pos.x != 0 || pos.y != 0)
		{
			sfSprite_setPosition(test.spriteArray[i], pos);
		}
	}

	return test;

}

Animation LoadAnimationSpriteSheet(const int numberOfFrame, const float totalDuration, const sfVector2f pos, const bool isLoop)
{
	Animation test;

	test.numberOfFrame = numberOfFrame;
	test.totalDuration = totalDuration;
	test.actualDuration = 0.0f;
	test.spriteArray = NULL;
	test.actualFrameToShow = 0;
	test.animPosition = pos;
	test.loop = isLoop;
	test.isLoaded = true;

	return test;
}

Animation LoadAnimation(const int numberOfFrame, const float totalDuration, const char* path, const char* extension, const sfVector2f pos, const bool isLoop, const char isCentered)
{
	Animation test;

	test.numberOfFrame = numberOfFrame;
	test.totalDuration = totalDuration;
	test.actualDuration = 0.0f;
	test.spriteArray = NULL;
	test.actualFrameToShow = 0;
	test.animPosition = pos;
	test.loop = isLoop;
	test.isLoaded = true;
	test.StartFrame = 0;
	test.EndFrame = numberOfFrame - 1;
	test.spriteArray = (sfSprite**)malloc(numberOfFrame * sizeof(sfSprite*));
	if (test.spriteArray == NULL)
	{
		printf("Error can't allocate dynamacily the array of sprite of the animation");
		test.actualDuration = -1.0f;
		return test;
	}

	printf("LOAD OF : %s\n", path);

	char directoryOfimage[250];
	char* toAdd;
	for (int i = 0; i < numberOfFrame; i++)
	{
		if (i < 10)
		{
			toAdd = "000";
		}
		else if (i < 100)
		{
			toAdd = "00";
		}
		else if (i < 1000)
		{
			toAdd = "0";
		}
		else
		{
			toAdd = "0";
		}

		sprintf(directoryOfimage, "%s%s%d.%s", path, toAdd, i, extension);
		test.spriteArray[i] = LoadSprite(directoryOfimage, isCentered);
		if (pos.x != 0 || pos.y != 0)
		{
			sfSprite_setPosition(test.spriteArray[i], pos);
		}
	}

	return test;
}

void UpdateAnimationKey(AnimationKey *ToUpdate, const float deltatTime)
{
	ToUpdate->actualDuration += deltatTime;
	float temp = ToUpdate->actualDuration / ToUpdate->totalDuration;

	if (temp >= 1.0f)
	{
		if (ToUpdate->loop)
		{
			ToUpdate->actualDuration -= ToUpdate->totalDuration * (int)temp;
			temp = ToUpdate->actualDuration / ToUpdate->totalDuration;
		}
		else
		{
			ToUpdate->actualFrameToShow = ToUpdate->numberOfFrame - 1;
			return;
		}
	}

	ToUpdate->actualFrameToShow = (int)(ToUpdate->numberOfFrame * temp);
}

void UpdateAnimationKeyReversed(AnimationKey *ToUpdate, const float deltatTime)
{
	ToUpdate->actualDuration += deltatTime;
	float temp = ToUpdate->actualDuration / ToUpdate->totalDuration;

	if (temp >= 1.0f)
	{
		if (ToUpdate->loop)
		{
			ToUpdate->actualDuration -= ToUpdate->totalDuration * (int)temp;
			temp = ToUpdate->actualDuration / ToUpdate->totalDuration;
		}
		else
		{
			ToUpdate->actualFrameToShow = ToUpdate->numberOfFrame - 1;
			return;
		}
	}

	ToUpdate->actualFrameToShow = (int)((ToUpdate->numberOfFrame - 1) - ToUpdate->numberOfFrame * temp);
}

void UpdateAnimationKeyWithSpecialLoop(AnimationKey *ToUpdate, const float deltatTime, const int frameToBegin)
{
	ToUpdate->actualDuration += deltatTime;
	float temp = ToUpdate->actualDuration / ToUpdate->totalDuration;

	if (temp >= 1.0f)
	{
		if (ToUpdate->loop)
		{
			ToUpdate->actualDuration -= (ToUpdate->totalDuration * ((int)temp) - (ToUpdate->totalDuration / ToUpdate->numberOfFrame * (frameToBegin - 1)));
			temp = ToUpdate->actualDuration / ToUpdate->totalDuration;
		}
		else
		{
			ToUpdate->actualFrameToShow = ToUpdate->numberOfFrame - 1;
			return;
		}
	}

	ToUpdate->actualFrameToShow = (int)(ToUpdate->numberOfFrame * temp);
}

AnimationKey InitAnimKey(const int numberOfFrame, const float totalDuration, const bool isLoop)
{
	AnimationKey newKey;
	newKey.actualDuration = 0.0f;
	newKey.actualFrameToShow = 0;
	newKey.numberOfFrame = numberOfFrame;
	newKey.totalDuration = totalDuration;
	newKey.loop = isLoop;

	return newKey;
}

void ResetAnimKey(AnimationKey* animKey)
{
	animKey->actualDuration = 0.0f;
	animKey->actualFrameToShow = 0;
}

void SetStartIndexAnimation(Animation* animation, const int index)
{
	animation->StartFrame = index;
}

void SetEndIndexAnimation(Animation* animation, const int index)
{
	animation->EndFrame = index;
}