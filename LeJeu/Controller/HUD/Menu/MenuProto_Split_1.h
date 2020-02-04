#ifndef _MENUPROTOSPLIT_1
#define _MENUPROTOSPLIT_1

#include "Menu.h"

Menu CreatePauseMenuProto(sfVideoMode mode);
void ResponsivePauseMenuProto(Menu *menuToUpdate, sfVideoMode mode);

Menu CreateSoundOptionProto(Config actualConfig);
void ResponsiveSoundOptionProto(Menu *menuToUpdate, sfVideoMode mode);

Menu CreateOptionPauseMenuProto(sfVideoMode mode);
void ResponsiveOptionPauseMenu(Menu *menuToUpdate, sfVideoMode mode);

Menu CreateSavingMenu(sfVideoMode mode);
void ResponsiveSavingMenu(Menu *menuToUpdate, sfVideoMode mode);

Menu CreateControlsMenuProto(sfVideoMode mode);
void ResponsiveControlsMenu(Menu *menuToUpdate, sfVideoMode mode);

Menu CreateCreditsMenuProto(sfVideoMode mode);
void ResponsiveCredits(Menu *menuToUpdate, sfVideoMode mode);

Menu CreateNewOrLoad(sfVideoMode mode);
void ResponsiveNewOrLoad(Menu *menuToUpdate, sfVideoMode mode);

#endif // !_MENUPROTOSPLIT_1