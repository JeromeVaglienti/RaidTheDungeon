#ifndef _PLAYERCONTROLLER_H
#define _PLAYERCONTROLLER_H

#include <stdio.h>
#include <stdlib.h>
#include <SFML\Graphics.h>
#include "..\SpellsController\SpellsController.h"
#include "..\ShadersController\ShadersController.h"
#include "..\..\Particles\Particles.h"
#include "..\..\Camera\Camera.h"
#include "..\..\State\State.h"
//Register Different action that the user can do as the player
typedef struct
{
	sfBool isFiring;
	sfBool isCastSpell;
	sfBool isDashing;
	short ChangingWeapon;
	int IDPlayer;
}ActionListener;

typedef struct
{
	sfSprite* Cursor;
	sfSprite* WetIndicator;
	sfSprite* WetLightIndicator;
	sfSprite* PlayerCircle[2];
	sfSprite* DustParticle;
	sfSprite* WaveParticle;
	Animation **AnimationMovement;
	sfSprite*** AnimationIdle;
	Particles* ParticlesPlayer[2];
	Particles* ParticlesMovementPlayer[2];
	Particles* ParticlesSlowPlayer[2];
	sfSprite* PlayerNormalMap;
	Animation DeathAnim;
}PlayerGraphics;


void InitPlayerGraphics(const sfVideoMode Mode);
void ResetActionListener(ActionListener* toReset);
void InitPlayerPointer(const void *playerToPointTo, const int idPlayer);
Player *GetPlayerPointer(const int IdPlayer);
void TakeInputMovement(const sfVector2f dirMov, const int idPlayer);
void TakeInputViewMovement(const sfVector2f, const int idPlayer);
void TakeInputAction(const sfVector2f dirShot, const ActionListener actions);
void TakeInputSkillTree(const sfEvent event, const int idPLayer);
void UpdatePlayerController(const int numberOfPlayer, const float deltaTime);
void DisplayPlayers(sfRenderWindow* mainWindow, const Camera Cam, const int numberOfPlayer);
void DisplayCursor(const int numberOfPlayer, sfRenderWindow* mainWindow, const sfView* view);
PlayerGraphics* GetPlayerGraphics();
void SetAnimationMovementPlayerGraphics(const char skinPlayer, const char* path, const int indexPlayer);
void SetAnimationIdlePlayerGraphics(const char skinPlayer, const char* path, const int indexPlayer);
void AllocateAnimation(const int numberOfPlayer);
void UnloadAnimationPlayerGraphics(const int numberOfPlayer);

#endif // !_PLAYERCONTROLLER_H