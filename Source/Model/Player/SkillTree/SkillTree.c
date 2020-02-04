#include "SkillTree.h"

SkillTree* pointerSkillTree[2] = { NULL, NULL };

void InitPointerSkillTree(SkillTree* skillTree, const int idPlayer)
{
	pointerSkillTree[idPlayer] = skillTree;
}

SkillTree* GetPointerToSkillTree(const int idPlayer)
{
	return pointerSkillTree[idPlayer];
}

void InitSkillTree(SkillTree* skillTree)
{
	SkillTree tempSkillTree = { 0 };
	(*skillTree) = tempSkillTree;

	skillTree->Branch[lifeBranch].Talent[regenLife].ActualUpgrade = 0;
	skillTree->Branch[lifeBranch].Talent[regenLife].NbrUpgradeMax = 5;
	skillTree->Branch[lifeBranch].Talent[regenLife].NbrUpgradeToUnlockNextOne = 3;
	skillTree->Branch[lifeBranch].Talent[regenLife].PercentageBuffPerUpgrade = 20.f;

	skillTree->Branch[lifeBranch].Talent[moreInvulnerability].ActualUpgrade = 0;
	skillTree->Branch[lifeBranch].Talent[moreInvulnerability].NbrUpgradeMax = 3;
	skillTree->Branch[lifeBranch].Talent[moreInvulnerability].NbrUpgradeToUnlockNextOne = 1;
	skillTree->Branch[lifeBranch].Talent[moreInvulnerability].PercentageBuffPerUpgrade = 10.f;

	skillTree->Branch[lifeBranch].Talent[moreLifeMax].ActualUpgrade = 0;
	skillTree->Branch[lifeBranch].Talent[moreLifeMax].NbrUpgradeMax = 100;
	skillTree->Branch[lifeBranch].Talent[moreLifeMax].NbrUpgradeToUnlockNextOne = 1;
	skillTree->Branch[lifeBranch].Talent[moreLifeMax].PercentageBuffPerUpgrade = 10.f;

	skillTree->Branch[manaBranch].Talent[regenMana].ActualUpgrade = 0;
	skillTree->Branch[manaBranch].Talent[regenMana].NbrUpgradeMax = 5;
	skillTree->Branch[manaBranch].Talent[regenMana].NbrUpgradeToUnlockNextOne = 3;
	skillTree->Branch[manaBranch].Talent[regenMana].PercentageBuffPerUpgrade = 20.f;

	skillTree->Branch[manaBranch].Talent[lessCostMana].ActualUpgrade = 0;
	skillTree->Branch[manaBranch].Talent[lessCostMana].NbrUpgradeMax = 5;
	skillTree->Branch[manaBranch].Talent[lessCostMana].NbrUpgradeToUnlockNextOne = 3;
	skillTree->Branch[manaBranch].Talent[lessCostMana].PercentageBuffPerUpgrade = 10.f;

	skillTree->Branch[manaBranch].Talent[moreManaMax].ActualUpgrade = 0;
	skillTree->Branch[manaBranch].Talent[moreManaMax].NbrUpgradeMax = 100;
	skillTree->Branch[manaBranch].Talent[moreManaMax].NbrUpgradeToUnlockNextOne = 1;
	skillTree->Branch[manaBranch].Talent[moreManaMax].PercentageBuffPerUpgrade = 10.f;



	skillTree->Branch[mobilityBranch].Talent[speedBuff].ActualUpgrade = 0;
	skillTree->Branch[mobilityBranch].Talent[speedBuff].NbrUpgradeMax = 5;
	skillTree->Branch[mobilityBranch].Talent[speedBuff].NbrUpgradeToUnlockNextOne = 3;
	skillTree->Branch[mobilityBranch].Talent[speedBuff].PercentageBuffPerUpgrade = 5.f;

	skillTree->Branch[mobilityBranch].Talent[lessSlowDown].ActualUpgrade = 0;
	skillTree->Branch[mobilityBranch].Talent[lessSlowDown].NbrUpgradeMax = 5;
	skillTree->Branch[mobilityBranch].Talent[lessSlowDown].NbrUpgradeToUnlockNextOne = 3;
	skillTree->Branch[mobilityBranch].Talent[lessSlowDown].PercentageBuffPerUpgrade = 5.f;

	skillTree->Branch[mobilityBranch].Talent[doubleDash].ActualUpgrade = 0;
	skillTree->Branch[mobilityBranch].Talent[doubleDash].NbrUpgradeMax = 1;
	skillTree->Branch[mobilityBranch].Talent[doubleDash].NbrUpgradeToUnlockNextOne = 1;
	skillTree->Branch[mobilityBranch].Talent[doubleDash].PercentageBuffPerUpgrade = 0.f;



	skillTree->Branch[damageBranch].Talent[unlockConeOfFire].ActualUpgrade = 0;
	skillTree->Branch[damageBranch].Talent[unlockConeOfFire].NbrUpgradeMax = 1;
	skillTree->Branch[damageBranch].Talent[unlockConeOfFire].NbrUpgradeToUnlockNextOne = 1;
	skillTree->Branch[damageBranch].Talent[unlockConeOfFire].PercentageBuffPerUpgrade = 0.f;

	skillTree->Branch[damageBranch].Talent[upgradeFireBall].ActualUpgrade = 0;
	skillTree->Branch[damageBranch].Talent[upgradeFireBall].NbrUpgradeMax = 1;
	skillTree->Branch[damageBranch].Talent[upgradeFireBall].NbrUpgradeToUnlockNextOne = 1;
	skillTree->Branch[damageBranch].Talent[upgradeFireBall].PercentageBuffPerUpgrade = 0.f;

	skillTree->Branch[damageBranch].Talent[moreDamage].ActualUpgrade = 0;
	skillTree->Branch[damageBranch].Talent[moreDamage].NbrUpgradeMax = 100;
	skillTree->Branch[damageBranch].Talent[moreDamage].NbrUpgradeToUnlockNextOne = 1;
	skillTree->Branch[damageBranch].Talent[moreDamage].PercentageBuffPerUpgrade = 5.f;

	skillTree->IsOpen = sfFalse;

	for (int i = 0; i < NBRBRANCH; i++)
	{
		int nbrUpgrade = 0;
		int cost = 100;
		for (int j = 0; j < NBRTALENTPERBRANCH; j++)
		{
			nbrUpgrade += skillTree->Branch[i].Talent[j].ActualUpgrade;
			cost += skillTree->Branch[i].Talent[j].ActualUpgrade * 100;
			if (skillTree->Branch[i].Talent[j].ActualUpgrade < skillTree->Branch[i].Talent[j].NbrUpgradeToUnlockNextOne)
			{
				j = NBRTALENTPERBRANCH;
			}
		}
		skillTree->Branch[i].CostUpgrade = cost;
		skillTree->Branch[i].NbrUpgrade = nbrUpgrade;
	}
}

void UpgradeAtMaxAllSkillTree(SkillTree* skillTree)
{
	for (int i = 0; i < NBRBRANCH; i++)
	{
		for (int j = 0; j < NBRTALENTPERBRANCH; j++)
		{
			skillTree->Branch[i].Talent[j].ActualUpgrade = skillTree->Branch[i].Talent[j].NbrUpgradeMax;
		}
	}
	for (int i = 0; i < NBRBRANCH; i++)
	{
		int nbrUpgrade = 0;
		int cost = 100;
		for (int j = 0; j < NBRTALENTPERBRANCH; j++)
		{
			nbrUpgrade += skillTree->Branch[i].Talent[j].ActualUpgrade;
			cost += skillTree->Branch[i].Talent[j].ActualUpgrade * 100;
			if (skillTree->Branch[i].Talent[j].ActualUpgrade < skillTree->Branch[i].Talent[j].NbrUpgradeToUnlockNextOne)
			{
				j = NBRTALENTPERBRANCH;
			}
		}
		skillTree->Branch[i].CostUpgrade = cost;
		skillTree->Branch[i].NbrUpgrade = nbrUpgrade;
	}
}

int UpgradeTalent(SkillTree* skillTree, unsigned int* playerXP)
{
	sfBool haveUnlockedNextTalent = sfFalse;
	if ((*playerXP) >= (int)skillTree->Branch[skillTree->IndexBranch].CostUpgrade &&
		(skillTree->IndexTalent == 0 ||
			skillTree->Branch[skillTree->IndexBranch].Talent[skillTree->IndexTalent - 1].ActualUpgrade >= skillTree->Branch[skillTree->IndexBranch].Talent[skillTree->IndexTalent - 1].NbrUpgradeToUnlockNextOne) &&
		skillTree->Branch[skillTree->IndexBranch].Talent[skillTree->IndexTalent].ActualUpgrade < skillTree->Branch[skillTree->IndexBranch].Talent[skillTree->IndexTalent].NbrUpgradeMax)
	{
		(*playerXP) -= skillTree->Branch[skillTree->IndexBranch].CostUpgrade;
		skillTree->Branch[skillTree->IndexBranch].CostUpgrade += 100;
		skillTree->Branch[skillTree->IndexBranch].NbrUpgrade++;
		skillTree->Branch[skillTree->IndexBranch].Talent[skillTree->IndexTalent].ActualUpgrade++;
		if (skillTree->Branch[skillTree->IndexBranch].Talent[skillTree->IndexTalent].ActualUpgrade == skillTree->Branch[skillTree->IndexBranch].Talent[skillTree->IndexTalent].NbrUpgradeToUnlockNextOne)
		{
			haveUnlockedNextTalent = sfTrue;
		}
	}
	return haveUnlockedNextTalent;
}
