#ifndef _KEYBOARD_H
#define _KEYBOARD_H
#include <stdio.h>
#include <stdlib.h>
#include <SFML\Graphics.h>
#include "..\Controller\MainSceneController\PlayerController\PlayerController.h"

void UpdateKeyboardMouseEvt(sfEvent event, ActionListener toUpdate[2], const int ID, const int numberOfPlayers);
void UpdateKeyboardMouse(sfRenderWindow *mainWindow, ActionListener toUpdate[2], const int ID);


#endif