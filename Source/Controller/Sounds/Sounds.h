#ifndef SOUNDS_H
#define SOUNDS_H
#include <stdio.h>
#include <stdlib.h>
#include "SFML\Audio.h"


typedef enum Sound
{
	match,
	fireball,
	flameThrower,
	explosion,
	openDoor,
	closeDoor,
	fireTrap,
	boi,
	beforeSpawn
}Sound;

typedef enum
{
	scratch,
	pageFlip
}UISound;


typedef enum
{
	mainMenuMusic = 0,
	creditsMusic = 1,
	scary = 2,
	battle1 = 3,
	battle2 = 4,
	battle3 = 5,
	battle4 = 6,
	battle5 = 7,
	bossSlime = 8,
	ambiantElem = 9,
	ambiantElem2 = 10,
	ambiantElem3 = 11,
	ambiantElem4 = 12,
	battleElem = 13,
	bossElem = 14,
	razarBattle = 15
}MusicID;

typedef struct
{
	sfSound** Sounds;
	unsigned int AmountSounds;
	sfSoundBuffer* Buffer;
}SoundsManager;

void InitSoundManager();

void InitializeSounds();

void PlayUISounds(const UISound sound);

void PlaySounds(const Sound sound);

void StopSounds(const Sound sound, const int index);

void PlayMusic(const MusicID musicID);

void UpdateMusic(const float deltaTime);

void StopAllMusic();

float GetVolume(const int id);

void SetVolume(const float value, const int id);

void StopMusic(const MusicID musicID);

#endif
