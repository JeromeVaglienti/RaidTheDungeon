#ifndef _HUDCONTROLLER_H
#define _HUDCONTROLLER_H

#include <stdio.h>
#include <stdlib.h>
#include <SFML\Graphics.h>
#include "..\..\Model\Config\Config.h"
#include "..\..\Model\GameData.h"
#include "..\State\State.h"
#include "..\MainSceneController\SkinsController\SkinsController.h"
#include "..\..\Model\Procedural\ProceduralMain.h"
#include "..\..\View\EventList\EventList.h"

typedef struct
{
	sfText *FPS;
	sfSprite* LifeBarBoss;
	sfSprite* LifeBarBossOutline;
	sfSprite* MiniMap;
}HUDDisplayer;

void InitUI(const sfVideoMode mode);
void InitHUD(const sfVideoMode mode, const PlayersInformations playerInformations);
int UpdateUIOnEvent(EventList *eventList, const int typeOfGamePad);
void UpdatePlayerHUD(const Player player, const int id, const float dt);
void UpdatePlayerCDHUD(const Player player, const int id);
void UpdatePlayerSpellSelectedHUD(const Player player, const int id);
void UpdateHUD(const GameData gameData, const float deltaTime, const sfVideoMode mode);
void UpdateUI(const float deltaTime);
void DisplayUI(sfRenderWindow *mainWindow);
void DisplayHUD(const Camera *camera, const Player *player, const int numberOfPlayer, const Dungeon mainDungeon, sfRenderWindow *mainWindow);
sfBool GetRestart();
void SetRestart(const sfBool ptrue);
#endif // !_HUDCONTROLLER_H