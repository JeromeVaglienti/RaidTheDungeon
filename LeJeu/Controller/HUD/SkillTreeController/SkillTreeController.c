#include "SkillTreeController.h"
#include "..\Text\Text.h"
#include "..\..\Localization\Localization.h"

SkillTreeController skillTreeController = { 0 };
float ratioX = 0;
float ratioY = 0;
sfBool TalentToUpdate[NBRBRANCH][NBRTALENTPERBRANCH] = { 0 };

void InitSkillTreeController(const sfVideoMode mode)
{
	for (int j = 0; j < NBRBRANCH; j++)
	{
		for (int i = 0; i < NBRTALENTPERBRANCH; i++)
		{
			TalentToUpdate[j][i] = sfFalse;
		}
	}
	ratioX = mode.width / 1920.0f;
	ratioY = mode.height / 1080.0f;

	skillTreeController.PosTalent[lifeBranch][0] = (sfVector2f) { 442.5f * ratioX, 340.5f * ratioY };
	skillTreeController.PosTalent[lifeBranch][1] = (sfVector2f) { 337.5f * ratioX, 203.5f * ratioY };
	skillTreeController.PosTalent[lifeBranch][2] = (sfVector2f) { 187.5f * ratioX, 272.5f * ratioY };

	skillTreeController.PosTalent[manaBranch][0] = (sfVector2f) { 602.5f * ratioX, 575.5f * ratioY };
	skillTreeController.PosTalent[manaBranch][1] = (sfVector2f) { 760.5f * ratioX, 422.5f * ratioY };
	skillTreeController.PosTalent[manaBranch][2] = (sfVector2f) { 786.5f * ratioX, 203.5f * ratioY };

	skillTreeController.PosTalent[mobilityBranch][0] = (sfVector2f) { 960.5f * ratioX, 917.5f * ratioY };
	skillTreeController.PosTalent[mobilityBranch][1] = (sfVector2f) { 1224.5f * ratioX, 906.5f * ratioY };
	skillTreeController.PosTalent[mobilityBranch][2] = (sfVector2f) { 1389.5f * ratioX, 759.5f * ratioY };

	skillTreeController.PosTalent[damageBranch][0] = (sfVector2f) { 760.5f * ratioX, 603.5f * ratioY };
	skillTreeController.PosTalent[damageBranch][1] = (sfVector2f) { 1006.5f * ratioX, 539.5f * ratioY };
	skillTreeController.PosTalent[damageBranch][2] = (sfVector2f) { 1028.5f * ratioX, 712.5f * ratioY };

	if (skillTreeController.Background == NULL)
	{

		skillTreeController.Sprite[lifeBranch][0][0] = LoadSprite("Ressources/Sprites/HUD/SkillTree/popovie.png", 1);
		skillTreeController.Sprite[lifeBranch][0][1] = LoadSprite("Ressources/Sprites/HUD/SkillTree/Icons_Full/popovie.png", 1);
		skillTreeController.Sprite[lifeBranch][1][0] = LoadSprite("Ressources/Sprites/HUD/SkillTree/invu.png", 1);
		skillTreeController.Sprite[lifeBranch][1][1] = LoadSprite("Ressources/Sprites/HUD/SkillTree/Icons_Full/invu.png", 1);
		skillTreeController.Sprite[lifeBranch][2][0] = LoadSprite("Ressources/Sprites/HUD/SkillTree/life.png", 1);
		skillTreeController.Sprite[lifeBranch][2][1] = LoadSprite("Ressources/Sprites/HUD/SkillTree/Icons_Full/life.png", 1);

		skillTreeController.Sprite[manaBranch][0][0] = LoadSprite("Ressources/Sprites/HUD/SkillTree/popomana.png", 1);
		skillTreeController.Sprite[manaBranch][0][1] = LoadSprite("Ressources/Sprites/HUD/SkillTree/Icons_Full/popomana.png", 1);
		skillTreeController.Sprite[manaBranch][1][0] = LoadSprite("Ressources/Sprites/HUD/SkillTree/cost.png", 1);
		skillTreeController.Sprite[manaBranch][1][1] = LoadSprite("Ressources/Sprites/HUD/SkillTree/Icons_Full/cost.png", 1);
		skillTreeController.Sprite[manaBranch][2][0] = LoadSprite("Ressources/Sprites/HUD/SkillTree/mana.png", 1);
		skillTreeController.Sprite[manaBranch][2][1] = LoadSprite("Ressources/Sprites/HUD/SkillTree/Icons_Full/mana.png", 1);

		skillTreeController.Sprite[damageBranch][0][0] = LoadSprite("Ressources/Sprites/HUD/SkillTree/firetrap.png", 1);
		skillTreeController.Sprite[damageBranch][0][1] = LoadSprite("Ressources/Sprites/HUD/SkillTree/Icons_Full/firetrap.png", 1);
		skillTreeController.Sprite[damageBranch][1][0] = LoadSprite("Ressources/Sprites/HUD/SkillTree/fireball.png", 1);
		skillTreeController.Sprite[damageBranch][1][1] = LoadSprite("Ressources/Sprites/HUD/SkillTree/Icons_Full/fireball.png", 1);
		skillTreeController.Sprite[damageBranch][2][0] = LoadSprite("Ressources/Sprites/HUD/SkillTree/power.png", 1);
		skillTreeController.Sprite[damageBranch][2][1] = LoadSprite("Ressources/Sprites/HUD/SkillTree/Icons_Full/power.png", 1);

		skillTreeController.Sprite[mobilityBranch][0][0] = LoadSprite("Ressources/Sprites/HUD/SkillTree/speed.png", 1);
		skillTreeController.Sprite[mobilityBranch][0][1] = LoadSprite("Ressources/Sprites/HUD/SkillTree/Icons_Full/speed.png", 1);
		skillTreeController.Sprite[mobilityBranch][1][0] = LoadSprite("Ressources/Sprites/HUD/SkillTree/tenacity.png", 1);
		skillTreeController.Sprite[mobilityBranch][1][1] = LoadSprite("Ressources/Sprites/HUD/SkillTree/Icons_Full/tenacity.png", 1);
		skillTreeController.Sprite[mobilityBranch][2][0] = LoadSprite("Ressources/Sprites/HUD/SkillTree/dash.png", 1);
		skillTreeController.Sprite[mobilityBranch][2][1] = LoadSprite("Ressources/Sprites/HUD/SkillTree/Icons_Full/dash.png", 1);
		skillTreeController.Background = LoadSprite("Ressources/Sprites/HUD/SkillTree/Flat_SKills.png", 0);
		skillTreeController.Border = LoadSprite("Ressources/Sprites/HUD/SkillTree/Cadre_Fer_02.png", 0);
		skillTreeController.GoldCircle = LoadSprite("Ressources/Sprites/HUD/SkillTree/Gold_Circle.png", 1);
		skillTreeController.Selector[0] = LoadSprite("Ressources/Sprites/HUD/SkillTree/selector1.png", 1);
		skillTreeController.Selector[1] = LoadSprite("Ressources/Sprites/HUD/SkillTree/selector2.png", 1);
		skillTreeController.Selector[2] = LoadSprite("Ressources/Sprites/HUD/SkillTree/selector3.png", 1);
		skillTreeController.Selector[3] = LoadSprite("Ressources/Sprites/HUD/SkillTree/selector4.png", 1);
		skillTreeController.Description = InitText("", (sfVector2f) { 1250.f * ratioX, 175.f * ratioY }, sfWhite, (int)(40 * ratioX));
		skillTreeController.Information = InitText("", (sfVector2f) { 1250.f * ratioX, 420.f * ratioY }, sfWhite, (int)(40 * ratioX));
		skillTreeController.Level = InitText("", (sfVector2f) { 1250.f * ratioX, 420.f * ratioY }, sfWhite, (int)(40 * ratioX));

		for (int i = 0; i < NBRBRANCH * 4; i++)
		{
			skillTreeController.VertexBranchs[i] = sfVertexArray_create();
			sfVertexArray_setPrimitiveType(skillTreeController.VertexBranchs[i], sfQuads);
		}
		char directory[200];
		for (int i = 0; i < NBRBRANCH; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				sprintf(directory, "Ressources/Sprites/HUD/SkillTree/Branche%d/%d.png", i + 1, j + 1);
				skillTreeController.TextureBranchs[i][j] = sfTexture_createFromFile(directory, NULL);
			}
		}
	}
	else
	{
		sfText_setCharacterSize(skillTreeController.Description, (int)(40 * ratioX));
		sfText_setCharacterSize(skillTreeController.Information, (int)(40 * ratioX));
		sfText_setCharacterSize(skillTreeController.Level, (int)(40 * ratioX));
		sfText_setPosition(skillTreeController.Description, (sfVector2f) { 1250.f * ratioX, 175.f * ratioY });
		sfText_setPosition(skillTreeController.Information, (sfVector2f) { 1250.f * ratioX, 420.f * ratioY });
	}
	for (int i = 0; i < NBRBRANCH; i++)
	{
		sfSprite_setScale(skillTreeController.Selector[i], (sfVector2f) { ratioX, ratioY });
		for (int j = 0; j < NBRTALENTPERBRANCH; j++)
		{
			for (int k = 0; k < 2; k++)
			{
				sfSprite_setScale(skillTreeController.Sprite[i][j][k], (sfVector2f) { ratioX, ratioY });
			}
		}
	}

	sfSprite_setScale(skillTreeController.GoldCircle, (sfVector2f) { ratioX, ratioY });
	sfSprite_setScale(skillTreeController.Background, (sfVector2f) { ratioX, ratioY });
	sfSprite_setScale(skillTreeController.Border, (sfVector2f) { ratioX, ratioY });

	//sfSprite_setPosition(skillTreeController.Branchs[0].Sprite, (sfVector2f) { 289.5f * ratioX, 453.f * ratioY });
	//sfSprite_setPosition(skillTreeController.Branchs[1].Sprite, (sfVector2f) { 551.f * ratioX, 459.5f * ratioY });
	//sfSprite_setPosition(skillTreeController.Branchs[2].Sprite, (sfVector2f) { 762.f * ratioX, 695.5f * ratioY });
	//sfSprite_setPosition(skillTreeController.Branchs[3].Sprite, (sfVector2f) { 884.5f * ratioX, 826.5f * ratioY });


	int cpt = 0;
	int cpt2 = 0;
	for (int i = 0; i < NBRBRANCH * 4; i++)
	{
		if (cpt2 > 3)
		{
			cpt++;
			cpt2 = 0;
		}

		sfVector2u size = sfTexture_getSize(skillTreeController.TextureBranchs[cpt][cpt2]);
		sfVector2f sizeTexture = { size.x * ratioX, size.y * ratioY };


		sfVector2f branchTextureSize = { (float)size.x, (float)size.y };
		sfVertex v;
		sfBool changeToX = sfFalse;
		skillTreeController.Branchs[i] = (sfRenderStates*)calloc(1, sizeof(sfRenderStates));
		skillTreeController.Branchs[i]->blendMode = sfBlendAlpha;
		skillTreeController.Branchs[i]->shader = NULL;
		skillTreeController.Branchs[i]->texture = sfTexture_copy(skillTreeController.TextureBranchs[cpt][cpt2]);
		skillTreeController.Branchs[i]->transform = sfTransform_Identity;
		if (sfTexture_getSize(skillTreeController.TextureBranchs[cpt][cpt2]).x > sfTexture_getSize(skillTreeController.TextureBranchs[cpt][cpt2]).y)
		{
			changeToX = sfTrue;
		}
		sfTransform_scale(&skillTreeController.Branchs[i]->transform, ratioX, ratioY);
		switch (i)
		{
		case 0:
			v = (sfVertex) { (sfVector2f) { (266.f - branchTextureSize.x / 2.f), (728.5f + branchTextureSize.y / 2.f) }, sfWhite, (sfVector2f) { 0.0f, branchTextureSize.y } };
			break;
		case 1:
			v = (sfVertex) { (sfVector2f) { 428.f - (branchTextureSize.x / 2), 239.5f + (branchTextureSize.y / 2) }, sfWhite, (sfVector2f) { 0.0f, branchTextureSize.y } };
			break;
		case 2:
			v = (sfVertex) { (sfVector2f) { 248.5f + (branchTextureSize.x / 2), 206.5f - (branchTextureSize.y / 2) }, sfWhite, (sfVector2f) { branchTextureSize.x, 0.f } };
			break;
		case 3:
			v = (sfVertex) { (sfVector2f) { 139.f - (branchTextureSize.x / 2), 334.5f - (branchTextureSize.y / 2) }, sfWhite, (sfVector2f) { 0.0f, 0.f } };
			break;
		case 4:
			v = (sfVertex) { (sfVector2f) { 278.5f - (branchTextureSize.x / 2), 777.5f + (branchTextureSize.y / 2) }, sfWhite, (sfVector2f) { 0.0f, branchTextureSize.y } };
			break;
		case 5:
			v = (sfVertex) { (sfVector2f) { 689.5f - (branchTextureSize.x / 2), 506.5f - (branchTextureSize.y / 2) }, sfWhite, (sfVector2f) { 0.0f, 0.f } };
			break;
		case 6:
			v = (sfVertex) { (sfVector2f) { 813.f - (branchTextureSize.x / 2), 311.5f + (branchTextureSize.y / 2) }, sfWhite, (sfVector2f) { 0.0f, branchTextureSize.y } };
			break;
		case 7:
			v = (sfVertex) { (sfVector2f) { 688.5f + (branchTextureSize.x / 2), 160.f - (branchTextureSize.y / 2) }, sfWhite, (sfVector2f) { branchTextureSize.x, 0.f } };
			break;
		case 8:
			v = (sfVertex) { (sfVector2f) { 480.5f - (branchTextureSize.x / 2), 849.f + (branchTextureSize.y / 2) }, sfWhite, (sfVector2f) { 0.0f, branchTextureSize.y } };
			break;
		case 9:
			v = (sfVertex) { (sfVector2f) { 880.5f - (branchTextureSize.x / 2), 558.5f - (branchTextureSize.y / 2) }, sfWhite, (sfVector2f) { 0.0f, 0.f } };
			break;
		case 10:
			v = (sfVertex) { (sfVector2f) { 1109.5f - (branchTextureSize.x / 2), 613.5f - (branchTextureSize.y / 2) }, sfWhite, (sfVector2f) { 0.0f, 0.f } };
			break;
		case 11:
			v = (sfVertex) { (sfVector2f) { 919.5f + (branchTextureSize.x / 2), 709.f - (branchTextureSize.y / 2) }, sfWhite, (sfVector2f) { branchTextureSize.x, 0.f } };
			break;
		case 12:
			v = (sfVertex) { (sfVector2f) { 605.f - (branchTextureSize.x / 2), 977.5f + (branchTextureSize.y / 2) }, sfWhite, (sfVector2f) { 0.0f, branchTextureSize.y } };
			break;
		case 13:
			v = (sfVertex) { (sfVector2f) { 1093.f - (branchTextureSize.x / 2), 925.f - (branchTextureSize.y / 2) }, sfWhite, (sfVector2f) { 0.0f, 0.f } };
			break;
		case 14:
			v = (sfVertex) { (sfVector2f) { 1350.5f - (branchTextureSize.x / 2), 859.5f - (branchTextureSize.y / 2) }, sfWhite, (sfVector2f) { 0.0f, 0.f } };
			break;
		case 15:
			v = (sfVertex) { (sfVector2f) { 1271.5f + (branchTextureSize.x / 2), 718.5f - (branchTextureSize.y / 2) }, sfWhite, (sfVector2f) { branchTextureSize.x, 0.f } };
			break;
		}

		if (cpt2 == 0)
		{
			sfVertexArray_append(skillTreeController.VertexBranchs[i], v);
			v.position.y -= (sfVector2f) { v.position.x, branchTextureSize.y }.y;
			v.texCoords.y -= branchTextureSize.y;
			sfVertexArray_append(skillTreeController.VertexBranchs[i], v);
			v.position.x += (sfVector2f) { branchTextureSize.x, v.position.y }.x;
			v.texCoords.x += branchTextureSize.x;
			sfVertexArray_append(skillTreeController.VertexBranchs[i], v);
			v.position.y += (sfVector2f) { v.position.x, branchTextureSize.y }.y;
			v.texCoords.y += branchTextureSize.y;
			sfVertexArray_append(skillTreeController.VertexBranchs[i], v);
		}
		else if (changeToX)
		{
			if (i == 2 || i == 7 || i == 11 || i == 15)
			{
				sfVertexArray_append(skillTreeController.VertexBranchs[i], v);

				v.position.y += (sfVector2f) { v.position.x, branchTextureSize.y }.y;
				v.texCoords.y += branchTextureSize.y;
				sfVertexArray_append(skillTreeController.VertexBranchs[i], v);

				sfVertexArray_append(skillTreeController.VertexBranchs[i], v);

				v.position.y -= (sfVector2f) { v.position.x, branchTextureSize.y }.y;
				v.texCoords.y -= branchTextureSize.y;
				sfVertexArray_append(skillTreeController.VertexBranchs[i], v);
			}
			else
			{
				sfVertexArray_append(skillTreeController.VertexBranchs[i], v);

				sfVertexArray_append(skillTreeController.VertexBranchs[i], v);

				v.position.y += (sfVector2f) { v.position.x, branchTextureSize.y }.y;
				v.texCoords.y += branchTextureSize.y;
				sfVertexArray_append(skillTreeController.VertexBranchs[i], v);

				sfVertexArray_append(skillTreeController.VertexBranchs[i], v);
			}
		}
		else
		{
			if (i == 3 || i == 10)
			{
				sfVertexArray_append(skillTreeController.VertexBranchs[i], v);

				v.position.x += (sfVector2f) { branchTextureSize.x, v.position.y }.x;
				v.texCoords.x += branchTextureSize.x;
				sfVertexArray_append(skillTreeController.VertexBranchs[i], v);

				sfVertexArray_append(skillTreeController.VertexBranchs[i], v);
				v.position.x -= (sfVector2f) { branchTextureSize.x, v.position.y }.x;
				v.texCoords.x -= branchTextureSize.x;
				sfVertexArray_append(skillTreeController.VertexBranchs[i], v);
			}
			else
			{
				sfVertexArray_append(skillTreeController.VertexBranchs[i], v);

				sfVertexArray_append(skillTreeController.VertexBranchs[i], v);

				v.position.x += (sfVector2f) { branchTextureSize.x, v.position.y }.x;
				v.texCoords.x += branchTextureSize.x;
				sfVertexArray_append(skillTreeController.VertexBranchs[i], v);

				sfVertexArray_append(skillTreeController.VertexBranchs[i], v);
			}
		}
		cpt2++;
	}
}

void ChangeTalentToUpdate(const int branch, const int talent)
{
	TalentToUpdate[branch][talent] = sfTrue;
}

void UpdateSkillTree(const SkillTree* skillTree, const float dt)
{
	if (skillTree->IsOpen)
	{
		for (int i = 0; i < NBRBRANCH; i++)
		{
			for (int j = 1; j <= NBRTALENTPERBRANCH; j++)
			{
				if (TalentToUpdate[i][j - 1])
				{
					int tempIndex = i * (NBRTALENTPERBRANCH + 1) + j;
					sfVertex* origin = sfVertexArray_getVertex(skillTreeController.VertexBranchs[tempIndex], 0);
					sfVertex* origin2 = sfVertexArray_getVertex(skillTreeController.VertexBranchs[tempIndex], 3);
					sfVertex* temp1;
					sfVertex* temp2;
					sfVector2u size = sfTexture_getSize(skillTreeController.TextureBranchs[i][j]);
					float sizeBranchX = (float)size.x;
					float sizeBranchY = (float)size.y;
					if (tempIndex == 3 || tempIndex == 10 || tempIndex == 2 || tempIndex == 7 || tempIndex == 11 || tempIndex == 15)
					{
						temp1 = sfVertexArray_getVertex(skillTreeController.VertexBranchs[tempIndex], 2);
						temp2 = sfVertexArray_getVertex(skillTreeController.VertexBranchs[tempIndex], 3);

					}
					else
					{
						temp1 = sfVertexArray_getVertex(skillTreeController.VertexBranchs[tempIndex], 1);
						temp2 = sfVertexArray_getVertex(skillTreeController.VertexBranchs[tempIndex], 2);
					}

					if (tempIndex == 1 || tempIndex == 6)
					{
						temp1->position.y -= 100.f * dt; temp2->position.y -= 100.f * dt;
						temp1->texCoords.y -= 100.f * dt; temp2->texCoords.y -= 100.f * dt;
					}
					else if (tempIndex == 3 || tempIndex == 10)
					{
						temp1->position.y += 100.f * dt; temp2->position.y += 100.f * dt;
						temp1->texCoords.y += 100.f * dt; temp2->texCoords.y += 100.f * dt;
					}
					else if (tempIndex == 2 || tempIndex == 7 || tempIndex == 11 || tempIndex == 15)
					{
						temp1->position.x -= 100.f * dt; temp2->position.x -= 100.f * dt;
						temp1->texCoords.x -= 100.f * dt; temp2->texCoords.x -= 100.f * dt;
					}
					else
					{
						temp1->position.x += 100.f * dt; temp2->position.x += 100.f * dt;
						temp1->texCoords.x += 100.f * dt; temp2->texCoords.x += 100.f * dt;
					}
				}
			}
		}
	}
}

void ChangeTextSkillTree(const SkillTree* skillTree, const unsigned int playerXp)
{
	int indexLocalization = skillTree->IndexBranch * 3 + skillTree->IndexTalent + 159;
	sfText_setString(skillTreeController.Description, GetLocalizationString( indexLocalization));
	if (skillTree->Branch[skillTree->IndexBranch].Talent[skillTree->IndexTalent].ActualUpgrade == skillTree->Branch[skillTree->IndexBranch].Talent[skillTree->IndexTalent].NbrUpgradeMax)
	{
		sfText_setString(skillTreeController.Information, GetLocalizationString(172));
	}
	else if (skillTree->IndexTalent == 0 || skillTree->Branch[skillTree->IndexBranch].Talent[skillTree->IndexTalent - 1].ActualUpgrade >= skillTree->Branch[skillTree->IndexBranch].Talent[skillTree->IndexTalent - 1].NbrUpgradeToUnlockNextOne)
	{
		char str[50];
		sprintf(str, "%s : %u\n%s : %u", GetLocalizationString( 173), skillTree->Branch[skillTree->IndexBranch].CostUpgrade, GetLocalizationString(174), playerXp);
		sfText_setString(skillTreeController.Information, str);
	}
	else

	{
		sfText_setString(skillTreeController.Information, GetLocalizationString( 171));
	}
}

void DisplaySkillTree(sfRenderWindow* window, const SkillTree* skillTree)
{
	if (skillTree == NULL || !skillTree->IsOpen)
	{
		return;
	}
	BlitSpriteNoRS(window, skillTreeController.Background, (sfVector2f) { 0.f, 0.f });
	for (int i = 0; i < NBRBRANCH; i++)
	{
		for (int j = 0; j < NBRTALENTPERBRANCH; j++)
		{
			BlitSpriteNoRS(window, skillTreeController.Sprite[i][j][0], skillTreeController.PosTalent[i][j]);
			if (skillTree->Branch[i].Talent[j].ActualUpgrade == skillTree->Branch[i].Talent[j].NbrUpgradeMax)
			{
				BlitSpriteNoRS(window, skillTreeController.Sprite[i][j][1], skillTreeController.PosTalent[i][j]);
			}

			if (skillTree->Branch[i].Talent[j].ActualUpgrade >= skillTree->Branch[i].Talent[j].NbrUpgradeToUnlockNextOne)
			{
				BlitSpriteNoRS(window, skillTreeController.GoldCircle, skillTreeController.PosTalent[i][j]);
			}
		}
	}
	BlitSpriteNoRS(window, skillTreeController.Selector[skillTree->IndexBranch], skillTreeController.PosTalent[skillTree->IndexBranch][skillTree->IndexTalent]);

	for (int i = 0; i < 4 * NBRBRANCH; i++)
	{
		sfRenderWindow_drawVertexArray(window, skillTreeController.VertexBranchs[i], skillTreeController.Branchs[i]);
	}
	for (int i = 0; i < NBRBRANCH; i++)
	{
		sfBool isUnlock = sfTrue;
		for (int j = 0; j < NBRTALENTPERBRANCH; j++)
		{
			char str[20] = "";
			sprintf(str, "%d/%d", skillTree->Branch[i].Talent[j].ActualUpgrade, skillTree->Branch[i].Talent[j].NbrUpgradeMax);
			sfText_setString(skillTreeController.Level, str);
			sfText_setPosition(skillTreeController.Level, (sfVector2f) { skillTreeController.PosTalent[i][j].x, skillTreeController.PosTalent[i][j].y + 85.f * ratioY });
			CenterText(skillTreeController.Level, (sfVector2f) { skillTreeController.PosTalent[i][j].x, skillTreeController.PosTalent[i][j].y + 85.f * ratioY });
			if (isUnlock)
			{
				sfRenderWindow_drawText(window, (const sfText*)skillTreeController.Level, NULL);
			}
			if (skillTree->Branch[i].Talent[j].ActualUpgrade < skillTree->Branch[i].Talent[j].NbrUpgradeToUnlockNextOne)
			{
				isUnlock = sfFalse;
			}
		}
	}

	BlitSpriteNoRS(window, skillTreeController.Border, (sfVector2f) { 0.f, 0.f });
	sfRenderWindow_drawText(window, (const sfText*)skillTreeController.Description, NULL);
	sfRenderWindow_drawText(window, (const sfText*)skillTreeController.Information, NULL);
}