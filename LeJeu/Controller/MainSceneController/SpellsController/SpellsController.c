#include "SpellsController.h"

MageSpell mageSpell = { 0 };

void InitSpells()
{
	mageSpell.coneOfFire = LoadAnimation(24, 2.0f, "Ressources/Sprites/Players/Spells/cone de feu 2/", "png", (sfVector2f) { 0.0f, 0.0f }, sfTrue, 0);
	mageSpell.FireBall = LoadAnimationSpriteSheet(16, 1.0f, (sfVector2f) { 0.0f, 0.0f }, sfTrue);
	mageSpell.SheetFireBall = LoadSprite("Ressources/Sprites/Players/Spells/fireBallAnim2/spritesheet.png", 1);
	mageSpell.Mine = LoadAnimation(14, 1.0f, "Ressources/Sprites/Players/Spells/Mine/", "png", (sfVector2f) { 0.0f, 0.0f }, sfTrue, 1);
	sfVector2f scale = { 0.5f, 0.5f };

	sfFloatRect tempSize2 = sfSprite_getLocalBounds(mageSpell.coneOfFire.spriteArray[0]);

	for (int i = 0; i < mageSpell.coneOfFire.numberOfFrame; i++)
	{
		sfSprite_setOrigin(mageSpell.coneOfFire.spriteArray[i], (sfVector2f) { tempSize2.width / 2.f, tempSize2.height });
	}

	tempSize2 = sfSprite_getLocalBounds(mageSpell.Mine.spriteArray[0]);
	scale = (sfVector2f) { 0.9f, 0.9f };
	for (int i = 0; i < mageSpell.Mine.numberOfFrame; i++)
	{
		sfSprite_setScale(mageSpell.Mine.spriteArray[i], scale);
		sfSprite_setOrigin(mageSpell.Mine.spriteArray[i], (sfVector2f) { 0.f, tempSize2.height / 2.f });
	}
}

MageSpell GetSpells()
{
	return(mageSpell);
}

MageSpell* GetSpellsPtr()
{
	return(&mageSpell);
}