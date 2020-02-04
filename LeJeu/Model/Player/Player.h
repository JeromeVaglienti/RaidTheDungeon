#ifndef _PLAYER_H
#define _PLAYER_H
#include <stdio.h>
#include <stdlib.h>
#include "SFML\System.h"
#include <SFML\Audio.h>
#include "..\Physics\Projectile\Shot.h"
#include "..\Math\MathUtilities.h"
#include "..\Physics\FunctionsCollisions\Colision.h"
#include "..\Spell\Spell.h"
#include "../Cursor/Cursor.h"
#include "..\Inventory\Inventory.h"
#include "..\Loot\Loot.h"
#include "..\..\Controller\GraphicsUtilities\Animation\Animation.h"
#include "SkillTree\SkillTree.h"


#define PLAYERBASICSPEED 400.f
#define PLAYERBASICMANA 500
#define PLAYERBASICLIFE 20
#define PLAYERBASICSHOTPERSECOND 5.f
#define PLAYERBASICTIMERINVU 1.f
#define PLAYERHEIGHT 128.0f
#define PLAYERWIDTH 64.0f
#define PLAYERHEIGHTx64 2.0f
#define PLAYERWIDTHx64 1.f

typedef enum DirectionPlayer
{
	right,
	down,
	left,
	up,
	idleRight,
	idleDown,
	idleLeft,
	idleUp
}DirectionPlayer;

typedef struct PlayerPhysics
{
	RigidBody Rb;
	Circle Collider;
	sfVector2f LastPosition;
	sfVector2f Direction;
	sfVector2f DirectionShot;
	char ActualDirection;
	sfVector2i PositionWorld;
	float TimerMovementParticles;
}PlayerPhysics;

typedef struct PlayerStats
{
	int Mana;
	int ManaMax;
	int Life;
	int LifeMax;
	float speed;
	float SpeedMax;
	float TimerInvulnerability;
	float TimeOfInvincibility;
	float FallingTimer;
	float ShotPerSecond;
	float DamageMultiplier;
	float HealthRegenMultiplier;
	float ManaRegenMultiplier;
	float CostManaMultiplier;
	float Tenacity;
	int XP;
}PlayerStats;

typedef struct PlayerHUD
{
	Sight Cursor;
}PlayerHUD;

typedef struct PlayerWeapon
{
	Shot *FirstShot;
	float CDShot;
}PlayerWeapon;

typedef struct PlayerState
{
	sfBool IsHit;
	sfBool IsDashing;
	sfBool IsSpitFire;
	sfBool IsSpitingFire;
	sfBool IsFiring;
	sfBool IsCastingFireBall;
	sfBool IsDead;
	sfBool IsIdle;
	sfBool IsSlowDown;
	sfBool IsAttacking;
	sfBool IsTeleporting;
	sfBool ISLeavingActualDungeon;
	sfBool IsWet;
	sfBool IsStopXl;
	sfBool IsStopYu;
	sfBool IsStopXr;
	sfBool IsStopYd;
}PlayerState;

typedef struct PlayerDashStat
{
	float TimerParticle;
	float Timer;
	char NbrDashMax;
	char NbrDashRemaining;
}PlayerDashStat;

typedef struct PlayerConeStat
{
	float Rotate;
	float TimerFireRate;
	float Timer;
	sfBool MustDamage;
}PlayerConeStat;

typedef struct PlayerMineStat
{
	int IdLight;
	sfBool IsActivate;
	sfBool Explodes;
	float Rotate;
	float Timer;
	unsigned short Damage;
	unsigned short RadiusExplosion;
	sfVector2f Pos;
	Circle Collider;
	AnimationKey AnimKey;
	AnimationKey ExplosionKey;
#if _DEBUG
	sfCircleShape* Shape;
#endif
}PlayerMineStat;

typedef struct PlayerSpells
{
	Spell spell[4];
	int spellSelect;
	PlayerDashStat DashState;
	PlayerConeStat ConeState;
	PlayerMineStat MineState;
}PlayerSpells;

typedef struct PLAYER
{
	int ID;
	int IdLight;
	PlayerPhysics Physics;
	PlayerStats Stats;
	PlayerHUD Hud;
	PlayerWeapon Weapons;
	PlayerState States;
	PlayerSpells Spells;
	sfBool TypeInput;
	SkillTree SkillTree;
	Node *ActualNode;
	AnimationKey DeathAnimation;
#if _DEBUG
	sfCircleShape* Shape;
#endif
}Player;

void InitPlayer(Player *player, const sfVector2f position, const int id);
void SetInitNodePlayer(Player *player, Node* firstNode);
void UpdateConeOfFire(Player *player, const float deltaTime, const sfVector2f dirCone);
void PlayerUpdateAngleOfConeOfFire(Player *player, sfVector2f dirCone);
void UpdatePlayer(Player *player, const float deltaTime, const int index, Node* firstNode);
void PlayerInputAffectationMovement(Player *player, sfVector2f dirMov, const float dt);
void PlayerInputAffectationViewMovement(Player *player, sfVector2f dirMov);
void PlayerFiring(Player *player, sfVector2f dirShot);
void PlayerCastSpell(Player *player, sfVector2f dirShot, const unsigned short id);
void PlayerChangeSpell(Player *player, const int newWeaponIndex);
void UpdateCooldownSpell(Player *player, const float deltaTime);
void DamagePlayer(Player* player, const unsigned short damage);
void AddXPToPlayer(Player* player, const int xp);
void AddLootToPlayer(Player* player, const LootTransfer lootTransfer);
void AttractLoot(LootData* lootData, Player* players, const int numberOfPlayer, const float dt);
void AssignSkillTreeToPlayer(Player* player);
void SlowPlayer(Player* player, const float percentageSlow);
void SetPlayerPos(Player* player, const sfVector2f NewPos);
void ResetPlayer(Player* player);
#endif // !_PLAYER_H
