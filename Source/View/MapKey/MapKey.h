#ifndef _MAPKEY_H
#define _MAPKEY_H
#include <stdio.h>
#include <stdlib.h>
#include <SFML\Graphics.h>

typedef enum MapKey
{
	MoveUp,
	MoveDown,
	MoveLeft,
	MoveRight,
	SelectFireBall,
	SelectConeOfFire,
	SelectFireTrap,
	UseDash,
	ShowMiniMap,
	BasicAttack,
	UseSpell
}MapKey;


void InitMapKey();
sfKeyCode GetMapKey(const MapKey MapKeyId);
void SetMapKey(const MapKey mapKayId, const sfKeyCode newBind);
sfBool IfCanUseThisKey(const sfKeyCode key);
void SaveMapKey();

#endif // !_MAPKEY_H

