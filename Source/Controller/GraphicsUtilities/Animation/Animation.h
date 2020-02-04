#ifndef _ANIMATION_H
#define _ANIMATION_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../Utilities.h"

typedef struct
{
	int StartFrame;
	int EndFrame;
	int numberOfFrame;
	int actualFrameToShow;
	float actualDuration;
	float totalDuration;
	sfVector2f animPosition;
	sfSprite** spriteArray;
	bool loop;
	bool isLoaded;
} Animation;

//Key Represent Data for sameAnimation
typedef struct
{
	int numberOfFrame;
	int actualFrameToShow;
	float actualDuration;
	float totalDuration;
	bool loop;
}AnimationKey;

//Update actualFrameToShow Based on the actualTimeElapsed / totalDuraction of the anim
void UpdateAnimation(Animation *animToPlay, const float timeElapsed);
void UpdateAnimationReversed(Animation *animToPlay, const float timeElapsed);
//Unload a Animation
void UnLoadAnimation(Animation *toUnLoad);

void ReScaleAnimation(Animation* toRescale, const float newSizeX, const float newSizeY, const float ratioX, const float ratioY);
//Unload only on frame of a Animation
void UnLoadFrameOfAnim(Animation *toUnLoad, const int index);
//Reload an Animation To Origin
void ReloadAnimationOrigin(Animation *toReload, const char* path, const char* extension);
//Reload an Animation To a Particular Position
void ReloadAnimation(Animation *toReload, const char* path, const char* extension, const sfVector2f pos);
//Load an Animation starting to a specified index of first frame
Animation LoadAnimationSpecifiedIndexStart(const int numberOfFrame, const float totalDuration, const char* path, const char* extension, const sfVector2f pos, const bool isLoop, const int index, const char isCentered);

Animation LoadAnimationSpriteSheet(const int numberOfFrame, const float totalDuration, const sfVector2f pos, const bool isLoop);
//Load an Animation
Animation LoadAnimation(const int numberOfFrame, const float totalDuration, const char* path, const char* extension, const sfVector2f pos, const bool isLoop, const char isCentered);
//Update an AnimationKey based on the same manipulation of UpdateAnimation
void UpdateAnimationKey(AnimationKey *ToUpdate, const float deltatTime);
void UpdateAnimationKeyReversed(AnimationKey *ToUpdate, const float deltatTime);
//Update an animation key but restart the animation to a specific frame
void UpdateAnimationKeyWithSpecialLoop(AnimationKey *ToUpdate, const float deltatTime, const int frameToBegin);
//Init an AnimKey
AnimationKey InitAnimKey(const int numberOfFrame, const float totalDuration, const bool isLoop);

void ResetAnimKey(AnimationKey* animKey);

void SetStartIndexAnimation(Animation* animation, const int index);

void SetEndIndexAnimation(Animation* animation, const int index);

#endif // !_ANIMATION_H