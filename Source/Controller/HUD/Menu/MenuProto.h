#ifndef _MENUPROTO
#define _MENUPROTO

#include "Menu.h"
#include "MenuProto_Split_1.h"

//ProtoType and Their Responsive alike
Menu CreateMainMenuProto(sfVideoMode mode);
void ResponsiveMainMenu(Menu *menuToUpdate, sfVideoMode mode);

Menu CreateLoadingMenu(sfVideoMode mode);
void ResponsiveLoadingMenu(Menu *menuToUpdate, sfVideoMode mode);

Menu CreateSelectionPlayerMenuProto(sfVideoMode mode);
void ResponsiveSelectionPlayerMenu(Menu *menuToUpdate, sfVideoMode mode);

Menu CreateSelectionSkinPlayerMenuProto(sfVideoMode mode);
void ResponsiveSelectionSkinPlayerMenu(Menu *menuToUpdate, sfVideoMode mode);

Menu CreateOptionMenuProto(sfVideoMode mode);
void ResponsiveOptionMenu(Menu *menuToUpdate, sfVideoMode mode);

Menu CreateGeneralMenuProto(Config actualConfig);
void ResponsiveGeneralMenu(Menu *menuToUpdate, Config actualConfig);

Menu CreateShortcutsMenuProto(sfVideoMode mode);
void ResponsiveShortcutsMenu(Menu *menuToUpdate, sfVideoMode mode);

Menu CreateGraphicsMenuProto(Config actualConfig);
void ResponsiveGraphicsMenu(Menu *menuToUpdate, Config actualConfig);

#endif // !_MENUPROTO
