#include "MenuProto.h"
#include "..\..\Serialization\Serialization.h"
#include "..\..\LeJeu\View\MapKey\MapKey.h"
#include "..\..\..\View\ViewManager.h"

/////////////////////ProtoType\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

Menu CreateMainMenuProto(sfVideoMode mode)
{
	float ratioX = mode.width / 1920.0f;
	float ratioY = mode.height / 1080.0f;
	Button MainMenuButtons[4];
	sfVector2f MainMenuButtonsPos = { 400.0f * ratioX, 400.0f * ratioY };
	sfVector2f MainMenuButtonsSize = { 620.0f * ratioX, 180.0f * ratioY };
	char* filesDirectories[4] =
	{
		"Ressources/Sprites/UI/Bouton.png",
		"Ressources/Sprites/UI/Bouton.png",
		"Ressources/Sprites/UI/Bouton.png",
		"Ressources/Sprites/UI/Bouton.png",
	};

	for (int i = 0; i < 4; i++)
	{
		MainMenuButtons[i] = InitButton(MainMenuButtonsSize, MainMenuButtonsPos, i, filesDirectories[i], true);
	}

	sfVector2f posFirst = { mode.width / 2.0f, mode.height / 2.0f - 100.0f * ratioY };

	sfVector2f vectorCur = { (posFirst.x) - 600.0f * ratioX, (posFirst.y) - 100.0f  * ratioY };
	Menu MainMenu = CreateGameMenu(vectorCur, MainMenuButtons, 4, 3, false);
	AddBackgroundToMenu(&MainMenu, "Ressources/Sprites/UI/Fond_Parchemain.png", (sfVector2f) { mode.width / 2.f, mode.height / 2.f }, sfTrue);
	AddImageToMenu(&MainMenu, "Ressources/Sprites/UI/creajeux.png", 0, (sfVector2f) { mode.width - 200.f * ratioX, mode.height - 200.f * ratioY }, 1);
	AddImageToMenu(&MainMenu, "Ressources/Sprites/UI/game_logo.png", 1, (sfVector2f) { mode.width / 2.f, 250.f * ratioY }, 1);
	AddImageToMenu(&MainMenu, "Ressources/Sprites/UI/Cadre_Fer.png", 2, (sfVector2f) { mode.width / 2.f, mode.height / 2.f }, 1);


	sfVector2f scaleImage = { ratioX , ratioY };
	for (int i = 0; i < MainMenu.imagesNumber; i++)
	{
		sfSprite_setScale(MainMenu.images[i], scaleImage);
	}

	sfVector2f scaleBackground;
	sfVector2u sizeBackground = sfTexture_getSize(sfSprite_getTexture(MainMenu.background));

	scaleBackground.x = (1920.0f * ratioX) / (float)sizeBackground.x;
	scaleBackground.y = (1080.0f * ratioY) / (float)sizeBackground.y;
	sfSprite_setScale(MainMenu.background, scaleBackground);

	sfVector2f scaleGameLogo;
	sfVector2u sizeGameLogo = sfTexture_getSize(sfSprite_getTexture(MainMenu.images[1]));

	scaleGameLogo.x = (800.0f * ratioX) / (float)sizeGameLogo.x;
	scaleGameLogo.y = (300.0f * ratioY) / (float)sizeGameLogo.y;
	sfSprite_setScale(MainMenu.images[1], scaleGameLogo);


	sfFloatRect temp = sfSprite_getGlobalBounds(MainMenu.images[1]);
	sfVector2f rePos = { temp.left, temp.top };
	rePos.x = mode.width / 2.f;
	rePos.y = temp.height / 2.0f + 60.0f * ratioY;
	sfSprite_setPosition(MainMenu.images[1], rePos);

	sfVector2f scaleCadre;
	sfVector2u sizeCadre = sfTexture_getSize(sfSprite_getTexture(MainMenu.images[2]));

	scaleCadre.x = (1920.0f * ratioX) / (float)sizeCadre.x;
	scaleCadre.y = (1080.0f * ratioY) / (float)sizeCadre.y;
	sfSprite_setScale(MainMenu.images[2], scaleCadre);


	MainMenu.selectionCursor.indexDisplay = LoadSprite("Ressources/Sprites/UI/Selecteur.png", 1);
	sfVector2f scale;
	sfVector2u tempSize = sfTexture_getSize(sfSprite_getTexture(MainMenu.selectionCursor.indexDisplay));
	scale.x = (620.0f * ratioX) / (float)tempSize.x;
	scale.y = (180.0f * ratioY) / (float)tempSize.y;

	sfSprite_setScale(MainMenu.selectionCursor.indexDisplay, scale);

	MainMenu.selectionCursor.indexPos = 0;
	MainMenu.selectionCursor.offset.x = 0.0f * ratioX;
	MainMenu.selectionCursor.offset.y = 150.0f * ratioY;

	AlignMenuButtons(&MainMenu, 0, posFirst, -20.0f * ratioY, 2);
	sfSprite_setPosition(MainMenu.selectionCursor.indexDisplay, sfSprite_getPosition(MainMenu.button[0].sprite));

	MainMenu.Position = sfSprite_getPosition(MainMenu.button[0].sprite);

	MainMenu.selectors = NULL;
	MainMenu.selectorsNumber = 0;
	MainMenu.classDisplayer.Class = NULL;
	MainMenu.classDisplayer.isOn = sfFalse;

	MainMenu.SliderSelectors = NULL;
	MainMenu.SliderSelectorsNumber = 0;

	InitTextButton(&MainMenu.button[0], /*"Play"*/GetLocalizationString(0), sfColor_fromRGB(171, 144, 111), TEXT_SIZE_MENU);
	InitTextButton(&MainMenu.button[1], /*"Options"*/GetLocalizationString(2), sfColor_fromRGB(171, 144, 111), TEXT_SIZE_MENU);
	InitTextButton(&MainMenu.button[2], /*"credits"*/GetLocalizationString(158), sfColor_fromRGB(171, 144, 111), TEXT_SIZE_MENU);
	InitTextButton(&MainMenu.button[3], /*"Quit"*/GetLocalizationString(3), sfColor_fromRGB(171, 144, 111), TEXT_SIZE_MENU);

	int newSize = (int)(TEXT_SIZE_MENU * (mode.height / 1080.0f));
	for (int i = 0; i < MainMenu.buttonNumber; i++)
	{
		MainMenu.button[i].text2 = NULL;
		sfText_setCharacterSize(MainMenu.button[i].text, newSize);
		RescaleText(MainMenu.button[i].text, 325.0f, 168.5f, ratioX, ratioY);
		sfVector2f pos = (sfVector2f) { MainMenu.button[i].rect.left + (MainMenu.button[i].rect.width) / 2.0f, MainMenu.button[i].rect.top + (MainMenu.button[i].rect.height) / 2.0f + 10.0f * ratioY };
		CenterText(MainMenu.button[i].text, pos);
	}

	return (MainMenu);
}

void ResponsiveMainMenu(Menu *menuToUpdate, sfVideoMode mode)
{
	float ratioX = mode.width / 1920.0f;
	float ratioY = mode.height / 1080.0f;


	sfVector2f MainMenuButtonsPos = { 400.0f * ratioX, 400.0f * ratioY };
	sfVector2f MainMenuButtonsSize = { 620.0f * ratioX, 180.0f * ratioY };


	for (int i = 0; i < menuToUpdate->buttonNumber; i++)
	{
		ResponsiveButton(&menuToUpdate->button[i], MainMenuButtonsSize, MainMenuButtonsPos, true);
	}

	sfVector2f posFirst = { mode.width / 2.0f, mode.height / 2.0f - 100.0f * ratioY };

	sfVector2f vectorCur = { (posFirst.x) - 600.0f * ratioX, (posFirst.y) - 100.0f  * ratioY };

	menuToUpdate->Position.x = vectorCur.x;
	menuToUpdate->Position.y = vectorCur.y;

	sfVector2f scale;
	sfVector2u size = sfTexture_getSize(sfSprite_getTexture(menuToUpdate->selectionCursor.indexDisplay));
	scale.x = (620.0f * ratioX) / (float)size.x;
	scale.y = (180.0f * ratioY) / (float)size.y;

	sfSprite_setScale(menuToUpdate->selectionCursor.indexDisplay, scale);
	sfSprite_setPosition(menuToUpdate->selectionCursor.indexDisplay, vectorCur);

	menuToUpdate->selectionCursor.offset.x = 0.0f * ratioX;
	menuToUpdate->selectionCursor.offset.y = 150.0f * ratioY;

	for (int i = 0; i < menuToUpdate->imagesNumber; i++)
	{
		sfVector2f scaleImage = { ratioX , ratioY };
		sfSprite_setScale(menuToUpdate->images[i], scaleImage);
	}

	sfVector2f scaleBackground;
	size = sfTexture_getSize(sfSprite_getTexture(menuToUpdate->background));
	scaleBackground.x = (1920.0f * ratioX) / (float)size.x;
	scaleBackground.y = (1080.0f * ratioY) / (float)size.y;
	sfSprite_setScale(menuToUpdate->background, scaleBackground);

	sfVector2f scaleGameLogo = { 1.0f, 1.0f };
	sfSprite_setScale(menuToUpdate->images[1], scaleGameLogo);

	sfVector2u temp = sfTexture_getSize(sfSprite_getTexture(menuToUpdate->images[1]));
	scaleGameLogo.x = (800.0f * ratioX) / temp.x;
	scaleGameLogo.y = (300.0f * ratioY) / temp.y;
	sfSprite_setScale(menuToUpdate->images[1], scaleGameLogo);

	sfFloatRect tempFloat = sfSprite_getGlobalBounds(menuToUpdate->images[1]);
	sfVector2f rePos = { tempFloat.left, tempFloat.top };
	rePos.x = mode.width / 2.f;
	rePos.y = tempFloat.height / 2.0f + 60.0f * ratioY;
	sfSprite_setPosition(menuToUpdate->images[1], rePos);

	sfVector2f scaleCadre;
	size = sfTexture_getSize(sfSprite_getTexture(menuToUpdate->images[2]));
	scaleCadre.x = (1920.0f * ratioX) / (float)size.x;
	scaleCadre.y = (1080.0f * ratioY) / (float)size.y;
	sfSprite_setScale(menuToUpdate->images[2], scaleCadre);

	sfSprite_setPosition(menuToUpdate->background, (sfVector2f) { mode.width / 2.f, mode.height / 2.f });
	sfSprite_setPosition(menuToUpdate->images[0], (sfVector2f) { mode.width - 200.f * ratioX, mode.height - 200.f * ratioY });
	sfSprite_setPosition(menuToUpdate->images[2], (sfVector2f) { mode.width / 2.f, mode.height / 2.f });

	AlignMenuButtons(menuToUpdate, 0, posFirst, -20.0f * ratioY, 2);

	sfSprite_setPosition(menuToUpdate->selectionCursor.indexDisplay, sfSprite_getPosition(menuToUpdate->button[0].sprite));
	menuToUpdate->Position = sfSprite_getPosition(menuToUpdate->button[0].sprite);


	int newSize = (int)(TEXT_SIZE_MENU * (mode.height / 1080.0f));
	SetStringButton(&menuToUpdate->button[0], /*play*/GetLocalizationString(0));
	SetStringButton(&menuToUpdate->button[1], /*options*/GetLocalizationString(2));
	SetStringButton(&menuToUpdate->button[2], /*Credits*/GetLocalizationString(158));
	SetStringButton(&menuToUpdate->button[3], /*quit*/GetLocalizationString(3));
	for (int i = 0; i < menuToUpdate->buttonNumber; i++)
	{
		sfText_setCharacterSize(menuToUpdate->button[i].text, newSize);
		RescaleText(menuToUpdate->button[i].text, 325.0f, 168.5f, ratioX, ratioY);
		sfVector2f pos = (sfVector2f) { menuToUpdate->button[i].rect.left + (menuToUpdate->button[i].rect.width) / 2, menuToUpdate->button[i].rect.top + (menuToUpdate->button[i].rect.height) / 2 + 10.0f * ratioY };
		CenterText(menuToUpdate->button[i].text, pos);
	}
}

Menu CreateLoadingMenu(sfVideoMode mode)
{
	float ratioX = mode.width / 1920.0f;
	float ratioY = mode.height / 1080.0f;
	Button LoadingMenuButtons[5];
	sfVector2f LoadingMenuButtonsPos = { 400.0f * ratioX, 400.0f * ratioY };
	sfVector2f LoadingMenuButtonsSize = { 620.0f * ratioX, 200.0f * ratioY };
	char* filesDirectories[5] =
	{
		"Ressources/Sprites/UI/Bouton.png",
		"Ressources/Sprites/UI/Bouton.png",
		"Ressources/Sprites/UI/Bouton.png",
		"Ressources/Sprites/UI/Bouton.png",
		"Ressources/Sprites/UI/Bouton.png",
	};

	for (int i = 0; i < 5; i++)
	{
		LoadingMenuButtons[i] = InitButton(LoadingMenuButtonsSize, LoadingMenuButtonsPos, i, filesDirectories[i], true);
	}

	sfVector2f posFirst = { mode.width / 2.0f, mode.height / 2.0f - 300.0f * ratioY };

	sfVector2f vectorCur = { (posFirst.x) - 600.0f * ratioX, (posFirst.y) - 100.0f  * ratioY };
	Menu LoadingMenu = CreateGameMenu(vectorCur, LoadingMenuButtons, 5, 0, false);

	LoadingMenu.selectionCursor.indexDisplay = LoadSprite("Ressources/Sprites/UI/Selecteur.png", 1);
	sfVector2f scale;
	sfVector2u size = sfTexture_getSize(sfSprite_getTexture(LoadingMenu.selectionCursor.indexDisplay));
	scale.x = (630.0f * ratioX) / (float)size.x;
	scale.y = (200.f * ratioY) / (float)size.y;

	sfSprite_setScale(LoadingMenu.selectionCursor.indexDisplay, scale);

	LoadingMenu.selectionCursor.indexPos = 0;
	LoadingMenu.selectionCursor.offset.x = 0.0f * ratioX;
	LoadingMenu.selectionCursor.offset.y = 150.0f * ratioY;

	AlignMenuButtons(&LoadingMenu, 0, posFirst, -20.0f * ratioY, 2);
	sfSprite_setPosition(LoadingMenu.selectionCursor.indexDisplay, sfSprite_getPosition(LoadingMenu.button[0].sprite));

	LoadingMenu.Position = sfSprite_getPosition(LoadingMenu.button[0].sprite);

	LoadingMenu.selectors = NULL;
	LoadingMenu.selectorsNumber = 0;
	LoadingMenu.classDisplayer.Class = NULL;
	LoadingMenu.classDisplayer.isOn = sfFalse;

	LoadingMenu.SliderSelectors = NULL;
	LoadingMenu.SliderSelectorsNumber = 0;

	/*
	InitTextButton(&LoadingMenu.button[0], ChangeSaveNameButton(1, 0), sfColor_fromRGB(54, 37, 33), TEXT_SIZE_MENU);
	InitTextButton(&LoadingMenu.button[1], ChangeSaveNameButton(1, 1), sfColor_fromRGB(54, 37, 33), TEXT_SIZE_MENU);
	InitTextButton(&LoadingMenu.button[2], ChangeSaveNameButton(1, 2), sfColor_fromRGB(54, 37, 33), TEXT_SIZE_MENU);
	InitTextButton(&LoadingMenu.button[3], ChangeSaveNameButton(1, 3), sfColor_fromRGB(54, 37, 33), TEXT_SIZE_MENU);
	InitTextButton(&LoadingMenu.button[4], GetLocalizationString( 6), sfColor_fromRGB(54, 37, 33), TEXT_SIZE_MENU);

	int newSize = (int)(TEXT_SIZE_MENU * (mode.height / 1080.0f));
	for (int i = 0; i < LoadingMenu.buttonNumber; i++)
	{
	LoadingMenu.button[i].text2 = NULL;
	sfText_setCharacterSize(LoadingMenu.button[i].text, newSize);
	sfVector2f pos = (sfVector2f) { LoadingMenu.button[i].rect.left + (LoadingMenu.button[i].rect.width) / 2, LoadingMenu.button[i].rect.top + (LoadingMenu.button[i].rect.height) / 2 };
	CenterText(LoadingMenu.button[i].text, pos);
	}*/
	for (int i = 0; i < 4; i++)
	{
		char save[50];
		if (i == 0)
			sprintf(save, "quickSave");
		else
			sprintf(save, "manualSave%d", i);
		int length = (int)strlen(save);
		sfVector2f pos = (sfVector2f) { 0.0f, 0.0f };
		LoadingMenu.button[i].text = InitText(save, pos, sfColor_fromRGB(171, 144, 111), TEXT_SIZE_MENU);
		LoadingMenu.button[i].text2 = InitText(ChangeSaveNameButton(1, i), pos, sfColor_fromRGB(171, 144, 111), TEXT_SIZE_MENU);
	}

	//InitTextButton(&LoadingMenu.button[1], /*"2 joueur"*/ChangeSaveNameButton(1, 2), sfColor_fromRGB(54, 37, 33), TEXT_SIZE_MENU);
	//InitTextButton(&LoadingMenu.button[2], /*"2 joueur"*/ChangeSaveNameButton(1, 3), sfColor_fromRGB(54, 37, 33), TEXT_SIZE_MENU);
	InitTextButton(&LoadingMenu.button[4], /*"Back"*/GetLocalizationString(6), sfColor_fromRGB(171, 144, 111), TEXT_SIZE_MENU);
	LoadingMenu.button[4].text2 = NULL;
	sfVector2f posText = { 1530 * ratioX, 800 * ratioY };
	LoadingMenu.TextToDisplayLoad = InitText(GetLocalizationString(184), posText, sfColor_fromRGB(105, 105, 105), TEXT_SIZE_MENU);
	int newSize = (int)(TEXT_SIZE_MENU * (mode.height / 1080.0f)) - 10;
	sfText_setCharacterSize(LoadingMenu.TextToDisplayLoad, newSize);


	for (int i = 0; i < LoadingMenu.buttonNumber; i++)
	{
		if (i != LoadingMenu.buttonNumber - 1)
		{
			sfText_setCharacterSize(LoadingMenu.button[i].text, newSize);
			RescaleText(LoadingMenu.button[i].text, 325.0f, 50.0f, ratioX, ratioY);
			sfVector2f pos = (sfVector2f) { LoadingMenu.button[i].rect.left + (LoadingMenu.button[i].rect.width) / 2, LoadingMenu.button[i].rect.top + (LoadingMenu.button[i].rect.height) / 2 - 20.f * ratioY };
			CenterText(LoadingMenu.button[i].text, pos);
			if (LoadingMenu.button[i].text2 != NULL)
			{
				sfText_setCharacterSize(LoadingMenu.button[i].text2, newSize);
				RescaleText(LoadingMenu.button[i].text2, 325.0f, 50.0f, ratioX, ratioY);
				pos = (sfVector2f) { LoadingMenu.button[i].rect.left + (LoadingMenu.button[i].rect.width) / 2, LoadingMenu.button[i].rect.top + (LoadingMenu.button[i].rect.height) / 2 + 20.f * ratioY };
				CenterText(LoadingMenu.button[i].text2, pos);
			}
		}
		else
		{
			sfText_setCharacterSize(LoadingMenu.button[i].text, newSize);
			RescaleText(LoadingMenu.button[i].text, 325.0f, 50.0f, ratioX, ratioY);
			sfVector2f pos = (sfVector2f) { LoadingMenu.button[i].rect.left + (LoadingMenu.button[i].rect.width) / 2, LoadingMenu.button[i].rect.top + (LoadingMenu.button[i].rect.height) / 2 + 10.0f * ratioY };
			CenterText(LoadingMenu.button[i].text, pos);
		}
	}

	return (LoadingMenu);
}

void ResponsiveLoadingMenu(Menu *menuToUpdate, sfVideoMode mode)
{
	float ratioX = mode.width / 1920.0f;
	float ratioY = mode.height / 1080.0f;


	sfVector2f MainMenuButtonsPos = { 400.0f * ratioX, 400.0f * ratioY };
	sfVector2f MainMenuButtonsSize = { 620.0f * ratioX, 200.0f * ratioY };


	for (int i = 0; i < menuToUpdate->buttonNumber; i++)
	{
		ResponsiveButton(&menuToUpdate->button[i], MainMenuButtonsSize, MainMenuButtonsPos, true);
	}

	sfVector2f posFirst = { mode.width / 2.0f, mode.height / 2.0f - 300.0f * ratioY };

	sfVector2f vectorCur = { (posFirst.x) - 600.0f * ratioX, (posFirst.y) - 100.0f  * ratioY };

	menuToUpdate->Position.x = vectorCur.x;
	menuToUpdate->Position.y = vectorCur.y;

	sfVector2f scale;
	sfVector2u size = sfTexture_getSize(sfSprite_getTexture(menuToUpdate->selectionCursor.indexDisplay));
	scale.x = (630.0f * ratioX) / (float)size.x;
	scale.y = (200.0f * ratioY) / (float)size.y;

	sfSprite_setScale(menuToUpdate->selectionCursor.indexDisplay, scale);
	sfSprite_setPosition(menuToUpdate->selectionCursor.indexDisplay, vectorCur);

	menuToUpdate->selectionCursor.offset.x = 0.0f * ratioX;
	menuToUpdate->selectionCursor.offset.y = 150.0f * ratioY;

	AlignMenuButtons(menuToUpdate, 0, posFirst, -20.0f * ratioY, 2);

	sfSprite_setPosition(menuToUpdate->selectionCursor.indexDisplay, sfSprite_getPosition(menuToUpdate->button[0].sprite));
	menuToUpdate->Position = sfSprite_getPosition(menuToUpdate->button[0].sprite);


	int newSize = (int)(TEXT_SIZE_MENU * (mode.height / 1080.0f)) - 5;
	SetStringButton(&menuToUpdate->button[4], /*quit*/GetLocalizationString(6));
	for (int i = 0; i < menuToUpdate->buttonNumber; i++)
	{
		sfText_setCharacterSize(menuToUpdate->button[i].text, newSize);
		sfVector2f pos = (sfVector2f) { menuToUpdate->button[i].rect.left + (menuToUpdate->button[i].rect.width) / 2, menuToUpdate->button[i].rect.top + (menuToUpdate->button[i].rect.height) / 2 + 10.0f * ratioY };
		CenterText(menuToUpdate->button[i].text, pos);
	}
	int numberPlayer = GetPlayersInformations().NumberOfPlayer;
	for (int i = 0; i < menuToUpdate->buttonNumber; i++)
	{
		if (i != menuToUpdate->buttonNumber - 1)
		{
			sfText_setCharacterSize(menuToUpdate->button[i].text, newSize);
			RescaleText(menuToUpdate->button[i].text, 325.0f, 50.0f, ratioX, ratioY);
			sfVector2f pos = (sfVector2f) { menuToUpdate->button[i].rect.left + (menuToUpdate->button[i].rect.width) / 2, menuToUpdate->button[i].rect.top + (menuToUpdate->button[i].rect.height) / 2 - 20.f * ratioY };
			CenterText(menuToUpdate->button[i].text, pos);
			//sfText_setPosition(menuToUpdate->button[0].text, pos2);
			if (menuToUpdate->button[i].text2 != NULL)
			{
				sfText_setCharacterSize(menuToUpdate->button[i].text2, newSize);
				RescaleText(menuToUpdate->button[i].text2, 325.0f, 50.0f, ratioX, ratioY);
				sfText_setString(menuToUpdate->button[i].text2, ChangeSaveNameButton(numberPlayer, i + 1));
				pos = (sfVector2f) { menuToUpdate->button[i].rect.left + (menuToUpdate->button[i].rect.width) / 2, menuToUpdate->button[i].rect.top + (menuToUpdate->button[i].rect.height) / 2 + 20.f * ratioY };
				CenterText(menuToUpdate->button[i].text2, pos);
			}
		}
		else
		{
			sfText_setCharacterSize(menuToUpdate->button[i].text, newSize);
			RescaleText(menuToUpdate->button[i].text, 325.0f, 50.0f, ratioX, ratioY);
			sfVector2f pos = (sfVector2f) { menuToUpdate->button[i].rect.left + (menuToUpdate->button[i].rect.width) / 2, menuToUpdate->button[i].rect.top + (menuToUpdate->button[i].rect.height) / 2 + 10.0f * ratioY };
			CenterText(menuToUpdate->button[i].text, pos);
		}
	}
}

Menu CreateSelectionPlayerMenuProto(sfVideoMode mode)
{
	float ratioX = mode.width / 1920.0f;
	float ratioY = mode.height / 1080.0f;
	Button selectPlayerMenuButtons[5];
	sfVector2f selectPlayerButtonsPos = { 400.0f * ratioX, 400.0f * ratioY };
	sfVector2f selectPlayerButtonsSize = { 620.0f * ratioX, 180.f * ratioY };
	char* filesDirectories[5] =
	{
		"Ressources/Sprites/UI/Bouton.png",
		"Ressources/Sprites/UI/Bouton.png",
		"Ressources/Sprites/UI/Bouton.png",
		"Ressources/Sprites/UI/Bouton.png",
		"Ressources/Sprites/UI/Bouton.png"
	};

	for (int i = 0; i < 5; i++)
	{
		selectPlayerMenuButtons[i] = InitButton(selectPlayerButtonsSize, selectPlayerButtonsPos, i, filesDirectories[i], true);
	}

	sfVector2f posFirst = { mode.width / 2.0f, mode.height / 2.0f - 250.0f * ratioY };
	sfVector2f vectorCur = { (posFirst.x) - 600.0f * ratioX, (posFirst.y) - 100.0f  * ratioY };

	Menu selectPlayerMenu = CreateGameMenu(vectorCur, selectPlayerMenuButtons, 5, 0, false);

	selectPlayerMenu.selectionCursor.indexDisplay = LoadSprite("Ressources/Sprites/UI/Selecteur.png", 1);
	sfVector2f scale;
	sfVector2u size = sfTexture_getSize(sfSprite_getTexture(selectPlayerMenu.selectionCursor.indexDisplay));
	scale.x = (620.0f * ratioX) / (float)size.x;
	scale.y = (180.f * ratioY) / (float)size.y;

	sfSprite_setScale(selectPlayerMenu.selectionCursor.indexDisplay, scale);

	selectPlayerMenu.selectionCursor.indexPos = 0;
	selectPlayerMenu.selectionCursor.offset.x = 0.0f * ratioX;
	selectPlayerMenu.selectionCursor.offset.y = 150.0f * ratioY;

	AlignMenuButtons(&selectPlayerMenu, 0, posFirst, -20.0f * ratioY, 2);

	selectPlayerMenu.selectorsNumber = 3;
	selectPlayerMenu.selectors = (Selector*)calloc(selectPlayerMenu.selectorsNumber, sizeof(Selector));

	selectPlayerMenu.classDisplayer.Class = NULL;
	selectPlayerMenu.classDisplayer.isOn = sfFalse;

	selectPlayerMenu.SliderSelectors = NULL;
	selectPlayerMenu.SliderSelectorsNumber = 0;

	InitSelector(&selectPlayerMenu.selectors[0], posFirst, selectPlayerButtonsSize, "Ressources/Sprites/UI/Bouton_2.png", 1);
	InitSelector(&selectPlayerMenu.selectors[1], posFirst, selectPlayerButtonsSize, "Ressources/Sprites/UI/Bouton_2.png", 1);
	InitSelector(&selectPlayerMenu.selectors[2], posFirst, selectPlayerButtonsSize, "Ressources/Sprites/UI/Bouton_2.png", 1);

	for (int i = 0; i < 3; i++)
	{
		selectPlayerMenu.selectors[i].textDisplay.textArray = (sfText**)calloc(i < 2 ? 2 : 4, sizeof(sfText*));
		selectPlayerMenu.selectors[i].textDisplay.numberOfText = i < 2 ? 2 : 4;
	}

	sfVector2f centerPos = CenterInMiddleElements(selectPlayerMenu, posFirst, 0, 0, 0, -20.0f * ratioY, 1);
	AlignButtonsAndSelector(&selectPlayerMenu, centerPos, 0, 0, 0, -20.0f * ratioY, 1);

	centerPos.y = selectPlayerMenu.button[1].rect.top + (selectPlayerMenu.button[1].rect.height / 2.0f);
	centerPos.x = mode.width / 2.0f;
	centerPos = CenterInMiddleElements(selectPlayerMenu, centerPos, 1, 1, 1, -20.0f * ratioY, 1);
	AlignButtonsAndSelector(&selectPlayerMenu, centerPos, 1, 1, 1, -20.0f * ratioY, 1);

	centerPos.y = selectPlayerMenu.button[2].rect.top + (selectPlayerMenu.button[2].rect.height / 2.0f);
	centerPos.x = mode.width / 2.0f;
	centerPos = CenterInMiddleElements(selectPlayerMenu, centerPos, 2, 2, 2, -20.0f * ratioY, 1);
	AlignButtonsAndSelector(&selectPlayerMenu, centerPos, 2, 2, 2, -20.0f * ratioY, 1);

	sfSprite_setPosition(selectPlayerMenu.selectionCursor.indexDisplay, sfSprite_getPosition(selectPlayerMenu.button[0].sprite));
	selectPlayerMenu.Position = sfSprite_getPosition(selectPlayerMenu.button[0].sprite);

	InitTextButton(&selectPlayerMenu.button[0], /*"Mode"*/GetLocalizationString(179), sfColor_fromRGB(171, 144, 111), TEXT_SIZE_MENU);
	InitTextButton(&selectPlayerMenu.button[1], /*"NumberPlayer"*/GetLocalizationString(181), sfColor_fromRGB(171, 144, 111), TEXT_SIZE_MENU);
	InitTextButton(&selectPlayerMenu.button[2], /*"Dungeon"*/GetLocalizationString(180), sfColor_fromRGB(171, 144, 111), TEXT_SIZE_MENU);
	InitTextButton(&selectPlayerMenu.button[3], /*"Play"*/GetLocalizationString(0), sfColor_fromRGB(171, 144, 111), TEXT_SIZE_MENU);
	InitTextButton(&selectPlayerMenu.button[4], /*"Back"*/GetLocalizationString(6), sfColor_fromRGB(171, 144, 111), TEXT_SIZE_MENU);

	selectPlayerMenu.selectors[0].positionText.x = selectPlayerMenu.selectors[0].rect.left + (selectPlayerMenu.selectors[0].rect.width / 2.0f);
	selectPlayerMenu.selectors[0].positionText.y = selectPlayerMenu.selectors[0].rect.top + (selectPlayerMenu.selectors[0].rect.height / 2.0f) + 10.0f * ratioY;

	selectPlayerMenu.selectors[1].positionText.x = selectPlayerMenu.selectors[1].rect.left + (selectPlayerMenu.selectors[1].rect.width / 2.0f);
	selectPlayerMenu.selectors[1].positionText.y = selectPlayerMenu.selectors[1].rect.top + (selectPlayerMenu.selectors[1].rect.height / 2.0f) + 10.0f * ratioY;

	selectPlayerMenu.selectors[2].positionText.x = selectPlayerMenu.selectors[2].rect.left + (selectPlayerMenu.selectors[2].rect.width / 2.0f);
	selectPlayerMenu.selectors[2].positionText.y = selectPlayerMenu.selectors[2].rect.top + (selectPlayerMenu.selectors[2].rect.height / 2.0f) + 10.0f * ratioY;

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			char *msgToDisplay;
			if (i == 0)
			{
				msgToDisplay = GetLocalizationString(182 + j);
				if (j >= 2)
				{
					continue;
				}
			}
			else if (i == 1)
			{
				msgToDisplay = GetLocalizationString(175 + j);
				if (j >= 2)
				{
					continue;
				}
			}
			else if (i == 2)
			{
				msgToDisplay = j == 0 ? GetLocalizationString(186) : GetLocalizationString(177 + j - 1);
				if (j == 3)
				{
					msgToDisplay = GetLocalizationString(187);
				}
			}
			selectPlayerMenu.selectors[i].textDisplay.textArray[j] = InitText(msgToDisplay, selectPlayerMenu.selectors[i].positionText, sfColor_fromRGB(171, 144, 111), (int)(TEXT_SIZE_MENU * (mode.height / 1080.0f)));
			RescaleText(selectPlayerMenu.selectors[i].textDisplay.textArray[j], 325.0f, 168.5f, ratioX, ratioY);
			CenterText(selectPlayerMenu.selectors[i].textDisplay.textArray[j], selectPlayerMenu.selectors[i].positionText);
		}
		selectPlayerMenu.selectors[i].textDisplay.actualTextToDisplay = 0;
	}

	int newSize = (int)(TEXT_SIZE_MENU * (mode.height / 1080.0f));
	for (int i = 0; i < selectPlayerMenu.buttonNumber; i++)
	{
		selectPlayerMenu.button[i].text2 = NULL;
		sfText_setCharacterSize(selectPlayerMenu.button[i].text, newSize);
		RescaleText(selectPlayerMenu.button[i].text, 325.0f, 168.5f, ratioX, ratioY);
		sfVector2f pos = (sfVector2f) { selectPlayerMenu.button[i].rect.left + (selectPlayerMenu.button[i].rect.width) / 2, selectPlayerMenu.button[i].rect.top + (selectPlayerMenu.button[i].rect.height) / 2 + 10.0f * ratioY };
		CenterText(selectPlayerMenu.button[i].text, pos);
	}


	return (selectPlayerMenu);
}

void ResponsiveSelectionPlayerMenu(Menu *menuToUpdate, sfVideoMode mode)
{
	float ratioX = mode.width / 1920.0f;
	float ratioY = mode.height / 1080.0f;


	sfVector2f selectPlayerButtonsPos = { 400.0f * ratioX, 400.0f * ratioY };
	sfVector2f selectPlayerButtonsSize = { 620.0f * ratioX, 180.0f * ratioY };


	for (int i = 0; i < menuToUpdate->buttonNumber; i++)
	{
		ResponsiveButton(&menuToUpdate->button[i], selectPlayerButtonsSize, selectPlayerButtonsPos, true);
	}

	sfVector2f posFirst = { mode.width / 2.0f, mode.height / 2.0f - 250.0f * ratioY };
	sfVector2f vectorCur = { (posFirst.x) - 600.0f * ratioX, (posFirst.y) - 100.0f  * ratioY };


	menuToUpdate->Position.x = vectorCur.x;
	menuToUpdate->Position.y = vectorCur.y;

	sfVector2f scale;
	sfVector2u size = sfTexture_getSize(sfSprite_getTexture(menuToUpdate->selectionCursor.indexDisplay));
	scale.x = (630.0f * ratioX) / (float)size.x;
	scale.y = (180.0f * ratioY) / (float)size.y;

	sfSprite_setScale(menuToUpdate->selectionCursor.indexDisplay, scale);

	menuToUpdate->selectionCursor.offset.x = 0.0f * ratioX;
	menuToUpdate->selectionCursor.offset.y = 150.0f * ratioY;

	AlignMenuButtons(menuToUpdate, 0, posFirst, -20.0f * ratioY, 2);

	ResponsiveSelector(&menuToUpdate->selectors[0], posFirst, selectPlayerButtonsSize, 1);
	ResponsiveSelector(&menuToUpdate->selectors[1], posFirst, selectPlayerButtonsSize, 1);
	ResponsiveSelector(&menuToUpdate->selectors[2], posFirst, selectPlayerButtonsSize, 1);

	sfVector2f centerPos = CenterInMiddleElements(*menuToUpdate, posFirst, 0, 0, 0, -20.0f * ratioY, 1);
	AlignButtonsAndSelector(menuToUpdate, centerPos, 0, 0, 0, -20.0f * ratioY, 1);

	centerPos.y = menuToUpdate->button[1].rect.top + (menuToUpdate->button[1].rect.height / 2.0f) + 10.0f * ratioY;
	centerPos.x = mode.width / 2.0f;
	centerPos = CenterInMiddleElements(*menuToUpdate, centerPos, 1, 1, 1, -20.0f * ratioY, 1);
	AlignButtonsAndSelector(menuToUpdate, centerPos, 1, 1, 1, -20.0f * ratioY, 1);

	centerPos.y = menuToUpdate->button[2].rect.top + (menuToUpdate->button[2].rect.height / 2.0f) + 10.0f * ratioY;
	centerPos.x = mode.width / 2.0f;
	centerPos = CenterInMiddleElements(*menuToUpdate, centerPos, 2, 2, 2, -20.0f * ratioY, 1);
	AlignButtonsAndSelector(menuToUpdate, centerPos, 2, 2, 2, -20.0f * ratioY, 1);

	sfSprite_setPosition(menuToUpdate->selectionCursor.indexDisplay, sfSprite_getPosition(menuToUpdate->button[0].sprite));
	menuToUpdate->Position = sfSprite_getPosition(menuToUpdate->button[0].sprite);


	for (int j = 0; j < 3; j++)
	{
		menuToUpdate->selectors[j].positionText.x = menuToUpdate->selectors[j].rect.left + (menuToUpdate->selectors[j].rect.width / 2.0f);
		menuToUpdate->selectors[j].positionText.y = menuToUpdate->selectors[j].rect.top + (menuToUpdate->selectors[j].rect.height / 2.0f) + 10.0f * ratioY;;
		for (int i = 0; i < 4; i++)
		{
			char *msgToDisplay;
			if (j == 0)
			{
				msgToDisplay = GetLocalizationString(182 + i);
				if (i >= 2)
				{
					continue;
				}
			}
			else if (j == 1)
			{
				msgToDisplay = GetLocalizationString(175 + i);
				if (i >= 2)
				{
					continue;
				}
			}
			else if (j == 2)
			{
				msgToDisplay = i == 0 ? GetLocalizationString(186) : GetLocalizationString(177 + i - 1);
				if (i == 3)
				{
					msgToDisplay = GetLocalizationString(187);
				}
			}
			sfText_setString(menuToUpdate->selectors[j].textDisplay.textArray[i], msgToDisplay);
			sfText_setCharacterSize(menuToUpdate->selectors[j].textDisplay.textArray[i], (int)(TEXT_SIZE_MENU * (mode.height / 1080.0f)));
			RescaleText(menuToUpdate->selectors[j].textDisplay.textArray[i], 325.0f, 168.5f, ratioX, ratioY);
			CenterText(menuToUpdate->selectors[j].textDisplay.textArray[i], menuToUpdate->selectors[j].positionText);
		}
	}


	int newSize = (int)(TEXT_SIZE_MENU * (mode.height / 1080.0f));
	SetStringButton(&menuToUpdate->button[0], /*one player*/GetLocalizationString(179));
	SetStringButton(&menuToUpdate->button[1], /*two players*/GetLocalizationString(181));
	SetStringButton(&menuToUpdate->button[2], /*back*/GetLocalizationString(180));
	SetStringButton(&menuToUpdate->button[3], /*back*/GetLocalizationString(0));
	SetStringButton(&menuToUpdate->button[4], /*back*/GetLocalizationString(6));

	for (int i = 0; i < menuToUpdate->buttonNumber; i++)
	{
		sfText_setCharacterSize(menuToUpdate->button[i].text, newSize);
		RescaleText(menuToUpdate->button[i].text, 325.0f, 168.5f, ratioX, ratioY);
		sfVector2f pos = (sfVector2f) { menuToUpdate->button[i].rect.left + (menuToUpdate->button[i].rect.width) / 2.0f, menuToUpdate->button[i].rect.top + (menuToUpdate->button[i].rect.height) / 2.0f + 10.0f * ratioY + 10.0f * ratioY };
		CenterText(menuToUpdate->button[i].text, pos);
	}
}

//skin player 1
Menu CreateSelectionSkinPlayerMenuProto(sfVideoMode mode)
{
	float ratioX = mode.width / 1920.0f;
	float ratioY = mode.height / 1080.0f;

	Button selectSkinPlayerMenuButtons[2];
	sfVector2f selectSkinPlayerButtonsPos = { mode.width / 2.f, mode.height / 2.f };
	sfVector2f selectSkinPlayerButtonsSize = { 620.f * ratioX, 180.0f * ratioY };
	char* filesDirectories[2] =
	{
		"Ressources/Sprites/UI/Bouton.png",
		"Ressources/Sprites/UI/Bouton.png"
	};

	for (int i = 0; i < 2; i++)
	{
		selectSkinPlayerMenuButtons[i] = InitButton(selectSkinPlayerButtonsSize, selectSkinPlayerButtonsPos, i, filesDirectories[i], true);
	}
	sfVector2f posFirst = { mode.width / 2.0f, mode.height * 3 / 4.0f };
	sfVector2f vectorCur = { (posFirst.x) - 450.0f * ratioX, (posFirst.y) - 100.0f  * ratioY };

	Menu selectPlayerMenu = CreateGameMenu(vectorCur, selectSkinPlayerMenuButtons, 2, 0, false);
	AddTitleToMenu(&selectPlayerMenu, "Ressources/Sprites/UI/p1.png", (sfVector2f) { mode.width / 2.f, 200.f * ratioY }, 1);

	sfVector2f scaleTitle;
	sfVector2u sizeTitle = sfTexture_getSize(sfSprite_getTexture(selectPlayerMenu.title));
	scaleTitle.x = (620.0f * ratioX) / (float)sizeTitle.x;
	scaleTitle.y = (180.0f * ratioY) / (float)sizeTitle.y;
	sfSprite_setScale(selectPlayerMenu.title, scaleTitle);

	selectPlayerMenu.selectionCursor.indexDisplay = LoadSprite("Ressources/Sprites/UI/Selecteur.png", 1);
	sfVector2f scale;
	sfVector2u size = sfTexture_getSize(sfSprite_getTexture(selectPlayerMenu.selectionCursor.indexDisplay));
	scale.x = (620.0f * ratioX) / (float)size.x;
	scale.y = (180.0f * ratioY) / (float)size.y;

	sfSprite_setScale(selectPlayerMenu.selectionCursor.indexDisplay, scale);

	selectPlayerMenu.selectionCursor.indexPos = 0;
	selectPlayerMenu.selectionCursor.offset.x = 0.0f * ratioX;
	selectPlayerMenu.selectionCursor.offset.y = 150.0f * ratioY;

	AlignMenuButtons(&selectPlayerMenu, 0, posFirst, -20.0f * ratioY, 2);

	sfSprite_setPosition(selectPlayerMenu.selectionCursor.indexDisplay, sfSprite_getPosition(selectPlayerMenu.button[0].sprite));
	selectPlayerMenu.Position = sfSprite_getPosition(selectPlayerMenu.button[0].sprite);

	selectPlayerMenu.selectors = NULL;
	selectPlayerMenu.selectorsNumber = 0;
	selectPlayerMenu.SliderSelectors = NULL;
	selectPlayerMenu.SliderSelectorsNumber = 0;

	selectPlayerMenu.classDisplayer.Class = (sfSprite**)malloc(4 * sizeof(sfSprite*));
	selectPlayerMenu.classDisplayer.NumberOfClass = 4;

	selectPlayerMenu.classDisplayer.Class[0] = LoadSprite("Ressources/Sprites/Players/RedMage.png", 1);
	selectPlayerMenu.classDisplayer.Class[1] = LoadSprite("Ressources/Sprites/Players/BlackMage.png", 1);
	selectPlayerMenu.classDisplayer.Class[2] = LoadSprite("Ressources/Sprites/Players/WhiteMage.png", 1);
	selectPlayerMenu.classDisplayer.Class[3] = LoadSprite("Ressources/Sprites/Players/GreenMage.png", 1);

	selectPlayerMenu.classDisplayer.ActualClassToDisplay = 0;
	selectPlayerMenu.classDisplayer.Position.x = mode.width / 2.f;
	selectPlayerMenu.classDisplayer.Position.y = mode.height / 2.f - 50 * ratioY;
	sfVector2f scaleSize = { ratioX, ratioY };

	for (int i = 0; i < 4; i++)
	{
		sfSprite_setScale(selectPlayerMenu.classDisplayer.Class[i], scaleSize);
		sfSprite_setPosition(selectPlayerMenu.classDisplayer.Class[i], selectPlayerMenu.classDisplayer.Position);
	}
	selectPlayerMenu.classDisplayer.isOn = sfTrue;

	InitTextButton(&selectPlayerMenu.button[0], /*"Play"*/GetLocalizationString(0), sfColor_fromRGB(171, 144, 111), TEXT_SIZE_MENU);
	InitTextButton(&selectPlayerMenu.button[1], /*"Back"*/GetLocalizationString(6), sfColor_fromRGB(171, 144, 111), TEXT_SIZE_MENU);

	int newSize = (int)(TEXT_SIZE_MENU * (mode.height / 1080.0f));
	for (int i = 0; i < selectPlayerMenu.buttonNumber; i++)
	{
		selectPlayerMenu.button[i].text2 = NULL;
		sfText_setCharacterSize(selectPlayerMenu.button[i].text, newSize);
		sfVector2f pos = (sfVector2f) { selectPlayerMenu.button[i].rect.left + (selectPlayerMenu.button[i].rect.width) / 2, selectPlayerMenu.button[i].rect.top + (selectPlayerMenu.button[i].rect.height) / 2 + 10.0f * ratioY };
		CenterText(selectPlayerMenu.button[i].text, pos);
	}

	selectPlayerMenu.imagesNumber = 2;
	selectPlayerMenu.images = (sfSprite**)calloc(selectPlayerMenu.imagesNumber, sizeof(sfSprite*));
	selectPlayerMenu.images[0] = LoadSprite("Ressources/Sprites/UI/Arrow_Left_Selector.png", 1);
	selectPlayerMenu.images[1] = LoadSprite("Ressources/Sprites/UI/Arrow_Right_Selector.png", 1);
	sfSprite_setPosition(selectPlayerMenu.images[0], (sfVector2f) { 790.f * ratioX, 500.f * ratioY });
	sfSprite_setPosition(selectPlayerMenu.images[1], (sfVector2f) { 1125.f * ratioX, 500.f * ratioY });
	for (int i = 0; i < selectPlayerMenu.imagesNumber; i++)
	{
		size = sfTexture_getSize(sfSprite_getTexture(selectPlayerMenu.images[i]));
		scale.x = (110.0f * ratioX) / (float)size.x;
		scale.y = (140.f * ratioY) / (float)size.y;
		sfSprite_setScale(selectPlayerMenu.images[i], scale);
	}

	return (selectPlayerMenu);
}

void ResponsiveSelectionSkinPlayerMenu(Menu *menuToUpdate, sfVideoMode mode)
{
	float ratioX = mode.width / 1920.0f;
	float ratioY = mode.height / 1080.0f;


	sfVector2f selectSkinPlayerButtonsPos = { mode.width / 2.f, mode.height / 2.f };
	sfVector2f selectSkinPlayerButtonsSize = { 620.0f * ratioX, 180.0f * ratioY };

	sfVector2f scaleTitle;
	sfVector2u sizeTitle = sfTexture_getSize(sfSprite_getTexture(menuToUpdate->title));
	scaleTitle.x = (620.0f * ratioX) / (float)sizeTitle.x;
	scaleTitle.y = (180.0f * ratioY) / (float)sizeTitle.y;
	sfSprite_setScale(menuToUpdate->title, scaleTitle);

	sfVector2f posTitle = { mode.width / 2.f, 200.f * ratioY };
	sfSprite_setPosition(menuToUpdate->title, posTitle);

	for (int i = 0; i < menuToUpdate->buttonNumber; i++)
	{
		ResponsiveButton(&menuToUpdate->button[i], selectSkinPlayerButtonsSize, selectSkinPlayerButtonsPos, true);
	}

	sfVector2f posFirst = { mode.width / 2.0f, mode.height * 3 / 4.0f };

	sfVector2f vectorCur = { (posFirst.x) - 450.0f * ratioX, (posFirst.y) - 100.0f  * ratioY };


	menuToUpdate->Position.x = vectorCur.x;
	menuToUpdate->Position.y = vectorCur.y;

	sfVector2f scale;
	sfVector2u size = sfTexture_getSize(sfSprite_getTexture(menuToUpdate->selectionCursor.indexDisplay));
	scale.x = (620.0f * ratioX) / (float)size.x;
	scale.y = (180.0f * ratioY) / (float)size.y;


	sfSprite_setScale(menuToUpdate->selectionCursor.indexDisplay, scale);

	menuToUpdate->selectionCursor.offset.x = 0.0f * ratioX;
	menuToUpdate->selectionCursor.offset.y = 150.0f * ratioY;

	AlignMenuButtons(menuToUpdate, 0, posFirst, -20.f * ratioY, 2);

	sfSprite_setPosition(menuToUpdate->selectionCursor.indexDisplay, sfSprite_getPosition(menuToUpdate->button[0].sprite));
	menuToUpdate->Position = sfSprite_getPosition(menuToUpdate->button[0].sprite);


	menuToUpdate->classDisplayer.Position.x = mode.width / 2.f;
	menuToUpdate->classDisplayer.Position.y = mode.height / 2.f - 50 * ratioY;
	sfVector2f scaleSize = { ratioX, ratioY };


	for (int i = 0; i < 4; i++)
	{
		sfSprite_setScale(menuToUpdate->classDisplayer.Class[i], scaleSize);
		sfSprite_setPosition(menuToUpdate->classDisplayer.Class[i], menuToUpdate->classDisplayer.Position);
	}

	int newSize = (int)(TEXT_SIZE_MENU * (mode.height / 1080.0f));
	SetStringButton(&menuToUpdate->button[0], /*play*/GetLocalizationString(0));
	SetStringButton(&menuToUpdate->button[1], /*back*/GetLocalizationString(6));
	for (int i = 0; i < menuToUpdate->buttonNumber; i++)
	{
		sfText_setCharacterSize(menuToUpdate->button[i].text, newSize);
		sfVector2f pos = (sfVector2f) { menuToUpdate->button[i].rect.left + (menuToUpdate->button[i].rect.width) / 2, menuToUpdate->button[i].rect.top + (menuToUpdate->button[i].rect.height) / 2 + 10.0f * ratioY };
		CenterText(menuToUpdate->button[i].text, pos);
	}

	sfSprite_setPosition(menuToUpdate->images[0], (sfVector2f) { 790.f * ratioX, 500.f * ratioY });
	sfSprite_setPosition(menuToUpdate->images[1], (sfVector2f) { 1125.f * ratioX, 500.f * ratioY });
	for (int i = 0; i < menuToUpdate->imagesNumber; i++)
	{
		size = sfTexture_getSize(sfSprite_getTexture(menuToUpdate->images[i]));
		scale.x = (110.0f * ratioX) / (float)size.x;
		scale.y = (140.f * ratioY) / (float)size.y;
		sfSprite_setScale(menuToUpdate->images[i], scale);
	}
	//menuToUpdate->classDisplayer.Class[0]
}

Menu CreateOptionMenuProto(sfVideoMode mode)
{
	float ratioX = mode.width / 1920.0f;
	float ratioY = mode.height / 1080.0f;
	Button selectOptionButtons[4];
	sfVector2f selectOptionButtonsPos = { mode.width / 2.0f,  mode.height / 2.0f };
	sfVector2f selectOptionButtonsSize = { 620.0f * ratioX, 180.0f * ratioY };
	char* filesDirectories[4] =
	{
		"Ressources/Sprites/UI/Bouton.png",
		"Ressources/Sprites/UI/Bouton.png",
		"Ressources/Sprites/UI/Bouton.png",
		"Ressources/Sprites/UI/Bouton.png"
	};

	for (int i = 0; i < 4; i++)
	{
		selectOptionButtons[i] = InitButton(selectOptionButtonsSize, selectOptionButtonsPos, i, filesDirectories[i], true);
	}
	sfVector2f posFirst = { mode.width / 2.0f,  350.0f * ratioY };
	sfVector2f vectorCur = { (posFirst.x) - 450.0f * ratioX, (posFirst.y) - 100.0f  * ratioY };

	Menu selectOptionMenu = CreateGameMenu(vectorCur, selectOptionButtons, 4, 0, false);


	selectOptionMenu.selectionCursor.indexDisplay = LoadSprite("Ressources/Sprites/UI/Selecteur.png", 1);

	sfVector2f scale;
	sfVector2u size = sfTexture_getSize(sfSprite_getTexture(selectOptionMenu.selectionCursor.indexDisplay));
	scale.x = (620.0f * ratioX) / (float)size.x;
	scale.y = (180.0f * ratioY) / (float)size.y;

	sfSprite_setScale(selectOptionMenu.selectionCursor.indexDisplay, scale);

	selectOptionMenu.selectionCursor.indexPos = 0;
	selectOptionMenu.selectionCursor.offset.x = 0.0f * ratioX;
	selectOptionMenu.selectionCursor.offset.y = 150.0f * ratioY;

	AlignMenuButtons(&selectOptionMenu, 0, posFirst, -20.0f * ratioY, 2);

	sfSprite_setPosition(selectOptionMenu.selectionCursor.indexDisplay, sfSprite_getPosition(selectOptionMenu.button[0].sprite));
	selectOptionMenu.Position = sfSprite_getPosition(selectOptionMenu.button[0].sprite);

	selectOptionMenu.selectors = NULL;
	selectOptionMenu.selectorsNumber = 0;
	selectOptionMenu.classDisplayer.Class = NULL;
	selectOptionMenu.classDisplayer.isOn = sfFalse;
	selectOptionMenu.SliderSelectors = NULL;
	selectOptionMenu.SliderSelectorsNumber = 0;

	sfVector2f scaleSize = { ratioX , ratioY };


	InitTextButton(&selectOptionMenu.button[0], /*"General"*/GetLocalizationString(20), sfColor_fromRGB(171, 144, 111), TEXT_SIZE_MENU);
	InitTextButton(&selectOptionMenu.button[1], /*"Graphics"*/GetLocalizationString(21), sfColor_fromRGB(171, 144, 111), TEXT_SIZE_MENU);
	InitTextButton(&selectOptionMenu.button[2], /*"Sounds"*/GetLocalizationString(22), sfColor_fromRGB(171, 144, 111), TEXT_SIZE_MENU);
	InitTextButton(&selectOptionMenu.button[3], /*"Back"*/GetLocalizationString(6), sfColor_fromRGB(171, 144, 111), TEXT_SIZE_MENU);

	int newSize = (int)(TEXT_SIZE_MENU * (mode.height / 1080.0f));
	for (int i = 0; i < selectOptionMenu.buttonNumber; i++)
	{
		selectOptionMenu.button[i].text2 = NULL;
		sfText_setCharacterSize(selectOptionMenu.button[i].text, newSize);
		RescaleText(selectOptionMenu.button[i].text, 325.0f, 168.5f, ratioX, ratioY);

		sfVector2f pos = (sfVector2f) { selectOptionMenu.button[i].rect.left + (selectOptionMenu.button[i].rect.width) / 2, selectOptionMenu.button[i].rect.top + (selectOptionMenu.button[i].rect.height) / 2 + 10.0f * ratioY };
		CenterText(selectOptionMenu.button[i].text, pos);
	}

	return (selectOptionMenu);
}

void ResponsiveOptionMenu(Menu *menuToUpdate, sfVideoMode mode)
{
	float ratioX = mode.width / 1920.0f;
	float ratioY = mode.height / 1080.0f;

	sfVector2f selectOptionButtonsPos = { mode.width / 2.f, mode.height / 2.f };
	sfVector2f selectOptionButtonsSize = { 620.0f * ratioX, 180.0f * ratioY };

	for (int i = 0; i < menuToUpdate->buttonNumber; i++)
	{
		ResponsiveButton(&menuToUpdate->button[i], selectOptionButtonsSize, selectOptionButtonsPos, true);
	}

	sfVector2f posFirst = { mode.width / 2.0f,  350.0f * ratioY };

	sfVector2f vectorCur = { (posFirst.x) - 450.0f * ratioX, (posFirst.y) - 100.0f  * ratioY };


	menuToUpdate->Position.x = vectorCur.x;
	menuToUpdate->Position.y = vectorCur.y;

	sfVector2f scale;
	sfVector2u size = sfTexture_getSize(sfSprite_getTexture(menuToUpdate->selectionCursor.indexDisplay));
	scale.x = (620.0f * ratioX) / (float)size.x;
	scale.y = (180.0f * ratioY) / (float)size.y;

	sfSprite_setScale(menuToUpdate->selectionCursor.indexDisplay, scale);

	menuToUpdate->selectionCursor.offset.x = 0.0f * ratioX;
	menuToUpdate->selectionCursor.offset.y = 150.0f * ratioY;

	AlignMenuButtons(menuToUpdate, 0, posFirst, -20.f * ratioY, 2);

	sfSprite_setPosition(menuToUpdate->selectionCursor.indexDisplay, sfSprite_getPosition(menuToUpdate->button[0].sprite));
	menuToUpdate->Position = sfSprite_getPosition(menuToUpdate->button[0].sprite);

	sfVector2f scaleSize = { ratioX, ratioY };


	int newSize = (int)(TEXT_SIZE_MENU * (mode.height / 1080.0f));
	SetStringButton(&menuToUpdate->button[0], /*General*/GetLocalizationString(20));
	SetStringButton(&menuToUpdate->button[1], /*Graphics*/GetLocalizationString(21));
	SetStringButton(&menuToUpdate->button[2], /*Sounds*/GetLocalizationString(22));
	SetStringButton(&menuToUpdate->button[3], /*back*/GetLocalizationString(6));
	for (int i = 0; i < menuToUpdate->buttonNumber; i++)
	{
		sfText_setCharacterSize(menuToUpdate->button[i].text, newSize);
		RescaleText(menuToUpdate->button[i].text, 325.0f, 168.5f, ratioX, ratioY);

		sfVector2f pos = (sfVector2f) { menuToUpdate->button[i].rect.left + (menuToUpdate->button[i].rect.width) / 2, menuToUpdate->button[i].rect.top + (menuToUpdate->button[i].rect.height) / 2 + 10.0f * ratioY };
		CenterText(menuToUpdate->button[i].text, pos);
	}
}

Menu CreateGeneralMenuProto(Config actualConfig)
{
	float ratioX = actualConfig.mode.width / 1920.0f;
	float ratioY = actualConfig.mode.height / 1080.0f;
	Button selectOptionButtons[4];
	sfVector2f selectOptionButtonsPos = { actualConfig.mode.width / 2.0f,  actualConfig.mode.height / 2.0f };
	sfVector2f selectOptionButtonsSize = { 620.0f * ratioX, 180.0f * ratioY };
	char* filesDirectories[4] =
	{
		"Ressources/Sprites/UI/Bouton.png",
		"Ressources/Sprites/UI/Bouton.png",
		"Ressources/Sprites/UI/Bouton.png",
		"Ressources/Sprites/UI/Bouton.png"
	};

	for (int i = 0; i < 4; i++)
	{
		selectOptionButtons[i] = InitButton(selectOptionButtonsSize, selectOptionButtonsPos, i, filesDirectories[i], true);
	}
	sfVector2f posFirst = { actualConfig.mode.width / 2.0f,  350.0f * ratioY };
	sfVector2f vectorCur = { (posFirst.x) - 450.0f * ratioX, (posFirst.y) - 100.0f  * ratioY };

	Menu selectOptionMenu = CreateGameMenu(vectorCur, selectOptionButtons, 4, 0, false);

	selectOptionMenu.selectionCursor.indexPos = 0;
	selectOptionMenu.selectionCursor.offset.x = 0.0f * ratioX;
	selectOptionMenu.selectionCursor.offset.y = 150.0f * ratioY;

	AlignMenuButtons(&selectOptionMenu, 0, posFirst, -10.0f * ratioY, 2);

	selectOptionMenu.selectors = malloc(1 * sizeof(Selector));
	selectOptionMenu.selectorsNumber = 1;
	selectOptionMenu.classDisplayer.Class = NULL;
	selectOptionMenu.classDisplayer.isOn = sfFalse;

	selectOptionMenu.SliderSelectors = NULL;
	selectOptionMenu.SliderSelectorsNumber = 0;

	InitSelector(&selectOptionMenu.selectors[0], posFirst, selectOptionButtonsSize, "Ressources/Sprites/UI/Bouton_2.png", 1);

	sfVector2f centerPos = CenterInMiddleElements(selectOptionMenu, posFirst, 0, 0, 0, -20.0f * ratioY, 1);
	AlignButtonsAndSelector(&selectOptionMenu, centerPos, 0, 0, 0, -20.0f * ratioY, 1);

	selectOptionMenu.selectors[0].positionText.x = selectOptionMenu.selectors[0].rect.left + (selectOptionMenu.selectors[0].rect.width / 2.0f);
	selectOptionMenu.selectors[0].positionText.y = selectOptionMenu.selectors[0].rect.top + (selectOptionMenu.selectors[0].rect.height / 2.0f) + 10.0f * ratioY;

	selectOptionMenu.selectors[0].textDisplay.textArray = (sfText**)calloc(2, sizeof(sfText*));
	selectOptionMenu.selectors[0].textDisplay.numberOfText = 2;
	selectOptionMenu.selectors[0].textDisplay.actualTextToDisplay = actualConfig.MainLanguage;

	selectOptionMenu.selectors[0].textDisplay.textArray[0] = InitText(/*"English"*/GetLocalizationString(17), selectOptionMenu.selectors[0].positionText, sfColor_fromRGB(171, 144, 111), (int)(TEXT_SIZE_MENU * (actualConfig.mode.height / 1080.0f)));
	CenterText(selectOptionMenu.selectors[0].textDisplay.textArray[0], selectOptionMenu.selectors[0].positionText);
	selectOptionMenu.selectors[0].textDisplay.textArray[1] = InitText(/*"Français"*/GetLocalizationString(18), selectOptionMenu.selectors[0].positionText, sfColor_fromRGB(171, 144, 111), (int)(TEXT_SIZE_MENU * (actualConfig.mode.height / 1080.0f)));
	CenterText(selectOptionMenu.selectors[0].textDisplay.textArray[1], selectOptionMenu.selectors[0].positionText);

	selectOptionMenu.selectors[0].isOn = sfFalse;

	selectOptionMenu.selectionCursor.indexDisplay = LoadSprite("Ressources/Sprites/UI/Selecteur.png", 1);
	sfVector2f scale;
	sfVector2u size = sfTexture_getSize(sfSprite_getTexture(selectOptionMenu.selectionCursor.indexDisplay));
	scale.x = (620.0f * ratioX) / (float)size.x;
	scale.y = (180.0f * ratioY) / (float)size.y;

	sfSprite_setScale(selectOptionMenu.selectionCursor.indexDisplay, scale);

	selectOptionMenu.selectionCursor.indexPos = 0;
	selectOptionMenu.selectionCursor.offset.x = 0.0f * ratioX;
	selectOptionMenu.selectionCursor.offset.y = 150.0f * ratioY;

	sfSprite_setPosition(selectOptionMenu.selectionCursor.indexDisplay, sfSprite_getPosition(selectOptionMenu.button[0].sprite));

	selectOptionMenu.Position = sfSprite_getPosition(selectOptionMenu.button[0].sprite);

	InitTextButton(&selectOptionMenu.button[0], /*"Language"*/GetLocalizationString(16), sfColor_fromRGB(171, 144, 111), TEXT_SIZE_MENU);
	InitTextButton(&selectOptionMenu.button[1], /*"Controls"*/GetLocalizationString(151), sfColor_fromRGB(171, 144, 111), TEXT_SIZE_MENU);
	InitTextButton(&selectOptionMenu.button[2], /*"Controls"*/GetLocalizationString(23), sfColor_fromRGB(171, 144, 111), TEXT_SIZE_MENU);
	InitTextButton(&selectOptionMenu.button[3], /*"Back"*/GetLocalizationString(6), sfColor_fromRGB(171, 144, 111), TEXT_SIZE_MENU);


	int newSize = (int)(TEXT_SIZE_MENU * (actualConfig.mode.height / 1080.0f));
	for (int i = 0; i < selectOptionMenu.buttonNumber; i++)
	{
		selectOptionMenu.button[i].text2 = NULL;
		sfText_setCharacterSize(selectOptionMenu.button[i].text, newSize);
		RescaleText(selectOptionMenu.button[i].text, 325.0f, 168.5f, ratioX, ratioY);
		sfVector2f pos = (sfVector2f) { selectOptionMenu.button[i].rect.left + (selectOptionMenu.button[i].rect.width) / 2, selectOptionMenu.button[i].rect.top + (selectOptionMenu.button[i].rect.height) / 2 + 10.0f * ratioY };
		CenterText(selectOptionMenu.button[i].text, pos);
	}

	return (selectOptionMenu);
}

void ResponsiveGeneralMenu(Menu *menuToUpdate, Config actualConfig)
{
	float ratioX = actualConfig.mode.width / 1920.0f;
	float ratioY = actualConfig.mode.height / 1080.0f;

	sfVector2f posFirst = { actualConfig.mode.width / 2.0f, 350.0f * ratioY };

	sfVector2f menuButtonsSize = { 620.0f * ratioX, 180.0f * ratioY };

	for (int i = 0; i < menuToUpdate->buttonNumber; i++)
	{
		ResponsiveButton(&menuToUpdate->button[i], menuButtonsSize, posFirst, true);
	}

	sfVector2f vectorCur = { (posFirst.x) - 600.0f * ratioX, (posFirst.y) - 100.0f  * ratioY };

	menuToUpdate->Position.x = vectorCur.x;
	menuToUpdate->Position.y = vectorCur.y;

	AlignMenuButtons(menuToUpdate, 0, posFirst, -10.0f * ratioY, 2);

	ResponsiveSelector(&menuToUpdate->selectors[0], posFirst, menuButtonsSize, 1);

	sfVector2f centerPos = CenterInMiddleElements(*menuToUpdate, posFirst, 0, 0, 0, -20.0f * ratioY, 1);
	AlignButtonsAndSelector(menuToUpdate, centerPos, 0, 0, 0, -20.0f * ratioY, 1);

	menuToUpdate->selectors[0].positionText.x = menuToUpdate->selectors[0].rect.left + (menuToUpdate->selectors[0].rect.width / 2.0f);
	menuToUpdate->selectors[0].positionText.y = menuToUpdate->selectors[0].rect.top + (menuToUpdate->selectors[0].rect.height / 2.0f) + 10.0f * ratioY;

	for (int i = 0; i < 2; i++)
	{
		sfText_setCharacterSize(menuToUpdate->selectors[0].textDisplay.textArray[i], (int)(TEXT_SIZE_MENU * (actualConfig.mode.height / 1080.0f)));
		CenterText(menuToUpdate->selectors[0].textDisplay.textArray[i], menuToUpdate->selectors[0].positionText);
	}


	int newSize = (int)(TEXT_SIZE_MENU * (actualConfig.mode.height / 1080.0f));
	SetStringButton(&menuToUpdate->button[0], /*Language*/GetLocalizationString(16));
	SetStringButton(&menuToUpdate->button[1], /*Shortcuts*/GetLocalizationString(151));
	SetStringButton(&menuToUpdate->button[2], /*Controls*/GetLocalizationString(23));
	SetStringButton(&menuToUpdate->button[3], /*back*/GetLocalizationString(6));
	for (int i = 0; i < menuToUpdate->buttonNumber; i++)
	{
		sfText_setCharacterSize(menuToUpdate->button[i].text, newSize);
		RescaleText(menuToUpdate->button[i].text, 325.0f, 168.5f, ratioX, ratioY);
		sfVector2f pos = (sfVector2f) { menuToUpdate->button[i].rect.left + (menuToUpdate->button[i].rect.width) / 2, menuToUpdate->button[i].rect.top + (menuToUpdate->button[i].rect.height) / 2 + 10.0f * ratioY };
		CenterText(menuToUpdate->button[i].text, pos);
	}

	sfVector2f scale;
	sfVector2u size = sfTexture_getSize(sfSprite_getTexture(menuToUpdate->selectionCursor.indexDisplay));
	scale.x = (620.0f * ratioX) / (float)size.x;
	scale.y = (180.0f * ratioY) / (float)size.y;

	sfSprite_setScale(menuToUpdate->selectionCursor.indexDisplay, scale);
	sfSprite_setPosition(menuToUpdate->selectionCursor.indexDisplay, sfSprite_getPosition(menuToUpdate->button[0].sprite));

	menuToUpdate->Position = sfSprite_getPosition(menuToUpdate->button[0].sprite);

	menuToUpdate->selectionCursor.offset.x = 0.0f * ratioX;
	menuToUpdate->selectionCursor.offset.y = 150.0f * ratioY;

	sfVector2f newPos = { menuToUpdate->Position.x + menuToUpdate->selectionCursor.indexPos * menuToUpdate->selectionCursor.offset.x,
		menuToUpdate->Position.y + menuToUpdate->selectionCursor.indexPos * menuToUpdate->selectionCursor.offset.y };
	sfSprite_setPosition(menuToUpdate->selectionCursor.indexDisplay, newPos);
}

Menu CreateShortcutsMenuProto(sfVideoMode mode)
{
	float ratioX = mode.width / 1920.0f;
	float ratioY = mode.height / 1080.0f;

	sfVector2f posFirst = { 200.f * ratioX, 100.0f * ratioY };

	Button controlsMenuButtons[12];
	sfVector2f controlsMenuButtonsPos = { 20.0f * ratioX, 20.0f * ratioY };
	sfVector2f controlsMenuButtonsSize = { 450.0f * ratioX, 100.0f * ratioY };
	char* filesDirectories[4] =
	{
		"Ressources/Sprites/UI/Key_Bind_Bouton.png",
		"Ressources/Sprites/UI/Key_Bind_Bouton.png",
		"Ressources/Sprites/UI/Key_Bind_Bouton.png",
		"Ressources/Sprites/UI/Key_Bind_Bouton.png"
	};

	for (int i = 0; i < 11; i++)
	{
		controlsMenuButtons[i] = InitButton(controlsMenuButtonsSize, controlsMenuButtonsPos, i, filesDirectories[rand() % 4], true);
	}

	controlsMenuButtons[11] = InitButton(controlsMenuButtonsSize, controlsMenuButtonsPos, 11, "Ressources/Sprites/UI/Key_Bind_Bouton.png", true);

	sfVector2f vectorCur = { (posFirst.x) - 600.0f * ratioX, (posFirst.y) - 100.0f  * ratioY };

	Menu controls = CreateGameMenu(vectorCur, controlsMenuButtons, 12, 0, false);

	controls.selectionCursor.indexDisplay = LoadSprite("Ressources/Sprites/UI/Slice_Selector.png", 1);
	sfVector2f scale;
	sfVector2u size = sfTexture_getSize(sfSprite_getTexture(controls.selectionCursor.indexDisplay));
	scale.x = (450.0f * ratioX) / (float)size.x;
	scale.y = (100.0f * ratioY) / (float)size.y;

	sfSprite_setScale(controls.selectionCursor.indexDisplay, scale);


	controls.selectionCursor.indexPos = 0;
	controls.selectionCursor.offset.x = 0.0f * ratioX;
	controls.selectionCursor.offset.y = 60.0f * ratioY;

	AlignMenuButtons(&controls, 0, posFirst, -25.0f * ratioY, 2);

	sfSprite_setPosition(controls.button[11].sprite, (sfVector2f) { 960.f * ratioX, 930.f * ratioY });
	controls.button[11].rect.left = sfSprite_getPosition(controls.button[11].sprite).x - controls.button[11].rect.width / 2.f;
	controls.button[11].rect.top = sfSprite_getPosition(controls.button[11].sprite).y - controls.button[11].rect.height / 2.f;

	controls.selectorsNumber = 11;
	controls.selectors = (Selector*)calloc(controls.selectorsNumber, sizeof(Selector));
	controls.classDisplayer.Class = NULL;
	controls.classDisplayer.isOn = sfFalse;
	controls.SliderSelectors = NULL;
	controls.SliderSelectorsNumber = 0;

	for (int i = 0; i < controls.selectorsNumber; i++)
	{
		InitSelector(&controls.selectors[i], posFirst, controlsMenuButtonsSize, filesDirectories[rand() % 4], 1);

		controls.selectors[i].isOn = sfFalse;
	}


	sfVector2f centerPos;

	for (int i = 0; i < controls.selectorsNumber; i++)
	{
		centerPos.y = controls.button[i].rect.top + (controls.button[i].rect.height / 2.0f);
		centerPos.x = mode.width / 2.0f;
		centerPos = CenterInMiddleElements(controls, centerPos, i, i, i, 30.0f * ratioY, 1);
		AlignButtonsAndSelector(&controls, centerPos, i, i, i, 30.0f * ratioY, 1);
	}

	for (int i = 0; i < controls.selectorsNumber; i++)
	{
		controls.selectors[i].positionText.x = controls.selectors[i].rect.left + (controls.selectors[0].rect.width / 2.0f);
		controls.selectors[i].positionText.y = controls.selectors[i].rect.top + (controls.selectors[0].rect.height / 2.0f) + 10.0f * ratioY;
	}

	for (int i = 0; i < controls.selectorsNumber; i++)
	{
		controls.selectors[i].textDisplay.textArray = (sfText**)calloc(1, sizeof(sfText*));
		controls.selectors[i].textDisplay.numberOfText = 1;
		controls.selectors[i].textDisplay.actualTextToDisplay = 0;
		char msgToDisplay[50];
		if (i < controls.selectorsNumber - 2)
		{
			sprintf(msgToDisplay, GetLocalizationString(36 + GetMapKey(i)), sfColor_fromRGB(171, 144, 111), TEXT_SIZE_MENU);
		}
		else
		{
			sprintf(msgToDisplay, GetLocalizationString(138 + GetMapKey(i)), sfColor_fromRGB(171, 144, 111), TEXT_SIZE_MENU);
		}
		controls.selectors[i].textDisplay.textArray[0] = InitText(msgToDisplay, controls.selectors[i].positionText, sfColor_fromRGB(171, 144, 111), (int)(TEXT_SIZE_MENU * (mode.height / 1080.0f)));
		CenterText(controls.selectors[i].textDisplay.textArray[0], controls.selectors[i].positionText);
	}

	for (int i = 0; i < 11; i++)
	{
		InitTextButton(&controls.button[i], /**/GetLocalizationString(24 + i), sfColor_fromRGB(171, 144, 111), TEXT_SIZE_MENU);
	}

	InitTextButton(&controls.button[11], /*"Back"*/GetLocalizationString(6), sfColor_fromRGB(171, 144, 111), TEXT_SIZE_MENU);


	int newSize = (int)(TEXT_SIZE_MENU * (mode.height / 1080.0f));
	for (int i = 0; i < controls.buttonNumber; i++)
	{
		controls.button[i].text2 = NULL;
		sfText_setCharacterSize(controls.button[i].text, newSize);

		sfVector2f pos = (sfVector2f) { controls.button[i].rect.left + (controls.button[i].rect.width) / 2, controls.button[i].rect.top + (controls.button[i].rect.height) / 2 + 10.0f * ratioY };
		CenterText(controls.button[i].text, pos);
	}
	controls.Position = sfSprite_getPosition(controls.button[0].sprite);
	sfSprite_setPosition(controls.selectionCursor.indexDisplay, sfSprite_getPosition(controls.button[0].sprite));

	return controls;
}

void ResponsiveShortcutsMenu(Menu *menuToUpdate, sfVideoMode mode)
{
	float ratioX = mode.width / 1920.0f;
	float ratioY = mode.height / 1080.0f;

	sfVector2f posFirst = { 200.f * ratioX, 100.0f * ratioY };
	sfVector2f ControlsMenuButtonsPos = { 20.0f * ratioX, 20.0f * ratioY };
	sfVector2f controlsMenuButtonsSize = { 450.0f * ratioX, 100.0f * ratioY };


	for (int i = 0; i < menuToUpdate->buttonNumber; i++)
	{
		ResponsiveButton(&menuToUpdate->button[i], controlsMenuButtonsSize, ControlsMenuButtonsPos, true);
	}

	sfVector2f vectorCur = { (posFirst.x) - 600.0f * ratioX, (posFirst.y) - 100.0f  * ratioY };

	menuToUpdate->Position.x = vectorCur.x;
	menuToUpdate->Position.y = vectorCur.y;

	sfVector2f scale;
	sfVector2u size = sfTexture_getSize(sfSprite_getTexture(menuToUpdate->selectionCursor.indexDisplay));
	scale.x = (450.0f * ratioX) / (float)size.x;
	scale.y = (100.0f * ratioY) / (float)size.y;

	sfSprite_setScale(menuToUpdate->selectionCursor.indexDisplay, scale);
	sfSprite_setPosition(menuToUpdate->selectionCursor.indexDisplay, vectorCur);

	menuToUpdate->selectionCursor.offset.x = 0.0f * ratioX;
	menuToUpdate->selectionCursor.offset.y = 60.0f * ratioY;

	AlignMenuButtons(menuToUpdate, 0, posFirst, -25.0f * ratioY, 2);

	sfSprite_setPosition(menuToUpdate->button[11].sprite, (sfVector2f) { 960.f * ratioX, 930.f * ratioY });
	menuToUpdate->button[11].rect.left = sfSprite_getPosition(menuToUpdate->button[11].sprite).x - menuToUpdate->button[11].rect.width / 2.f;
	menuToUpdate->button[11].rect.top = sfSprite_getPosition(menuToUpdate->button[11].sprite).y - menuToUpdate->button[11].rect.height / 2.f;

	menuToUpdate->Position = sfSprite_getPosition(menuToUpdate->button[0].sprite);

	for (int i = 0; i < menuToUpdate->selectorsNumber; i++)
	{
		ResponsiveSelector(&menuToUpdate->selectors[i], posFirst, controlsMenuButtonsSize, 1);
	}

	sfVector2f centerPos;
	int newSize = (int)(TEXT_SIZE_MENU * (mode.height / 1080.0f));

	for (int i = 0; i < menuToUpdate->selectorsNumber; i++)
	{
		centerPos.y = menuToUpdate->button[i].rect.top + (menuToUpdate->button[i].rect.height / 2.0f);
		centerPos.x = mode.width / 2.0f;
		centerPos = CenterInMiddleElements(*menuToUpdate, centerPos, i, i, i, 30.0f * ratioY, 1);
		AlignButtonsAndSelector(menuToUpdate, centerPos, i, i, i, 30.0f * ratioY, 1);

		menuToUpdate->selectors[i].isOn = sfFalse;

		if (i < menuToUpdate->selectorsNumber - 2)
		{
			sfText_setString(menuToUpdate->selectors[i].textDisplay.textArray[0], GetLocalizationString(36 + GetMapKey(i)));
		}
		else
		{
			sfText_setString(menuToUpdate->selectors[i].textDisplay.textArray[0], GetLocalizationString(138 + GetMapKey(i)));
		}

		menuToUpdate->selectors[i].positionText.x = menuToUpdate->selectors[i].rect.left + (menuToUpdate->selectors[i].rect.width / 2.0f);
		menuToUpdate->selectors[i].positionText.y = menuToUpdate->selectors[i].rect.top + (menuToUpdate->selectors[i].rect.height / 2.0f) + 10.0f * ratioY;
		sfText_setCharacterSize(menuToUpdate->selectors[i].textDisplay.textArray[0], newSize);
		CenterText(menuToUpdate->selectors[i].textDisplay.textArray[0], menuToUpdate->selectors[i].positionText);
	}

	for (int i = 0; i < 11; i++)
	{
		sfText_setString(menuToUpdate->button[i].text, /*borderless*/GetLocalizationString(24 + i));
	}

	for (int i = 0; i < menuToUpdate->buttonNumber; i++)
	{
		menuToUpdate->button[i].text2 = NULL;
		sfText_setCharacterSize(menuToUpdate->button[i].text, newSize);

		sfVector2f pos = (sfVector2f) { menuToUpdate->button[i].rect.left + (menuToUpdate->button[i].rect.width) / 2, menuToUpdate->button[i].rect.top + (menuToUpdate->button[i].rect.height) / 2 + 10.0f * ratioY };
		CenterText(menuToUpdate->button[i].text, pos);
	}
	menuToUpdate->Position = sfSprite_getPosition(menuToUpdate->button[0].sprite);
	sfSprite_setPosition(menuToUpdate->selectionCursor.indexDisplay, sfSprite_getPosition(menuToUpdate->button[0].sprite));
}

Menu CreateGraphicsMenuProto(Config actualConfig)
{
	float ratioX = actualConfig.mode.width / 1920.0f;
	float ratioY = actualConfig.mode.height / 1080.0f;
	///////////////////////////////////////////////////////////////////////////////////////////////////

	Button menuButtons[4];
	sfVector2f posFirst = { actualConfig.mode.width / 2.0f, 350.0f * ratioY };

	sfVector2f menuButtonsSize = { 620.0f * ratioX, 180.0f * ratioY };

	char* filesDirectories[4] =
	{
		"Ressources/Sprites/UI/Bouton.png",
		"Ressources/Sprites/UI/Bouton.png",
		"Ressources/Sprites/UI/Bouton.png",
		"Ressources/Sprites/UI/Bouton.png"
	};

	for (int i = 0; i < 4; i++)
	{
		menuButtons[i] = InitButton(menuButtonsSize, posFirst, i, filesDirectories[i], true);
	}

	sfVector2f vectorCur = { (posFirst.x) - 600.0f * ratioX, (posFirst.y) - 100.0f  * ratioY };
	Menu menu = CreateGameMenu(vectorCur, menuButtons, 4, 0, false);

	AlignMenuButtons(&menu, 0, posFirst, -20.0f * ratioY, 2);

	menu.classDisplayer.Class = NULL;
	menu.classDisplayer.isOn = sfFalse;
	menu.selectors = (Selector*)malloc(3 * sizeof(Selector));
	menu.selectorsNumber = 3;

	InitSelector(&menu.selectors[0], posFirst, menuButtonsSize, "Ressources/Sprites/UI/Bouton_2.png", 1);
	InitSelector(&menu.selectors[1], posFirst, menuButtonsSize, "Ressources/Sprites/UI/Bouton_2.png", 1);
	InitSelector(&menu.selectors[2], posFirst, menuButtonsSize, "Ressources/Sprites/UI/Bouton_2.png", 1);

	sfVector2f centerPos = CenterInMiddleElements(menu, posFirst, 0, 0, 0, -20.0f * ratioY, 1);
	AlignButtonsAndSelector(&menu, centerPos, 0, 0, 0, -20.0f * ratioY, 1);

	centerPos.y = menu.button[1].rect.top + (menu.button[1].rect.height / 2.0f);
	centerPos.x = actualConfig.mode.width / 2.0f;
	centerPos = CenterInMiddleElements(menu, centerPos, 1, 1, 1, -20.0f * ratioY, 1);
	AlignButtonsAndSelector(&menu, centerPos, 1, 1, 1, -20.0f * ratioY, 1);

	centerPos.y = menu.button[2].rect.top + (menu.button[2].rect.height / 2.0f);
	centerPos.x = actualConfig.mode.width / 2.0f;
	centerPos = CenterInMiddleElements(menu, centerPos, 2, 2, 2, -20.0f * ratioY, 1);
	AlignButtonsAndSelector(&menu, centerPos, 2, 2, 2, -20.0f * ratioY, 1);


	menu.selectors[0].positionText.x = menu.selectors[0].rect.left + (menu.selectors[0].rect.width / 2.0f);
	menu.selectors[0].positionText.y = menu.selectors[0].rect.top + (menu.selectors[0].rect.height / 2.0f) + 10.0f * ratioY;

	menu.selectors[1].positionText.x = menu.selectors[1].rect.left + (menu.selectors[1].rect.width / 2.0f);
	menu.selectors[1].positionText.y = menu.selectors[1].rect.top + (menu.selectors[1].rect.height / 2.0f) + 10.0f * ratioY;

	menu.selectors[2].positionText.x = menu.selectors[2].rect.left + (menu.selectors[2].rect.width / 2.0f);
	menu.selectors[2].positionText.y = menu.selectors[2].rect.top + (menu.selectors[2].rect.height / 2.0f) + 10.0f * ratioY;

	menu.selectors[0].textDisplay.textArray = (sfText**)calloc(actualConfig.numberResPossible, sizeof(sfText*));
	menu.selectors[0].textDisplay.numberOfText = actualConfig.numberResPossible;
	menu.selectors[0].textDisplay.actualTextToDisplay = actualConfig.indexActualRes;

	for (int i = 0; i < actualConfig.numberResPossible; i++)
	{
		char msgToDisplay[50];
		sprintf(msgToDisplay, "%d %s %d", actualConfig.resPossible[i].x, "X", actualConfig.resPossible[i].y);
		menu.selectors[0].textDisplay.textArray[i] = InitText(msgToDisplay, menu.selectors[0].positionText, sfColor_fromRGB(171, 144, 111), (int)(TEXT_SIZE_MENU * (actualConfig.mode.height / 1080.0f)));
		RescaleText(menu.selectors[0].textDisplay.textArray[i], 325.0f, 168.5f, ratioX, ratioY);
		CenterText(menu.selectors[0].textDisplay.textArray[i], menu.selectors[0].positionText);
	}

	menu.selectors[1].textDisplay.numberOfText = 3;
	menu.selectors[1].textDisplay.textArray = (sfText**)calloc(menu.selectors[1].textDisplay.numberOfText, sizeof(sfText*));
	menu.selectors[1].textDisplay.actualTextToDisplay = actualConfig.indexActualWindowMode;

	menu.selectors[1].textDisplay.textArray[0] = InitText(/*"Windowed"*/GetLocalizationString(7), menu.selectors[1].positionText, sfColor_fromRGB(171, 144, 111), (int)(TEXT_SIZE_MENU * (actualConfig.mode.height / 1080.0f)));
	CenterText(menu.selectors[1].textDisplay.textArray[0], menu.selectors[1].positionText);
	menu.selectors[1].textDisplay.textArray[1] = InitText(/*"FullScreen"*/GetLocalizationString(8), menu.selectors[1].positionText, sfColor_fromRGB(171, 144, 111), (int)(TEXT_SIZE_MENU * (actualConfig.mode.height / 1080.0f)));
	CenterText(menu.selectors[1].textDisplay.textArray[1], menu.selectors[1].positionText);
	menu.selectors[1].textDisplay.textArray[2] = InitText(/*"Borderless"*/GetLocalizationString(9), menu.selectors[1].positionText, sfColor_fromRGB(171, 144, 111), (int)(TEXT_SIZE_MENU * (actualConfig.mode.height / 1080.0f)));
	CenterText(menu.selectors[1].textDisplay.textArray[2], menu.selectors[1].positionText);

	for (int i = 0; i < 3; i++)
	{
		RescaleText(menu.selectors[1].textDisplay.textArray[i], 325.0f, 168.5f, ratioX, ratioY);
		CenterText(menu.selectors[1].textDisplay.textArray[i], menu.selectors[1].positionText);
	}

	menu.selectors[2].textDisplay.textArray = (sfText**)calloc(2, sizeof(sfText*));
	menu.selectors[2].textDisplay.numberOfText = 2;
	menu.selectors[2].textDisplay.actualTextToDisplay = actualConfig.dynamicLight;

	menu.selectors[2].textDisplay.textArray[0] = InitText(/*"Desable"*/GetLocalizationString(152), menu.selectors[1].positionText, sfColor_fromRGB(171, 144, 111), (int)(TEXT_SIZE_MENU * (actualConfig.mode.height / 1080.0f)));
	CenterText(menu.selectors[2].textDisplay.textArray[0], menu.selectors[2].positionText);
	menu.selectors[2].textDisplay.textArray[1] = InitText(/*"Enable"*/GetLocalizationString(153), menu.selectors[1].positionText, sfColor_fromRGB(171, 144, 111), (int)(TEXT_SIZE_MENU * (actualConfig.mode.height / 1080.0f)));
	CenterText(menu.selectors[2].textDisplay.textArray[1], menu.selectors[2].positionText);

	for (int i = 0; i < 2; i++)
	{
		RescaleText(menu.selectors[2].textDisplay.textArray[i], 325.0f, 168.5f, ratioX, ratioY);
	}

	menu.selectors[0].isOn = sfFalse;
	menu.selectors[1].isOn = sfFalse;
	menu.selectors[2].isOn = sfFalse;

	menu.selectionCursor.indexDisplay = LoadSprite("Ressources/Sprites/UI/Selecteur.png", 1);
	sfTexture_setSmooth((sfTexture*)sfSprite_getTexture(menu.selectionCursor.indexDisplay), sfTrue);

	sfVector2f scale;
	sfVector2u size = sfTexture_getSize(sfSprite_getTexture(menu.selectionCursor.indexDisplay));
	scale.x = (620.0f * ratioX) / (float)size.x;
	scale.y = (180.0f * ratioY) / (float)size.y;

	sfSprite_setScale(menu.selectionCursor.indexDisplay, scale);

	menu.selectionCursor.indexPos = 0;
	menu.selectionCursor.offset.x = 0.0f * ratioX;
	menu.selectionCursor.offset.y = 150.0f * ratioY;

	sfSprite_setPosition(menu.selectionCursor.indexDisplay, sfSprite_getPosition(menu.button[0].sprite));

	menu.Position = sfSprite_getPosition(menu.button[0].sprite);


	char *PATH[4] = { "Ressources/Sprites/UI/Arrow_Left.png", "Ressources/Sprites/UI/Arrow_Right.png",
		"Ressources/Sprites/UI/Arrow_Left_Selector.png", "Ressources/Sprites/UI/Arrow_Right_Selector.png" };

	InitArrow(menu.selectors[0].Arrows, sfSprite_getPosition(menu.selectors[0].selectorBackground), ratioX, ratioY, menu.selectors[0].rect.width, 1, PATH, 100.0f);
	InitArrow(menu.selectors[1].Arrows, sfSprite_getPosition(menu.selectors[1].selectorBackground), ratioX, ratioY, menu.selectors[1].rect.width, 1, PATH, 100.0f);
	InitArrow(menu.selectors[2].Arrows, sfSprite_getPosition(menu.selectors[2].selectorBackground), ratioX, ratioY, menu.selectors[2].rect.width, 1, PATH, 100.0f);

	InitTextButton(&menu.button[0], /*"Resolution"*/GetLocalizationString(10), sfColor_fromRGB(171, 144, 111), TEXT_SIZE_MENU);
	InitTextButton(&menu.button[1], /*"Window Mode"*/GetLocalizationString(11), sfColor_fromRGB(171, 144, 111), TEXT_SIZE_MENU);
	InitTextButton(&menu.button[2], /*"Dynamic Light"*/GetLocalizationString(143), sfColor_fromRGB(171, 144, 111), TEXT_SIZE_MENU);
	InitTextButton(&menu.button[3], /*"Back"*/GetLocalizationString(6), sfColor_fromRGB(171, 144, 111), TEXT_SIZE_MENU);

	int newSize = (int)(TEXT_SIZE_MENU * (actualConfig.mode.height / 1080.0f));
	for (int i = 0; i < menu.buttonNumber; i++)
	{
		sfText_setCharacterSize(menu.button[i].text, newSize);
		RescaleText(menu.button[i].text, 325.0f, 168.5f, ratioX, ratioY);

		sfVector2f pos = (sfVector2f) { menu.button[i].rect.left + (menu.button[i].rect.width) / 2, menu.button[i].rect.top + (menu.button[i].rect.height) / 2 + 10.0f * ratioY };
		CenterText(menu.button[i].text, pos);
	}

	menu.SliderSelectors = NULL;
	menu.SliderSelectorsNumber = 0;

	return menu;
}

void ResponsiveGraphicsMenu(Menu *menuToUpdate, Config actualConfig)
{
	float ratioX = actualConfig.mode.width / 1920.0f;
	float ratioY = actualConfig.mode.height / 1080.0f;

	sfVector2f posFirst = { actualConfig.mode.width / 2.0f, 350.0f * ratioY };

	sfVector2f menuButtonsSize = { 620.0f * ratioX, 180.0f * ratioY };

	for (int i = 0; i < menuToUpdate->buttonNumber; i++)
	{
		ResponsiveButton(&menuToUpdate->button[i], menuButtonsSize, posFirst, true);
	}

	sfVector2f vectorCur = { (posFirst.x) - 600.0f * ratioX, (posFirst.y) - 100.0f  * ratioY };

	menuToUpdate->Position.x = vectorCur.x;
	menuToUpdate->Position.y = vectorCur.y;

	AlignMenuButtons(menuToUpdate, 0, posFirst, -20.0f * ratioY, 2);

	ResponsiveSelector(&menuToUpdate->selectors[0], posFirst, menuButtonsSize, 1);
	ResponsiveSelector(&menuToUpdate->selectors[1], posFirst, menuButtonsSize, 1);
	ResponsiveSelector(&menuToUpdate->selectors[2], posFirst, menuButtonsSize, 1);

	sfVector2f centerPos = CenterInMiddleElements(*menuToUpdate, posFirst, 0, 0, 0, -20.0f * ratioY, 1);
	AlignButtonsAndSelector(menuToUpdate, centerPos, 0, 0, 0, -20.0f * ratioY, 1);

	centerPos.y = menuToUpdate->button[1].rect.top + (menuToUpdate->button[1].rect.height / 2.0f);
	centerPos.x = actualConfig.mode.width / 2.0f;
	centerPos = CenterInMiddleElements(*menuToUpdate, centerPos, 1, 1, 1, -20.0f * ratioY, 1);
	AlignButtonsAndSelector(menuToUpdate, centerPos, 1, 1, 1, -20.0f * ratioY, 1);

	centerPos.y = menuToUpdate->button[2].rect.top + (menuToUpdate->button[2].rect.height / 2.0f);
	centerPos.x = actualConfig.mode.width / 2.0f;
	centerPos = CenterInMiddleElements(*menuToUpdate, centerPos, 2, 2, 2, -20.0f * ratioY, 1);
	AlignButtonsAndSelector(menuToUpdate, centerPos, 2, 2, 2, -20.0f * ratioY, 1);

	menuToUpdate->selectors[0].positionText.x = menuToUpdate->selectors[0].rect.left + (menuToUpdate->selectors[0].rect.width / 2.0f);
	menuToUpdate->selectors[0].positionText.y = menuToUpdate->selectors[0].rect.top + (menuToUpdate->selectors[0].rect.height / 2.0f) + 10.0f * ratioY;

	for (int i = 0; i < actualConfig.numberResPossible; i++)
	{
		sfText_setCharacterSize(menuToUpdate->selectors[0].textDisplay.textArray[i], (int)(TEXT_SIZE_MENU * (actualConfig.mode.height / 1080.0f)));
		RescaleText(menuToUpdate->selectors[0].textDisplay.textArray[i], 325.0f, 168.5f, ratioX, ratioY);
		CenterText(menuToUpdate->selectors[0].textDisplay.textArray[i], menuToUpdate->selectors[0].positionText);
	}

	menuToUpdate->selectors[1].positionText.x = menuToUpdate->selectors[1].rect.left + (menuToUpdate->selectors[1].rect.width / 2.0f);
	menuToUpdate->selectors[1].positionText.y = menuToUpdate->selectors[1].rect.top + (menuToUpdate->selectors[1].rect.height / 2.0f) + 10.0f * ratioY;

	sfText_setString(menuToUpdate->selectors[1].textDisplay.textArray[0],/*Windowed*/ GetLocalizationString(7));
	sfText_setString(menuToUpdate->selectors[1].textDisplay.textArray[1], /*fullscreen*/GetLocalizationString(8));
	sfText_setString(menuToUpdate->selectors[1].textDisplay.textArray[2], /*borderless*/GetLocalizationString(9));
	for (int i = 0; i < 3; i++)
	{
		sfText_setCharacterSize(menuToUpdate->selectors[1].textDisplay.textArray[i], (int)(TEXT_SIZE_MENU * (actualConfig.mode.height / 1080.0f)));
		RescaleText(menuToUpdate->selectors[1].textDisplay.textArray[i], 325.0f, 168.5f, ratioX, ratioY);
		CenterText(menuToUpdate->selectors[1].textDisplay.textArray[i], menuToUpdate->selectors[1].positionText);
	}

	menuToUpdate->selectors[2].positionText.x = menuToUpdate->selectors[2].rect.left + (menuToUpdate->selectors[2].rect.width / 2.0f);
	menuToUpdate->selectors[2].positionText.y = menuToUpdate->selectors[2].rect.top + (menuToUpdate->selectors[2].rect.height / 2.0f) + 10.0f * ratioY;

	sfText_setString(menuToUpdate->selectors[2].textDisplay.textArray[0],/*Disable*/ GetLocalizationString(152));
	sfText_setString(menuToUpdate->selectors[2].textDisplay.textArray[1], /*Enable*/GetLocalizationString(153));

	for (int i = 0; i < 2; i++)
	{
		sfText_setCharacterSize(menuToUpdate->selectors[2].textDisplay.textArray[i], (int)(TEXT_SIZE_MENU * (actualConfig.mode.height / 1080.0f)));
		RescaleText(menuToUpdate->selectors[2].textDisplay.textArray[i], 325.0f, 168.5f, ratioX, ratioY);
		CenterText(menuToUpdate->selectors[2].textDisplay.textArray[i], menuToUpdate->selectors[2].positionText);
	}

	int newSize = (int)(TEXT_SIZE_MENU * (actualConfig.mode.height / 1080.0f));
	SetStringButton(&menuToUpdate->button[0], /*Resolution*/GetLocalizationString(10));
	SetStringButton(&menuToUpdate->button[1], /*window mode*/GetLocalizationString(11));
	SetStringButton(&menuToUpdate->button[2], /*dynamic light*/GetLocalizationString(143));
	SetStringButton(&menuToUpdate->button[3], /*back*/GetLocalizationString(6));
	for (int i = 0; i < menuToUpdate->buttonNumber; i++)
	{
		sfText_setCharacterSize(menuToUpdate->button[i].text, newSize);
		RescaleText(menuToUpdate->button[i].text, 325.0f, 168.5f, ratioX, ratioY);


		sfVector2f pos = (sfVector2f) { menuToUpdate->button[i].rect.left + (menuToUpdate->button[i].rect.width) / 2, menuToUpdate->button[i].rect.top + (menuToUpdate->button[i].rect.height) / 2 + 10.0f * ratioY };
		CenterText(menuToUpdate->button[i].text, pos);
	}

	sfVector2f scale;
	sfVector2u size = sfTexture_getSize(sfSprite_getTexture(menuToUpdate->selectionCursor.indexDisplay));
	scale.x = (620.0f * ratioX) / (float)size.x;
	scale.y = (180.0f * ratioY) / (float)size.y;

	sfSprite_setScale(menuToUpdate->selectionCursor.indexDisplay, scale);
	sfSprite_setPosition(menuToUpdate->selectionCursor.indexDisplay, sfSprite_getPosition(menuToUpdate->button[0].sprite));

	menuToUpdate->Position = sfSprite_getPosition(menuToUpdate->button[0].sprite);

	menuToUpdate->selectionCursor.offset.x = 0.0f * ratioX;
	menuToUpdate->selectionCursor.offset.y = 150.0f * ratioY;

	sfVector2f newPos = { menuToUpdate->Position.x + menuToUpdate->selectionCursor.indexPos * menuToUpdate->selectionCursor.offset.x,
		menuToUpdate->Position.y + menuToUpdate->selectionCursor.indexPos * menuToUpdate->selectionCursor.offset.y };
	sfSprite_setPosition(menuToUpdate->selectionCursor.indexDisplay, newPos);
}
