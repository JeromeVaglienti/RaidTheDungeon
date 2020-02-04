#ifndef _MAINSCENECONTROLLER_H
#define _MAINSCENECONTROLLER_H

#include <stdio.h>
#include <stdlib.h>
#include <SFML\Graphics.h>
#include "..\..\Model\Physics\Projectile\Shot.h"
#include "..\..\Model\Enemy\Enemy.h"
#include "..\..\Model\Loot\Loot.h"
#include "..\Camera\Camera.h"
#include "..\HUD\HUDController.h"
#include "..\WorldMapController\WorldMapController.h"
#include "..\State\State.h"

void InitGamedataForCampaign();
void LoadMainScene(const sfVideoMode Mode, const DungeonId ID);
void UpdateMainScene(sfRenderWindow *mainWindow, const float deltaTime);
void DisplayMainScene(sfRenderWindow *mainWindow);
#endif // !_MAINSCENECONTROLLER_H
