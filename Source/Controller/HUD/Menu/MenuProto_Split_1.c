#include "MenuProto_Split_1.h"
#include "..\..\Serialization\Serialization.h"
#include "..\..\LeJeu\View\MapKey\MapKey.h"
#include "..\..\..\View\ViewManager.h"

Menu CreatePauseMenuProto(sfVideoMode mode)
{
	float ratioX = mode.width / 1920.0f;
	float ratioY = mode.height / 1080.0f;

	sfVector2f posFirst = { mode.width / 2.0f, mode.height / 2.0f - 250.0f * ratioY };

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

	sfVector2f vectorCur = { (posFirst.x) - 600.0f * ratioX, (posFirst.y) - 100.0f  * ratioY };

	Menu pause = CreateGameMenu(vectorCur, MainMenuButtons, 4, 0, false);

	pause.selectionCursor.indexDisplay = LoadSprite("Ressources/Sprites/UI/Selecteur.png", 1);
	sfVector2f scale;
	sfVector2u size = sfTexture_getSize(sfSprite_getTexture(pause.selectionCursor.indexDisplay));
	scale.x = (620.0f * ratioX) / (float)size.x;
	scale.y = (180.0f * ratioY) / (float)size.y;

	sfSprite_setScale(pause.selectionCursor.indexDisplay, scale);

	pause.selectionCursor.indexPos = 0;
	pause.selectionCursor.offset.x = 0.0f * ratioX;
	pause.selectionCursor.offset.y = 150.0f * ratioY;

	AlignMenuButtons(&pause, 0, posFirst, -20.0f * ratioY, 2);
	sfSprite_setPosition(pause.selectionCursor.indexDisplay, sfSprite_getPosition(pause.button[0].sprite));
	pause.Position = sfSprite_getPosition(pause.button[0].sprite);

	pause.selectors = NULL;
	pause.selectorsNumber = 0;
	pause.classDisplayer.Class = NULL;
	pause.classDisplayer.isOn = sfFalse;
	pause.SliderSelectors = NULL;
	pause.SliderSelectorsNumber = 0;

	InitTextButton(&pause.button[0], /*"Continue"*/GetLocalizationString(14), sfColor_fromRGB(171, 144, 111), TEXT_SIZE_MENU);
	InitTextButton(&pause.button[1], /*"Save"*/GetLocalizationString(144), sfColor_fromRGB(171, 144, 111), TEXT_SIZE_MENU);
	InitTextButton(&pause.button[2], /*"Options"*/GetLocalizationString(2), sfColor_fromRGB(171, 144, 111), TEXT_SIZE_MENU);
	InitTextButton(&pause.button[3], /*"Return to menu"*/GetLocalizationString(15), sfColor_fromRGB(171, 144, 111), TEXT_SIZE_MENU);

	int newSize = (int)(TEXT_SIZE_MENU * (mode.height / 1080.0f));
	for (int i = 0; i < pause.buttonNumber; i++)
	{
		pause.button[i].text2 = NULL;
		sfText_setCharacterSize(pause.button[i].text, newSize);
		RescaleText(pause.button[i].text, 325.0f, 168.5f, ratioX, ratioY);
		sfVector2f pos = (sfVector2f) { pause.button[i].rect.left + (pause.button[i].rect.width) / 2, pause.button[i].rect.top + (pause.button[i].rect.height) / 2 + 10.0f * ratioY };
		CenterText(pause.button[i].text, pos);
	}

	return pause;
}

void ResponsivePauseMenuProto(Menu *menuToUpdate, sfVideoMode mode)
{
	float ratioX = mode.width / 1920.0f;
	float ratioY = mode.height / 1080.0f;

	sfVector2f posFirst = { mode.width / 2.0f, mode.height / 2.0f - 250.0f * ratioY };
	sfVector2f MainMenuButtonsPos = { 400.0f * ratioX, 400.0f * ratioY };
	sfVector2f MainMenuButtonsSize = { 620.0f * ratioX, 180.0f * ratioY };


	for (int i = 0; i < menuToUpdate->buttonNumber; i++)
	{
		ResponsiveButton(&menuToUpdate->button[i], MainMenuButtonsSize, MainMenuButtonsPos, true);
	}

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

	AlignMenuButtons(menuToUpdate, 0, posFirst, -20.0f * ratioY, 2);

	sfSprite_setPosition(menuToUpdate->selectionCursor.indexDisplay, sfSprite_getPosition(menuToUpdate->button[0].sprite));

	menuToUpdate->Position = sfSprite_getPosition(menuToUpdate->button[0].sprite);

	int newSize = (int)(TEXT_SIZE_MENU * (mode.height / 1080.0f));
	SetStringButton(&menuToUpdate->button[0], /*continue*/GetLocalizationString(14));
	SetStringButton(&menuToUpdate->button[1], /*option*/GetLocalizationString(144));
	SetStringButton(&menuToUpdate->button[2], /*option*/GetLocalizationString(2));
	SetStringButton(&menuToUpdate->button[3], /*return to menu*/GetLocalizationString(15));
	for (int i = 0; i < menuToUpdate->buttonNumber; i++)
	{
		sfText_setCharacterSize(menuToUpdate->button[i].text, newSize);
		RescaleText(menuToUpdate->button[i].text, 325.0f, 168.5f, ratioX, ratioY);
		sfVector2f pos = (sfVector2f) { menuToUpdate->button[i].rect.left + (menuToUpdate->button[i].rect.width) / 2, menuToUpdate->button[i].rect.top + (menuToUpdate->button[i].rect.height) / 2 + 10.0f * ratioY };
		CenterText(menuToUpdate->button[i].text, pos);
	}
}

Menu CreateSoundOptionProto(Config actualConfig)
{
	float ratioX = actualConfig.mode.width / 1920.0f;
	float ratioY = actualConfig.mode.height / 1080.0f;
	///////////////////////////////////////////////////////////////////////////////////////////////////

	Button menuButtons[1];
	sfVector2f posFirst = { actualConfig.mode.width / 2.0f, 350.0f * ratioY };

	sfVector2f menuButtonsSize = { 620.0f * ratioX, 180.0f * ratioY };

	sfVector2f MainMenuButtonsPos = { 400.0f * ratioX, 400.0f * ratioY };
	sfVector2f MainMenuButtonsSize = { 620.0f * ratioX, 180.0f * ratioY };
	char* filesDirectories = "Ressources/Sprites/UI/Bouton.png";

	for (int i = 0; i < 1; i++)
	{
		menuButtons[i] = InitButton(MainMenuButtonsSize, MainMenuButtonsPos, i, filesDirectories, true);
	}

	sfVector2f vectorCur = { (posFirst.x) - 600.0f * ratioX, (posFirst.y) - 100.0f  * ratioY };
	Menu menu = CreateGameMenu(vectorCur, menuButtons, 1, 0, false);

	AlignMenuButtons(&menu, 0, posFirst, 30.0f * ratioY, 1);

	menu.selectionCursor.indexDisplay = LoadSprite("Ressources/Sprites/UI/Selecteur.png", 1);
	sfVector2f scale;
	sfVector2u size = sfTexture_getSize(sfSprite_getTexture(menu.selectionCursor.indexDisplay));
	scale.x = (620.0f * ratioX) / (float)size.x;
	scale.y = (180.0f * ratioY) / (float)size.y;

	sfSprite_setScale(menu.selectionCursor.indexDisplay, scale);

	menu.selectionCursor.indexPos = 0;
	menu.selectionCursor.offset.x = 0.0f * ratioX;
	menu.selectionCursor.offset.y = 150.0f * ratioY;

	menu.SliderCursor = LoadSprite("Ressources/Sprites/UI/Slice_Selector.png", 1);

	sfSprite_setPosition(menu.selectionCursor.indexDisplay, sfSprite_getPosition(menu.button[0].sprite));
	sfSprite_setPosition(menu.SliderCursor, sfSprite_getPosition(menu.button[0].sprite));

	menu.Position = sfSprite_getPosition(menu.button[0].sprite);

	menu.SliderSelectorsNumber = 3;
	menu.SliderSelectors = (SliderSelector*)calloc(menu.SliderSelectorsNumber, sizeof(SliderSelector));

	menu.SliderSelectors[0].valueOfSlider = GetVolume(0);
	menu.SliderSelectors[1].valueOfSlider = GetVolume(1);
	menu.SliderSelectors[2].valueOfSlider = GetVolume(2);

	menu.SliderSelectors[0].isSelected = sfFalse;
	menu.SliderSelectors[1].isSelected = sfFalse;
	menu.SliderSelectors[2].isSelected = sfFalse;

	sfVector2f sizeSliderSelector = { 600.0f * ratioX, 90.0f * ratioY };

	InitSliderSelector(&menu.SliderSelectors[0], (sfVector2f) { posFirst.x, posFirst.y }, sizeSliderSelector, "Ressources/Sprites/UI/Slice.png", "Ressources/Sprites/UI/Arrow_Left.png", "Ressources/Sprites/UI/Arrow_Right.png",
		"Ressources/Sprites/UI/Arrow_Left_Selector.png", "Ressources/Sprites/UI/Arrow_Right_Selector.png", 1, ratioX, ratioY);
	InitSliderSelector(&menu.SliderSelectors[1], (sfVector2f) { posFirst.x, posFirst.y + 150.0f * ratioY }, sizeSliderSelector, "Ressources/Sprites/UI/Slice.png", "Ressources/Sprites/UI/Arrow_Left.png", "Ressources/Sprites/UI/Arrow_Right.png",
		"Ressources/Sprites/UI/Arrow_Left_Selector.png", "Ressources/Sprites/UI/Arrow_Right_Selector.png", 1, ratioX, ratioY);
	InitSliderSelector(&menu.SliderSelectors[2], (sfVector2f) { posFirst.x, posFirst.y + 300.0f * ratioY }, sizeSliderSelector, "Ressources/Sprites/UI/Slice.png", "Ressources/Sprites/UI/Arrow_Left.png", "Ressources/Sprites/UI/Arrow_Right.png",
		"Ressources/Sprites/UI/Arrow_Left_Selector.png", "Ressources/Sprites/UI/Arrow_Right_Selector.png", 1, ratioX, ratioY);

	sfSprite_setPosition(menuButtons[0].sprite, (sfVector2f) { posFirst.x, posFirst.y + 450.0f * ratioY });
	menu.button[0].rect = sfSprite_getGlobalBounds(menuButtons[0].sprite);

	menu.SliderSelectors[0].ValueOfSlider = InitText("", (sfVector2f) { 0.0f, 0.0f }, sfColor_fromRGB(54, 37, 33), TEXT_SIZE_MENU);
	CenterText(menu.SliderSelectors[0].ValueOfSlider, (sfVector2f) { posFirst.x, posFirst.y - 20.0f * ratioY });
	menu.SliderSelectors[1].ValueOfSlider = InitText("", (sfVector2f) { 0.0f, 0.0f }, sfColor_fromRGB(54, 37, 33), TEXT_SIZE_MENU);
	CenterText(menu.SliderSelectors[1].ValueOfSlider, (sfVector2f) { posFirst.x, posFirst.y + 165.0f * ratioY });
	menu.SliderSelectors[2].ValueOfSlider = InitText("", (sfVector2f) { 0.0f, 0.0f }, sfColor_fromRGB(54, 37, 33), TEXT_SIZE_MENU);
	CenterText(menu.SliderSelectors[2].ValueOfSlider, (sfVector2f) { posFirst.x, posFirst.y + 370.0f * ratioY });

	menu.SliderSelectors[0].NameOfSlider = InitText(GetLocalizationString(154), (sfVector2f) { 0.0f, 0.0f }, sfColor_fromRGB(54, 37, 33), TEXT_SIZE_MENU);
	CenterText(menu.SliderSelectors[0].NameOfSlider, (sfVector2f) { posFirst.x, posFirst.y - 55.0f * ratioY });
	menu.SliderSelectors[1].NameOfSlider = InitText(GetLocalizationString(155), (sfVector2f) { 0.0f, 0.0f }, sfColor_fromRGB(54, 37, 33), TEXT_SIZE_MENU);
	CenterText(menu.SliderSelectors[1].NameOfSlider, (sfVector2f) { posFirst.x, posFirst.y + 95.0f * ratioY });
	menu.SliderSelectors[2].NameOfSlider = InitText(GetLocalizationString(156), (sfVector2f) { 0.0f, 0.0f }, sfColor_fromRGB(54, 37, 33), TEXT_SIZE_MENU);
	CenterText(menu.SliderSelectors[2].NameOfSlider, (sfVector2f) { posFirst.x, posFirst.y + 245.0f * ratioY });

	UpdateTextDisplay(menu.SliderSelectors[0].ValueOfSlider, (int)menu.SliderSelectors[0].valueOfSlider);
	UpdateTextDisplay(menu.SliderSelectors[1].ValueOfSlider, (int)menu.SliderSelectors[1].valueOfSlider);
	UpdateTextDisplay(menu.SliderSelectors[2].ValueOfSlider, (int)menu.SliderSelectors[2].valueOfSlider);

	size = sfTexture_getSize(sfSprite_getTexture(menu.SliderCursor));
	scale.x = (600.0f * ratioX) / (float)size.x;
	scale.y = (90.0f * ratioY) / (float)size.y;

	sfSprite_setScale(menu.SliderCursor, scale);

	InitTextButton(&menu.button[0], /*"Back"*/GetLocalizationString(6), sfColor_fromRGB(171, 144, 111), TEXT_SIZE_MENU);

	int newSize = (int)(TEXT_SIZE_MENU * (actualConfig.mode.height / 1080.0f));

	for (int i = 0; i < menu.buttonNumber; i++)
	{
		menu.button[i].text2 = NULL;
		sfText_setCharacterSize(menu.button[i].text, newSize);
		RescaleText(menu.button[i].text, 325.0f, (90.0f * 0.6f), ratioX, ratioY);
		sfVector2f pos = (sfVector2f) { menu.button[i].rect.left + (menu.button[i].rect.width) / 2.0f, menu.button[i].rect.top + (menu.button[i].rect.height) / 2.0f + 10.0f * ratioY };
		CenterText(menu.button[i].text, pos);
	}

	for (int i = 0; i < menu.SliderSelectorsNumber; i++)
	{
		sfText_setCharacterSize(menu.SliderSelectors[i].ValueOfSlider, newSize);
		sfText_setCharacterSize(menu.SliderSelectors[i].NameOfSlider, newSize);
		RescaleText(menu.SliderSelectors[i].ValueOfSlider, 325.0f, (90.0f * 0.7f), ratioX, ratioY);
		RescaleText(menu.SliderSelectors[i].NameOfSlider, 325.0f, (90.0f * 0.7f), ratioX, ratioY);
		CenterText(menu.SliderSelectors[i].ValueOfSlider, menu.SliderSelectors[i].Pos);
		sfVector2f tempPos = menu.SliderSelectors[i].Pos;
		tempPos.y -= 60.0f * ratioY;
		CenterText(menu.SliderSelectors[i].NameOfSlider, tempPos);
	}

	menu.selectors = NULL;
	menu.selectorsNumber = 0;
	menu.classDisplayer.Class = NULL;
	menu.classDisplayer.isOn = sfFalse;

	return (menu);
}

void ResponsiveSoundOptionProto(Menu *menuToUpdate, sfVideoMode mode)
{
	float ratioX = mode.width / 1920.0f;
	float ratioY = mode.height / 1080.0f;
	///////////////////////////////////////////////////////////////////////////////////////////////////

	sfVector2f posFirst = { mode.width / 2.0f, 350.0f * ratioY };

	sfVector2f menuButtonsSize = { 620.0f * ratioX, 180.0f * ratioY };

	sfVector2f MainMenuButtonsPos = { 400.0f * ratioX, 400.0f * ratioY };
	sfVector2f MainMenuButtonsSize = { 620.0f * ratioX, 180.0f * ratioY };

	for (int i = 0; i < menuToUpdate->buttonNumber; i++)
	{
		ResponsiveButton(&menuToUpdate->button[i], MainMenuButtonsSize, MainMenuButtonsPos, true);
	}

	sfVector2f vectorCur = { (posFirst.x) - 600.0f * ratioX, (posFirst.y) - 100.0f  * ratioY };

	menuToUpdate->Position.x = vectorCur.x;
	menuToUpdate->Position.y = vectorCur.y;


	AlignMenuButtons(menuToUpdate, 0, posFirst, 30.0f * ratioY, 1);

	sfVector2f scale;
	sfVector2u size = sfTexture_getSize(sfSprite_getTexture(menuToUpdate->selectionCursor.indexDisplay));
	scale.x = (620.0f * ratioX) / (float)size.x;
	scale.y = (180.0f * ratioY) / (float)size.y;

	sfSprite_setScale(menuToUpdate->selectionCursor.indexDisplay, scale);

	menuToUpdate->selectionCursor.indexPos = 0;
	menuToUpdate->selectionCursor.offset.x = 0.0f * ratioX;
	menuToUpdate->selectionCursor.offset.y = 150.0f * ratioY;

	sfSprite_setPosition(menuToUpdate->selectionCursor.indexDisplay, sfSprite_getPosition(menuToUpdate->button[0].sprite));
	sfSprite_setPosition(menuToUpdate->SliderCursor, sfSprite_getPosition(menuToUpdate->button[0].sprite));

	sfVector2f sizeSliderSelector = { 600.0f * ratioX, 90.0f * ratioY };

	sfSprite_setPosition(menuToUpdate->button[0].sprite, (sfVector2f) { posFirst.x, posFirst.y + 450.0f * ratioY });
	menuToUpdate->button[0].rect = sfSprite_getGlobalBounds(menuToUpdate->button[0].sprite);

	CenterText(menuToUpdate->SliderSelectors[0].ValueOfSlider, (sfVector2f) { posFirst.x, posFirst.y - 20.0f * ratioY });
	CenterText(menuToUpdate->SliderSelectors[1].ValueOfSlider, (sfVector2f) { posFirst.x, posFirst.y + 165.0f * ratioY });
	CenterText(menuToUpdate->SliderSelectors[2].ValueOfSlider, (sfVector2f) { posFirst.x, posFirst.y + 370.0f * ratioY });

	CenterText(menuToUpdate->SliderSelectors[0].NameOfSlider, (sfVector2f) { posFirst.x, posFirst.y - 55.0f * ratioY });
	CenterText(menuToUpdate->SliderSelectors[1].NameOfSlider, (sfVector2f) { posFirst.x, posFirst.y + 95.0f * ratioY });
	CenterText(menuToUpdate->SliderSelectors[2].NameOfSlider, (sfVector2f) { posFirst.x, posFirst.y + 245.0f * ratioY });

	size = sfTexture_getSize(sfSprite_getTexture(menuToUpdate->SliderCursor));
	scale.x = (600.0f * ratioX) / (float)size.x;
	scale.y = (90.0f * ratioY) / (float)size.y;

	sfSprite_setScale(menuToUpdate->SliderCursor, scale);

	sfText_setString(menuToUpdate->button[0].text, /*"Back"*/GetLocalizationString(6));

	int newSize = (int)(TEXT_SIZE_MENU * (mode.height / 1080.0f));

	for (int i = 0; i < menuToUpdate->buttonNumber; i++)
	{
		sfText_setCharacterSize(menuToUpdate->button[i].text, newSize);
		RescaleText(menuToUpdate->button[i].text, 325.0f, (90.0f * 0.6f), ratioX, ratioY);
		sfVector2f pos = (sfVector2f) { menuToUpdate->button[i].rect.left + (menuToUpdate->button[i].rect.width) / 2.0f, menuToUpdate->button[i].rect.top + (menuToUpdate->button[i].rect.height) / 2.0f + 10.0f * ratioY };
		CenterText(menuToUpdate->button[i].text, pos);
	}

	ResponsiveSliderSelector(&menuToUpdate->SliderSelectors[0], (sfVector2f) { posFirst.x, posFirst.y }, sizeSliderSelector, ratioX, ratioY, 1);
	ResponsiveSliderSelector(&menuToUpdate->SliderSelectors[1], (sfVector2f) { posFirst.x, posFirst.y + 150.0f * ratioY }, sizeSliderSelector, ratioX, ratioY, 1);
	ResponsiveSliderSelector(&menuToUpdate->SliderSelectors[2], (sfVector2f) { posFirst.x, posFirst.y + 300.0f * ratioY }, sizeSliderSelector, ratioX, ratioY, 1);

	sfText_setString(menuToUpdate->SliderSelectors[0].NameOfSlider, GetLocalizationString(154));
	sfText_setString(menuToUpdate->SliderSelectors[1].NameOfSlider, GetLocalizationString(155));
	sfText_setString(menuToUpdate->SliderSelectors[2].NameOfSlider, GetLocalizationString(156));
	CenterText(menuToUpdate->SliderSelectors[0].NameOfSlider, (sfVector2f) { posFirst.x, posFirst.y - 55.0f * ratioY });
	CenterText(menuToUpdate->SliderSelectors[1].NameOfSlider, (sfVector2f) { posFirst.x, posFirst.y + 95.0f * ratioY });
	CenterText(menuToUpdate->SliderSelectors[2].NameOfSlider, (sfVector2f) { posFirst.x, posFirst.y + 245.0f  * ratioY });

	for (int i = 0; i < menuToUpdate->SliderSelectorsNumber; i++)
	{
		sfText_setCharacterSize(menuToUpdate->SliderSelectors[i].ValueOfSlider, newSize);
		sfText_setCharacterSize(menuToUpdate->SliderSelectors[i].NameOfSlider, newSize);
		RescaleText(menuToUpdate->SliderSelectors[i].ValueOfSlider, 325.0f, (90.0f * 0.7f), ratioX, ratioY);
		RescaleText(menuToUpdate->SliderSelectors[i].NameOfSlider, 325.0f, (90.0f * 0.7f), ratioX, ratioY);
		CenterText(menuToUpdate->SliderSelectors[i].ValueOfSlider, menuToUpdate->SliderSelectors[i].Pos);
		sfVector2f tempPos = menuToUpdate->SliderSelectors[i].Pos;
		tempPos.y -= 60.0f * ratioY;
		CenterText(menuToUpdate->SliderSelectors[i].NameOfSlider, tempPos);
	}
}

Menu CreateOptionPauseMenuProto(sfVideoMode mode)
{
	float ratioX = mode.width / 1920.0f;
	float ratioY = mode.height / 1080.0f;
	Button selectOptionButtons[3];
	sfVector2f selectOptionButtonsPos = { mode.width / 2.0f,  mode.height / 2.0f - 100.f * ratioY };
	sfVector2f selectOptionButtonsSize = { 620.0f * ratioX, 180.0f * ratioY };
	char* filesDirectories[3] =
	{
		"Ressources/Sprites/UI/Bouton.png",
		"Ressources/Sprites/UI/Bouton.png",
		"Ressources/Sprites/UI/Bouton.png",
	};

	for (int i = 0; i < 3; i++)
	{
		selectOptionButtons[i] = InitButton(selectOptionButtonsSize, selectOptionButtonsPos, i, filesDirectories[i], true);
	}
	sfVector2f posFirst = { mode.width / 2.0f,  mode.height / 2.0f - 100.f * ratioY };
	sfVector2f vectorCur = { (posFirst.x) - 450.0f * ratioX, (posFirst.y) - 100.0f  * ratioY };

	Menu selectOptionMenu = CreateGameMenu(vectorCur, selectOptionButtons, 3, 0, false);


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
	InitTextButton(&selectOptionMenu.button[1], /*"Sounds"*/GetLocalizationString(22), sfColor_fromRGB(171, 144, 111), TEXT_SIZE_MENU);
	InitTextButton(&selectOptionMenu.button[2], /*"Back"*/GetLocalizationString(6), sfColor_fromRGB(171, 144, 111), TEXT_SIZE_MENU);

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

void ResponsiveOptionPauseMenu(Menu *menuToUpdate, sfVideoMode mode)
{
	float ratioX = mode.width / 1920.0f;
	float ratioY = mode.height / 1080.0f;

	sfVector2f selectOptionButtonsPos = { mode.width / 2.f, mode.height / 2.f - 100.f * ratioY };
	sfVector2f selectOptionButtonsSize = { 620.0f * ratioX, 180.0f * ratioY };

	for (int i = 0; i < menuToUpdate->buttonNumber; i++)
	{
		ResponsiveButton(&menuToUpdate->button[i], selectOptionButtonsSize, selectOptionButtonsPos, true);
	}

	sfVector2f posFirst = { mode.width / 2.0f,  mode.height / 2.0f - 100.f * ratioY };

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
	SetStringButton(&menuToUpdate->button[1], /*Sounds*/GetLocalizationString(22));
	SetStringButton(&menuToUpdate->button[2], /*back*/GetLocalizationString(6));
	for (int i = 0; i < menuToUpdate->buttonNumber; i++)
	{
		sfText_setCharacterSize(menuToUpdate->button[i].text, newSize);
		RescaleText(menuToUpdate->button[i].text, 325.0f, 168.5f, ratioX, ratioY);
		sfVector2f pos = (sfVector2f) { menuToUpdate->button[i].rect.left + (menuToUpdate->button[i].rect.width) / 2, menuToUpdate->button[i].rect.top + (menuToUpdate->button[i].rect.height) / 2 + 10.0f * ratioY };
		CenterText(menuToUpdate->button[i].text, pos);
	}
}

Menu CreateSavingMenu(sfVideoMode mode)
{
	float ratioX = mode.width / 1920.0f;
	float ratioY = mode.height / 1080.0f;
	Button SavingMenuButtons[4];
	sfVector2f SavingMenuButtonsPos = { 400.0f * ratioX, 400.0f * ratioY };
	sfVector2f SavingMenuButtonsSize = { 620.0f * ratioX, 200.0f * ratioY };
	char* filesDirectories[4] =
	{
		"Ressources/Sprites/UI/Bouton.png",
		"Ressources/Sprites/UI/Bouton.png",
		"Ressources/Sprites/UI/Bouton.png",
		"Ressources/Sprites/UI/Bouton.png"
	};

	for (int i = 0; i < 4; i++)
	{
		SavingMenuButtons[i] = InitButton(SavingMenuButtonsSize, SavingMenuButtonsPos, i, filesDirectories[i], true);
	}

	sfVector2f posFirst = { mode.width / 2.0f, mode.height / 2.0f - 100.0f };

	sfVector2f vectorCur = { (posFirst.x) - 600.0f * ratioX, (posFirst.y) - 100.0f  * ratioY };
	Menu SavingMenu = CreateGameMenu(vectorCur, SavingMenuButtons, 4, 0, false);

	SavingMenu.selectionCursor.indexDisplay = LoadSprite("Ressources/Sprites/UI/Selecteur.png", 1);
	sfVector2f scale;
	sfVector2u size = sfTexture_getSize(sfSprite_getTexture(SavingMenu.selectionCursor.indexDisplay));
	scale.x = (630.0f * ratioX) / (float)size.x;
	scale.y = (200.0f * ratioY) / (float)size.y;

	sfSprite_setScale(SavingMenu.selectionCursor.indexDisplay, scale);

	SavingMenu.selectionCursor.indexPos = 0;
	SavingMenu.selectionCursor.offset.x = 0.0f * ratioX;
	SavingMenu.selectionCursor.offset.y = 150.0f * ratioY;

	AlignMenuButtons(&SavingMenu, 0, posFirst, -20.0f * ratioY, 2);
	sfSprite_setPosition(SavingMenu.selectionCursor.indexDisplay, sfSprite_getPosition(SavingMenu.button[0].sprite));

	SavingMenu.Position = sfSprite_getPosition(SavingMenu.button[0].sprite);

	SavingMenu.selectors = NULL;
	SavingMenu.selectorsNumber = 0;
	SavingMenu.classDisplayer.Class = NULL;
	SavingMenu.classDisplayer.isOn = sfFalse;

	SavingMenu.SliderSelectors = NULL;
	SavingMenu.SliderSelectorsNumber = 0;

	for (int i = 0; i < 3; i++)
	{
		char save[50];
		sprintf(save, "manualSave%d", i + 1);
		int length = (int)strlen(save);
		sfVector2f pos = (sfVector2f) { 0.0f, 0.0f };
		SavingMenu.button[i].text = InitText(save, pos, sfColor_fromRGB(171, 144, 111), TEXT_SIZE_MENU);
		SavingMenu.button[i].text2 = InitText(ChangeSaveNameButton(1, i + 1), pos, sfColor_fromRGB(171, 144, 111), TEXT_SIZE_MENU);
	}

	//InitTextButton(&SavingMenu.button[1], /*"2 joueur"*/ChangeSaveNameButton(1, 2), sfColor_fromRGB(54, 37, 33), TEXT_SIZE_MENU);
	//InitTextButton(&SavingMenu.button[2], /*"2 joueur"*/ChangeSaveNameButton(1, 3), sfColor_fromRGB(54, 37, 33), TEXT_SIZE_MENU);
	InitTextButton(&SavingMenu.button[3], /*"Back"*/GetLocalizationString(6), sfColor_fromRGB(171, 144, 111), TEXT_SIZE_MENU);
	SavingMenu.button[3].text2 = NULL;


	int newSize = (int)(TEXT_SIZE_MENU * (mode.height / 1080.0f)) - 10;
	//sfVector2f pos;
	for (int i = 0; i < SavingMenu.buttonNumber; i++)
	{
		if (i != SavingMenu.buttonNumber - 1)
		{
			sfText_setCharacterSize(SavingMenu.button[i].text, newSize);
			RescaleText(SavingMenu.button[i].text, 325.0f, 50.f, ratioX, ratioY);
			sfVector2f pos = (sfVector2f) { SavingMenu.button[i].rect.left + (SavingMenu.button[i].rect.width) / 2, SavingMenu.button[i].rect.top + (SavingMenu.button[i].rect.height) / 2 - 30.f * ratioY };
			CenterText(SavingMenu.button[i].text, pos);
			//sfText_setPosition(SavingMenu.button[0].text, pos2);
			if (SavingMenu.button[i].text2 != NULL)
			{
				sfText_setCharacterSize(SavingMenu.button[i].text2, newSize);
				RescaleText(SavingMenu.button[i].text2, 325.0f, 50.f, ratioX, ratioY);
				pos = (sfVector2f) { SavingMenu.button[i].rect.left + (SavingMenu.button[i].rect.width) / 2, SavingMenu.button[i].rect.top + (SavingMenu.button[i].rect.height) / 2 + 20.f * ratioY };
				CenterText(SavingMenu.button[i].text2, pos);
			}
		}
		else
		{
			sfText_setCharacterSize(SavingMenu.button[i].text, newSize);
			RescaleText(SavingMenu.button[i].text, 325.0f, 50.f, ratioX, ratioY);
			sfVector2f pos = (sfVector2f) { SavingMenu.button[i].rect.left + (SavingMenu.button[i].rect.width) / 2, SavingMenu.button[i].rect.top + (SavingMenu.button[i].rect.height) / 2 + 10.0f * ratioY };
			CenterText(SavingMenu.button[i].text, pos);
		}
	}

	return (SavingMenu);
}

void ResponsiveSavingMenu(Menu *menuToUpdate, sfVideoMode mode)
{
	float ratioX = mode.width / 1920.0f;
	float ratioY = mode.height / 1080.0f;


	sfVector2f MainMenuButtonsPos = { 400.0f * ratioX, 400.0f * ratioY };
	sfVector2f MainMenuButtonsSize = { 620.0f * ratioX, 120.0f * ratioY };


	for (int i = 0; i < menuToUpdate->buttonNumber; i++)
	{
		ResponsiveButton(&menuToUpdate->button[i], MainMenuButtonsSize, MainMenuButtonsPos, true);
	}

	sfVector2f posFirst = { mode.width / 2.0f, mode.height / 2.0f - 100.0f };

	sfVector2f vectorCur = { (posFirst.x) - 600.0f * ratioX, (posFirst.y) - 100.0f  * ratioY };

	menuToUpdate->Position.x = vectorCur.x;
	menuToUpdate->Position.y = vectorCur.y;

	sfVector2f scale;
	sfVector2u size = sfTexture_getSize(sfSprite_getTexture(menuToUpdate->selectionCursor.indexDisplay));
	scale.x = (620.0f * ratioX) / (float)size.x;
	scale.y = (120.0f * ratioY) / (float)size.y;

	sfSprite_setScale(menuToUpdate->selectionCursor.indexDisplay, scale);
	sfSprite_setPosition(menuToUpdate->selectionCursor.indexDisplay, vectorCur);

	menuToUpdate->selectionCursor.offset.x = 0.0f * ratioX;
	menuToUpdate->selectionCursor.offset.y = 150.0f * ratioY;

	AlignMenuButtons(menuToUpdate, 0, posFirst, -20.0f * ratioY, 2);

	sfSprite_setPosition(menuToUpdate->selectionCursor.indexDisplay, sfSprite_getPosition(menuToUpdate->button[0].sprite));
	menuToUpdate->Position = sfSprite_getPosition(menuToUpdate->button[0].sprite);


	int newSize = (int)(TEXT_SIZE_MENU * (mode.height / 1080.0f)) - 5;

	SetStringButton(&menuToUpdate->button[3], /*quit*/GetLocalizationString(6));
	for (int i = 0; i < menuToUpdate->buttonNumber; i++)
	{
		if (i != menuToUpdate->buttonNumber - 1)
		{
			sfText_setCharacterSize(menuToUpdate->button[i].text, newSize);
			RescaleText(menuToUpdate->button[i].text, 325.0f, 50.f, ratioX, ratioY);
			sfVector2f pos = (sfVector2f) { menuToUpdate->button[i].rect.left + (menuToUpdate->button[i].rect.width) / 2, menuToUpdate->button[i].rect.top + (menuToUpdate->button[i].rect.height) / 2 - 30.f * ratioY };
			CenterText(menuToUpdate->button[i].text, pos);
			if (menuToUpdate->button[i].text2 != NULL)
			{
				sfText_setCharacterSize(menuToUpdate->button[i].text2, newSize);
				RescaleText(menuToUpdate->button[i].text2, 325.0f, 50.f, ratioX, ratioY);
				sfText_setString(menuToUpdate->button[i].text2, ChangeSaveNameButton(GetPlayersInformations().NumberOfPlayer, i + 1));
				sfVector2f pos = (sfVector2f) { menuToUpdate->button[i].rect.left + (menuToUpdate->button[i].rect.width) / 2, menuToUpdate->button[i].rect.top + (menuToUpdate->button[i].rect.height) / 2 + 20.f * ratioY };
				CenterText(menuToUpdate->button[i].text2, pos);
			}
		}
		else
		{
			sfText_setCharacterSize(menuToUpdate->button[i].text, newSize);
			RescaleText(menuToUpdate->button[i].text, 325.0f, 50.f, ratioX, ratioY);
			sfVector2f pos = (sfVector2f) { menuToUpdate->button[i].rect.left + (menuToUpdate->button[i].rect.width) / 2, menuToUpdate->button[i].rect.top + (menuToUpdate->button[i].rect.height) / 2 };
			CenterText(menuToUpdate->button[i].text, pos);
		}
	}
}

Menu CreateControlsMenuProto(sfVideoMode mode)
{
	float ratioX = mode.width / 1920.0f;
	float ratioY = mode.height / 1080.0f;
	Button selectOptionButtons[3];
	sfVector2f selectOptionButtonsPos = { mode.width / 2.0f,  mode.height / 2.0f };
	sfVector2f selectOptionButtonsSize = { 620.0f * ratioX, 180.0f * ratioY };
	char* filesDirectories[3] =
	{
		"Ressources/Sprites/UI/Bouton.png",
		"Ressources/Sprites/UI/Bouton.png",
		"Ressources/Sprites/UI/Bouton.png"
	};

	for (int i = 0; i < 3; i++)
	{
		selectOptionButtons[i] = InitButton(selectOptionButtonsSize, selectOptionButtonsPos, i, filesDirectories[i], true);
	}
	sfVector2f posFirst = { mode.width / 2.0f,  350.0f * ratioY };
	sfVector2f vectorCur = { (posFirst.x) - 600.0f * ratioX, (posFirst.y) - 100.0f  * ratioY };

	Menu selectOptionMenu = CreateGameMenu(vectorCur, selectOptionButtons, 3, 0, false);

	selectOptionMenu.selectionCursor.indexPos = 0;
	selectOptionMenu.selectionCursor.offset.x = 0.0f * ratioX;
	selectOptionMenu.selectionCursor.offset.y = 150.0f * ratioY;

	AlignMenuButtons(&selectOptionMenu, 0, posFirst, -20.0f * ratioY, 2);

	selectOptionMenu.selectors = (Selector*)malloc(2 * sizeof(Selector));
	selectOptionMenu.selectorsNumber = 2;
	selectOptionMenu.classDisplayer.Class = NULL;
	selectOptionMenu.classDisplayer.isOn = sfFalse;

	selectOptionMenu.SliderSelectors = NULL;
	selectOptionMenu.SliderSelectorsNumber = 0;

	InitSelector(&selectOptionMenu.selectors[0], posFirst, selectOptionButtonsSize, "Ressources/Sprites/UI/Bouton.png", 1);
	InitSelector(&selectOptionMenu.selectors[1], posFirst, selectOptionButtonsSize, "Ressources/Sprites/UI/Bouton.png", 1);

	sfVector2f centerPos = CenterInMiddleElements(selectOptionMenu, posFirst, 0, 0, 0, -20.0f * ratioY, 1);
	AlignButtonsAndSelector(&selectOptionMenu, centerPos, 0, 0, 0, -20.0f * ratioY, 1);

	centerPos.y = selectOptionMenu.button[1].rect.top + (selectOptionMenu.button[1].rect.height / 2.0f);
	centerPos.x = mode.width / 2.0f;
	centerPos = CenterInMiddleElements(selectOptionMenu, centerPos, 1, 1, 1, -20.0f * ratioY, 1);
	AlignButtonsAndSelector(&selectOptionMenu, centerPos, 1, 1, 1, -20.0f * ratioY, 1);

	selectOptionMenu.selectors[0].positionText.x = selectOptionMenu.selectors[0].rect.left + (selectOptionMenu.selectors[0].rect.width / 2.0f);
	selectOptionMenu.selectors[0].positionText.y = selectOptionMenu.selectors[0].rect.top + (selectOptionMenu.selectors[0].rect.height / 2.0f) + 10.0f * ratioY;

	selectOptionMenu.selectors[1].positionText.x = selectOptionMenu.selectors[1].rect.left + (selectOptionMenu.selectors[1].rect.width / 2.0f);
	selectOptionMenu.selectors[1].positionText.y = selectOptionMenu.selectors[1].rect.top + (selectOptionMenu.selectors[1].rect.height / 2.0f) + 10.0f * ratioY;

	selectOptionMenu.selectors[0].textDisplay.textArray = (sfText**)calloc(3, sizeof(sfText*));
	selectOptionMenu.selectors[0].textDisplay.numberOfText = 3;
	selectOptionMenu.selectors[0].textDisplay.actualTextToDisplay = GetTypeHardware(0) + GetPortID(0);

	char* gamepad = (char*)calloc(15, sizeof(char));
	sprintf(gamepad, "%s %d", GetLocalizationString(149), 0);
	selectOptionMenu.selectors[0].textDisplay.textArray[0] = InitText(/*keyboard*/GetLocalizationString(148), selectOptionMenu.selectors[0].positionText, sfColor_fromRGB(171, 144, 111), (int)(TEXT_SIZE_MENU * (mode.height / 1080.0f)));
	CenterText(selectOptionMenu.selectors[0].textDisplay.textArray[0], selectOptionMenu.selectors[0].positionText);
	selectOptionMenu.selectors[0].textDisplay.textArray[1] = InitText(/*gamePad1*/gamepad, selectOptionMenu.selectors[0].positionText, sfColor_fromRGB(171, 144, 111), (int)(TEXT_SIZE_MENU * (mode.height / 1080.0f)));
	CenterText(selectOptionMenu.selectors[0].textDisplay.textArray[1], selectOptionMenu.selectors[0].positionText);
	sprintf(gamepad, "%s %d", GetLocalizationString(149), 1);
	selectOptionMenu.selectors[0].textDisplay.textArray[2] = InitText(/*gamePad2*/gamepad, selectOptionMenu.selectors[0].positionText, sfColor_fromRGB(171, 144, 111), (int)(TEXT_SIZE_MENU * (mode.height / 1080.0f)));
	CenterText(selectOptionMenu.selectors[0].textDisplay.textArray[2], selectOptionMenu.selectors[0].positionText);

	for (int i = 0; i < 3; i++)
	{
		sfText_setCharacterSize(selectOptionMenu.selectors[0].textDisplay.textArray[i], (int)(TEXT_SIZE_MENU * (mode.height / 1080.0f)));
		RescaleText(selectOptionMenu.selectors[0].textDisplay.textArray[i], 325.0f, 168.5f, ratioX, ratioY);
		CenterText(selectOptionMenu.selectors[0].textDisplay.textArray[i], selectOptionMenu.selectors[0].positionText);
	}

	selectOptionMenu.selectors[0].isOn = sfFalse;

	selectOptionMenu.selectors[1].textDisplay.textArray = (sfText**)calloc(4, sizeof(sfText*));
	selectOptionMenu.selectors[1].textDisplay.numberOfText = 4;
	selectOptionMenu.selectors[1].textDisplay.actualTextToDisplay = GetTypeHardware(1) + GetPortID(1);

	selectOptionMenu.selectors[1].textDisplay.textArray[0] = InitText(/*keyboard*/GetLocalizationString(148), selectOptionMenu.selectors[1].positionText, sfColor_fromRGB(171, 144, 111), (int)(TEXT_SIZE_MENU * (mode.height / 1080.0f)));
	CenterText(selectOptionMenu.selectors[1].textDisplay.textArray[0], selectOptionMenu.selectors[1].positionText);
	sprintf(gamepad, "%s %d", GetLocalizationString(149), 0);
	selectOptionMenu.selectors[1].textDisplay.textArray[1] = InitText(/*gamePad*/gamepad, selectOptionMenu.selectors[1].positionText, sfColor_fromRGB(171, 144, 111), (int)(TEXT_SIZE_MENU * (mode.height / 1080.0f)));
	CenterText(selectOptionMenu.selectors[1].textDisplay.textArray[1], selectOptionMenu.selectors[1].positionText);
	sprintf(gamepad, "%s %d", GetLocalizationString(149), 1);
	selectOptionMenu.selectors[1].textDisplay.textArray[2] = InitText(/*No device*/gamepad, selectOptionMenu.selectors[1].positionText, sfColor_fromRGB(171, 144, 111), (int)(TEXT_SIZE_MENU * (mode.height / 1080.0f)));
	CenterText(selectOptionMenu.selectors[1].textDisplay.textArray[2], selectOptionMenu.selectors[1].positionText);
	selectOptionMenu.selectors[1].textDisplay.textArray[3] = InitText(/*No device*/GetLocalizationString(150), selectOptionMenu.selectors[1].positionText, sfColor_fromRGB(171, 144, 111), (int)(TEXT_SIZE_MENU * (mode.height / 1080.0f)));

	free(gamepad);

	for (int i = 0; i < 4; i++)
	{
		sfText_setCharacterSize(selectOptionMenu.selectors[1].textDisplay.textArray[i], (int)(TEXT_SIZE_MENU * (mode.height / 1080.0f)));
		RescaleText(selectOptionMenu.selectors[1].textDisplay.textArray[i], 325.0f, 168.5f, ratioX, ratioY);
		CenterText(selectOptionMenu.selectors[1].textDisplay.textArray[i], selectOptionMenu.selectors[1].positionText);
	}

	selectOptionMenu.selectors[1].isOn = sfFalse;
	selectOptionMenu.selectionCursor.indexDisplay = LoadSprite("Ressources/Sprites/UI/Selecteur.png", 1);
	sfVector2f scale;
	sfVector2u size = sfTexture_getSize(sfSprite_getTexture(selectOptionMenu.selectionCursor.indexDisplay));
	scale.x = (620.f * ratioX) / (float)size.x;
	scale.y = (180.f * ratioY) / (float)size.y;

	sfSprite_setScale(selectOptionMenu.selectionCursor.indexDisplay, scale);

	selectOptionMenu.selectionCursor.indexPos = 0;
	selectOptionMenu.selectionCursor.offset.x = 0.0f * ratioX;
	selectOptionMenu.selectionCursor.offset.y = 150.0f * ratioY;

	sfSprite_setPosition(selectOptionMenu.selectionCursor.indexDisplay, sfSprite_getPosition(selectOptionMenu.button[0].sprite));

	selectOptionMenu.Position = sfSprite_getPosition(selectOptionMenu.button[0].sprite);

	InitTextButton(&selectOptionMenu.button[0], /*"Player one"*/GetLocalizationString(146), sfColor_fromRGB(171, 144, 111), TEXT_SIZE_MENU);
	InitTextButton(&selectOptionMenu.button[1], /*"Player two"*/GetLocalizationString(147), sfColor_fromRGB(171, 144, 111), TEXT_SIZE_MENU);
	InitTextButton(&selectOptionMenu.button[2], /*"Back"*/GetLocalizationString(6), sfColor_fromRGB(171, 144, 111), TEXT_SIZE_MENU);


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

void ResponsiveControlsMenu(Menu *menuToUpdate, sfVideoMode mode)
{
	float ratioX = mode.width / 1920.0f;
	float ratioY = mode.height / 1080.0f;

	sfVector2f posFirst = { mode.width / 2.0f, 350.0f * ratioY };

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

	sfVector2f centerPos = CenterInMiddleElements(*menuToUpdate, posFirst, 0, 0, 0, -20.0f * ratioY, 1);
	AlignButtonsAndSelector(menuToUpdate, centerPos, 0, 0, 0, -20.0f * ratioY, 1);

	centerPos.y = menuToUpdate->button[1].rect.top + (menuToUpdate->button[1].rect.height / 2.0f);
	centerPos.x = mode.width / 2.0f;
	centerPos = CenterInMiddleElements(*menuToUpdate, centerPos, 1, 1, 1, -20.0f * ratioY, 1);
	AlignButtonsAndSelector(menuToUpdate, centerPos, 1, 1, 1, -20.0f * ratioY, 1);

	menuToUpdate->selectors[0].positionText.x = menuToUpdate->selectors[0].rect.left + (menuToUpdate->selectors[0].rect.width / 2.0f);
	menuToUpdate->selectors[0].positionText.y = menuToUpdate->selectors[0].rect.top + (menuToUpdate->selectors[0].rect.height / 2.0f) + 10.0f * ratioY;
	char* gamepad = calloc(15, sizeof(char));
	sfText_setString(menuToUpdate->selectors[0].textDisplay.textArray[0],/*Keyboard*/ GetLocalizationString(148));
	sprintf(gamepad, "%s %d", GetLocalizationString(149), 0);
	sfText_setString(menuToUpdate->selectors[0].textDisplay.textArray[1], /*Controller*/gamepad);
	sprintf(gamepad, "%s %d", GetLocalizationString(149), 1);
	sfText_setString(menuToUpdate->selectors[0].textDisplay.textArray[2], /*Controller*/gamepad);
	for (int i = 0; i < 3; i++)
	{
		sfText_setCharacterSize(menuToUpdate->selectors[0].textDisplay.textArray[i], (int)(TEXT_SIZE_MENU * (mode.height / 1080.0f)));
		RescaleText(menuToUpdate->selectors[0].textDisplay.textArray[i], 325.0f, 168.5f, ratioX, ratioY);
		CenterText(menuToUpdate->selectors[0].textDisplay.textArray[i], menuToUpdate->selectors[0].positionText);
	}

	menuToUpdate->selectors[1].positionText.x = menuToUpdate->selectors[1].rect.left + (menuToUpdate->selectors[1].rect.width / 2.0f);
	menuToUpdate->selectors[1].positionText.y = menuToUpdate->selectors[1].rect.top + (menuToUpdate->selectors[1].rect.height / 2.0f) + 10.0f * ratioY;
	sfText_setString(menuToUpdate->selectors[1].textDisplay.textArray[0],/*Keyboard*/ GetLocalizationString(148));
	sprintf(gamepad, "%s %d", GetLocalizationString(149), 0);
	sfText_setString(menuToUpdate->selectors[1].textDisplay.textArray[1], /*Controller*/gamepad);
	sprintf(gamepad, "%s %d", GetLocalizationString(149), 1);
	sfText_setString(menuToUpdate->selectors[1].textDisplay.textArray[2], /*controller*/gamepad);
	sfText_setString(menuToUpdate->selectors[1].textDisplay.textArray[3], /*noce device*/GetLocalizationString(150));
	free(gamepad);
	for (int i = 0; i < 4; i++)
	{
		sfText_setCharacterSize(menuToUpdate->selectors[1].textDisplay.textArray[i], (int)(TEXT_SIZE_MENU * (mode.height / 1080.0f)));
		RescaleText(menuToUpdate->selectors[1].textDisplay.textArray[i], 325.0f, 168.5f, ratioX, ratioY);
		CenterText(menuToUpdate->selectors[1].textDisplay.textArray[i], menuToUpdate->selectors[1].positionText);
	}

	int newSize = (int)(TEXT_SIZE_MENU * (mode.height / 1080.0f)) - 5;
	SetStringButton(&menuToUpdate->button[0], /*Player one*/GetLocalizationString(146));
	SetStringButton(&menuToUpdate->button[1], /*Player two*/GetLocalizationString(147));
	SetStringButton(&menuToUpdate->button[2], /*back*/GetLocalizationString(6));
	for (int i = 0; i < menuToUpdate->buttonNumber; i++)
	{
		sfText_setCharacterSize(menuToUpdate->button[i].text, newSize);
		RescaleText(menuToUpdate->button[i].text, 325.0f, 168.5f, ratioX, ratioY);
		sfVector2f pos = (sfVector2f) { menuToUpdate->button[i].rect.left + (menuToUpdate->button[i].rect.width) / 2, menuToUpdate->button[i].rect.top + (menuToUpdate->button[i].rect.height) / 2 + 10.0f * ratioY };
		CenterText(menuToUpdate->button[i].text, pos);
	}

	sfVector2f scale;
	sfVector2u size = sfTexture_getSize(sfSprite_getTexture(menuToUpdate->selectionCursor.indexDisplay));
	scale.x = (620.f * ratioX) / (float)size.x;
	scale.y = (180.f * ratioY) / (float)size.y;

	sfSprite_setScale(menuToUpdate->selectionCursor.indexDisplay, scale);
	sfSprite_setPosition(menuToUpdate->selectionCursor.indexDisplay, sfSprite_getPosition(menuToUpdate->button[0].sprite));

	menuToUpdate->Position = sfSprite_getPosition(menuToUpdate->button[0].sprite);

	menuToUpdate->selectionCursor.offset.x = 0.0f * ratioX;
	menuToUpdate->selectionCursor.offset.y = 150.0f * ratioY;

	sfVector2f newPos = { menuToUpdate->Position.x + menuToUpdate->selectionCursor.indexPos * menuToUpdate->selectionCursor.offset.x,
		menuToUpdate->Position.y + menuToUpdate->selectionCursor.indexPos * menuToUpdate->selectionCursor.offset.y };
	sfSprite_setPosition(menuToUpdate->selectionCursor.indexDisplay, newPos);
}

Menu CreateCreditsMenuProto(sfVideoMode mode)
{
	float ratioX = mode.width / 1920.0f;
	float ratioY = mode.height / 1080.0f;
	sfVector2f vectorCur = { ratioX, ratioY };
	Menu CreditMenu = CreateGameMenu(vectorCur, NULL, 0, 0, 0);

	sfVector2f scale = { ratioX, ratioY };

	AddBackgroundToMenu(&CreditMenu, "Ressources/Sprites/UI/Credits.png", (sfVector2f) { 0.0f, 1080.0f * ratioY }, 0);
	sfSprite_setScale(CreditMenu.background, scale);

	CreditMenu.Position.y = 2169.f * ratioY;
	CreditMenu.Position.x = 1080.0f * ratioY;

	CreditMenu.selectors = NULL;
	CreditMenu.selectorsNumber = 0;
	CreditMenu.classDisplayer.Class = NULL;
	CreditMenu.classDisplayer.isOn = sfFalse;

	CreditMenu.SliderSelectors = NULL;
	CreditMenu.SliderSelectorsNumber = 0;
	CreditMenu.selectionCursor.indexPos = -1;

	return(CreditMenu);
}

void ResponsiveCredits(Menu *menuToUpdate, sfVideoMode mode)
{
	float ratioX = mode.width / 1920.0f;
	float ratioY = mode.height / 1080.0f;
	sfVector2f scale = { ratioX, ratioY };
	sfSprite_setScale(menuToUpdate->background, scale);
	sfVector2f pos = { 0.0f, 1080.0f * ratioY };
	sfSprite_setPosition(menuToUpdate->background, pos);

	menuToUpdate->Position.y = 2169.f * ratioY;
	menuToUpdate->Position.x = 1080.0f * ratioY;
}

Menu CreateNewOrLoad(sfVideoMode mode)
{
	float ratioX = mode.width / 1920.0f;
	float ratioY = mode.height / 1080.0f;
	Button selectPlayerMenuButtons[3];
	sfVector2f selectPlayerButtonsPos = { 400.0f * ratioX, 400.0f * ratioY };
	sfVector2f selectPlayerButtonsSize = { 620.0f * ratioX, 180.f * ratioY };
	char* filesDirectories[3] =
	{
		"Ressources/Sprites/UI/Bouton.png",
		"Ressources/Sprites/UI/Bouton.png",
		"Ressources/Sprites/UI/Bouton.png"
	};

	for (int i = 0; i < 3; i++)
	{
		selectPlayerMenuButtons[i] = InitButton(selectPlayerButtonsSize, selectPlayerButtonsPos, i, filesDirectories[i], true);
	}

	sfVector2f posFirst = { mode.width / 2.0f, mode.height / 2.0f - 100.0f };
	sfVector2f vectorCur = { (posFirst.x) - 600.0f * ratioX, (posFirst.y) - 100.0f  * ratioY };

	Menu selectPlayerMenu = CreateGameMenu(vectorCur, selectPlayerMenuButtons, 3, 0, false);

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

	sfSprite_setPosition(selectPlayerMenu.selectionCursor.indexDisplay, sfSprite_getPosition(selectPlayerMenu.button[0].sprite));
	selectPlayerMenu.Position = sfSprite_getPosition(selectPlayerMenu.button[0].sprite);

	selectPlayerMenu.selectors = NULL;
	selectPlayerMenu.selectorsNumber = 0;
	selectPlayerMenu.classDisplayer.Class = NULL;
	selectPlayerMenu.classDisplayer.isOn = sfFalse;

	selectPlayerMenu.SliderSelectors = NULL;
	selectPlayerMenu.SliderSelectorsNumber = 0;

	InitTextButton(&selectPlayerMenu.button[0], /*"New"*/GetLocalizationString(157), sfColor_fromRGB(171, 144, 111), TEXT_SIZE_MENU);
	if (!SearchingSave(1, -1) && !SearchingSave(2, -1))
	{
		InitTextButton(&selectPlayerMenu.button[1], /*"Load"*/GetLocalizationString(1), sfColor_fromRGB(105, 105, 105), TEXT_SIZE_MENU);
	}
	else
	{
		InitTextButton(&selectPlayerMenu.button[1], /*"Load"*/GetLocalizationString(1), sfColor_fromRGB(171, 144, 111), TEXT_SIZE_MENU);
	}
	InitTextButton(&selectPlayerMenu.button[2], /*"Back"*/GetLocalizationString(6), sfColor_fromRGB(171, 144, 111), TEXT_SIZE_MENU);

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

void ResponsiveNewOrLoad(Menu *menuToUpdate, sfVideoMode mode)
{
	float ratioX = mode.width / 1920.0f;
	float ratioY = mode.height / 1080.0f;


	sfVector2f selectPlayerButtonsPos = { 400.0f * ratioX, 400.0f * ratioY };
	sfVector2f selectPlayerButtonsSize = { 620.0f * ratioX, 180.0f * ratioY };


	for (int i = 0; i < menuToUpdate->buttonNumber; i++)
	{
		ResponsiveButton(&menuToUpdate->button[i], selectPlayerButtonsSize, selectPlayerButtonsPos, true);
	}

	sfVector2f posFirst = { mode.width / 2.0f, mode.height / 2.0f - 100.0f };

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

	sfSprite_setPosition(menuToUpdate->selectionCursor.indexDisplay, sfSprite_getPosition(menuToUpdate->button[0].sprite));
	menuToUpdate->Position = sfSprite_getPosition(menuToUpdate->button[0].sprite);


	int newSize = (int)(TEXT_SIZE_MENU * (mode.height / 1080.0f));
	SetStringButton(&menuToUpdate->button[0], /*New*/GetLocalizationString(157));
	if (!SearchingSave(1, -1) && !SearchingSave(2, -1))
	{
		InitTextButton(&menuToUpdate->button[1], /*"Load"*/GetLocalizationString(1), sfColor_fromRGB(105, 105, 105), TEXT_SIZE_MENU);
	}
	else
	{
		InitTextButton(&menuToUpdate->button[1], /*"Load"*/GetLocalizationString(1), sfColor_fromRGB(171, 144, 111), TEXT_SIZE_MENU);
	}
	SetStringButton(&menuToUpdate->button[2], /*back*/GetLocalizationString(6));

	for (int i = 0; i < menuToUpdate->buttonNumber; i++)
	{
		sfText_setCharacterSize(menuToUpdate->button[i].text, newSize);
		RescaleText(menuToUpdate->button[i].text, 325.0f, 168.5f, ratioX, ratioY);
		sfVector2f pos = (sfVector2f) { menuToUpdate->button[i].rect.left + (menuToUpdate->button[i].rect.width) / 2, menuToUpdate->button[i].rect.top + (menuToUpdate->button[i].rect.height) / 2 + 10.0f * ratioY };
		CenterText(menuToUpdate->button[i].text, pos);
	}
}
