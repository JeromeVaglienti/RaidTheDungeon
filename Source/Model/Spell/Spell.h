#ifndef _SPELL_H
#define _SPELL_H
#include "SFML\System.h"

typedef enum TypeOfSpell
{
	FireBall,
	ConeOfFire,
	mine,
	Dash
}TypeOfSpell;



typedef struct
{
	TypeOfSpell type;
	float fireRate;
	float cooldown;
	float timerCooldown;
	unsigned short cost;
	unsigned short damage;
	sfBool isAvailable;
	sfBool isUpgrade;
	sfBool isUnlocked;
}Spell;


Spell ReturnProtoSpell(const int idOfProto);


#endif // !_SPELL_H
