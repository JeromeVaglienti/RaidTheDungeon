#ifndef _SKILLTREECONTROLLER_H
#define _SKILLTREECONTROLLER_H
#include "..\..\..\Model\Player\Player.h"

typedef struct
{
	sfSprite* Sprite[NBRBRANCH][NBRTALENTPERBRANCH][2];
	sfSprite* Background;
	sfSprite* Selector[NBRBRANCH];
	sfSprite* GoldCircle;
	sfSprite* Border;
	sfSprite* BranchsSprite[NBRBRANCH * 4];
	sfTexture* TextureBranchs[NBRBRANCH][4];
	sfRenderStates* Branchs[NBRBRANCH * 4];
	sfVertexArray* VertexBranchs[NBRBRANCH * 4];
	sfVector2f PosTalent[NBRBRANCH][NBRTALENTPERBRANCH];
	sfText* Description;
	sfText* Information;
	sfText* Level;
}SkillTreeController;

void InitSkillTreeController(const sfVideoMode mode);
void UpdateSkillTree(const SkillTree* skillTree, const float dt);
void ChangeTextSkillTree(const SkillTree* skillTree, const unsigned int playerXp);
void ChangeTalentToUpdate(const int branch, const int talent);
void DisplaySkillTree(sfRenderWindow* window, const SkillTree* skillTree);

#endif // !_SKILLTREECONTROLLER_H
