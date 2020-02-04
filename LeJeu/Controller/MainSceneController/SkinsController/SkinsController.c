#include <stdlib.h>
#include "SkinsController.h"
#include "..\..\..\Model\Player\Player.h"
#include "..\PlayerController\PlayerController.h"

PlayersInformations playerInformations = { 0 };
void InitSkin()
{
	AllocateAnimation(playerInformations.NumberOfPlayer);
	for (int i = 0; i < playerInformations.NumberOfPlayer; i++)
	{
		SetAnimationMovementPlayerGraphics(playerInformations.SkinPlayer[i], "Ressources/Sprites/Players/AnimationPlayer", i);
		SetAnimationIdlePlayerGraphics(playerInformations.SkinPlayer[i], "Ressources/Sprites/Players/AnimationPlayer", i);
	}
}

PlayersInformations GetPlayersInformations()
{
	return(playerInformations);
}

void SetNumberPlayer(const int playerNumber)
{
	if (playerInformations.SkinPlayer != NULL)
	{
		free(playerInformations.SkinPlayer);
		playerInformations.SkinPlayer = NULL;
	}
	playerInformations.NumberOfPlayer = playerNumber;
	playerInformations.SkinPlayer = (SkinPlayer*)malloc(playerNumber * sizeof(SkinPlayer));
}

void SetActualPlayer(const int actualPlayer)
{
	playerInformations.ActualPlayer = actualPlayer;
}
//informations about the class of the player
void SetPlayer1Informations(const SkinPlayer skinPlayer)
{
	playerInformations.SkinPlayer[0] = skinPlayer;
}

void SetPlayer2Informations(const SkinPlayer skinPlayer)
{
	playerInformations.SkinPlayer[1] = skinPlayer;
}