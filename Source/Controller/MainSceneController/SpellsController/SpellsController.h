#ifndef _SPELLSCONTROLLER
#define _SPELLSCONTROLLER

#include <stdio.h>
#include <stdlib.h>
#include "..\..\GraphicsUtilities\Animation\Animation.h"

typedef struct
{
	Animation coneOfFire;
	Animation FireBall;
	sfSprite* SheetFireBall;
	Animation Mine;
}MageSpell;

void InitSpells();
MageSpell GetSpells();
MageSpell* GetSpellsPtr();

#endif // !_SPELLSCONTROLLER
