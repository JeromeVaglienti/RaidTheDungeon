#include "Sounds.h"

#define NBR_MUSIC 100
#define NBR_BUFFER 9
#define NBR_SOUND_UI 2

SoundsManager UIsounds[NBR_SOUND_UI];
SoundsManager soundsM[NBR_BUFFER];
sfMusic* musics[NBR_MUSIC];

float MASTER_VOLUME;
float MUSIC_VOLUME;
float FX_VOLUME;

void ChangeMainVolume();
void InitSoundsOfManager(SoundsManager* toInit, const int size);

void InitSoundManager()
{
	FILE* f;

	if (!(f = fopen("Ressources/Configuration/Sounds.s", "rb")))
	{
		f = fopen("Ressources/Configuration/Sounds.s", "w+");
		MASTER_VOLUME = 100.0f;
		MUSIC_VOLUME = 100.0f;
		FX_VOLUME = 100.0f;
		fwrite(&MASTER_VOLUME, 1, sizeof(float), f);
		fwrite(&MUSIC_VOLUME, 1, sizeof(float), f);
		fwrite(&FX_VOLUME, 1, sizeof(float), f);
	}
	else
	{
		fread(&MASTER_VOLUME, 1, sizeof(float), f);
		fread(&MUSIC_VOLUME, sizeof(float), 1, f);
		fread(&FX_VOLUME, sizeof(float), 1, f);
	}

	fclose(f);

	for (int i = 0; i < NBR_MUSIC; i++)
	{
		musics[i] = NULL;
	}

	musics[0] = sfMusic_createFromFile("Ressources/Sounds/MainMenu.flac");
	musics[1] = sfMusic_createFromFile("Ressources/Sounds/Credit.flac");
	sfMusic_setLoop(musics[0], sfTrue);

	UIsounds[scratch].Buffer = sfSoundBuffer_createFromFile("Ressources/Sounds/scratch_1.flac");
	UIsounds[scratch].AmountSounds = 10;

	UIsounds[pageFlip].Buffer = sfSoundBuffer_createFromFile("Ressources/Sounds/page-flip.wav");
	UIsounds[pageFlip].AmountSounds = 3;
	
	InitSoundsOfManager(UIsounds, NBR_SOUND_UI);

	for (int j = 0; j < NBR_BUFFER; j++)
	{
		soundsM[j].AmountSounds = 0;
	}

	ChangeMainVolume();
}

void WriteToFile()
{
	FILE* f;
	f = fopen("Ressources/Configuration/Sounds.s", "w+");
	fwrite(&MASTER_VOLUME, 1, sizeof(float), f);
	fwrite(&MUSIC_VOLUME, 1, sizeof(float), f);
	fwrite(&FX_VOLUME, 1, sizeof(float), f);
}

void InitSoundsOfManager(SoundsManager* toInit, const int size)
{
	for (int j = 0; j < size; j++)
	{
		toInit[j].Sounds = (sfSound**)calloc(toInit[j].AmountSounds, sizeof(sfSound*));
		for (unsigned int i = 0; i < toInit[j].AmountSounds; i++)
		{
			toInit[j].Sounds[i] = sfSound_create();
			sfSound_setBuffer(toInit[j].Sounds[i], toInit[j].Buffer);
		}
	}
}


void InitializeSounds()
{
	soundsM[0].Buffer = sfSoundBuffer_createFromFile("Ressources/Sounds/match.wav");
	soundsM[0].AmountSounds = 10;

	soundsM[1].Buffer = sfSoundBuffer_createFromFile("Ressources/Sounds/fireBall.wav");
	soundsM[1].AmountSounds = 3;

	soundsM[2].Buffer = sfSoundBuffer_createFromFile("Ressources/Sounds/flameThrower.wav");
	soundsM[2].AmountSounds = 3;
	
	soundsM[3].Buffer = sfSoundBuffer_createFromFile("Ressources/Sounds/explosion.wav");
	soundsM[3].AmountSounds = 10;

	soundsM[boi].Buffer = sfSoundBuffer_createFromFile("Ressources/Sounds/boi.wav");
	soundsM[boi].AmountSounds = 20;

	soundsM[beforeSpawn].Buffer = sfSoundBuffer_createFromFile("Ressources/Sounds/beforeSpawn.wav");
	soundsM[beforeSpawn].AmountSounds = 20;

	soundsM[openDoor].Buffer = sfSoundBuffer_createFromFile("Ressources/Sounds/Metal Open sound.wav");
	soundsM[openDoor].AmountSounds = 1;

	soundsM[closeDoor].Buffer = sfSoundBuffer_createFromFile("Ressources/Sounds/Metal Door Closing.wav");
	soundsM[closeDoor].AmountSounds = 1;

	soundsM[fireTrap].Buffer = sfSoundBuffer_createFromFile("Ressources/Sounds/fireTrap.flac");
	soundsM[fireTrap].AmountSounds = 1;

	musics[scary] = sfMusic_createFromFile("Ressources/Sounds/Scary Ambient Wind.flac");
	sfMusic_setLoop(musics[scary], sfTrue);

	musics[battle1] = sfMusic_createFromFile("Ressources/Sounds/Battle Theme 1.flac");
	sfMusic_setLoop(musics[battle1], sfTrue);

	musics[battle2] = sfMusic_createFromFile("Ressources/Sounds/Battle Theme 2.flac");
	sfMusic_setLoop(musics[battle2], sfTrue);
	
	musics[battle3] = sfMusic_createFromFile("Ressources/Sounds/Battle Theme 3.flac");
	sfMusic_setLoop(musics[battle3], sfTrue);
	
	musics[battle4] = sfMusic_createFromFile("Ressources/Sounds/Battle Theme 4.flac");
	sfMusic_setLoop(musics[battle4], sfTrue);
	
	musics[battle5] = sfMusic_createFromFile("Ressources/Sounds/Battle Theme 5.flac");
	sfMusic_setLoop(musics[battle5], sfTrue);

	musics[bossSlime] = sfMusic_createFromFile("Ressources/Sounds/BossSlime.flac");
	sfMusic_setLoop(musics[bossSlime], sfTrue);

	musics[ambiantElem] = sfMusic_createFromFile("Ressources/Sounds/Map_dirt.wav");
	sfMusic_setLoop(musics[ambiantElem], sfTrue);

	musics[ambiantElem2] = sfMusic_createFromFile("Ressources/Sounds/Map_Vent.wav");
	sfMusic_setLoop(musics[ambiantElem2], sfTrue);

	musics[ambiantElem3] = sfMusic_createFromFile("Ressources/Sounds/Map_Lava.wav");
	sfMusic_setLoop(musics[ambiantElem3], sfTrue);

	musics[ambiantElem4] = sfMusic_createFromFile("Ressources/Sounds/Map_Ice.wav");
	sfMusic_setLoop(musics[ambiantElem4], sfTrue);

	musics[battleElem] = sfMusic_createFromFile("Ressources/Sounds/Boss_Battle_Loop.wav");
	sfMusic_setLoop(musics[battleElem], sfTrue);

	musics[bossElem] = sfMusic_createFromFile("Ressources/Sounds/bossElem.wav");
	sfMusic_setLoop(musics[bossElem], sfTrue);

	musics[razarBattle] = sfMusic_createFromFile("Ressources/Sounds/Razar_Battle.wav");
	sfMusic_setLoop(musics[razarBattle], sfTrue);


	InitSoundsOfManager(soundsM, 9);

	ChangeMainVolume();
}

void PlayUISounds(const UISound sound)
{
	for (unsigned int i = 0; i < UIsounds[sound].AmountSounds; i++)
	{
		if (sfSound_getStatus(UIsounds[sound].Sounds[i]) != sfPlaying)
		{
			sfSound_play(UIsounds[sound].Sounds[i]);
			i = UIsounds[sound].AmountSounds;
		}
	}
}

void PlaySounds(const Sound sound)
{
	for (unsigned int i = 0; i < soundsM[sound].AmountSounds; i++)
	{
		if (sfSound_getStatus(soundsM[sound].Sounds[i]) != sfPlaying)
		{
			sfSound_play(soundsM[sound].Sounds[i]);
			i = soundsM[sound].AmountSounds;
		}
	}
}

void StopSounds(const Sound sound, const int index)
{
	sfSound_stop(soundsM[sound].Sounds[index]);
}

void PlayMusic(const MusicID musicID)
{
	if (sfMusic_getStatus(musics[musicID]) != sfPlaying)
	{
		sfMusic_play(musics[musicID]);
	}
}

void StopAllMusic()
{
	for (int i = 0; i < NBR_MUSIC; i++)
	{
		if (musics[i] == NULL)
		{
			return;
		}
		else if (sfMusic_getStatus(musics[i]) == sfPlaying)
		{
			sfMusic_stop(musics[i]);
		}
	}
}

void StopMusic(const MusicID musicID)
{
	if (sfMusic_getStatus(musics[musicID]) != sfStopped)
	{
		sfMusic_stop(musics[musicID]);
	}
}

void UpdateMusic(const float deltaTime)
{
}


float GetVolume(const int id)
{
	switch (id)
	{
		case 0:
			return(MASTER_VOLUME);
		case 1:
			return(MUSIC_VOLUME);
		case 2:
			return(FX_VOLUME);
		default:
			return(-1.0f);
	}
}

void ChangeMusicVolume()
{
	for (int i = 0; i < NBR_MUSIC; i++)
	{
		if (musics[i] != NULL)
		{
			sfMusic_setVolume(musics[i], ((MASTER_VOLUME/100.0f) * (MUSIC_VOLUME/100.0f)) * 100.0f);
		}
		else
		{
			i = NBR_MUSIC;
		}
	}
}

void ChangeFXVolume()
{
	for (int i = 0; i < NBR_BUFFER; i++)
	{
		if (soundsM[i].AmountSounds != 0)
		{
			for (unsigned int j = 0; j < soundsM[i].AmountSounds; j++)
			{
				sfSound_setVolume(soundsM[i].Sounds[j], ((MASTER_VOLUME / 100.0f) * (FX_VOLUME / 100.0f)) * 100.0f);
			}
		}
	}

	for (int i = 0; i < NBR_SOUND_UI; i++)
	{
		for (unsigned int j = 0; j < UIsounds[i].AmountSounds; j++)
		{
			sfSound_setVolume(UIsounds[i].Sounds[j], ((MASTER_VOLUME / 100.0f) * (FX_VOLUME / 100.0f)) * 100.0f);
		}
	}
}

void ChangeMainVolume()
{
	ChangeFXVolume();
	ChangeMusicVolume();
}

void SetVolume(const float value, const int id)
{
	switch (id)
	{
		case 0:
			MASTER_VOLUME = value;
			ChangeMainVolume();
			break;
		case 1:
			MUSIC_VOLUME = value;
			ChangeMusicVolume();
			break;
		case 2:
			FX_VOLUME = value;
			ChangeFXVolume();
			break;
		default:
#if _DEBUG
			printf("Error id of Volume not recognized\n");
#endif
			break;
	}

	WriteToFile();
}