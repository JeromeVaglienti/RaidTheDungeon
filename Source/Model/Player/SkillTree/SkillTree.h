#ifndef _SKILLTREE_H
#define _SKILLTREE_H
#include <stdlib.h>
#include <SFML\Window.h>
#define NBRBRANCH 4
#define NBRTALENTPERBRANCH 3
typedef enum IdBranch
{
	lifeBranch,
	manaBranch,
	damageBranch,
	mobilityBranch
}IdBranch;

typedef enum IdSpeedBranch
{
	speedBuff,
	lessSlowDown,
	doubleDash
}IdSpeedBranch;

typedef enum IdManaBranch
{
	regenMana,
	lessCostMana,
	moreManaMax
}IdManaBranch;

typedef enum IdDamageBranch
{
	unlockConeOfFire,
	upgradeFireBall,
	moreDamage
}IdDamageBranch;

typedef enum IdLifeBranch
{
	regenLife,
	moreInvulnerability,
	moreLifeMax
}IdLifeBranch;

typedef struct Talent
{
	unsigned char NbrUpgradeMax;
	unsigned char ActualUpgrade;
	unsigned char NbrUpgradeToUnlockNextOne;
	float PercentageBuffPerUpgrade;
}Talent;

typedef struct Branch
{
	int NbrUpgrade;
	unsigned int CostUpgrade;
	Talent Talent[NBRTALENTPERBRANCH];
}Branch;

typedef struct SkillTree
{
	Branch Branch[NBRBRANCH];
	sfBool IsOpen;
	short IndexBranch;
	short IndexTalent;
}SkillTree;





void InitPointerSkillTree(SkillTree* skillTree, const int idPlayer);
SkillTree* GetPointerToSkillTree(const int idPlayer);
void InitSkillTree(SkillTree* skillTree);
void UpgradeAtMaxAllSkillTree(SkillTree* skillTree);
int UpgradeTalent(SkillTree* skillTree, unsigned int* playerXP);

#endif // !_SKILLTREE_H
