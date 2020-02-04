#ifndef SKINS_CONTROLLER_H
#define SKINS_CONTROLLER_H
#include "..\..\GraphicsUtilities\Animation\Animation.h"

typedef enum SkinPlayer
{
	red,
	black,
	white,
	green
}SkinPlayer;

typedef struct
{
	int NumberOfPlayer;
	int ActualPlayer;
	SkinPlayer *SkinPlayer;
}PlayersInformations;

void InitSkin();
PlayersInformations GetPlayersInformations();
void SetNumberPlayer(const int playerNumber);
void SetActualPlayer(const int actualPlayer);
void SetPlayer1Informations(const SkinPlayer skinPlayer);
void SetPlayer2Informations(const SkinPlayer skinPlayer);


#endif
