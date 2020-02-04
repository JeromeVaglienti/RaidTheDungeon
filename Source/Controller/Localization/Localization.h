#ifndef LOCALIZATION_H
#define LOCALIZATION_H
#include <string.h>

typedef struct LOCALIZATION
{
	int Id;
	char* String;
}Localization;

typedef struct LOCALIZATIONARRAY
{
	Localization* Array;
	int Size;
}LocalizationArray;

//Initialize all string display in the game with pLanguage 0 FR 1 EN 
void InitializeLocalization(LocalizationArray* pArrayString, const int pLanguage);
//Change all the string display in the game with pLanguage 0 FR 1 EN
void ChangeLocalization(LocalizationArray* pArrayString, const int pLanguage);
//Add a string and giving a new index increment by one
void AddLocalizationString(LocalizationArray* pArrayString, const char* pString);
//Return the string link to a index
char* GetLocalizationString(const int pIndex);
//Return main localization
LocalizationArray GetMainLocalization();
//Return main localization
LocalizationArray* GetPointerMainLocalization();

#endif
