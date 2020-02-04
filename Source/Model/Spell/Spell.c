#include "Spell.h"

// spell id, fire rate, cooldown, timer cooldown, cost of mana, damage ,is available, is upgrade, is unlocked

Spell proto[4] = {
	{ FireBall, 0.f, 0.75f, 0.f, 20, 30, sfTrue, sfFalse, sfTrue},
	{ ConeOfFire, 0.05f, 10.f, 0.f, 100, 300, sfTrue, sfFalse, sfTrue },
	{ mine , 0.f, 10.f, 0.f, 75, 50, sfTrue, sfFalse, sfTrue },
	{ Dash , 0.f, 1.f, 0.f, 5, 0, sfTrue, sfFalse, sfTrue }
};


Spell ReturnProtoSpell(const int idOfProto)
{
	return(proto[idOfProto]);
}