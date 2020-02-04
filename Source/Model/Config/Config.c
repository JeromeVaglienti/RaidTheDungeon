
#include "Config.h"
#include "..\..\View\MapKey\MapKey.h"

Config mainConfig;

Config* LoadConfigFile(const sfVector2i* pResolutions)
{
	FILE* ptrFile;
	Config* cfFile = (Config*)calloc(1, sizeof(Config));
	cfFile->mainWindow = NULL;

	if (!(ptrFile = fopen("Ressources/Configuration/MainConfig.config", "r")))
	{
		printf("error reading config file\n");
		free(cfFile);
		return NULL;
	}
	else
	{
		char tempWord[40];
		int resWidth, resHeight;
		int booleanParameter;
		while (fscanf(ptrFile, "%s", tempWord) != EOF)
		{
			if (strcmp(tempWord, "Language") == 0)
			{
				fscanf(ptrFile, "%s", tempWord);

				if (strcmp(tempWord, "FR") == 0)
				{
					cfFile->MainLanguage = FR;
				}
				else if (strcmp(tempWord, "EN") == 0)
				{
					cfFile->MainLanguage = EN;
				}
				else
				{
					free(cfFile);
					return NULL;
				}
			}

			if (strcmp(tempWord, "Resolution") == 0)
			{
				sfBool b = sfFalse;
				fscanf(ptrFile, "%d %d", &resWidth, &resHeight);
				for (int i = 0; i < 7; i++)
				{
					if (pResolutions[i].x == resWidth && pResolutions[i].y == resHeight)
					{
						b = sfTrue;
						i = 7;
					}
				}
				if (!b)
				{
					free(cfFile);
					return NULL;
				}
				sfVideoMode* tempP = (sfVideoMode*)&cfFile->mode;
				(*tempP) = (sfVideoMode) { resWidth, resHeight, 32 };
			}

			if (strcmp(tempWord, "WindowStyle") == 0)
			{
				fscanf(ptrFile, "%s", tempWord);
				if (strcmp(tempWord, "Close") == 0)
				{
					cfFile->indexActualWindowMode = 0;
				}
				else if (strcmp(tempWord, "Fullscreen") == 0)
				{
					cfFile->indexActualWindowMode = 1;
				}
				else if (strcmp(tempWord, "None") == 0)
				{
					cfFile->indexActualWindowMode = 2;
				}
				else
				{
					free(cfFile);
					return NULL;
				}
			}

			if (strcmp(tempWord, "DynamicLight") == 0)
			{
				fscanf(ptrFile, "%d", &booleanParameter);
				if (booleanParameter != 0 && booleanParameter != 1)
				{
					booleanParameter = 0;
					printf("Error incorrect value read for Dynamic light, set as desactivated by default\n");
				}
				cfFile->dynamicLight = booleanParameter;
			}
		}

		fclose(ptrFile);

		return cfFile;

	}

}

void SaveConfigFile(const Config newConfig)
{
	FILE* ptrFile;

	if (!(ptrFile = fopen("Ressources/Configuration/MainConfig.config", "w")))
	{
		printf("error writting config file\n");
	}
	else
	{
		printf("writting\n");
		char* language = (char*)malloc(sizeof(char) * 3);
		char* windowMode = (char*)malloc(sizeof(char) * 20);
		int dynamicLight;
		switch (newConfig.MainLanguage)
		{
		case FR:
			language = "FR";
			break;
		case EN:
			language = "EN";
			break;
		default:
			language = "EN";
			break;
		}

		switch (newConfig.indexActualWindowMode)
		{
		case 0:
			windowMode = "Close";
			break;
		case 1:
			windowMode = "Fullscreen";
			break;
		case 2:
			windowMode = "None";
			break;
		default:
			windowMode = "Close";
			break;
		}

		dynamicLight = newConfig.dynamicLight;


		fprintf(ptrFile, "Language %s\n", language);
		fprintf(ptrFile, "Resolution %d %d\n", newConfig.mode.width, newConfig.mode.height);
		fprintf(ptrFile, "WindowStyle %s\n", windowMode);
		fprintf(ptrFile, "DynamicLight %d", dynamicLight);

		fclose(ptrFile);
	}
}

void InitConfig(sfRenderWindow **window)
{
	sfVector2i resPossible[7] = {
		{ 800, 600 },
		{ 1024, 768 },
		{ 1280, 800 },
		{ 1280, 1024 },
		{ 1600, 900 },
		{ 1680, 1050 },
		{ 1920, 1080 } };

	Config* LoadConfig = LoadConfigFile(resPossible);

	//count number of screen resolution that are possible 
	sfVideoMode deskMode = sfVideoMode_getDesktopMode();
	int cpt = 0;
	int map[8] = { 0 };

	for (int i = 0; i < 7; i++)
	{
		if ((int)deskMode.width >= resPossible[i].x && (int)deskMode.height >= resPossible[i].y)
		{
			map[cpt] = i;
			cpt++;
		}
	}

	mainConfig.resPossible = (sfVector2i*)malloc(cpt * sizeof(sfVector2i));
	mainConfig.numberResPossible = cpt;

	for (int i = 0; i < mainConfig.numberResPossible; i++)
	{
		mainConfig.resPossible[i] = resPossible[map[i]];
	}
	//Set the mainConfig if the loadConfig exists
	if (LoadConfig)
	{
		sfVideoMode* tempP = (sfVideoMode*)&mainConfig.mode;
		*tempP = LoadConfig->mode;
		mainConfig.MainLanguage = LoadConfig->MainLanguage;
		mainConfig.indexActualWindowMode = LoadConfig->indexActualWindowMode;
		mainConfig.dynamicLight = LoadConfig->dynamicLight;
		free(LoadConfig->mainWindow);
		free(LoadConfig->resPossible);
		free(LoadConfig);
	}
	else
	{
		//Default parameter Config
		sfVideoMode temp = sfVideoMode_getDesktopMode();
		sfVideoMode* tempP = (sfVideoMode*)&mainConfig.mode;
		*tempP = temp;
		mainConfig.MainLanguage = EN;
		mainConfig.indexActualWindowMode = 0;
		mainConfig.dynamicLight = sfFalse;
	}

	//set label button 
	for (int i = 0; i < mainConfig.numberResPossible; i++)
	{
		if (mainConfig.resPossible[i].x == mainConfig.mode.width && mainConfig.resPossible[i].y == mainConfig.mode.height)
		{
			mainConfig.indexActualRes = i;
			i = 7;
		}
	}

	// 0 is Windowed 1 is FullScreen 2 is Bordeless	
	mainConfig.windowMode[0] = sfClose;
	mainConfig.windowMode[1] = sfFullscreen;
	mainConfig.windowMode[2] = sfNone;

	mainConfig.mainWindow = window;

	InitializeLocalization(GetPointerMainLocalization(), mainConfig.MainLanguage);

	*window = sfRenderWindow_create(mainConfig.mode, "RaidTheDungeon", mainConfig.windowMode[mainConfig.indexActualWindowMode], NULL);

	InitMapKey();
}

Config GetConfig()
{
	return(mainConfig);
}

int GetIndexActualRes()
{
	return(mainConfig.indexActualRes);
}

int GetIndexActualWinMode()
{
	return(mainConfig.indexActualWindowMode);
}

int GetActualLanguageSelected()
{
	return(mainConfig.MainLanguage);
}

//Return if Resolution has been changed
sfBool ChangeResolution(const int index)
{
	if ((mainConfig.resPossible[index].x != mainConfig.mode.width || mainConfig.resPossible[index].y != mainConfig.mode.height) && mainConfig.windowMode[mainConfig.indexActualWindowMode] != sfNone)
	{
		sfVideoMode* tempP = (sfVideoMode*)&mainConfig.mode;

		tempP->width = mainConfig.resPossible[index].x;
		tempP->height = mainConfig.resPossible[index].y;
		sfRenderWindow_destroy(*mainConfig.mainWindow);
		*mainConfig.mainWindow = sfRenderWindow_create(mainConfig.mode, "RaidTheDungeon", mainConfig.windowMode[mainConfig.indexActualWindowMode], NULL);
		sfRenderWindow_setMouseCursorVisible(*mainConfig.mainWindow, sfFalse);
		SaveConfigFile(GetConfig());
		return sfTrue;
	}
	else
	{
		return sfFalse;
	}
}

//Return if has to do responsive handle
sfBool ChangeWindowMode(const int index)
{
	if (index != mainConfig.indexActualWindowMode)
	{
		sfBool toReturn = sfFalse;
		if (index == 2)
		{
			sfVideoMode temp = sfVideoMode_getDesktopMode();
			sfVideoMode* tempP = (sfVideoMode*)&mainConfig.mode;

			tempP->width = temp.width;
			tempP->height = temp.height;

			for (int i = 0; i < mainConfig.numberResPossible; i++)
			{
				if (mainConfig.resPossible[i].x == temp.width && mainConfig.resPossible[i].y == temp.height)
				{
					mainConfig.indexActualRes = i;
					i = mainConfig.numberResPossible;
				}
			}

			toReturn = sfTrue;
		}
		mainConfig.indexActualWindowMode = index;

		sfRenderWindow_destroy(*mainConfig.mainWindow);
		*mainConfig.mainWindow = sfRenderWindow_create(mainConfig.mode, "RaidTheDungeon", mainConfig.windowMode[mainConfig.indexActualWindowMode], NULL);
		sfRenderWindow_setMouseCursorVisible(*mainConfig.mainWindow, sfFalse);
		SaveConfigFile(GetConfig());

		return(toReturn);
	}
	else
	{
		return sfFalse;
	}
}

sfBool ChangeLanguage(const int index)
{
	if (mainConfig.MainLanguage != index)
	{
		mainConfig.MainLanguage = index;
		SaveConfigFile(GetConfig());
		return sfTrue;
	}
	else return sfFalse;
}

void ChangeConfigOption(const int indexOfOption)
{
	switch (indexOfOption)
	{
		//Dynamic light	
	case 0:
		mainConfig.dynamicLight = !(mainConfig.dynamicLight);
		SaveConfigFile(GetConfig());
		break;
	default:
		printf("Error index of Option not valid\n");
		break;
	}
}

sfBool GetConfigDynamicLight()
{
	return mainConfig.dynamicLight;
}

sfVideoMode getConfigMode()
{
	return(mainConfig.mode);
}