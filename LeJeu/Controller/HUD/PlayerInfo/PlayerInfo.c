#include "PlayerInfo.h"
#include "..\..\..\Model\Math\MathUtilities.h"


void InitPlayerInfo(PlayerInfo *toInit, const int id, const sfVideoMode mode, const PlayersInformations playerInfo)
{
	if (toInit->LifeBar == NULL)
	{
		toInit->LifeBar = LoadSprite("Ressources/Sprites/HUD/Life_Barre.png", 0);
		sfTexture_setSmooth((sfTexture*)sfSprite_getTexture(toInit->LifeBar), sfTrue);

		if (id == 0)
		{
			toInit->ManaBar = LoadSprite("Ressources/Sprites/HUD/p1_Mana_Barre.png", 0);
			toInit->PlayerBackGround = LoadSprite("Ressources/Sprites/HUD/p1_HUD_Mage.png", 0);
			toInit->Selector = LoadSprite("Ressources/Sprites/HUD/Spell_Bar/P1_Selected.png", 0);
			sfTexture_setSmooth((sfTexture*)sfSprite_getTexture(toInit->PlayerBackGround), sfTrue);
			sfTexture_setSmooth((sfTexture*)sfSprite_getTexture(toInit->ManaBar), sfTrue);
		}
		else
		{
			toInit->ManaBar = LoadSprite("Ressources/Sprites/HUD/p2_Mana_Barre.png", 0);
			toInit->PlayerBackGround = LoadSprite("Ressources/Sprites/HUD/p2_HUD_Mage.png", 0);
			toInit->Selector = LoadSprite("Ressources/Sprites/HUD/Spell_Bar/P2_Selected.png", 0);

			sfVector2f ori = { sfSprite_getLocalBounds(toInit->Selector).width, 0.f };

			sfSprite_setOrigin(toInit->Selector, ori);
			sfTexture_setSmooth((sfTexture*)sfSprite_getTexture(toInit->PlayerBackGround), sfTrue);
			sfTexture_setSmooth((sfTexture*)sfSprite_getTexture(toInit->ManaBar), sfTrue);
		}

		if (playerInfo.SkinPlayer[id] == (SkinPlayer)red)
		{
			toInit->PlayerSkin = LoadSprite("Ressources/Sprites/HUD/PlayerMiniature/Logo_Perso_Rouge.png", 0);
		}
		else if (playerInfo.SkinPlayer[id] == (SkinPlayer)black)
		{
			toInit->PlayerSkin = LoadSprite("Ressources/Sprites/HUD/PlayerMiniature/Logo_Perso_Noir.png", 0);
		}
		else if (playerInfo.SkinPlayer[id] == (SkinPlayer)white)
		{
			toInit->PlayerSkin = LoadSprite("Ressources/Sprites/HUD/PlayerMiniature/Logo_Perso_Blanc.png", 0);
		}
		else if (playerInfo.SkinPlayer[id] == (SkinPlayer)green)
		{
			toInit->PlayerSkin = LoadSprite("Ressources/Sprites/HUD/PlayerMiniature/Logo_Perso_Vert.png", 0);
		}

		//fireball
		toInit->Spell[0][0] = LoadSprite("Ressources/Sprites/HUD/Spell_Bar/Fire_Ball_Off.png", 0);
		toInit->Spell[1][0] = LoadSprite("Ressources/Sprites/HUD/Spell_Bar/Fire_Ball_On.png", 0);
		//cone of fire
		toInit->Spell[0][1] = LoadSprite("Ressources/Sprites/HUD/Spell_Bar/Fire_Cone_Off.png", 0);
		toInit->Spell[1][1] = LoadSprite("Ressources/Sprites/HUD/Spell_Bar/Fire_Cone_On.png", 0);
		//fire trap
		toInit->Spell[0][2] = LoadSprite("Ressources/Sprites/HUD/Spell_Bar/Fire_Trap_Off.png", 0);
		toInit->Spell[1][2] = LoadSprite("Ressources/Sprites/HUD/Spell_Bar/Fire_Trap_On.png", 0);
		//dash
		toInit->Spell[0][3] = LoadSprite("Ressources/Sprites/HUD/Spell_Bar/Dashe_Off.png", 0);
		toInit->Spell[1][3] = LoadSprite("Ressources/Sprites/HUD/Spell_Bar/Dashe_On.png", 0);
	}
	else
	{
		DestroySfSprite(&toInit->PlayerSkin);

		if (playerInfo.SkinPlayer[id] == (SkinPlayer)red)
		{
			toInit->PlayerSkin = LoadSprite("Ressources/Sprites/HUD/PlayerMiniature/Logo_Perso_Rouge.png", 0);
		}
		else if (playerInfo.SkinPlayer[id] == (SkinPlayer)black)
		{
			toInit->PlayerSkin = LoadSprite("Ressources/Sprites/HUD/PlayerMiniature/Logo_Perso_Noir.png", 0);
		}
		else if (playerInfo.SkinPlayer[id] == (SkinPlayer)white)
		{
			toInit->PlayerSkin = LoadSprite("Ressources/Sprites/HUD/PlayerMiniature/Logo_Perso_Blanc.png", 0);
		}
		else if (playerInfo.SkinPlayer[id] == (SkinPlayer)green)
		{
			toInit->PlayerSkin = LoadSprite("Ressources/Sprites/HUD/PlayerMiniature/Logo_Perso_Vert.png", 0);
		}
	}
	for (int i = 0; i < 4; i++)
	{
		toInit->spellIsInCD[i] = 1;
	}
	if (toInit->ManaVertexScale != NULL)
	{
		sfVertexArray_clear(toInit->ManaVertexScale);
	}

	float ratioX = mode.width / 1920.0f;
	float ratioY = mode.height / 1080.0f;

	sfVector2f scale;
	sfVector2u size = sfTexture_getSize(sfSprite_getTexture(toInit->PlayerBackGround));
	scale.x = (310.0f * ratioX) / (float)size.x;
	scale.y = (145.0f * ratioY) / (float)size.y;
	sfSprite_setScale(toInit->PlayerBackGround, scale);

	size = sfTexture_getSize(sfSprite_getTexture(toInit->PlayerSkin));
	scale.x = (106.0f * ratioX) / (float)size.x;
	scale.y = (106.0f * ratioY) / (float)size.y;
	sfSprite_setScale(toInit->PlayerSkin, scale);

	size = sfTexture_getSize(sfSprite_getTexture(toInit->LifeBar));
	scale.x = (176.0f * ratioX) / (float)size.x;
	scale.y = (17.0f * ratioY) / (float)size.y;
	sfSprite_setScale(toInit->LifeBar, scale);

	toInit->LifeBarScaleFull = scale;
	toInit->LifeBarRatio = -1;

	size = sfTexture_getSize(sfSprite_getTexture(toInit->ManaBar));
	scale.x = (155.0f * ratioX) / (float)size.x;
	scale.y = (42.0f * ratioY) / (float)size.y;
	sfSprite_setScale(toInit->ManaBar, scale);

	toInit->ManaBarScaleFull = scale;
	toInit->ManaBarRatio = -1;

	toInit->ManaTextureSize.x = (float)size.x * scale.x;
	toInit->ManaTextureSize.y = (float)size.y * scale.y;

	size = sfTexture_getSize(sfSprite_getTexture(toInit->Spell[0][0]));
	scale.x = (62.0f * ratioX) / (float)size.x;
	scale.y = (70.0f * ratioY) / (float)size.y;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			sfSprite_setScale(toInit->Spell[i][j], scale);
		}
	}
	sfSprite_setScale(toInit->Selector, scale);

	sfVector2f posMana;
	if (id == 0)
	{
		sfVector2f backGround = { 20 * ratioX, 20 * ratioY };
		sfSprite_setPosition(toInit->PlayerBackGround, backGround);
		sfVector2f SkinIcon = { backGround.x + 25.0f * ratioX, backGround.y + 25.0f * ratioX };
		sfSprite_setPosition(toInit->PlayerSkin, SkinIcon);


		sfVector2f posHP = { backGround.x + 131 * ratioX, backGround.y + 48.0f * ratioY };
		sfSprite_setPosition(toInit->LifeBar, posHP);

		posMana.x = backGround.x + 105.0f * ratioX;
		posMana.y = backGround.y + 85.0f * ratioY;
		sfSprite_setPosition(toInit->ManaBar, posMana);

		sfVector2f posSpell = backGround;
		posSpell.y += 140.0f * ratioY;
		posSpell.x += 50.0f * ratioX;
		for (int i = 0; i < 4; i++)
		{
			sfSprite_setPosition(toInit->Spell[0][i], posSpell);
			sfSprite_setPosition(toInit->Spell[1][i], posSpell);
			posSpell.x += 62.0f * ratioX;
		}
	}
	else
	{
		sfVector2f ori = { sfSprite_getLocalBounds(toInit->LifeBar).width, 0.f };
		sfSprite_setOrigin(toInit->LifeBar, ori);

		ori.x = sfSprite_getLocalBounds(toInit->ManaBar).width;
		sfSprite_setOrigin(toInit->ManaBar, ori);

		ori.x = sfSprite_getLocalBounds(toInit->PlayerBackGround).width;
		sfSprite_setOrigin(toInit->PlayerBackGround, ori);

		ori.x = sfSprite_getLocalBounds(toInit->PlayerSkin).width;
		sfSprite_setOrigin(toInit->PlayerSkin, ori);

		ori.x = sfSprite_getLocalBounds(toInit->Spell[0][0]).width;
		for (int i = 0; i < 4; i++)
		{
			sfSprite_setOrigin(toInit->Spell[0][i], ori);
			sfSprite_setOrigin(toInit->Spell[1][i], ori);
		}

		sfVector2f backGround = { 1900 * ratioX, 20 * ratioY };
		sfSprite_setPosition(toInit->PlayerBackGround, backGround);

		sfVector2f SkinIcon = { backGround.x - 25.0f * ratioX, backGround.y + 25.0f * ratioX };
		sfSprite_setPosition(toInit->PlayerSkin, SkinIcon);

		sfVector2f posHP = { backGround.x - 131 * ratioX, backGround.y + 48.0f * ratioY };
		sfSprite_setPosition(toInit->LifeBar, posHP);

		posMana.x = backGround.x - 105.0f * ratioX;
		posMana.y = backGround.y + 85.0f * ratioY;
		sfSprite_setPosition(toInit->ManaBar, posMana);

		sfVector2f posSpell = backGround;
		posSpell.y += 140.0f * ratioY;
		posSpell.x -= 50.0f * ratioX;
		for (int i = 3; i >= 0; i--)
		{
			sfSprite_setPosition(toInit->Spell[0][i], posSpell);
			sfSprite_setPosition(toInit->Spell[1][i], posSpell);
			posSpell.x -= 62.0f * ratioX;
		}
	}

	toInit->ManaVertexScale = sfVertexArray_create();
	sfVertexArray_setPrimitiveType(toInit->ManaVertexScale, sfQuads);
	if (id == 0)
	{
		sfVertex v = { posMana, sfWhite, (sfVector2f) { 0.0f, 0.0f } };
		sfVertexArray_append(toInit->ManaVertexScale, v);

		v.position.x += toInit->ManaTextureSize.x;
		v.texCoords.x += toInit->ManaTextureSize.x / scale.x;

		sfVertexArray_append(toInit->ManaVertexScale, v);

		v.position.y += toInit->ManaTextureSize.y;
		v.texCoords.y += toInit->ManaTextureSize.y / scale.y;

		sfVertexArray_append(toInit->ManaVertexScale, v);

		v.position.x -= toInit->ManaTextureSize.x;
		v.texCoords.x -= toInit->ManaTextureSize.x / scale.x;

		sfVertexArray_append(toInit->ManaVertexScale, v);
	}
	else
	{
		posMana.y += toInit->ManaTextureSize.y;
		sfVertex v = { posMana, sfWhite, (sfVector2f) { toInit->ManaTextureSize.x / scale.x, toInit->ManaTextureSize.y / scale.y } };
		sfVertexArray_append(toInit->ManaVertexScale, v);

		v.position.x -= toInit->ManaTextureSize.x;
		v.texCoords.x -= toInit->ManaTextureSize.x / scale.x;

		sfVertexArray_append(toInit->ManaVertexScale, v);

		v.position.y -= toInit->ManaTextureSize.y;
		v.texCoords.y -= toInit->ManaTextureSize.y / scale.y;

		sfVertexArray_append(toInit->ManaVertexScale, v);

		v.position.x += toInit->ManaTextureSize.x;
		v.texCoords.x += toInit->ManaTextureSize.x / scale.x;

		sfVertexArray_append(toInit->ManaVertexScale, v);
	}

	sfSprite_setPosition(toInit->Selector, sfSprite_getPosition(toInit->Spell[0][0]));
	toInit->ManaSpriteRS = (sfRenderStates*)calloc(1, sizeof(sfRenderStates));

	toInit->ManaSpriteRS->blendMode = sfBlendAlpha;
	toInit->ManaSpriteRS->shader = NULL;
	toInit->ManaSpriteRS->texture = sfTexture_copy(sfSprite_getTexture(toInit->ManaBar));
	toInit->ManaSpriteRS->transform = sfTransform_Identity;
}


void UpdatePlayerInfo(PlayerInfo *toUpdate, const int data[], const int id, const float dt)
{
	float RatioLife = (float)data[0] / (float)data[1];

	//Change life bar size
	if (!Comparefloat(toUpdate->LifeBarRatio, RatioLife))
	{
		if (toUpdate->LifeBarRatio < 0)
		{
			toUpdate->LifeBarRatio = RatioLife;
		}
		toUpdate->LifeBarRatio = Lerp(toUpdate->LifeBarRatio, RatioLife, dt*1.7f);

		sfVector2f scale = { Clamp(0, toUpdate->LifeBarRatio * toUpdate->LifeBarScaleFull.x, toUpdate->LifeBarScaleFull.x) , 1.0f * toUpdate->LifeBarScaleFull.y };
		sfSprite_setScale(toUpdate->LifeBar, scale);
	}

	float RatioMana = (float)data[2] / (float)data[3];

	//Change mana bar size
	if (!Comparefloat(toUpdate->ManaBarRatio, RatioMana))
	{
		if (toUpdate->ManaBarRatio < 0)
		{
			toUpdate->ManaBarRatio = RatioMana;
		}
		toUpdate->ManaBarRatio = Lerp(toUpdate->ManaBarRatio, RatioMana, dt*1.7f);

		sfVector2f Origin = sfVertexArray_getVertex(toUpdate->ManaVertexScale, 0)->position;
		sfVertex* temp1 = sfVertexArray_getVertex(toUpdate->ManaVertexScale, 1);
		sfVertex* temp2 = sfVertexArray_getVertex(toUpdate->ManaVertexScale, 2);
		if (id == 0)
		{
			temp1->position.x = temp2->position.x = toUpdate->ManaTextureSize.x * toUpdate->ManaBarRatio + Origin.x;
			temp1->texCoords.x = temp2->texCoords.x = (toUpdate->ManaTextureSize.x / toUpdate->ManaBarScaleFull.x) * toUpdate->ManaBarRatio;
		}
		else
		{
			temp1->position.x = temp2->position.x = Origin.x - toUpdate->ManaTextureSize.x * toUpdate->ManaBarRatio;
			temp1->texCoords.x = temp2->texCoords.x = (toUpdate->ManaTextureSize.x / toUpdate->ManaBarScaleFull.x) - ((toUpdate->ManaTextureSize.x / toUpdate->ManaBarScaleFull.x) * toUpdate->ManaBarRatio);
		}
	}
}

void UpdatePlayerCDInfo(PlayerInfo *toUpdate, const int data[])
{
	for (int i = 0; i < 4; i++)
	{
		toUpdate->spellIsInCD[i] = data[i];
	}
}

void UpdatePlayerSpellSelected(PlayerInfo *toUpdate, const short spellSelected)
{
	sfSprite_setPosition(toUpdate->Selector, sfSprite_getPosition(toUpdate->Spell[0][spellSelected]));
}

void DisplayPlayerInfo(sfRenderWindow* mainWindow, const PlayerInfo ToDisplay)
{
	sfRenderWindow_drawVertexArray(mainWindow, ToDisplay.ManaVertexScale, ToDisplay.ManaSpriteRS);
	sfRenderWindow_drawSprite(mainWindow, ToDisplay.PlayerSkin, NULL);
	sfRenderWindow_drawSprite(mainWindow, ToDisplay.PlayerBackGround, NULL);
	sfRenderWindow_drawSprite(mainWindow, ToDisplay.LifeBar, NULL);
	for (int i = 0; i < 4; i++)
	{
		sfRenderWindow_drawSprite(mainWindow, ToDisplay.Spell[ToDisplay.spellIsInCD[i]][i], NULL);
	}
	sfRenderWindow_drawSprite(mainWindow, ToDisplay.Selector, NULL);
}