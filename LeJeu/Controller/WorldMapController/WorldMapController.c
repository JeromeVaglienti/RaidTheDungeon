#include "WorldMapController.h"
#include "..\State\State.h"
#include "..\HUD\SkillTreeController\SkillTreeController.h"
#include "..\MainSceneController\PlayerController\PlayerController.h"
#include "..\..\View\GamePad.h"

WordlMapRessources MapRes;

typedef enum
{
	Map_Phase1,
	Map_Phase2,
	Map_Phase3
}WorldPhase;

float delay = 0.0f;
float delayMax = 2.0f;

WorldPhase actualMapPhase;

void InitWorldMapController(const sfVideoMode mode)
{
	MapRes.WorldMap = LoadSprite("Ressources/Sprites/WorldMap/Main_Map.png", 0);
	sfSprite_setPosition(MapRes.WorldMap, (sfVector2f) { 0.0f, 0.0f });
	sfVector2f temp = { mode.width / 1920.0f, mode.height / 1080.0f };
	sfSprite_setScale(MapRes.WorldMap, temp);
	delay = 0.0f;
	delayMax = 2.0f;

	actualMapPhase = Map_Phase1;
}

void UpdateWorldMapController(const float deltaTime)
{
	if (actualMapPhase != Map_Phase2)
	{
		delay += deltaTime;

		if (delay >= delayMax)
		{
			if (actualMapPhase == Map_Phase1)
			{
				SkillTree* skillTree = GetPointerToSkillTree(0);
				actualMapPhase = Map_Phase2;
				skillTree->IsOpen = sfTrue;
				ChangeTextSkillTree(skillTree, GetPlayerPointer(0)->Stats.XP);
			}
			else if (actualMapPhase == Map_Phase3)
			{
				SetSceneAfterWorldMap();
				actualMapPhase = Map_Phase1;
			}
			delay = 0.0f;
		}
	}

	if (Map_Phase2)
	{
		if (sfKeyboard_isKeyPressed(sfKeyTab) || JoystickIsButtonPressed(0, BACK) || JoystickIsButtonPressed(1, BACK))
		{
			AssignSkillTreeToPlayer(GetPlayerPointer(0));
			actualMapPhase = Map_Phase3;
			GetPointerToSkillTree(0)->IsOpen = sfFalse;
		}
	}
}

void DisplayWorldMapController(sfRenderWindow* mainWindow)
{
	sfRenderWindow_drawSprite(mainWindow, MapRes.WorldMap, NULL);
	DisplaySkillTree(mainWindow, GetPointerToSkillTree(0));
}