#ifndef _BOSSCONTROLLERH
#define _BOSSCONTROLLERH
#include <stdio.h>
#include <stdlib.h>
#include "..\..\GraphicsUtilities\Animation\Animation.h"
#include "..\..\..\Model\Boss\Boss.h"
#include "..\..\Camera\Camera.h"

typedef struct
{
	Animation AnimationBoss[3];
}BossGraphics;

void InitBossControllerSlime();
void InitBossControllerElem();
void InitBossControllerRazar();
void UnloadBossControllerSlime();
void UnloadBossControllerElem();
void UnloadBossControllerRazar();
void UpdateAnimationSyncWithAttack(Boss* boss, const float deltaTime);
void ManageBossControllerElem();
void UpdateAnimationFirstLayer(Boss* boss, const float deltaTime);
void UpdateBossController(Boss* boss, const float deltaTime);
void DisplayBossController(const Boss boss, sfRenderWindow* window, const Camera pCam, const DungeonGraphicData dungeonGraphicData);
void DisplayBossControllerLayer1(const Boss boss, sfRenderWindow* window, const Camera pCam);
#endif // !_BOSSCONTROLLERH
