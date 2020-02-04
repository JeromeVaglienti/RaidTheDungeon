#include "Enemy.h"
#include "..\Trap\Trap.h"
#include "..\..\Controller\Sounds\Sounds.h"
#include "..\..\DevTools\NodeDisplayer.h"
#include "..\..\Controller\State\State.h"
#include <Windows.h>

StatEnemy statEnemiesSlime[7] = { 0 };
StatEnemy statEnemiesElem[9] = { 0 };

void KamikazeExplosion(Enemy* firstEnemy, Enemy* enemyWhoExplode, Player player[], const short nbrOfPlayer, Shot** firstShot);
void PaternSlime(Player player[], const short nbrOfPlayer, Enemy* enemy, Shot** firstShot, const float deltaTime);
void PaternElem(const Player player[], const short nbrOfPlayer, Enemy* enemy, Shot** firstShot, const float deltaTime, const sfVector2f posRoom, const sfVector2f sizeRoom);

void InitStatEnemies()
{
	statEnemiesSlime[distance].Damage = 1;
	statEnemiesSlime[distance].Life = 15;
	statEnemiesSlime[distance].radius = 40.f;
	statEnemiesSlime[distance].Speed = 150.f;
	statEnemiesSlime[distance].CooldownShot = 1.f;
	statEnemiesSlime[distance].XPGives = 2;
	statEnemiesSlime[cac].Damage = 4;
	statEnemiesSlime[cac].Life = 25;
	statEnemiesSlime[cac].radius = 46.f;
	statEnemiesSlime[cac].Speed = 200.f;
	statEnemiesSlime[cac].CooldownShot = 0.f;
	statEnemiesSlime[cac].XPGives = 2;
	statEnemiesSlime[kamikaze].Damage = 7;
	statEnemiesSlime[kamikaze].Life = 10;
	statEnemiesSlime[kamikaze].radius = 49.f;
	statEnemiesSlime[kamikaze].Speed = 400.f;
	statEnemiesSlime[kamikaze].CooldownShot = 0.f;
	statEnemiesSlime[kamikaze].XPGives = 4;
	statEnemiesSlime[miniBoss].Damage = 4;
	statEnemiesSlime[miniBoss].Life = 150;
	statEnemiesSlime[miniBoss].radius = 100.f;
	statEnemiesSlime[miniBoss].Speed = 150.f;
	statEnemiesSlime[miniBoss].CooldownShot = 2.f;
	statEnemiesSlime[miniBoss].XPGives = 40;
	statEnemiesSlime[distanceUpgrade].Damage = 2;
	statEnemiesSlime[distanceUpgrade].Life = 30;
	statEnemiesSlime[distanceUpgrade].radius = 40.f * 1.5f;
	statEnemiesSlime[distanceUpgrade].Speed = 150.f;
	statEnemiesSlime[distanceUpgrade].CooldownShot = 2.f;
	statEnemiesSlime[distanceUpgrade].XPGives = 4;
	statEnemiesSlime[cacUpgrade].Damage = 8;
	statEnemiesSlime[cacUpgrade].Life = 50;
	statEnemiesSlime[cacUpgrade].radius = 46.f * 1.5f;
	statEnemiesSlime[cacUpgrade].Speed = 200.f;
	statEnemiesSlime[cacUpgrade].CooldownShot = 0.f;
	statEnemiesSlime[cacUpgrade].XPGives = 0;
	statEnemiesSlime[kamikazeUpgrade].Damage = 7;
	statEnemiesSlime[kamikazeUpgrade].Life = 15;
	statEnemiesSlime[kamikazeUpgrade].radius = 49.f * 1.5f;
	statEnemiesSlime[kamikazeUpgrade].Speed = 400.f;
	statEnemiesSlime[kamikazeUpgrade].CooldownShot = 0.f;
	statEnemiesSlime[kamikazeUpgrade].XPGives = 8;

	statEnemiesElem[fireElem].Damage = 2;
	statEnemiesElem[fireElem].Life = 25;
	statEnemiesElem[fireElem].radius = 49.f * 1.5f;
	statEnemiesElem[fireElem].Speed = 200.f;
	statEnemiesElem[fireElem].CooldownShot = 2.f;
	statEnemiesElem[fireElem].XPGives = 15;

	statEnemiesElem[waterElem].Damage = 2;
	statEnemiesElem[waterElem].Life = 25;
	statEnemiesElem[waterElem].radius = 49.f * 1.5f;
	statEnemiesElem[waterElem].Speed = 200.f;
	statEnemiesElem[waterElem].CooldownShot = 2.f;
	statEnemiesElem[waterElem].XPGives = 15;

	statEnemiesElem[windElem].Damage = 2;
	statEnemiesElem[windElem].Life = 25;
	statEnemiesElem[windElem].radius = 49.f * 1.5f;
	statEnemiesElem[windElem].Speed = 200.f;
	statEnemiesElem[windElem].CooldownShot = 2.f;
	statEnemiesElem[windElem].XPGives = 15;

	statEnemiesElem[dirtElem].Damage = 2;
	statEnemiesElem[dirtElem].Life = 25;
	statEnemiesElem[dirtElem].radius = 49.f * 1.5f;
	statEnemiesElem[dirtElem].Speed = 200.f;
	statEnemiesElem[dirtElem].CooldownShot = 2.f;
	statEnemiesElem[dirtElem].XPGives = 15;

	statEnemiesElem[lavaElem].Damage = 3;
	statEnemiesElem[lavaElem].Life = 60;
	statEnemiesElem[lavaElem].radius = 49.f * 1.5f;
	statEnemiesElem[lavaElem].Speed = 200.f;
	statEnemiesElem[lavaElem].CooldownShot = 3.f;
	statEnemiesElem[lavaElem].XPGives = 30;

	statEnemiesElem[iceElem].Damage = 4;
	statEnemiesElem[iceElem].Life = 60;
	statEnemiesElem[iceElem].radius = 49.f * 1.5f;
	statEnemiesElem[iceElem].Speed = 200.f;
	statEnemiesElem[iceElem].CooldownShot = 2.f;
	statEnemiesElem[iceElem].XPGives = 30;

	statEnemiesElem[stormElem].Damage = 3;
	statEnemiesElem[stormElem].Life = 50;
	statEnemiesElem[stormElem].radius = 49.f * 1.5f;
	statEnemiesElem[stormElem].Speed = 200.f;
	statEnemiesElem[stormElem].CooldownShot = 0.3f;
	statEnemiesElem[stormElem].XPGives = 30;

	statEnemiesElem[rockElem].Damage = 3;
	statEnemiesElem[rockElem].Life = 70;
	statEnemiesElem[rockElem].radius = 49.f * 1.5f;
	statEnemiesElem[rockElem].Speed = 200.f;
	statEnemiesElem[rockElem].CooldownShot = 2.f;
	statEnemiesElem[rockElem].XPGives = 30;

	statEnemiesElem[littleElemOfLava].Damage = 4;
	statEnemiesElem[littleElemOfLava].Life = 10;
	statEnemiesElem[littleElemOfLava].radius = 49.f;
	statEnemiesElem[littleElemOfLava].Speed = 400.f;
	statEnemiesElem[littleElemOfLava].CooldownShot = 2.f;
	statEnemiesElem[littleElemOfLava].XPGives = 0;

};

// add one anemy at the linked list
void AddEnemy(Enemy** firstEnemy, const int id, const sfVector2f pos, Node* nodeArray)
{

	Enemy* tempEnemy = (Enemy*)calloc(1, sizeof(Enemy));
	DungeonId tempId = GetActualDungeonId();
	tempEnemy->Circle.center = pos;
	if (tempId == slimeDungeon || tempId == beginningDungeon)
	{
		tempEnemy->Circle.radius = statEnemiesSlime[id].radius;
		tempEnemy->Speed = statEnemiesSlime[id].Speed;
		tempEnemy->Life = statEnemiesSlime[id].Life;
		tempEnemy->Damage = statEnemiesSlime[id].Damage;
		tempEnemy->XPGives = statEnemiesSlime[id].XPGives;
	}
	else if (tempId == elemDungeon)
	{
		tempEnemy->Circle.radius = statEnemiesElem[id].radius;
		tempEnemy->Speed = statEnemiesElem[id].Speed;
		tempEnemy->Life = statEnemiesElem[id].Life;
		tempEnemy->Damage = statEnemiesElem[id].Damage;
		tempEnemy->XPGives = statEnemiesElem[id].XPGives;
	}
	tempEnemy->RigidBody.Position.x = pos.x;
	tempEnemy->RigidBody.Position.y = pos.y;
	tempEnemy->RigidBody.Mass = 1;
	tempEnemy->Id = id;
	tempEnemy->DirectionAnimation = idle;
	tempEnemy->IsMoving = sfFalse;
	tempEnemy->IsHit = sfFalse;
	tempEnemy->timer.Hit = 0;
	tempEnemy->TimerSideWalk = rand() / (float)RAND_MAX * 1.5f + 0.5f;
	tempEnemy->DirectionSideWalk = (rand() % 2) ? 1 : -1;
	tempEnemy->CooldownShot = 2.f + ((float)rand() / RAND_MAX) * 2.f;
	if (tempId == slimeDungeon || tempId == beginningDungeon)
	{
		tempEnemy->DeathAnimKey = InitAnimKey(10, 0.7f, sfFalse);
		if (tempEnemy->Id == cac || tempEnemy->Id == cacUpgrade)
		{
			tempEnemy->MoveAnimKey = InitAnimKey(12, 1.0f, sfTrue);
		}
		tempEnemy->SpawnAnimKey = InitAnimKey(11, 1.f, sfFalse);
	}
	else if (tempId == elemDungeon)
	{
		tempEnemy->DeathAnimKey = InitAnimKey(24, 0.5f, sfFalse);
		tempEnemy->SpawnAnimKey = InitAnimKey(24, 2.f, sfFalse);
		tempEnemy->MoveAnimKey = InitAnimKey(12, 1.0f, sfTrue);
	}
	tempEnemy->AttackAnimKey = InitAnimKey(12, 0.5f, sfFalse);
	tempEnemy->MustShot = sfTrue;
	tempEnemy->TransitionTimer = 0.0f;
	tempEnemy->IsTransitioning = sfFalse;
	tempEnemy->TransitionDirection = 0;
	if (tempId == elemDungeon && id == littleElemOfLava)
	{
		tempEnemy->ActualNode = nodeArray;
	}
	else
	{
		tempEnemy->ActualNode = GetNodeInArray(tempEnemy->RigidBody.Position.x, tempEnemy->RigidBody.Position.y, nodeArray);
	}
	tempEnemy->PathList = NULL;
	tempEnemy->timer.Smudge = 0.0f;
	if (tempEnemy->Id == miniBoss && tempId == slimeDungeon)
	{
		tempEnemy->MoveAnimKey = InitAnimKey(12, 1.0f, sfTrue);
	}

	tempEnemy->StateAnim = spawn;
#if _DEBUG
	tempEnemy->Shape = sfCircleShape_create();
	sfCircleShape_setOutlineThickness(tempEnemy->Shape, 1.f);
	sfCircleShape_setOutlineColor(tempEnemy->Shape, sfGreen);
	sfCircleShape_setFillColor(tempEnemy->Shape, sfTransparent);
	sfCircleShape_setRadius(tempEnemy->Shape, tempEnemy->Circle.radius);
	sfCircleShape_setOrigin(tempEnemy->Shape, (sfVector2f) { tempEnemy->Circle.radius, tempEnemy->Circle.radius });
#endif
	if (*firstEnemy != NULL)
	{
		(*firstEnemy)->Previous = tempEnemy;
		tempEnemy->Next = *firstEnemy;
	}
	else
	{
		tempEnemy->Next = NULL;
	}
	tempEnemy->Previous = NULL;
	*firstEnemy = tempEnemy;
}

void AddEnemyInMiddle(Enemy* enemy, const int id, const sfVector2f pos, Node* nodeArray)
{
	Enemy* tempEnemy = (Enemy*)calloc(1, sizeof(Enemy));
	tempEnemy->Circle.center = pos;
	DungeonId tempId = GetActualDungeonId();
	if (tempId == slimeDungeon || tempId == beginningDungeon)
	{
		tempEnemy->Circle.radius = statEnemiesSlime[id].radius;
		tempEnemy->Speed = statEnemiesSlime[id].Speed;
		tempEnemy->Life = statEnemiesSlime[id].Life;
		tempEnemy->Damage = statEnemiesSlime[id].Damage;
		tempEnemy->XPGives = statEnemiesSlime[id].XPGives;
	}
	else if (tempId == elemDungeon)
	{
		tempEnemy->Circle.radius = statEnemiesElem[id].radius;
		tempEnemy->Speed = statEnemiesElem[id].Speed;
		tempEnemy->Life = statEnemiesElem[id].Life;
		tempEnemy->Damage = statEnemiesElem[id].Damage;
		tempEnemy->XPGives = statEnemiesElem[id].XPGives;
	}
	tempEnemy->RigidBody.Position.x = pos.x;
	tempEnemy->RigidBody.Position.y = pos.y;
	tempEnemy->RigidBody.Mass = 1;
	tempEnemy->Id = id;
	tempEnemy->DirectionAnimation = idle;
	tempEnemy->IsMoving = sfFalse;
	tempEnemy->IsHit = sfFalse;
	tempEnemy->timer.Hit = 0;
	tempEnemy->timer.PathFinding = (float)rand() / (float)RAND_MAX * 0.1f;
	tempEnemy->TimerSideWalk = (float)rand() / (float)RAND_MAX * 1.5f + 0.5f;
	tempEnemy->DirectionSideWalk = (rand() % 2) ? 1 : -1;
	tempEnemy->CooldownShot = 2.f + ((float)rand() / RAND_MAX) * 2.f;
	if (tempId == slimeDungeon || tempId == beginningDungeon)
	{
		tempEnemy->DeathAnimKey = InitAnimKey(10, 0.3f, sfFalse);
		if (tempEnemy->Id == cac)
		{
			tempEnemy->MoveAnimKey = InitAnimKey(12, 1.0f, sfTrue);
		}
		tempEnemy->SpawnAnimKey = InitAnimKey(11, 1.f, sfFalse);
	}
	else if (tempId == elemDungeon)
	{
		tempEnemy->DeathAnimKey = InitAnimKey(24, 0.5f, sfFalse);
		tempEnemy->SpawnAnimKey = InitAnimKey(24, 2.f, sfFalse);
		tempEnemy->MoveAnimKey = InitAnimKey(12, 1.0f, sfTrue);
	}
	tempEnemy->AttackAnimKey = InitAnimKey(12, 0.5f, sfFalse);
	tempEnemy->MustShot = sfTrue;
	tempEnemy->TransitionTimer = 0.0f;
	tempEnemy->IsTransitioning = sfFalse;
	tempEnemy->TransitionDirection = 0;
	if (tempId == elemDungeon && id == littleElemOfLava)
	{
		tempEnemy->ActualNode = nodeArray;
	}
	else
	{
		tempEnemy->ActualNode = GetNodeInArray(tempEnemy->RigidBody.Position.x, tempEnemy->RigidBody.Position.y, nodeArray);
	}
	tempEnemy->PathList = NULL;
	if (tempEnemy->Id == miniBoss && tempId == slimeDungeon)
	{
		tempEnemy->MoveAnimKey = InitAnimKey(12, 1.0f, sfTrue);
	}

	tempEnemy->StateAnim = spawn;
#if _DEBUG
	tempEnemy->Shape = sfCircleShape_create();
	sfCircleShape_setOutlineThickness(tempEnemy->Shape, 1.f);
	sfCircleShape_setOutlineColor(tempEnemy->Shape, sfGreen);
	sfCircleShape_setFillColor(tempEnemy->Shape, sfTransparent);
	sfCircleShape_setRadius(tempEnemy->Shape, tempEnemy->Circle.radius);
	sfCircleShape_setOrigin(tempEnemy->Shape, (sfVector2f) { tempEnemy->Circle.radius, tempEnemy->Circle.radius });
#endif

	tempEnemy->Next = enemy->Next;
	tempEnemy->Previous = enemy;
	if (tempEnemy->Next != NULL)
	{
		tempEnemy->Next->Previous = tempEnemy;
	}
	enemy->Next = tempEnemy;
}

void SetInitNodeEnemy(Enemy *enemy, Node* firstNode)
{
	enemy->ActualNode = GetNodeInArray(enemy->RigidBody.Position.x, enemy->RigidBody.Position.y, firstNode);
}

// remove an enemy of the linked list
Enemy* RemoveEnemy(Enemy* enemy)
{
	Enemy tempEnemy = *enemy;
#if _DEBUG
	sfCircleShape_destroy(enemy->Shape);
#endif // _DEBUG

	if (enemy->Previous != NULL && enemy->Next != NULL)
	{
		enemy->Previous->Next = enemy->Next;
		enemy->Next->Previous = enemy->Previous;
		free(enemy);
		return tempEnemy.Previous;
	}
	else if (enemy->Previous == NULL && enemy->Next == NULL)
	{
		free(enemy);
		return NULL;
	}
	else if (enemy->Previous == NULL && enemy->Next != NULL)
	{
		enemy->Next->Previous = NULL;
		Enemy* tmpEnemy = enemy->Next;
		free(enemy);
		return tmpEnemy;
	}
	else if (enemy->Next == NULL && enemy->Previous != NULL)
	{
		enemy->Previous->Next = NULL;
		free(enemy);
		return tempEnemy.Previous;
	}
	return NULL;
}

// remove all the enemies of the linked list
void ClearEnemies(Enemy** firstEnemy)
{
	if (*firstEnemy == NULL)
	{
		return;
	}
	else if ((*firstEnemy)->Next == NULL)
	{

#if _DEBUG
		sfCircleShape_destroy((*firstEnemy)->Shape);
#endif // _DEBUG
		free(*firstEnemy);
		*firstEnemy = NULL;
		return;
	}
	Enemy* tempEnemy = (*firstEnemy)->Next;
	while (tempEnemy != NULL)
	{
		if (tempEnemy->Previous != NULL)
		{
#if _DEBUG
			sfCircleShape_destroy(tempEnemy->Previous->Shape);
#endif // _DEBUG
			free(tempEnemy->Previous);
			tempEnemy->Previous = NULL;
			if (tempEnemy->Next == NULL)
			{
#if _DEBUG
				sfCircleShape_destroy(tempEnemy->Shape);
#endif // _DEBUG
				free(tempEnemy);
				tempEnemy = NULL;
			}
			else
			{
				tempEnemy = tempEnemy->Next;
			}
		}
		else if (tempEnemy->Next == NULL)
		{
#if _DEBUG
			sfCircleShape_destroy(tempEnemy->Shape);
#endif // _DEBUG
			free(tempEnemy);
			tempEnemy = NULL;
		}
	}
	*firstEnemy = NULL;
}

void UpdateSlimes(Player player[], const short nbrOfPlayer, Enemy** firstEnemy, Shot** firstShot, const float deltaTime, LootData* lootData, Node* firstNode, Particles** particles)
{
	Enemy** tempFirstEnemy = firstEnemy;
	Enemy* tempEnemy = *firstEnemy;
	sfBool canRepeat = (tempEnemy != NULL);
	while (canRepeat)
	{
		tempEnemy->CooldownShot -= deltaTime;
		tempEnemy->timer.PathFinding -= deltaTime;

		sfVector2f tempPos = { tempEnemy->RigidBody.Position.x, tempEnemy->RigidBody.Position.y };
		tempEnemy->Circle.center = tempPos;
		if (tempEnemy->IsHit)
		{
			tempEnemy->timer.Hit += deltaTime;
			if (tempEnemy->timer.Hit > 0.1f)
			{
				tempEnemy->timer.Hit = 0.f;
				tempEnemy->IsHit = sfFalse;
			}
		}
		if (tempEnemy->StateAnim == spawn)
		{
			UpdateAnimationKey(&tempEnemy->SpawnAnimKey, deltaTime);
			if (tempEnemy->SpawnAnimKey.actualFrameToShow == tempEnemy->SpawnAnimKey.numberOfFrame - 1)
			{
				tempEnemy->StateAnim = move;
			}
		}
		else if (tempEnemy->StateAnim == move)
		{
			if (tempEnemy->ActualNode == NULL)
			{
				SetInitNodeEnemy(tempEnemy, firstNode);
			}

			if (tempEnemy->Id == cac || tempEnemy->Id == cacUpgrade)
			{
				UpdateAnimationKey(&tempEnemy->MoveAnimKey, deltaTime);
			}

			//to draw smudge particles
			if (tempEnemy->timer.Smudge > 0.15f)
			{
				sfVector2f startScale = { 0.f, 0.f };
				sfVector2f posSmudge = tempPos;
				sfColor colorSmudge = sfTransparent;
				if (tempEnemy->Id == distance)
				{
					colorSmudge = sfColor_fromRGB(0, 175, 239);
					startScale = (sfVector2f) { 0.3f, 0.3f };
					posSmudge.y += 10.f;
				}
				else if (tempEnemy->Id == distanceUpgrade)
				{
					colorSmudge = sfColor_fromRGB(0, 175, 239);
					startScale = (sfVector2f) { 0.45f, 0.45f };
					posSmudge.y += 30.0f;
				}
				else if (tempEnemy->Id == cac)
				{
					colorSmudge = sfColor_fromRGB(100, 168, 58);
					startScale = (sfVector2f) { 0.3f, 0.3f };
					posSmudge.y += 10.f;
				}
				else if (tempEnemy->Id == cacUpgrade)
				{
					colorSmudge = sfColor_fromRGB(100, 168, 58);
					startScale = (sfVector2f) { 0.45f, 0.45f };
					posSmudge.y += 30.0f;
				}
				else if (tempEnemy->Id == kamikaze)
				{
					colorSmudge = sfColor_fromRGB(218, 40, 39);
					startScale = (sfVector2f) { 0.3f, 0.3f };
					posSmudge.y += 30.0f;
				}
				else if (tempEnemy->Id == kamikazeUpgrade)
				{
					colorSmudge = sfColor_fromRGB(218, 40, 39);
					startScale = (sfVector2f) { 0.6f, 0.6f };
					posSmudge.y += 30.0f;
				}
				else if (tempEnemy->Id == miniBoss)
				{
					colorSmudge = sfColor_fromRGB(139, 76, 158);
					startScale = (sfVector2f) { 0.6f, 0.6f };
					posSmudge.y += 30.0f;
				}
				if (tempEnemy->Id == kamikazeUpgrade || tempEnemy->Id == kamikaze)
				{
					tempEnemy->timer.Smudge -= 0.07f;
				}
				else
				{
					tempEnemy->timer.Smudge -= 0.15f;
				}
				AddParticles(&particles[0]);
				SetParticlePosition(particles[0], posSmudge);
				SetParticleTTL(particles[0], 5.0f);
				SetParticleScale(particles[0], startScale, (sfVector2f) { 0.0f, 0.0f });
				SetParticleColor(particles[0], colorSmudge, sfTransparent);
				SetFixedRotation(particles[0], (float)(rand() % 360));
				SetImageIndex(particles[0], rand() % 3);
			}
			else
			{
				tempEnemy->timer.Smudge += deltaTime;
			}

			PaternSlime(player, nbrOfPlayer, tempEnemy, firstShot, deltaTime);
			if (tempEnemy->Id == miniBoss)
			{
				UpdateAnimationKey(&tempEnemy->MoveAnimKey, deltaTime);
			}

			sfVector2f tempPos = { tempEnemy->RigidBody.Position.x, tempEnemy->RigidBody.Position.y };
			tempEnemy->Circle.center = tempPos;
		}
		else if (tempEnemy->StateAnim == deathAnim)
		{
			UpdateAnimationKey(&tempEnemy->DeathAnimKey, deltaTime);
			if (tempEnemy->DeathAnimKey.actualFrameToShow == tempEnemy->DeathAnimKey.numberOfFrame - 1)
			{
				sfColor colorSmudge;
				if (tempEnemy->Id == distance || tempEnemy->Id == distanceUpgrade)
				{
					colorSmudge = sfColor_fromRGB(0, 175, 239);
				}
				if (tempEnemy->Id == cac || tempEnemy->Id == cacUpgrade)
				{
					colorSmudge = sfColor_fromRGB(100, 168, 58);
				}
				if (tempEnemy->Id == kamikaze || tempEnemy->Id == kamikazeUpgrade)
				{
					colorSmudge = sfColor_fromRGB(218, 40, 39);
				}
				if (tempEnemy->Id == miniBoss)
				{
					colorSmudge = sfColor_fromRGB(139, 76, 158);
				}
				for (int i = 0; i < 7; i++)
				{
					tempEnemy->timer.Smudge = 0.0f;
					AddParticles(&particles[0]);
					float angle = (float)rand() / (float)RAND_MAX * M_PI * 2;
					float distanceOffset = (float)(rand() % 40);
					SetParticlePosition(particles[0], (sfVector2f) { tempPos.x + cosf(angle) * distanceOffset, tempPos.y + sinf(angle) * distanceOffset });
					SetParticleTTL(particles[0], 5.0f);
					SetParticleScale(particles[0], (sfVector2f) { 0.3f, 0.3f }, (sfVector2f) { 0.0f, 0.0f });
					SetParticleColor(particles[0], colorSmudge, sfTransparent);

					SetFixedRotation(particles[0], (float)(rand() % 360));
					SetImageIndex(particles[0], rand() % 3);
				}
				AddParticles(&particles[2]);
				sfVector2f position = { tempEnemy->RigidBody.Position.x, tempEnemy->RigidBody.Position.y - 20.f };
				SetParticlePosition(particles[2], tempEnemy->RigidBody.Position);
				SetParticleTTL(particles[2], 3.0f);
				SetParticleText(particles[2], "+", tempEnemy->XPGives, "XP");
				sfVector2f dir = { 0.f, -1.f };
				SetParticleDirection(particles[2], dir);
				SetParticleSpeed(particles[2], 600.f);
				if (tempEnemy->Id == kamikaze || tempEnemy->Id == kamikazeUpgrade)
				{
					sfVector2f* directionArray = GetDirectionalArray();
					for (int nbPart = 0; nbPart < 20; nbPart++)
					{
						AddParticles(&particles[1]);
						SetParticlePosition(particles[1], (sfVector2f) { tempPos.x, tempPos.y });
						SetParticleTTL(particles[1], (float)(rand() / RAND_MAX) * 6.f + 2.f);
						int index = rand() % 180;
						SetParticleDirection(particles[1], (sfVector2f) { -(directionArray[index].x *  (float)(rand() % 800 + 200)), -(directionArray[index].y *  (float)(rand() % 200 + 800)) });
						SetParticleGravity(particles[1], (sfVector2f) { 0.0f, 4.f });
						SetParticleSpeed(particles[1], 1.9f);
						float scale = rand() % 20 * 0.1f;
						SetParticleScale(particles[1], (sfVector2f) { scale, scale }, (sfVector2f) { scale, scale });
						SetParticleColor(particles[1], sfWhite, sfTransparent);
						SetImageIndex(particles[1], rand() % 6);
					}
					KamikazeExplosion(*tempFirstEnemy, tempEnemy, player, nbrOfPlayer, firstShot);
				}
				else if (tempEnemy->Id == cacUpgrade)
				{
					int nbrEnemiesSpawned = 2;
					float angle = (float)rand() / RAND_MAX * 360.f;
					for (int i = 0; i < nbrEnemiesSpawned; i++)
					{
						sfVector2f pos = { tempEnemy->RigidBody.Position.x + 40.f * cosf(RadianToDegree(angle)), tempEnemy->RigidBody.Position.y + 40.f * sinf(RadianToDegree(angle)) };
						AddEnemy(firstEnemy, cac, pos, tempEnemy->ActualNode);
						angle += 360.f / (float)nbrEnemiesSpawned;
					}
				}
				if (lootData != NULL)
				{
					short randomSpawn = rand() % 100;
					if (randomSpawn < 7)
					{
						AddLoot(lootData, tempEnemy->RigidBody.Position, SpawnMana());
					}
					else if (randomSpawn < 12)
					{
						AddLoot(lootData, tempEnemy->RigidBody.Position, SpawnLife());
					}
				}

				if (tempEnemy != NULL)
				{
					tempEnemy = RemoveEnemy(tempEnemy);
					if (tempEnemy == NULL)
					{
						*firstEnemy = NULL;
						return;
					}
					else if (tempEnemy->Previous == NULL)
					{
						*firstEnemy = tempEnemy;
					}
				}
			}
		}
		else
		{
			if (tempEnemy != NULL)
			{
				tempEnemy = RemoveEnemy(tempEnemy);
				if (tempEnemy == NULL)
				{
					*firstEnemy = NULL;
					return;
				}
				else if (tempEnemy->Previous == NULL)
				{
					*firstEnemy = tempEnemy;
				}
			}
		}
		if (tempEnemy != NULL && tempEnemy->Next != NULL)
		{
			tempEnemy = tempEnemy->Next;
		}
		else
		{
			canRepeat = sfFalse;
		}
	}

	if (tempEnemy != NULL)
	{
		while (tempEnemy->Previous != NULL)
		{
			tempEnemy = tempEnemy->Previous;
		}
	}
	*firstEnemy = tempEnemy;
}

void UpdateElem(const Player player[], const short nbrOfPlayer, Enemy** firstEnemy, Shot** firstShot, const float deltaTime, LootData* lootData, Node* firstNode, const sfVector2f posRoom, const sfVector2f sizeRoom)
{
	Enemy** tempFirstEnemy = firstEnemy;
	Enemy* tempEnemy = *firstEnemy;
	sfBool canRepeat = (tempEnemy != NULL);
	while (canRepeat)
	{
		tempEnemy->CooldownShot -= deltaTime;
		tempEnemy->timer.PathFinding -= deltaTime;

		sfVector2f tempPos = { tempEnemy->RigidBody.Position.x, tempEnemy->RigidBody.Position.y };
		tempEnemy->Circle.center = tempPos;
		if (tempEnemy->IsHit)
		{
			tempEnemy->timer.Hit += deltaTime;
			if (tempEnemy->timer.Hit > 0.1f)
			{
				tempEnemy->timer.Hit = 0.f;
				tempEnemy->IsHit = sfFalse;
			}
		}
		if (tempEnemy->StateAnim == spawn)
		{
			UpdateAnimationKey(&tempEnemy->SpawnAnimKey, deltaTime);
			if (tempEnemy->SpawnAnimKey.actualFrameToShow == tempEnemy->SpawnAnimKey.numberOfFrame - 1)
			{
				tempEnemy->StateAnim = move;
			}
		}
		else if (tempEnemy->StateAnim == move)
		{
			if (tempEnemy->ActualNode == NULL)
			{
				SetInitNodeEnemy(tempEnemy, firstNode);
			}
			UpdateAnimationKey(&tempEnemy->MoveAnimKey, deltaTime);
			PaternElem(player, nbrOfPlayer, tempEnemy, firstShot, deltaTime, posRoom, sizeRoom);
			sfVector2f tempPos = { tempEnemy->RigidBody.Position.x, tempEnemy->RigidBody.Position.y };
			tempEnemy->Circle.center = tempPos;
		}
		else if (tempEnemy->StateAnim == deathAnim)
		{
			UpdateAnimationKey(&tempEnemy->DeathAnimKey, deltaTime);
			if (tempEnemy->DeathAnimKey.actualFrameToShow == tempEnemy->DeathAnimKey.numberOfFrame - 1)
			{
				if (lootData != NULL)
				{
					short randomSpawn = rand() % 100;
					if (randomSpawn < 7)
					{
						AddLoot(lootData, tempEnemy->RigidBody.Position, SpawnMana());
					}
					else if (randomSpawn < 10)
					{
						AddLoot(lootData, tempEnemy->RigidBody.Position, SpawnLife());
					}
				}

				if (tempEnemy != NULL)
				{
					tempEnemy = RemoveEnemy(tempEnemy);
					if (tempEnemy == NULL)
					{
						*firstEnemy = NULL;
						return;
					}
					else if (tempEnemy->Previous == NULL)
					{
						*firstEnemy = tempEnemy;
					}
				}
			}
		}
		else
		{
			if (tempEnemy != NULL)
			{
				tempEnemy = RemoveEnemy(tempEnemy);
				if (tempEnemy == NULL)
				{
					*firstEnemy = NULL;
					return;
				}
				else if (tempEnemy->Previous == NULL)
				{
					*firstEnemy = tempEnemy;
				}
			}
		}
		if (tempEnemy != NULL && tempEnemy->Next != NULL)
		{
			tempEnemy = tempEnemy->Next;
		}
		else
		{
			canRepeat = sfFalse;
		}
	}

	if (tempEnemy != NULL)
	{
		while (tempEnemy->Previous != NULL)
		{
			tempEnemy = tempEnemy->Previous;
		}
	}
	*firstEnemy = tempEnemy;
}

//Called every frame
void UpdateEnemies(Player player[], const short nbrOfPlayer, Enemy** firstEnemy, Shot** firstShot, const float deltaTime, LootData* lootData, Node* firstNode, const sfVector2f posRoom, const sfVector2f sizeRoom, Particles** particles)
{
	if (*firstEnemy == NULL)
	{
		return;
	}
	DungeonId tempDungeonId = GetActualDungeonId();
	if (tempDungeonId == slimeDungeon || tempDungeonId == beginningDungeon)
	{
		UpdateSlimes(player, nbrOfPlayer, firstEnemy, firstShot, deltaTime, lootData, firstNode, particles);
	}
	else if (tempDungeonId == elemDungeon)
	{
		UpdateElem(player, nbrOfPlayer, firstEnemy, firstShot, deltaTime, lootData, firstNode, posRoom, sizeRoom);
	}
}

// assign the good patern to the slimes
void PaternSlime(Player player[], const short nbrOfPlayer, Enemy* enemy, Shot** firstShot, const float deltaTime)
{
	short nearestIndexPlayer = 0;
	float distanceNearestPlayer = 0.f;
	sfBool haveVisionOnPlayer;
	sfBool haveVisionOnPlayerForShot;
	// if the game is in two player check what is the nearest player 
	if (nbrOfPlayer > 1)
	{
		if (!player[0].States.IsDead && player[1].States.IsDead)
		{
			nearestIndexPlayer = 0;
		}
		else if (player[0].States.IsDead && !player[1].States.IsDead)
		{
			nearestIndexPlayer = 1;
		}
		else
		{
			if (Distance(player[1].Physics.Rb.Position, enemy->RigidBody.Position) < Distance(player[0].Physics.Rb.Position, enemy->RigidBody.Position))
			{
				nearestIndexPlayer = 1;
			}
		}
	}
	distanceNearestPlayer = Distance(player[nearestIndexPlayer].Physics.Rb.Position, enemy->RigidBody.Position);
	haveVisionOnPlayer = !HaveObstacleBetween(enemy->ActualNode, player[nearestIndexPlayer].ActualNode, sfFalse, sfTrue, sfFalse);
	haveVisionOnPlayerForShot = !HaveObstacleBetween(enemy->ActualNode, player[nearestIndexPlayer].ActualNode, sfFalse, sfFalse, sfTrue);
	// if the enemy is knockbacked, increment his speed
	/*if (enemy->IsKnockBacked)
	{
		enemy->TimerKnock += deltaTime * 2.f;
		if (enemy->TimerKnock > 1.f)
		{
			enemy->TimerKnock = 1.f;
			enemy->IsKnockBacked = sfFalse;
		}
		enemy->Speed = Lerp(0.f, statEnemiesSlime[enemy->Id].Speed, enemy->TimerKnock);
	}*/

	if (enemy->IsTransitioning = sfTrue)
	{
		if (enemy->TransitionTimer >= 0.1f)
		{
			enemy->IsTransitioning = sfFalse;
		}
		else
		{
			enemy->TransitionTimer += deltaTime;
		}
	}

	// actualize the direction of the enemy to look at the player
	if (enemy->Id != miniBoss && enemy->timer.PathFinding <= 0 /*&& !haveVisionOnPlayer*/)
	{
		CleanNodeListNode(&enemy->PathList);
		enemy->PathList = PathAStar(enemy->ActualNode, player[nearestIndexPlayer].ActualNode);
		//SaveNodeList(&enemy->PathList);

		if (enemy->PathList != NULL && enemy->PathList->Node != NULL)
		{
			sfVector2f temp2f = { enemy->PathList->Node->posX*64.f, enemy->PathList->Node->posY*64.f };
			sfVector2f distance2vectors = AddTwoVectors(enemy->RigidBody.Position, temp2f);
			if (Magnitude(distance2vectors) < 10.f)
			{
				NodeList* toDelete = enemy->PathList;
				enemy->PathList = enemy->PathList->Next;
				free(toDelete);

				temp2f.x = enemy->PathList->Node->posX*64.f;
				temp2f.y = enemy->PathList->Node->posY*64.f;
				enemy->Direction = AddTwoVectors(enemy->RigidBody.Position, temp2f);
				UpdateNodeList(&enemy->PathList);
			}
			else
			{
				enemy->Direction = AddTwoVectors(enemy->RigidBody.Position, temp2f);
			}
		}
		else
		{
			enemy->Direction = AddTwoVectors(enemy->RigidBody.Position, player[nearestIndexPlayer].Physics.Rb.Position);
		}

		enemy->Direction = Normalize(enemy->Direction);
		enemy->RigidBody.Velocity.x = enemy->Direction.x * enemy->Speed;
		enemy->RigidBody.Velocity.y = enemy->Direction.y * enemy->Speed;
		enemy->timer.PathFinding = 0.1f;
	}
	//else if (haveVisionOnPlayer)
	//{
	//	enemy->Direction = Normalize(AddTwoVectors(enemy->RigidBody.Position, player[nearestIndexPlayer].Physics.Rb.Position));
	//	enemy->RigidBody.Velocity.x = enemy->Direction.x * enemy->Speed;
	//	enemy->RigidBody.Velocity.y = enemy->Direction.y * enemy->Speed;
	//}

	if (enemy->Id != miniBoss)
	{
		if ((enemy->Id == distance || enemy->Id == distanceUpgrade) && distanceNearestPlayer <= 450 && haveVisionOnPlayer)
		{
			enemy->Direction = Normalize(AddTwoVectors(enemy->RigidBody.Position, player[nearestIndexPlayer].Physics.Rb.Position));
			enemy->RigidBody.Velocity.x = enemy->Direction.x * enemy->Speed;
			enemy->RigidBody.Velocity.y = enemy->Direction.y * enemy->Speed;
		}

		char temp = (char)((int)(RadianToDegree(atan2f(-enemy->Direction.y, -enemy->Direction.x)) + 203) % 360 / 45);
		if (temp != enemy->DirectionAnimation)
		{
			enemy->IsTransitioning = sfTrue;

			enemy->TransitionTimer = 0.0f;
			if (temp < enemy->DirectionAnimation && temp != 0)
			{
				enemy->TransitionDirection = temp;
			}
			else if (temp == 7 && enemy->DirectionAnimation == 0)
			{
				enemy->TransitionDirection = temp;
			}
			else
			{
				enemy->TransitionDirection = enemy->DirectionAnimation;
			}
		}
		enemy->DirectionAnimation = temp;
	}

	if (enemy->Id == distance)
	{
		CheckNextNodeE(enemy->ActualNode, &enemy->RigidBody, enemy->RigidBody.Position, sfFalse);
		if (distanceNearestPlayer > 450 || !haveVisionOnPlayerForShot)
		{
			UpdateRigidBody(&enemy->RigidBody, deltaTime);
			ChangeNode(&enemy->ActualNode, enemy->RigidBody.Position);
		}
		else
		{
			if (Magnitude(enemy->RigidBody.KnockBackForce) > 1)
			{
				enemy->RigidBody.Velocity = (sfVector2f) { 0.f, 0.f };
				UpdateRigidBody(&enemy->RigidBody, deltaTime);
			}
			//sfVector2f tempPos;
			//if (distanceNearestPlayer < 440)
			//{
			//	enemy->RigidBody.Velocity.x *= -1.3f;
			//	enemy->RigidBody.Velocity.y *= -1.3f;
			//	UpdateRigidBody(&enemy->RigidBody, deltaTime);
			//}
			//// increment the timer of side walk to change the direction
			//enemy->TimerSideWalk -= deltaTime;
			//if (enemy->TimerSideWalk <= 0)
			//{
			//	enemy->TimerSideWalk = rand() / (float)RAND_MAX * 1.5f + 0.5f;
			//	enemy->DirectionSideWalk = (rand() % 2) ? 1 : -1;
			//}
			//// lateral move of the enemy relative to the player
			//tempPos = RotateAround(enemy->RigidBody.Position, player[nearestIndexPlayer].Physics.Rb.Position, DegreeToRadian(30 * enemy->DirectionSideWalk * deltaTime));
			//enemy->RigidBody.Position = tempPos;
			//enemy->Circle.center = enemy->RigidBody.Position;
			if (enemy->CooldownShot <= 0.f && haveVisionOnPlayerForShot)
			{
				sfVector2f dirShot;
				if (distanceNearestPlayer >= 150 && rand() % 3)
				{
					// try to predict the player's movement
					sfVector2f posPlayerAfterFewSecond = player[nearestIndexPlayer].Physics.Rb.Position;
					posPlayerAfterFewSecond.x += player[nearestIndexPlayer].Physics.Rb.Velocity.x * (200 / (distanceNearestPlayer + 1) / 1.2f);
					posPlayerAfterFewSecond.y += player[nearestIndexPlayer].Physics.Rb.Velocity.y * (200 / (distanceNearestPlayer + 1) / 1.2f);
					dirShot = Normalize(AddTwoVectors(enemy->RigidBody.Position, posPlayerAfterFewSecond));
				}
				else
				{
					// shoot on the player
					dirShot = Normalize(AddTwoVectors(enemy->RigidBody.Position, player[nearestIndexPlayer].Physics.Rb.Position));
				}
				AddShot(firstShot, enemy->RigidBody.Position, dirShot, enemiesShot);
				enemy->CooldownShot = 1.f;
			}
		}
	}
	else if (enemy->Id == distanceUpgrade)
	{
		CheckNextNodeE(enemy->ActualNode, &enemy->RigidBody, enemy->RigidBody.Position, sfFalse);
		if (distanceNearestPlayer > 550 || !haveVisionOnPlayerForShot)
		{
			UpdateRigidBody(&enemy->RigidBody, deltaTime);
			ChangeNode(&enemy->ActualNode, enemy->RigidBody.Position);
		}
		else
		{
			if (Magnitude(enemy->RigidBody.KnockBackForce) > 1)
			{
				enemy->RigidBody.Velocity = (sfVector2f) { 0.f, 0.f };
				UpdateRigidBody(&enemy->RigidBody, deltaTime);
			}
			//sfVector2f tempPos;
			//if (distanceNearestPlayer < 540)
			//{
			//	enemy->RigidBody.Velocity.x *= -1.3f;
			//	enemy->RigidBody.Velocity.y *= -1.3f;
			//	UpdateRigidBody(&enemy->RigidBody, deltaTime);
			//}
			// increment the timer of side walk to change the direction
			//enemy->TimerSideWalk -= deltaTime;
			//if (enemy->TimerSideWalk <= 0)
			//{
			//	enemy->TimerSideWalk = rand() / (float)RAND_MAX * 1.5f + 0.5f;
			//	enemy->DirectionSideWalk = (rand() % 2) ? 1 : -1;
			//}
			//// lateral move of the enemy relative to the player
			//tempPos = RotateAround(enemy->RigidBody.Position, player[nearestIndexPlayer].Physics.Rb.Position, DegreeToRadian(20 * enemy->DirectionSideWalk * deltaTime));
			//enemy->RigidBody.Position = tempPos;
			//enemy->Circle.center = enemy->RigidBody.Position;
			if (enemy->CooldownShot <= 0.f && haveVisionOnPlayer)
			{
				sfVector2f dirShot;
				sfVector2f tempDir;
				// shoot on the player
				dirShot = Normalize(AddTwoVectors(enemy->RigidBody.Position, player[nearestIndexPlayer].Physics.Rb.Position));
				float angleRotate = DegreeToRadian(20.f);
				AddShot(firstShot, enemy->RigidBody.Position, dirShot, enemiesShot);
				tempDir = Rotate(dirShot, angleRotate);
				AddShot(firstShot, enemy->RigidBody.Position, tempDir, enemiesShot);
				tempDir = Rotate(dirShot, -angleRotate);
				AddShot(firstShot, enemy->RigidBody.Position, tempDir, enemiesShot);
				enemy->CooldownShot = 1.5f;
			}
		}
	}
	else if (enemy->Id == cac || enemy->Id == cacUpgrade)
	{
		CheckNextNodeE(enemy->ActualNode, &enemy->RigidBody, enemy->RigidBody.Position, sfFalse);
		UpdateRigidBody(&enemy->RigidBody, deltaTime);
		ChangeNode(&enemy->ActualNode, enemy->RigidBody.Position);
	}
	else if (enemy->Id == kamikaze || enemy->Id == kamikazeUpgrade)
	{
		if (Distance(player[nearestIndexPlayer].Physics.Rb.Position, enemy->RigidBody.Position) <= 30 + enemy->Circle.radius + player[nearestIndexPlayer].Physics.Collider.radius)
		{
			enemy->StateAnim = deathAnim;
			PlaySounds(explosion);
		}
		CheckNextNodeE(enemy->ActualNode, &enemy->RigidBody, enemy->RigidBody.Position, sfFalse);
		UpdateRigidBody(&enemy->RigidBody, deltaTime);
		ChangeNode(&enemy->ActualNode, enemy->RigidBody.Position);
	}
	else if (enemy->Id == miniBoss)
	{
		enemy->timer.Movement -= deltaTime;
		enemy->timer.Flask -= deltaTime;
		if (enemy->timer.Movement <= 0)
		{
			if (rand() % 2)
			{
				enemy->Direction = AddTwoVectors(enemy->RigidBody.Position, player[nearestIndexPlayer].Physics.Rb.Position);
				enemy->Direction = Normalize(enemy->Direction);
				enemy->Direction = Rotate(enemy->Direction, DegreeToRadian((float)(rand() % 180 - 90.f)));
			}
			else
			{
				float tempAngle = (float)rand() / RAND_MAX * 360.f;
				enemy->Direction = (sfVector2f) { cosf(tempAngle), sinf(tempAngle) };
			}
			enemy->RigidBody.Velocity.x = enemy->Direction.x * enemy->Speed;
			enemy->RigidBody.Velocity.y = enemy->Direction.y * enemy->Speed;
			enemy->timer.Movement = 1.f + (float)rand() / RAND_MAX * 2.f;
		}
		if (enemy->timer.Flask <= 0)
		{
			AddTrap(enemy->RigidBody.Position, (sfVector2f) { 0, 0 }, flask, 75.f, sfTrue, 30.f, 6.f, 3, 0);
			enemy->timer.Flask = 1.f + (float)rand() / RAND_MAX * 1.f;
		}
		UpdateRigidBody(&enemy->RigidBody, deltaTime);
		if (enemy->CooldownShot <= 0)
		{
			enemy->Attack = sfTrue;
		}
		if (enemy->Attack)
		{
			UpdateAnimationKey(&enemy->AttackAnimKey, deltaTime);
			if (enemy->AttackAnimKey.actualFrameToShow == 8 && enemy->MustShot)
			{
				sfVector2f directionPlayer = AddTwoVectors(enemy->RigidBody.Position, player[nearestIndexPlayer].Physics.Rb.Position);
				directionPlayer = Normalize(directionPlayer);
				float angle = atan2f(directionPlayer.y, directionPlayer.x);
				float angleRotate = DegreeToRadian(30.f);
				sfVector2f dirShot = { cosf(angle),sinf(angle) };
				sfVector2f dirShot2 = Rotate(dirShot, angleRotate);
				sfVector2f dirShot3 = Rotate(dirShot, -angleRotate);
				sfVector2f tempPos = { enemy->RigidBody.Position.x, enemy->RigidBody.Position.y - 50 };
				for (int i = 2; i < 16; i++)
				{
					float speed = 50.f * (float)i;
					float distanceForDestroy = 50.f * (float)i;
					AddMiniBossShot(firstShot, tempPos, dirShot2, speed, distanceForDestroy, -1, (long long int)enemy);
					speed += 25.f;
					distanceForDestroy += 25.f;
					AddMiniBossShot(firstShot, tempPos, dirShot3, speed, distanceForDestroy, 1, (long long int)enemy);
				}
				enemy->MustShot = sfFalse;
			}
			if (enemy->AttackAnimKey.actualFrameToShow == enemy->AttackAnimKey.numberOfFrame - 1)
			{
				enemy->AttackAnimKey.actualFrameToShow = 0;
				enemy->AttackAnimKey.actualDuration = 0.f;
				enemy->CooldownShot = 2.f + (float)rand() / RAND_MAX * 1.f;
				enemy->Attack = sfFalse;
				enemy->MustShot = sfTrue;
			}
		}
	}
}

// assign the good patern to the elems
void PaternElem(const Player player[], const short nbrOfPlayer, Enemy* enemy, Shot** firstShot, const float deltaTime, const sfVector2f posRoom, const sfVector2f sizeRoom)
{
	short nearestIndexPlayer = 0;
	float distanceNearestPlayer = 0.f;
	//sfBool haveVisionOnPlayer;
	// if the game is in two player check what is the nearest player 
	if (nbrOfPlayer > 1)
	{
		if (!player[0].States.IsDead && player[1].States.IsDead)
		{
			nearestIndexPlayer = 0;
		}
		else if (player[0].States.IsDead && !player[1].States.IsDead)
		{
			nearestIndexPlayer = 1;
		}
		else
		{
			if (Distance(player[1].Physics.Rb.Position, enemy->RigidBody.Position) < Distance(player[0].Physics.Rb.Position, enemy->RigidBody.Position))
			{
				nearestIndexPlayer = 1;
			}
		}
	}
	distanceNearestPlayer = Distance(player[nearestIndexPlayer].Physics.Rb.Position, enemy->RigidBody.Position);
	// if the enemy is knockbacked, increment his speed
	/*if (enemy->IsKnockBacked)
	{
		enemy->TimerKnock += deltaTime * 2.f;
		if (enemy->TimerKnock > 1.f)
		{
			enemy->TimerKnock = 1.f;
			enemy->IsKnockBacked = sfFalse;
		}
		enemy->Speed = Lerp(0.f, statEnemiesElem[enemy->Id].Speed, enemy->TimerKnock);
	}*/

	if (enemy->IsTransitioning = sfTrue)
	{
		if (enemy->TransitionTimer >= 0.1f)
		{
			enemy->IsTransitioning = sfFalse;
		}
		else
		{
			enemy->TransitionTimer += deltaTime;
		}
	}

	char temp = (char)((int)(RadianToDegree(atan2f(-enemy->Direction.y, -enemy->Direction.x)) + 203) % 360 / 90);
	/*if (temp != enemy->DirectionAnimation)
	{
		enemy->IsTransitioning = sfTrue;

		enemy->TransitionTimer = 0.0f;
		if (temp < enemy->DirectionAnimation && temp != 0)
		{
			enemy->TransitionDirection = temp;
		}
		else if (temp == 7 && enemy->DirectionAnimation == 0)
		{
			enemy->TransitionDirection = temp;
		}
		else
		{
			enemy->TransitionDirection = enemy->DirectionAnimation;
		}
	}*/

	enemy->DirectionAnimation = temp;

	// actualize the direction of the enemy to look at the player
	if (enemy->timer.PathFinding <= 0)
	{
		CleanNodeListNode(&enemy->PathList);
		enemy->PathList = PathAStar(enemy->ActualNode, player[nearestIndexPlayer].ActualNode);
		//SaveNodeList(&enemy->PathList);

		if (enemy->PathList != NULL && enemy->PathList->Node != NULL)
		{
			sfVector2f temp2f = { enemy->PathList->Node->posX*64.f, enemy->PathList->Node->posY*64.f };
			sfVector2f distance2vectors = AddTwoVectors(enemy->RigidBody.Position, temp2f);
			if (Magnitude(distance2vectors) < 10.f && enemy->PathList->Next->Node != NULL)
			{
				NodeList* toDelete = enemy->PathList;
				enemy->PathList = enemy->PathList->Next;
				free(toDelete);

				temp2f.x = enemy->PathList->Node->posX*64.f;
				temp2f.y = enemy->PathList->Node->posY*64.f;
				enemy->Direction = AddTwoVectors(enemy->RigidBody.Position, temp2f);
				UpdateNodeList(&enemy->PathList);
			}
			else
			{
				enemy->Direction = distance2vectors;
			}
		}
		else
		{
			enemy->Direction = AddTwoVectors(enemy->RigidBody.Position, player[nearestIndexPlayer].Physics.Rb.Position);
		}

		enemy->Direction = Normalize(enemy->Direction);
		enemy->RigidBody.Velocity.x = enemy->Direction.x * enemy->Speed;
		enemy->RigidBody.Velocity.y = enemy->Direction.y * enemy->Speed;
		enemy->timer.PathFinding = 0.1f;
	}
	if (enemy->Id != lavaElem)
	{
		CheckNextNodeE(enemy->ActualNode, &enemy->RigidBody, enemy->RigidBody.Position, sfFalse);
		UpdateRigidBody(&enemy->RigidBody, deltaTime);
		ChangeNode(&enemy->ActualNode, enemy->RigidBody.Position);
	}
	if (enemy->Id == fireElem)
	{
		if (enemy->CooldownShot <= 0.f)
		{
			if (distanceNearestPlayer <= 1400)
			{
				sfVector2f dirShot;
				// shoot on the player
				dirShot = Normalize(AddTwoVectors(enemy->RigidBody.Position, player[nearestIndexPlayer].Physics.Rb.Position));
				AddShot(firstShot, enemy->RigidBody.Position, dirShot, fireElemShot);
			}
			enemy->CooldownShot += 2.f;
		}
	}
	else if (enemy->Id == waterElem)
	{
		if (enemy->CooldownShot <= 0.f)
		{
			if (distanceNearestPlayer <= 1000)
			{
				sfVector2f dirShot;
				// shoot on the player
				dirShot = Normalize(AddTwoVectors(enemy->RigidBody.Position, player[nearestIndexPlayer].Physics.Rb.Position));
				AddShot(firstShot, enemy->RigidBody.Position, dirShot, waterElemShot);
			}
			enemy->CooldownShot += 2.f;
		}
	}
	else if (enemy->Id == windElem)
	{
		if (enemy->CooldownShot <= 0.f)
		{
			if (distanceNearestPlayer <= 1600)
			{
				sfVector2f dirShot;
				// shoot on the player
				dirShot = Normalize(AddTwoVectors(enemy->RigidBody.Position, player[nearestIndexPlayer].Physics.Rb.Position));
				AddWindElemShot(firstShot, enemy->RigidBody.Position, player[nearestIndexPlayer].Physics.Rb.Position, dirShot);
			}
			enemy->CooldownShot += 2.f;
		}
	}
	else if (enemy->Id == dirtElem)
	{
		if (enemy->CooldownShot <= 0.f)
		{
			if (distanceNearestPlayer <= 1000)
			{
				sfVector2f dirShot;
				// shoot on the player
				dirShot = Normalize(AddTwoVectors(enemy->RigidBody.Position, player[nearestIndexPlayer].Physics.Rb.Position));
				AddShot(firstShot, enemy->RigidBody.Position, dirShot, dirtElemShot);
			}
			enemy->CooldownShot += 2.f;
		}
	}
	else if (enemy->Id == lavaElem)
	{
		if (enemy->CooldownShot <= 0.f)
		{
			AddEnemyInMiddle(enemy, littleElemOfLava, enemy->RigidBody.Position, enemy->ActualNode);
			enemy->CooldownShot += 4.f;
		}
	}
	else if (enemy->Id == iceElem)
	{
		if (enemy->CooldownShot <= 0.f)
		{
			if (distanceNearestPlayer <= 1000)
			{
				sfVector2f dirShot;
				// shoot on the player
				dirShot = Normalize(AddTwoVectors(enemy->RigidBody.Position, player[nearestIndexPlayer].Physics.Rb.Position));
				AddShot(firstShot, enemy->RigidBody.Position, dirShot, iceElemShot);
			}
			enemy->CooldownShot += 2.f;
		}
	}
	else if (enemy->Id == stormElem)
	{
		if (enemy->CooldownShot <= 0.f)
		{
			sfVector2f tempPos = posRoom;
			tempPos.x += (float)(rand() % (int)(sizeRoom.x - 130)) - sizeRoom.x / 2.f + 66;
			tempPos.y += (float)(rand() % (int)(sizeRoom.y - 130)) - sizeRoom.y / 2.f + 66;
			AddTrap(tempPos, (sfVector2f) { 0, 0 }, lightning, 90.f, sfFalse, 0.f, 2.f, enemy->Damage, 0);
			enemy->CooldownShot += 0.5f;
		}
	}
	else if (enemy->Id == rockElem)
	{
		if (enemy->CooldownShot <= 0.f)
		{
			AddTrap(player[nearestIndexPlayer].Physics.Rb.Position, (sfVector2f) { 0, 0 }, rockSpike, 75.f, sfTrue, 75.f, 1.5f, enemy->Damage, 0);
			enemy->CooldownShot += 2.f;
		}
	}
	else if (enemy->Id == littleElemOfLava)
	{

	}
}

// inflict damage to the enemy and if he's dead delete it and add loot
void DamageEnemy(Enemy* enemy, const float damage)
{
	Enemy* tempEnemy = enemy;
	tempEnemy->Life -= damage;

	if (tempEnemy->Life <= 0)
	{
		tempEnemy->StateAnim = deathAnim;
		DungeonId dungeonId = GetActualDungeonId();
		if ((dungeonId == slimeDungeon || dungeonId == beginningDungeon) && (tempEnemy->Id == kamikaze || tempEnemy->Id == kamikazeUpgrade))
		{
			PlaySounds(explosion);
		}
	}
	else
	{
		tempEnemy->IsHit = sfTrue;
	}
}

void KamikazeExplosion(Enemy* firstEnemy, Enemy* enemyWhoExplode, Player player[], const short nbrOfPlayer, Shot** firstShot)
{
	if (enemyWhoExplode->Id == kamikazeUpgrade)
	{
		AddShotInExplosion(firstShot, enemyWhoExplode->RigidBody.Position, kamikazeShot, 10);
	}
	for (int i = 0; i < nbrOfPlayer; i++)
	{
		if (Distance(player[i].Physics.Rb.Position, enemyWhoExplode->RigidBody.Position) <= player[i].Physics.Collider.radius + enemyWhoExplode->Circle.radius + 100)
		{
			DamagePlayer(&player[i], enemyWhoExplode->Damage);
			//	make a knockback on the player
			sfVector2f normalExplosion = Normalize((sfVector2f) { player[i].Physics.Rb.Position.x - enemyWhoExplode->RigidBody.Position.x, player[i].Physics.Rb.Position.y - enemyWhoExplode->RigidBody.Position.y });
			player[i].Physics.Rb.KnockBackForce.x += normalExplosion.x * 350;
			player[i].Physics.Rb.KnockBackForce.y += normalExplosion.y * 350;
		}
	}
	Enemy* tempEnemy = firstEnemy;
	sfBool canRepeatEnemy = (tempEnemy != NULL);
	while (canRepeatEnemy)
	{
		if (tempEnemy->StateAnim == move)
		{
			if (Distance(tempEnemy->RigidBody.Position, enemyWhoExplode->RigidBody.Position) <= tempEnemy->Circle.radius + enemyWhoExplode->Circle.radius + 100)
			{
				if (tempEnemy->Id == kamikaze || tempEnemy->Id == kamikazeUpgrade)
				{
					tempEnemy->StateAnim = deathAnim;
					PlaySounds(explosion);
				}
				else
				{
					sfVector2f normalExplosion = Normalize((sfVector2f) { tempEnemy->RigidBody.Position.x - enemyWhoExplode->RigidBody.Position.x, tempEnemy->RigidBody.Position.y - enemyWhoExplode->RigidBody.Position.y });
					tempEnemy->RigidBody.Position.x += normalExplosion.x * 30;
					tempEnemy->RigidBody.Position.y += normalExplosion.y * 30;
					tempEnemy->Circle.center = tempEnemy->RigidBody.Position;
					DamageEnemy(tempEnemy, enemyWhoExplode->Damage);
				}
			}
		}
		if (tempEnemy != NULL && tempEnemy->Next != NULL)
		{
			tempEnemy = tempEnemy->Next;
		}
		else
		{
			canRepeatEnemy = sfFalse;
		}
	}
}

void UpdateShotsOfMiniBoss(Enemy* firstEnemy, Shot* firstShot, const float deltaTime)
{
	Enemy* tempEnemy = firstEnemy;
	sfBool canRepeatEnemy = (tempEnemy != NULL);
	while (canRepeatEnemy)
	{
		if (tempEnemy->Id == miniBoss)
		{
			Shot* tempShot = firstShot;
			sfBool canRepeatShot = (tempShot != NULL);
			while (canRepeatShot)
			{
				if (tempShot->IdShot == miniBossShot && (long long int)tempEnemy == tempShot->AdressRelative)
				{
					tempShot->PivotPoint = tempEnemy->Circle.center;
					tempShot->PivotPoint.y -= 50;
					tempShot->RigidBody.Position.x += tempEnemy->RigidBody.Velocity.x * deltaTime;
					tempShot->RigidBody.Position.y += tempEnemy->RigidBody.Velocity.y * deltaTime;
					tempShot->Circle.center = tempShot->RigidBody.Position;
				}
				if (tempShot != NULL && tempShot->Next != NULL)
				{
					tempShot = tempShot->Next;
				}
				else
				{
					canRepeatShot = sfFalse;
				}
			}
		}
		if (tempEnemy->Next != NULL)
		{
			tempEnemy = tempEnemy->Next;
		}
		else
		{
			canRepeatEnemy = sfFalse;
		}
	}
}
