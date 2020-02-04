#include "HUDController.h"
#include "Menu\Menu.h"
#include "Menu\MenuProto.h"
#include "Text\Text.h"
#include "PlayerInfo\PlayerInfo.h"
#include "..\Transition\Transition.h"
#include "..\MainSceneController\ShadersController\ShadersController.h"
#include "..\Localization\Localization.h"
#include "..\MainSceneController\PlayerController\PlayerController.h"
#include "..\..\View\MapKey\MapKey.h"
#include "..\Serialization\Serialization.h"
#include "..\Sounds\Sounds.h"
#include "..\..\View\ViewManager.h"
#include "..\..\View\GamePad.h"
#include "SkillTreeController\SkillTreeController.h"

Menu menus[15];
sfEvent evt;
sfText* Version;

typedef enum ACTUALMENU
{
	main,
	selectNumberPlayer,
	selectSkinPlayer,
	option,
	general,
	controls,
	graphics,
	soundOption,
	pause,
	optionPause,
	loadingGame,
	savingGame,
	shortcuts,
	credit,
	newOrLoad
}ActualMenu;

ActualMenu actualMenu = main;
HUDDisplayer hudDisplayer;
sfBool needToDisplayBossLife = sfFalse;
PlayerInfo *_HUDController_playerInfo[2] = { 0 };
sfBool isReload = sfFalse;
sfBool isRestarted = sfFalse;
sfBool AlreadyInPaused = sfFalse;
sfBool IsLoadingGame = sfFalse;
SkillTree* pointerToSkillTrees[2] = { NULL, NULL };
//sfBool skillTreeIsOpen[2] = { sfFalse, sfFalse };

void InitUI(const sfVideoMode mode)
{
	Config config = GetConfig();
	InitTransitions(mode);
	actualMenu = main;
	menus[main] = CreateMainMenuProto(mode);
	menus[selectNumberPlayer] = CreateSelectionPlayerMenuProto(mode);
	menus[selectSkinPlayer] = CreateSelectionSkinPlayerMenuProto(mode);
	menus[option] = CreateOptionMenuProto(mode);
	menus[general] = CreateGeneralMenuProto(config);
	menus[graphics] = CreateGraphicsMenuProto(config);
	menus[soundOption] = CreateSoundOptionProto(config);
	menus[pause] = CreatePauseMenuProto(mode);
	menus[optionPause] = CreateOptionPauseMenuProto(mode);
	menus[controls] = CreateControlsMenuProto(mode);
	menus[loadingGame] = CreateLoadingMenu(mode);
	menus[savingGame] = CreateSavingMenu(mode);
	menus[shortcuts] = CreateShortcutsMenuProto(mode);
	menus[credit] = CreateCreditsMenuProto(mode);
	menus[newOrLoad] = CreateNewOrLoad(mode);
	for (int i = 0; i < 15; i++)
	{
		menus[i].Index = i;
	}

	_HUDController_playerInfo[0] = _HUDController_playerInfo[1] = NULL;


	Version = InitText("V0.6", (sfVector2f) { (mode.width / 1920.0f) * 1790.0f, (mode.height / 1080.0f) * 990.0f}, sfBlack, (const int)(20 * (mode.height / 1080.0f)));
}

void InitHUD(const sfVideoMode mode, const PlayersInformations playerInformations)
{
	float ratioX = mode.width / 1920.0f;
	float ratioY = mode.height / 1080.0f;
	if (!isReload)
	{
		hudDisplayer.FPS = InitText("", (sfVector2f) { 0.f, 0.f }, sfWhite, 25);
		hudDisplayer.LifeBarBoss = LoadSprite("Ressources/Sprites/Boss/Dungeon1/LifeBar.png", 0);
		hudDisplayer.LifeBarBossOutline = LoadSprite("Ressources/Sprites/Boss/Dungeon1/LifeBarContour.png", 0);
		hudDisplayer.MiniMap = LoadSprite("Ressources/Sprites/HUD/MiniMap/Cadre_minimap.png", 0);

		isReload = sfTrue;
	}
	InitSkillTreeController(mode);
	sfVector2f scale;
	sfVector2u size = sfTexture_getSize(sfSprite_getTexture(hudDisplayer.LifeBarBoss));
	scale.x = (500.f * ratioX) / (float)size.x;
	scale.y = (20.f * ratioY) / (float)size.y;
	sfSprite_setScale(hudDisplayer.LifeBarBoss, scale);
	size = sfTexture_getSize(sfSprite_getTexture(hudDisplayer.LifeBarBossOutline));
	scale.x = (510.f * ratioX) / (float)size.x;
	scale.y = (30.f * ratioY) / (float)size.y;
	sfSprite_setScale(hudDisplayer.LifeBarBossOutline, scale);
	size = sfTexture_getSize(sfSprite_getTexture(hudDisplayer.MiniMap));
	scale.x = (308.f * ratioX) / (float)size.x;
	scale.y = (308.f * ratioY) / (float)size.y;
	sfSprite_setScale(hudDisplayer.MiniMap, scale);

	for (int i = 0; i < playerInformations.NumberOfPlayer; i++)
	{
		if (_HUDController_playerInfo[i] == NULL)
		{
			_HUDController_playerInfo[i] = (PlayerInfo*)calloc(1, sizeof(PlayerInfo));
		}

		InitPlayerInfo(_HUDController_playerInfo[i], i, getConfigMode(), playerInformations);
	}
}

int UpdateUIOnEvent(EventList *eventList, const int typeOfGamePad)
{
	KeyMaping returnedValue = UpdateMenu(&menus[actualMenu], eventList, typeOfGamePad);
	if (returnedValue.indexSelector != -1)
	{
		switch (actualMenu)
		{
		case(main):
			if (returnedValue.indexSelector == 0)
			{//go to the selection player(1)
				menus[actualMenu].selectionCursor.indexPos = 0;
				sfVector2f newPos = { menus[actualMenu].Position.x, menus[actualMenu].Position.y };
				sfSprite_setPosition(menus[actualMenu].selectionCursor.indexDisplay, newPos);
				actualMenu = newOrLoad;
				if (!SearchingSave(1, -1) && !SearchingSave(2, -1))
				{
					SetColorButton(&menus[actualMenu].button[1], sfColor_fromRGB(105, 105, 105));
				}
				else
				{
					SetColorButton(&menus[actualMenu].button[1], sfColor_fromRGB(171, 144, 111));
				}
			}
			else if (returnedValue.indexSelector == 1)
			{
				menus[actualMenu].selectionCursor.indexPos = 0;
				sfVector2f newPos = { menus[actualMenu].Position.x, menus[actualMenu].Position.y };
				sfSprite_setPosition(menus[actualMenu].selectionCursor.indexDisplay, newPos);
				actualMenu = option;
			}
			else if (returnedValue.indexSelector == 2)
			{//go to option
				menus[actualMenu].selectionCursor.indexPos = 0;
				sfVector2f newPos = { menus[actualMenu].Position.x, menus[actualMenu].Position.y };
				sfSprite_setPosition(menus[actualMenu].selectionCursor.indexDisplay, newPos);
				actualMenu = credit;
				StopMusic(0);
				PlayMusic(1);
			}
			else if (returnedValue.indexSelector == 3)
			{
				exit(0);
			}
			break;
		case(selectNumberPlayer):
			if (returnedValue.indexSelector == 0)
			{
				if (!menus[actualMenu].selectors[0].isOn)
				{
					menus[actualMenu].selectors[0].isOn = sfTrue;

					sfVector2f newPos = sfSprite_getPosition(menus[actualMenu].selectors[menus[actualMenu].selectionCursor.indexPos].selectorBackground);
					sfSprite_setPosition(menus[actualMenu].selectionCursor.indexDisplay, newPos);
				}
				else
				{
					menus[actualMenu].selectors[0].isOn = sfFalse;
					sfVector2f newPos = sfSprite_getPosition(menus[actualMenu].button[menus[actualMenu].selectionCursor.indexPos].sprite);
					sfSprite_setPosition(menus[actualMenu].selectionCursor.indexDisplay, newPos);
				}
			}
			else if (returnedValue.indexSelector == 1)
			{
				if (!menus[actualMenu].selectors[1].isOn)
				{
					menus[actualMenu].selectors[1].isOn = sfTrue;

					sfVector2f newPos = sfSprite_getPosition(menus[actualMenu].selectors[menus[actualMenu].selectionCursor.indexPos].selectorBackground);
					sfSprite_setPosition(menus[actualMenu].selectionCursor.indexDisplay, newPos);
				}
				else
				{
					menus[actualMenu].selectors[1].isOn = sfFalse;
					sfVector2f newPos = sfSprite_getPosition(menus[actualMenu].button[menus[actualMenu].selectionCursor.indexPos].sprite);
					sfSprite_setPosition(menus[actualMenu].selectionCursor.indexDisplay, newPos);
				}
			}
			else if (returnedValue.indexSelector == 2 && menus[actualMenu].selectors[0].textDisplay.actualTextToDisplay == 1)//1 = OneLevel
			{
				if (!menus[actualMenu].selectors[2].isOn)
				{
					menus[actualMenu].selectors[2].isOn = sfTrue;

					sfVector2f newPos = sfSprite_getPosition(menus[actualMenu].selectors[menus[actualMenu].selectionCursor.indexPos].selectorBackground);
					sfSprite_setPosition(menus[actualMenu].selectionCursor.indexDisplay, newPos);
				}
				else
				{
					menus[actualMenu].selectors[2].isOn = sfFalse;
					sfVector2f newPos = sfSprite_getPosition(menus[actualMenu].button[menus[actualMenu].selectionCursor.indexPos].sprite);
					sfSprite_setPosition(menus[actualMenu].selectionCursor.indexDisplay, newPos);
					SetIDDungeon(menus[actualMenu].selectors[1].textDisplay.actualTextToDisplay);
				}
			}
			else if (returnedValue.indexSelector == 3)
			{
				SetTypeOfGame(menus[actualMenu].selectors[0].textDisplay.actualTextToDisplay + 1);
				SetIDDungeon(menus[actualMenu].selectors[2].textDisplay.actualTextToDisplay);

				if (menus[actualMenu].selectors[1].textDisplay.actualTextToDisplay == 0)
				{
					if (!IsLoadingGame)
					{
						menus[actualMenu].selectionCursor.indexPos = 0;
						sfVector2f newPos = { menus[actualMenu].Position.x, menus[actualMenu].Position.y };
						sfSprite_setPosition(menus[actualMenu].selectionCursor.indexDisplay, newPos);
						actualMenu = selectSkinPlayer;
						SetActualPlayer(1);
						SetNumberPlayer(1);
					}
					else
					{
						if (SearchingSave(1, -1))
						{
							menus[actualMenu].selectionCursor.indexPos = 0;
							sfVector2f newPos = { menus[actualMenu].Position.x, menus[actualMenu].Position.y };
							sfSprite_setPosition(menus[actualMenu].selectionCursor.indexDisplay, newPos);
							actualMenu = loadingGame;
							SetNumberPlayer(1);
							UpdateMenuLoading(&menus[actualMenu], getConfigMode());
						}
					}
				}
				else
				{
					if (!IsLoadingGame)
					{
						menus[actualMenu].selectionCursor.indexPos = 0;
						sfVector2f newPos = { menus[actualMenu].Position.x, menus[actualMenu].Position.y };
						sfSprite_setPosition(menus[actualMenu].selectionCursor.indexDisplay, newPos);
						actualMenu = selectSkinPlayer;
						SetActualPlayer(1);
						SetNumberPlayer(2);
					}
					else
					{
						if (SearchingSave(2, -1))
						{
							menus[actualMenu].selectionCursor.indexPos = 0;
							sfVector2f newPos = { menus[actualMenu].Position.x, menus[actualMenu].Position.y };
							sfSprite_setPosition(menus[actualMenu].selectionCursor.indexDisplay, newPos);
							actualMenu = loadingGame;
							SetNumberPlayer(2);
							UpdateMenuLoading(&menus[actualMenu], getConfigMode());
						}
					}
				}
			}
			else if (returnedValue.indexSelector == 4)
			{//go back to main menu
				menus[actualMenu].selectionCursor.indexPos = 0;
				sfVector2f newPos = { menus[actualMenu].Position.x, menus[actualMenu].Position.y };
				sfSprite_setPosition(menus[actualMenu].selectionCursor.indexDisplay, newPos);
				actualMenu = newOrLoad;
				if (!SearchingSave(1, -1) && !SearchingSave(2, -1))
				{
					SetColorButton(&menus[actualMenu].button[1], sfColor_fromRGB(105, 105, 105));
				}
				else
				{
					SetColorButton(&menus[actualMenu].button[1], sfColor_fromRGB(171, 144, 111));
				}
			}
			break;
		case(selectSkinPlayer):
			if (returnedValue.indexSelector == 0)
			{
				PlayersInformations playerInfo = GetPlayersInformations();
				//go to the second selection class player
				if (playerInfo.NumberOfPlayer == 2 && playerInfo.ActualPlayer == 1)
				{
					menus[actualMenu].selectionCursor.indexPos = 0;
					sfVector2f newPos = { menus[actualMenu].Position.x, menus[actualMenu].Position.y };
					sfSprite_setPosition(menus[actualMenu].selectionCursor.indexDisplay, newPos);
					SetPlayer1Informations(menus[actualMenu].classDisplayer.ActualClassToDisplay);
					menus[actualMenu].classDisplayer.ActualClassToDisplay = 0;
					SetTitleMenu(&menus[actualMenu], "Ressources/Sprites/UI/p2.png");
					actualMenu = selectSkinPlayer;
					SetActualPlayer(2);
				}
				else if (playerInfo.NumberOfPlayer == 2 && playerInfo.ActualPlayer == 2)
				{
					SetPlayer2Informations(menus[actualMenu].classDisplayer.ActualClassToDisplay);
					SetScene(mainScene);
					StopMusic(0);
					//PlayMusic(2);*/
					SetTitleMenu(&menus[actualMenu], "Ressources/Sprites/UI/p1.png");
					actualMenu = main;
				}
				else
				{
					SetPlayer1Informations(menus[actualMenu].classDisplayer.ActualClassToDisplay);
					SetScene(mainScene);
					StopMusic(0);
					//PlayMusic(2);*/
					actualMenu = main;
				}
			}
			else if (returnedValue.indexSelector == 1)
			{//go back to the choice of the number of the player
				if (GetPlayersInformations().ActualPlayer == 1)
				{
					menus[actualMenu].selectionCursor.indexPos = 0;
					sfVector2f newPos = { menus[actualMenu].Position.x, menus[actualMenu].Position.y };
					sfSprite_setPosition(menus[actualMenu].selectionCursor.indexDisplay, newPos);
					menus[actualMenu].classDisplayer.ActualClassToDisplay = 0;
					actualMenu = selectNumberPlayer;
				}
				else
				{
					menus[actualMenu].selectionCursor.indexPos = 0;
					sfVector2f newPos = { menus[actualMenu].Position.x, menus[actualMenu].Position.y };
					sfSprite_setPosition(menus[actualMenu].selectionCursor.indexDisplay, newPos);
					menus[actualMenu].classDisplayer.ActualClassToDisplay = 0;
					SetTitleMenu(&menus[actualMenu], "Ressources/Sprites/UI/p1.png");
					actualMenu = selectSkinPlayer;
					SetActualPlayer(1);
				}
			}
			break;
		case(loadingGame):
			if (returnedValue.indexSelector < 4)
			{//go to the first selection class player	
				if (SearchingSave(GetPlayersInformations().NumberOfPlayer, returnedValue.indexSelector))
				{
					LoadGame(GetPlayersInformations().NumberOfPlayer, returnedValue.indexSelector);
					SetIDDungeon(GetSaveData().IdDungeon);
					SetTypeOfGame(GetSaveData().TypeOfGame);
					printf("type game %d\n", GetSaveData().TypeOfGame);
					menus[actualMenu].selectionCursor.indexPos = 0;
					sfVector2f newPos = { menus[actualMenu].Position.x, menus[actualMenu].Position.y };
					sfSprite_setPosition(menus[actualMenu].selectionCursor.indexDisplay, newPos);
					StopMusic(0);
					SetScene(mainScene);
				}
			}
			else
			{
				menus[actualMenu].selectionCursor.indexPos = 0;
				sfVector2f newPos = { menus[actualMenu].Position.x, menus[actualMenu].Position.y };
				sfSprite_setPosition(menus[actualMenu].selectionCursor.indexDisplay, newPos);
				actualMenu = newOrLoad;
				if (!SearchingSave(1, -1) && !SearchingSave(2, -1))
				{
					SetColorButton(&menus[actualMenu].button[1], sfColor_fromRGB(105, 105, 105));
				}
				else
				{
					SetColorButton(&menus[actualMenu].button[1], sfColor_fromRGB(171, 144, 111));
				}
			}
			break;
		case(option):
			if (returnedValue.indexSelector == 0)//language
			{
				menus[actualMenu].selectionCursor.indexPos = 0;
				sfVector2f newPos = { menus[actualMenu].Position.x, menus[actualMenu].Position.y };
				sfSprite_setPosition(menus[actualMenu].selectionCursor.indexDisplay, newPos);
				actualMenu = general;
			}
			else if (returnedValue.indexSelector == 1)//graphics (resolution, window, quality)
			{
				menus[actualMenu].selectionCursor.indexPos = 0;
				sfVector2f newPos = { menus[actualMenu].Position.x, menus[actualMenu].Position.y };
				sfSprite_setPosition(menus[actualMenu].selectionCursor.indexDisplay, newPos);
				actualMenu = graphics;
			}
			else if (returnedValue.indexSelector == 2)//do nothing
			{
				menus[actualMenu].selectionCursor.indexPos = 0;
				sfVector2f newPos = { menus[actualMenu].Position.x, menus[actualMenu].Position.y };
				sfSprite_setPosition(menus[actualMenu].selectionCursor.indexDisplay, newPos);
				actualMenu = soundOption;
			}
			else if (returnedValue.indexSelector == 3)//go back to the main menu
			{
				menus[actualMenu].selectionCursor.indexPos = 0;
				sfVector2f newPos = { menus[actualMenu].Position.x, menus[actualMenu].Position.y };
				sfSprite_setPosition(menus[actualMenu].selectionCursor.indexDisplay, newPos);
				actualMenu = main;
			}
			break;
		case(general):
			if (returnedValue.indexSelector == 0)//change language
			{
				if (!menus[actualMenu].selectors[0].isOn)
				{
					menus[actualMenu].selectors[0].isOn = sfTrue;

					sfVector2f newPos = sfSprite_getPosition(menus[actualMenu].selectors[menus[actualMenu].selectionCursor.indexPos].selectorBackground);
					sfSprite_setPosition(menus[actualMenu].selectionCursor.indexDisplay, newPos);
				}
				else
				{
					char index = GetMainLocalization().Array[menus[actualMenu].selectors[menus[actualMenu].selectionCursor.indexPos].textDisplay.actualTextToDisplay].Id;
					if (ChangeLanguage(index))
					{
						Config config = GetConfig();
						sfVideoMode videoMode = getConfigMode();
						ChangeLocalization(GetPointerMainLocalization(), config.MainLanguage);
						ResponsiveMainMenu(&menus[main], videoMode);
						ResponsiveSelectionPlayerMenu(&menus[selectNumberPlayer], videoMode);
						ResponsiveSelectionSkinPlayerMenu(&menus[selectSkinPlayer], videoMode);
						ResponsiveOptionMenu(&menus[option], videoMode);
						ResponsiveGeneralMenu(&menus[general], config);
						ResponsiveGraphicsMenu(&menus[graphics], config);
						ResponsivePauseMenuProto(&menus[pause], videoMode);
						ResponsiveOptionPauseMenu(&menus[optionPause], videoMode);
						ResponsiveControlsMenu(&menus[controls], videoMode);
						ResponsiveLoadingMenu(&menus[loadingGame], videoMode);
						ResponsiveSavingMenu(&menus[savingGame], videoMode);
						ResponsiveSoundOptionProto(&menus[soundOption], videoMode);
						ResponsiveNewOrLoad(&menus[newOrLoad], videoMode);
						ResponsiveCredits(&menus[credit], videoMode);
					}
					menus[actualMenu].selectors[0].isOn = sfFalse;
					sfVector2f newPos = sfSprite_getPosition(menus[actualMenu].button[menus[actualMenu].selectionCursor.indexPos].sprite);
					sfSprite_setPosition(menus[actualMenu].selectionCursor.indexDisplay, newPos);
				}
			}
			else if (returnedValue.indexSelector == 1)
			{
				menus[actualMenu].selectionCursor.indexPos = 0;
				sfVector2f newPos = { menus[actualMenu].Position.x, menus[actualMenu].Position.y };
				sfSprite_setPosition(menus[actualMenu].selectionCursor.indexDisplay, newPos);
				actualMenu = shortcuts;
			}
			else if (returnedValue.indexSelector == 2)
			{
				menus[actualMenu].selectionCursor.indexPos = 0;
				sfVector2f newPos = { menus[actualMenu].Position.x, menus[actualMenu].Position.y };
				sfSprite_setPosition(menus[actualMenu].selectionCursor.indexDisplay, newPos);
				actualMenu = controls;
				UpdateControlsMenu(&menus[actualMenu]);
			}
			else if (returnedValue.indexSelector == 3)
			{
				menus[actualMenu].selectionCursor.indexPos = 0;
				sfVector2f newPos = { menus[actualMenu].Position.x, menus[actualMenu].Position.y };
				sfSprite_setPosition(menus[actualMenu].selectionCursor.indexDisplay, newPos);
				if (GetState().isPaused)
				{
					actualMenu = optionPause;
				}
				else
				{
					actualMenu = option;
				}
			}
			break;
		case(shortcuts):
			if (returnedValue.indexSelector >= 0 && returnedValue.indexSelector < 9)
			{
				if (!menus[actualMenu].selectors[returnedValue.indexSelector].isOn)
				{
					menus[actualMenu].selectors[returnedValue.indexSelector].isOn = sfTrue;

					sfVector2f newPos = sfSprite_getPosition(menus[actualMenu].selectors[menus[actualMenu].selectionCursor.indexPos].selectorBackground);
					sfSprite_setPosition(menus[actualMenu].selectionCursor.indexDisplay, newPos);

					sfText_setString(menus[actualMenu].selectors[menus[actualMenu].selectionCursor.indexPos].textDisplay.textArray[0], /*borderless*/GetLocalizationString( 35));
					sfText_setCharacterSize(menus[actualMenu].selectors[menus[actualMenu].selectionCursor.indexPos].textDisplay.textArray[0], (int)(50 * (getConfigMode().height / 1080.0f)));
					CenterText(menus[actualMenu].selectors[menus[actualMenu].selectionCursor.indexPos].textDisplay.textArray[0], menus[actualMenu].selectors[menus[actualMenu].selectionCursor.indexPos].positionText);
				}
				else if (returnedValue.Key != sfKeyUnknown)
				{
					if (IfCanUseThisKey(returnedValue.Key))
					{
						SetMapKey(returnedValue.indexSelector, returnedValue.Key);
					}
					ResponsiveShortcutsMenu(&menus[shortcuts], getConfigMode());
					sfVector2f newPos = sfSprite_getPosition(menus[actualMenu].button[menus[actualMenu].selectionCursor.indexPos].sprite);
					sfSprite_setPosition(menus[actualMenu].selectionCursor.indexDisplay, newPos);
					menus[actualMenu].selectors[returnedValue.indexSelector].isOn = sfFalse;
				}
			}
			else if (returnedValue.indexSelector == 9 || returnedValue.indexSelector == 10)
			{
				if (!menus[actualMenu].selectors[returnedValue.indexSelector].isOn)
				{
					menus[actualMenu].selectors[returnedValue.indexSelector].isOn = sfTrue;

					sfVector2f newPos = sfSprite_getPosition(menus[actualMenu].selectors[menus[actualMenu].selectionCursor.indexPos].selectorBackground);
					sfSprite_setPosition(menus[actualMenu].selectionCursor.indexDisplay, newPos);

					sfText_setString(menus[actualMenu].selectors[menus[actualMenu].selectionCursor.indexPos].textDisplay.textArray[0], /*borderless*/GetLocalizationString( 35));
					sfText_setCharacterSize(menus[actualMenu].selectors[menus[actualMenu].selectionCursor.indexPos].textDisplay.textArray[0], (int)(50 * (getConfigMode().height / 1080.0f)));
					CenterText(menus[actualMenu].selectors[menus[actualMenu].selectionCursor.indexPos].textDisplay.textArray[0], menus[actualMenu].selectors[menus[actualMenu].selectionCursor.indexPos].positionText);
				}
				else if (returnedValue.Key != sfKeyUnknown)
				{
					if (returnedValue.Key != sfKeyEscape && returnedValue.Key != sfKeyReturn)
					{
						SetMapKey(returnedValue.indexSelector, returnedValue.Key);
					}
					ResponsiveShortcutsMenu(&menus[shortcuts], getConfigMode());
					sfVector2f newPos = sfSprite_getPosition(menus[actualMenu].button[menus[actualMenu].selectionCursor.indexPos].sprite);
					sfSprite_setPosition(menus[actualMenu].selectionCursor.indexDisplay, newPos);
					menus[actualMenu].selectors[returnedValue.indexSelector].isOn = sfFalse;
				}

			}
			else if (returnedValue.indexSelector == 11 && returnedValue.Key == sfKeyReturn)//Return to general menu
			{
				menus[actualMenu].selectionCursor.indexPos = 0;
				sfVector2f newPos = { menus[actualMenu].Position.x, menus[actualMenu].Position.y };
				sfSprite_setPosition(menus[actualMenu].selectionCursor.indexDisplay, newPos);
				actualMenu = general;
				SaveMapKey();
			}
			break;
		case controls:
			if (returnedValue.indexSelector == 0)//change player 1 device
			{
				if (!menus[actualMenu].selectors[0].isOn)
				{
					menus[actualMenu].selectors[0].isOn = sfTrue;

					sfVector2f newPos = sfSprite_getPosition(menus[actualMenu].selectors[menus[actualMenu].selectionCursor.indexPos].selectorBackground);
					sfSprite_setPosition(menus[actualMenu].selectionCursor.indexDisplay, newPos);
				}
				else
				{
					if (!((menus[actualMenu].selectors[0].textDisplay.actualTextToDisplay == 1 || menus[actualMenu].selectors[0].textDisplay.actualTextToDisplay == 2) &&
						!GetControllerState()[0].bConnected && !GetControllerState()[1].bConnected) &&
						!(menus[actualMenu].selectors[0].textDisplay.actualTextToDisplay == 1 && !GetControllerState()[0].bConnected) &&
						!(menus[actualMenu].selectors[0].textDisplay.actualTextToDisplay == 2 && !GetControllerState()[1].bConnected))
					{
						SetTypeOfHardware(0, menus[actualMenu].selectors[0].textDisplay.actualTextToDisplay);
						menus[actualMenu].selectors[0].isOn = sfFalse;
						sfVector2f newPos = sfSprite_getPosition(menus[actualMenu].button[menus[actualMenu].selectionCursor.indexPos].sprite);
						sfSprite_setPosition(menus[actualMenu].selectionCursor.indexDisplay, newPos);
						UpdateControlsMenu(&menus[actualMenu]);
					}
				}
			}
			else if (returnedValue.indexSelector == 1)//change player 2 device
			{
				if (!menus[actualMenu].selectors[1].isOn)
				{
					menus[actualMenu].selectors[1].isOn = sfTrue;

					sfVector2f newPos = sfSprite_getPosition(menus[actualMenu].selectors[menus[actualMenu].selectionCursor.indexPos].selectorBackground);
					sfSprite_setPosition(menus[actualMenu].selectionCursor.indexDisplay, newPos);
				}
				else
				{
					if (!((menus[actualMenu].selectors[1].textDisplay.actualTextToDisplay == 0 || menus[actualMenu].selectors[1].textDisplay.actualTextToDisplay == 1 || menus[actualMenu].selectors[1].textDisplay.actualTextToDisplay == 2) &&
						!GetControllerState()[0].bConnected && !GetControllerState()[1].bConnected) &&
						!(menus[actualMenu].selectors[1].textDisplay.actualTextToDisplay == 1 && !GetControllerState()[0].bConnected) &&
						!(menus[actualMenu].selectors[1].textDisplay.actualTextToDisplay == 2 && !GetControllerState()[1].bConnected) &&
						!(GetActualScene() == mainScene && GetPlayersInformations().NumberOfPlayer == 2 && menus[actualMenu].selectors[1].textDisplay.actualTextToDisplay == 3))
					{
						SetTypeOfHardware(1, menus[actualMenu].selectors[1].textDisplay.actualTextToDisplay);
						menus[actualMenu].selectors[1].isOn = sfFalse;
						sfVector2f newPos = sfSprite_getPosition(menus[actualMenu].button[menus[actualMenu].selectionCursor.indexPos].sprite);
						sfSprite_setPosition(menus[actualMenu].selectionCursor.indexDisplay, newPos);
						UpdateControlsMenu(&menus[actualMenu]);
					}
				}
			}
			else if (returnedValue.indexSelector == 2)//back to options
			{
				menus[actualMenu].selectionCursor.indexPos = 0;
				sfVector2f newPos = { menus[actualMenu].Position.x, menus[actualMenu].Position.y };
				sfSprite_setPosition(menus[actualMenu].selectionCursor.indexDisplay, newPos);
				actualMenu = general;
			}
			break;
		case(graphics):
			if (returnedValue.indexSelector == 0)//change the resolution
			{
				if (!menus[actualMenu].selectors[0].isOn)
				{
					menus[actualMenu].selectors[0].isOn = sfTrue;

					sfVector2f newPos = sfSprite_getPosition(menus[actualMenu].selectors[menus[actualMenu].selectionCursor.indexPos].selectorBackground);
					sfSprite_setPosition(menus[actualMenu].selectionCursor.indexDisplay, newPos);
				}
				else //call all the responsive menu
				{
					if (ChangeResolution(menus[actualMenu].selectors[0].textDisplay.actualTextToDisplay))
					{
						Config config = GetConfig();
						sfVideoMode videoMode = getConfigMode();
						ChangeLocalization(GetPointerMainLocalization(), config.MainLanguage);
						ResponsiveMainMenu(&menus[main], videoMode);
						ResponsiveSelectionPlayerMenu(&menus[selectNumberPlayer], videoMode);
						ResponsiveSelectionSkinPlayerMenu(&menus[selectSkinPlayer], videoMode);
						ResponsiveOptionMenu(&menus[option], videoMode);
						ResponsiveGeneralMenu(&menus[general], config);
						ResponsiveGraphicsMenu(&menus[graphics], config);
						ResponsivePauseMenuProto(&menus[pause], videoMode);
						ResponsiveOptionPauseMenu(&menus[optionPause], videoMode);
						ResponsiveControlsMenu(&menus[controls], videoMode);
						ResponsiveLoadingMenu(&menus[loadingGame], videoMode);
						ResponsiveSavingMenu(&menus[savingGame], videoMode);
						ResponsiveSoundOptionProto(&menus[soundOption], videoMode);
						ResponsiveNewOrLoad(&menus[newOrLoad], videoMode);
						ResponsiveCredits(&menus[credit], videoMode);
						sfText_setCharacterSize(Version, (int)(20 * (videoMode.height / 1080.0f)));
						sfText_setPosition(Version, (sfVector2f) { (videoMode.width / 1920.0f) * 1790.0f, (videoMode.height / 1080.0f) * 990.0f });
					}
					else
					{
						menus[actualMenu].selectors[0].textDisplay.actualTextToDisplay = GetIndexActualRes();
					}
					menus[actualMenu].selectors[0].isOn = sfFalse;
					sfVector2f newPos = sfSprite_getPosition(menus[actualMenu].button[menus[actualMenu].selectionCursor.indexPos].sprite);
					sfSprite_setPosition(menus[actualMenu].selectionCursor.indexDisplay, newPos);
				}
			}
			else if (returnedValue.indexSelector == 1)//change the window's style
			{
				if (!menus[actualMenu].selectors[1].isOn)
				{
					menus[actualMenu].selectors[1].isOn = sfTrue;

					sfVector2f newPos = sfSprite_getPosition(menus[actualMenu].selectors[menus[actualMenu].selectionCursor.indexPos].selectorBackground);
					sfSprite_setPosition(menus[actualMenu].selectionCursor.indexDisplay, newPos);
				}
				else  //call all the responsive menu
				{
					if (ChangeWindowMode(menus[actualMenu].selectors[1].textDisplay.actualTextToDisplay))
					{
						Config config = GetConfig();
						sfVideoMode videoMode = getConfigMode();
						ChangeLocalization(GetPointerMainLocalization(), config.MainLanguage);
						ResponsiveMainMenu(&menus[main], videoMode);
						ResponsiveSelectionPlayerMenu(&menus[selectNumberPlayer], videoMode);
						ResponsiveSelectionSkinPlayerMenu(&menus[selectSkinPlayer], videoMode);
						ResponsiveOptionMenu(&menus[option], videoMode);
						ResponsiveGeneralMenu(&menus[general], config);
						ResponsiveGraphicsMenu(&menus[graphics], config);
						ResponsivePauseMenuProto(&menus[pause], videoMode);
						ResponsiveOptionPauseMenu(&menus[optionPause], videoMode);
						ResponsiveControlsMenu(&menus[controls], videoMode);
						ResponsiveLoadingMenu(&menus[loadingGame], videoMode);
						ResponsiveSavingMenu(&menus[savingGame], videoMode);
						ResponsiveSoundOptionProto(&menus[soundOption], videoMode);
						ResponsiveNewOrLoad(&menus[newOrLoad], videoMode);
						ResponsiveCredits(&menus[credit], videoMode);
						menus[actualMenu].selectors[0].textDisplay.actualTextToDisplay = GetIndexActualRes();
						sfText_setCharacterSize(Version, (int)(20 * (videoMode.height / 1080.0f)));
						sfText_setPosition(Version, (sfVector2f) { (videoMode.width / 1920.0f) * 1790.0f, (videoMode.height / 1080.0f) * 990.0f });
					}
					menus[actualMenu].selectors[1].isOn = sfFalse;

					sfVector2f newPos = sfSprite_getPosition(menus[actualMenu].button[menus[actualMenu].selectionCursor.indexPos].sprite);
					sfSprite_setPosition(menus[actualMenu].selectionCursor.indexDisplay, newPos);
				}
			}
			else if (returnedValue.indexSelector == 2)//Activate/Desactivate Dynamic Light
			{
				if (!menus[actualMenu].selectors[2].isOn)
				{
					menus[actualMenu].selectors[2].isOn = sfTrue;

					sfVector2f newPos = sfSprite_getPosition(menus[actualMenu].selectors[menus[actualMenu].selectionCursor.indexPos].selectorBackground);
					sfSprite_setPosition(menus[actualMenu].selectionCursor.indexDisplay, newPos);
				}
				else  //call the model to apply modification
				{
					if (menus[actualMenu].selectors[2].textDisplay.actualTextToDisplay != GetConfigDynamicLight())
					{
						ChangeConfigOption(0);
						SetNmEnabled(GetConfigDynamicLight());
						menus[actualMenu].selectors[2].textDisplay.actualTextToDisplay = GetConfigDynamicLight();
					}
					menus[actualMenu].selectors[2].isOn = sfFalse;
					sfVector2f newPos = sfSprite_getPosition(menus[actualMenu].button[menus[actualMenu].selectionCursor.indexPos].sprite);
					sfSprite_setPosition(menus[actualMenu].selectionCursor.indexDisplay, newPos);
				}
			}
			else if (returnedValue.indexSelector == 3)//go back to the option menu
			{
				menus[actualMenu].selectionCursor.indexPos = 0;
				sfVector2f newPos = { menus[actualMenu].Position.x, menus[actualMenu].Position.y };
				sfSprite_setPosition(menus[actualMenu].selectionCursor.indexDisplay, newPos);
				actualMenu = option;
			}
			break;
		case(soundOption):
			if (returnedValue.indexSelector < 3)
			{
				if (!menus[actualMenu].SliderSelectors[returnedValue.indexSelector].isSelected)
				{
					menus[actualMenu].SliderSelectors[returnedValue.indexSelector].isSelected = sfTrue;
				}
				else
				{
					menus[actualMenu].SliderSelectors[returnedValue.indexSelector].isSelected = sfFalse;
					SetVolume(menus[actualMenu].SliderSelectors[returnedValue.indexSelector].valueOfSlider, returnedValue.indexSelector);
				}
			}
			else if (returnedValue.indexSelector == 3)//go back to the option menu
			{
				menus[actualMenu].selectionCursor.indexPos = 0;
				sfVector2f newPos = { menus[actualMenu].Position.x, menus[actualMenu].Position.y };
				sfSprite_setPosition(menus[actualMenu].selectionCursor.indexDisplay, newPos);
				sfSprite_setPosition(menus[actualMenu].SliderCursor, newPos);
				if (GetState().isPaused)
					actualMenu = optionPause;
				else actualMenu = option;
			}
			break;
		case (pause):
			if (returnedValue.indexSelector == 0)//go back to the current game
			{
				SetStatePause(sfFalse);
			}
			else if (returnedValue.indexSelector == 1)
			{
				if (!IsPlayerFighting())
				{
					menus[actualMenu].selectionCursor.indexPos = 0;
					sfVector2f newPos = { menus[actualMenu].Position.x, menus[actualMenu].Position.y };
					sfSprite_setPosition(menus[actualMenu].selectionCursor.indexDisplay, newPos);
					actualMenu = savingGame;
					UpdateMenuSaving(&menus[actualMenu], getConfigMode());
				}
			}
			else if (returnedValue.indexSelector == 2)
			{
				menus[actualMenu].selectionCursor.indexPos = 0;
				sfVector2f newPos = { menus[actualMenu].Position.x, menus[actualMenu].Position.y };
				sfSprite_setPosition(menus[actualMenu].selectionCursor.indexDisplay, newPos);
				actualMenu = optionPause;
			}
			else if (returnedValue.indexSelector == 3)//go back to the main menu
			{
				menus[actualMenu].selectionCursor.indexPos = 0;
				sfVector2f newPos = { menus[actualMenu].Position.x, menus[actualMenu].Position.y };
				sfSprite_setPosition(menus[actualMenu].selectionCursor.indexDisplay, newPos);
				isRestarted = sfTrue;
				SetStatePause(sfFalse);
				actualMenu = main;
				ChangeScene(mainMenu);
				StopAllMusic();
				PlayMusic(mainMenuMusic);
				UpdateMenuLoading(&menus[loadingGame], getConfigMode());
				if (SearchingSave(1, -1) || SearchingSave(2, -1))
				{
					SetColorButton(&menus[loadingGame].button[1], sfColor_fromRGB(171, 144, 111));
				}
				else
				{
					SetColorButton(&menus[loadingGame].button[1], sfColor_fromRGB(105, 105, 105));
				}
			}
			break;

		case(savingGame):
			if (returnedValue.indexSelector < 3)//go back to the current game
			{
				SetSave(sfTrue);
				SetManualSaveChoosen(returnedValue.indexSelector + 1);
				SetTypeOfSave(manual);
			}
			if (returnedValue.indexSelector == 3)//go back to the current game
			{
				menus[actualMenu].selectionCursor.indexPos = 0;
				sfVector2f newPos = { menus[actualMenu].Position.x, menus[actualMenu].Position.y };
				sfSprite_setPosition(menus[actualMenu].selectionCursor.indexDisplay, newPos);
				actualMenu = pause;
			}
			break;
		case(optionPause):
			if (returnedValue.indexSelector == 0)//go back to the current game
			{
				menus[actualMenu].selectionCursor.indexPos = 0;
				sfVector2f newPos = { menus[actualMenu].Position.x, menus[actualMenu].Position.y };
				sfSprite_setPosition(menus[actualMenu].selectionCursor.indexDisplay, newPos);
				actualMenu = general;
			}
			else if (returnedValue.indexSelector == 1)//go back to the current game
			{
				menus[actualMenu].selectionCursor.indexPos = 0;
				sfVector2f newPos = { menus[actualMenu].Position.x, menus[actualMenu].Position.y };
				sfSprite_setPosition(menus[actualMenu].selectionCursor.indexDisplay, newPos);
				actualMenu = soundOption;
			}
			else if (returnedValue.indexSelector == 2)//go back to the current game
			{
				menus[actualMenu].selectionCursor.indexPos = 0;
				sfVector2f newPos = { menus[actualMenu].Position.x, menus[actualMenu].Position.y };
				sfSprite_setPosition(menus[actualMenu].selectionCursor.indexDisplay, newPos);
				actualMenu = pause;
			}
			break;

		case(credit):
			if (returnedValue.indexSelector == 0)
			{
				sfVector2f pos = sfSprite_getPosition(menus[credit].background);
				pos.y = menus[credit].Position.x;
				sfSprite_setPosition(menus[credit].background, pos);
				actualMenu = main;
				StopMusic(creditsMusic);
				PlayMusic(mainMenuMusic);
			}
			break;
		case(newOrLoad):
			if (returnedValue.indexSelector == 0)
			{
				menus[actualMenu].selectionCursor.indexPos = 0;
				sfVector2f newPos = { menus[actualMenu].Position.x, menus[actualMenu].Position.y };
				sfSprite_setPosition(menus[actualMenu].selectionCursor.indexDisplay, newPos);
				actualMenu = selectNumberPlayer;
				IsLoadingGame = sfFalse;
				SetColorButton(&menus[actualMenu].button[0], sfColor_fromRGB(171, 144, 111));
				SetColorButton(&menus[actualMenu].button[1], sfColor_fromRGB(171, 144, 111));
			}
			else if (returnedValue.indexSelector == 1)
			{
				if (SearchingSave(1, -1) || SearchingSave(2, -1))
				{
					menus[actualMenu].selectionCursor.indexPos = 0;
					sfVector2f newPos = { menus[actualMenu].Position.x, menus[actualMenu].Position.y };
					sfSprite_setPosition(menus[actualMenu].selectionCursor.indexDisplay, newPos);
					actualMenu = selectNumberPlayer;
					IsLoadingGame = sfTrue;
					if (SearchingSave(1, -1))
					{
						SetColorButton(&menus[actualMenu].button[0], sfColor_fromRGB(171, 144, 111));
					}
					else
					{
						SetColorButton(&menus[actualMenu].button[0], sfColor_fromRGB(105, 105, 105));
					}
					if (SearchingSave(2, -1))
					{
						SetColorButton(&menus[actualMenu].button[1], sfColor_fromRGB(171, 144, 111));
					}
					else
					{
						SetColorButton(&menus[actualMenu].button[1], sfColor_fromRGB(105, 105, 105));
					}
				}
			}
			else if (returnedValue.indexSelector == 2)
			{
				menus[actualMenu].selectionCursor.indexPos = 0;
				sfVector2f newPos = { menus[actualMenu].Position.x, menus[actualMenu].Position.y };
				sfSprite_setPosition(menus[actualMenu].selectionCursor.indexDisplay, newPos);
				actualMenu = main;
			}
			break;
		default:
			break;
		}
		return actualMenu;
	}
	if (CanReloadSaveName())
	{
		UpdateMenuSaving(&menus[actualMenu], getConfigMode());
		SetCanReloadSaveName(sfFalse);
	}
	return actualMenu;
}

void UpdatePlayerHUD(const Player player, const int id, const float dt)
{
	int data[4] = { player.Stats.Life, player.Stats.LifeMax, player.Stats.Mana, player.Stats.ManaMax };
	UpdatePlayerInfo(_HUDController_playerInfo[id], data, id, dt);
}

void UpdatePlayerCDHUD(const Player player, const int id)
{
	int data[4];
	for (int i = 0; i < 4; i++)
	{
		data[i] = player.Spells.spell[i].isAvailable;
	}
	UpdatePlayerCDInfo(_HUDController_playerInfo[id], data);
}

void UpdatePlayerSpellSelectedHUD(const Player player, const int id)
{
	UpdatePlayerSpellSelected(_HUDController_playerInfo[id], player.Spells.spellSelect);
}

void UpdateHUD(const GameData gameData, const float deltaTime, const sfVideoMode mode)
{
#if _DEBUG 
	UpdateTextWithStringDisplay(hudDisplayer.FPS, "FPS : ", (int)(1 / deltaTime));
#endif

	//rescale boss life bar and decrease it from the left to the right
	needToDisplayBossLife = (gameData.Boss.IsSpawned && !gameData.Boss.IsDead);
	if (needToDisplayBossLife)
	{
		float ratioX = mode.width / 1920.0f;
		float ratioY = mode.height / 1080.0f;
		sfVector2f scale;
		sfVector2u size = sfTexture_getSize(sfSprite_getTexture(hudDisplayer.LifeBarBoss));
		scale.x = (500.f * ratioX) / (float)size.x;
		scale.y = (20.f * ratioY) / (float)size.y;
		sfSprite_setScale(hudDisplayer.LifeBarBoss, (sfVector2f) { ((float)gameData.Boss.Life / (float)gameData.Boss.LifeMax)  * scale.x, scale.y });
	}
	pointerToSkillTrees[0] = &gameData.Player[0].SkillTree;
	UpdateSkillTree(&gameData.Player[0].SkillTree, deltaTime);
}

void UpdateUI(const float deltaTime)
{
	if (actualMenu == credit)
	{
		sfVector2f pos = sfSprite_getPosition(menus[credit].background);
		pos.y -= (deltaTime * 80.0f);
		if (pos.y < -(menus[credit].Position.y))
		{
			pos.y = menus[credit].Position.x;
		}

		sfSprite_setPosition(menus[credit].background, pos);
	}
}

void DisplayUI(sfRenderWindow *mainWindow)
{
	if (((actualMenu >= 1 && actualMenu <= 7) || actualMenu == 10 || actualMenu == shortcuts || actualMenu == newOrLoad) && !GetState().isPaused)
	{
		sfRenderWindow_drawSprite(mainWindow, menus[0].background, NULL);
	}
	else if (actualMenu == 7)
	{
		sfRenderWindow_drawSprite(mainWindow, menus[0].background, NULL);
	}
	DisplayMenu(menus[actualMenu], mainWindow);
	if (((actualMenu >= 1 && actualMenu <= 7) || actualMenu == 10 || actualMenu == shortcuts || actualMenu == newOrLoad) && !GetState().isPaused)
	{
		sfRenderWindow_drawSprite(mainWindow, menus[0].images[2], NULL);
	}
	else if (actualMenu == 7)
	{
		sfRenderWindow_drawSprite(mainWindow, menus[0].images[2], NULL);
	}

	if (actualMenu == 0)
	{
		sfRenderWindow_drawText(mainWindow, Version, NULL);
	}
}

void DisplayHUD(const Camera *camera, const Player *player, const int numberOfPlayer, const Dungeon mainDungeon, sfRenderWindow *mainWindow)
{
	sfVector2u sizeWindow = sfRenderWindow_getSize(mainWindow);
	float ratioX = sizeWindow.x / 1920.0f;
	float ratioY = sizeWindow.y / 1080.0f;
	DisplayCameraHud(camera, mainWindow);
	if (IsNmEnabled())
	{
		DisplayNormalMap(mainWindow, camera->View);
	}

	DisplayCursor(numberOfPlayer, mainWindow, camera->View);

#if _DEBUG
	sfVector2f posFPS = { sizeWindow.x / 2.f, 0 };
	sfText_setPosition(hudDisplayer.FPS, posFPS);
	sfRenderWindow_drawText(mainWindow, hudDisplayer.FPS, NULL);
#endif
	for (int i = 0; i < numberOfPlayer; i++)
	{
		DisplayPlayerInfo(mainWindow, *_HUDController_playerInfo[i]);
	}

	//position of boss life bar
	if (needToDisplayBossLife)
	{
		sfVector2f posLifeBar = { (sizeWindow.x / 2) - (sfSprite_getLocalBounds(hudDisplayer.LifeBarBossOutline).width / 2.f * ratioX), 45 * ratioY };
		BlitSprite(mainWindow, hudDisplayer.LifeBarBossOutline, posLifeBar, NULL);
		posLifeBar.x += 5 * ratioX;
		posLifeBar.y += 5 * ratioY;
		BlitSprite(mainWindow, hudDisplayer.LifeBarBoss, posLifeBar, NULL);
	}
	//display UI pause
	sfShader* temp = NULL;
	if (IsPlayerFighting())
	{
		temp = GetShaderList().TransparenceMap;
	}
	DisplayViewMiniMap(camera, player, numberOfPlayer, mainWindow, temp);
	DisplayCameraHud(camera, mainWindow);
	if (!IsEnableMap())
	{
		sfVector2f miniMap = { sizeWindow.x * 0.825f,  sizeWindow.y * 0.69f };
		BlitSprite(mainWindow, hudDisplayer.MiniMap, miniMap, temp);
	}

	if (GetState().isPaused)
	{
		sfTexture* textureWindow = GetScreenTexture();
		sfSprite* spriteWindow = GetScreenSprite();
		sfTexture_updateFromRenderWindow(textureWindow, mainWindow, 0, 0);
		sfSprite_setTexture(spriteWindow, textureWindow, 1);
		BlitSprite(mainWindow, spriteWindow, (sfVector2f) { 0, 0 }, GetShaderList().Greyscale);

		if (!AlreadyInPaused)
		{
			actualMenu = pause;
			if (!IsPlayerFighting())
			{
				SetColorButton(&menus[actualMenu].button[1], sfColor_fromRGB(171, 144, 111));
			}
			else
			{
				SetColorButton(&menus[actualMenu].button[1], sfColor_fromRGB(63, 63, 63));
			}
			AlreadyInPaused = sfTrue;
		}
		DisplayUI(mainWindow);
	}
	else
	{
		actualMenu = main;
		AlreadyInPaused = sfFalse;
	}
}

sfBool GetRestart()
{
	return isRestarted;
}

void SetRestart(const sfBool ptrue)
{
	isRestarted = ptrue;
}