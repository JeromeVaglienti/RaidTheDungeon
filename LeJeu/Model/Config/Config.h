#ifndef _CONFIG_H
#define _CONFIG_H

#include <stdio.h>
#include <stdlib.h>
#include <SFML\System.h>
#include <SFML\Window.h>
#include <SFML\Graphics.h>
#include "..\..\Controller\Localization\Localization.h"

typedef enum
{
	FR,
	EN
}Language;

typedef struct
{
	const sfVideoMode mode;
	sfRenderWindow **mainWindow;
	sfVector2i *resPossible;
	sfUint32 windowMode[3];
	Language MainLanguage;
	unsigned short indexActualWindowMode;
	unsigned short numberResPossible;
	unsigned short indexActualRes;
	sfBool dynamicLight;
}Config;

Config* LoadConfigFile(const sfVector2i* pResolutions);
void SaveConfigFile(const Config newConfig);
void InitConfig(sfRenderWindow **window);
Config GetConfig();
int GetIndexActualRes();
int GetIndexActualWinMode();
int GetActualLanguageSelected();
//Return if Resolution has been changed
sfBool ChangeResolution(const int index);
//Return if has to do responsive handle
sfBool ChangeWindowMode(const int index);

sfBool ChangeLanguage(const int index);
void ChangeConfigOption(const int indexOfOption);
sfBool GetConfigDynamicLight();
sfVideoMode getConfigMode();


#endif // !_CONFIG_H

