#ifndef _ENEMYCONTROLLER_H
#define _ENEMYCONTROLLER_H

#include <stdio.h>
#include <stdlib.h>
#include <SFML\Graphics.h>
#include "..\..\..\Model\Enemy\Enemy.h"
#include "..\..\Camera\Camera.h"
#include "..\..\..\Model\Boss\Boss.h"

typedef struct
{
	sfSprite** Slime;
	sfSprite* Smudge[3];
	sfSprite** PartKamikaze;
	sfSprite** SlimeTransition;
	Animation* SlimeAnim;
	Animation* Death;
	sfSprite** SheetSpawnSlime;
	Animation MouthMiniBoss;
	sfVector2f Ratio;
	sfVector2f* Scale;
	Particles** ParticlesEnnemies;
	sfVertexArray* enemyVertexArraySlime[6];
	sfVertexArray* transitionVertexArraySlime[6];
	sfVertexArray* spawnVertexArraySlime[7];
	Animation UpgradeSlime[6][8];
}EnemySpriteContainer;

typedef struct
{
	sfVector2f* Scale;
	sfSprite*** Elem;
	sfSprite** SheetSpawnElem;
	sfVertexArray* enemyVertexArrayElem[9][4];
	sfVertexArray* spawnVertexArrayElem[9];
}EnemySpriteElemContainer;

void InitEnemySlimeController(const sfVideoMode mode);
void UnloadEnemySlimeController();
void InitEnemyElemController(const sfVideoMode mode);
void UnloadEnemyElemController();
void DisplayEnemiesElem(const Enemy* firstElement, sfRenderWindow* window, const Camera pCam);
void DisplayEnemiesSlime(const Enemy* firstElement, sfRenderWindow* window, const Camera pCam);
Particles** GetParticlesEnnemies();
void DisplayEnemyParticles(sfRenderWindow* window);
void DisplayEnemyVertexArrayElem(sfRenderWindow* window);
void DisplayEnemyVertexArraySlime(sfRenderWindow* window);
#endif // !_PLAYERCONTROLLER_H