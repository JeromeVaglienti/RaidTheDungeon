#ifndef _PORTAL_H
#define _PORTAL_H

#include <stdio.h>
#include <stdlib.h>
#include <SFML\Graphics.h>
#include "..\Player\Player.h"
#include "..\..\Controller\GraphicsUtilities\Animation\Animation.h"

typedef struct PORTAL
{
	sfVector2f Dimension;
	sfVector2f PosCenter;
	int ID;
	float Timer;
	float TimeForTransfert;
	float TimerEndOfTransfer;
	float TimeForEndOfTransfer;
	sfBool IsTransfering;
	sfBool IsNOTActive;
	struct PORTAL* LinkPortal;
	AnimationKey TelePortAnimKey;
	AnimationKey OpenAnimKey;
	AnimationKey ReceivingAnimKey;
	sfBool IsReceivingTransfert;
}Portal;


void InitPortal(Portal* ToInit, const sfVector2f Pos, const sfVector2f Dimension, const float TimeForTransfert, const float TimeForEndOfTransfer);

sfBool UpdatePortals(Portal* ToUpdate, Player* ArrayOfPlayer, const int nbPlayer, const float deltaTime);


#endif // !_PORTAL_H
