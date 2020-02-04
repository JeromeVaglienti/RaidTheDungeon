#ifndef _PLAYERINFO_H
#define _PLAYERINFO_H

#include <stdio.h>
#include <stdlib.h>
#include <SFML\Graphics.h>
#include "..\..\GraphicsUtilities\Utilities.h"
#include "..\..\MainSceneController\SkinsController\SkinsController.h"

typedef struct
{
	sfSprite* LifeBar;
	sfSprite* ManaBar;
	sfSprite* PlayerBackGround;
	sfSprite* PlayerSkin;
	sfVertexArray* ManaVertexScale;
	sfRenderStates* ManaSpriteRS;
	sfVector2f ManaTextureSize;
	sfVector2f LifeBarScaleFull;
	sfVector2f ManaBarScaleFull;
	float LifeBarRatio;
	float ManaBarRatio;
	sfSprite* Spell[2][4];
	sfSprite* Selector;
	unsigned char spellIsInCD[4];
}PlayerInfo;


void InitPlayerInfo(PlayerInfo *toInit, const int id, const sfVideoMode mode, const PlayersInformations playerInfo);
void UpdatePlayerInfo(PlayerInfo *toUpdate, const int data[], const int id, const float dt);
void UpdatePlayerCDInfo(PlayerInfo *toUpdate, const int data[]);
void UpdatePlayerSpellSelected(PlayerInfo *toUpdate, const short spellSelected);
void DisplayPlayerInfo(sfRenderWindow* mainWindow, const PlayerInfo ToDisplay);

#endif // !_PLAYERINFO_H
