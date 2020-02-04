#ifndef COLLISION_H
#define COLLISION_H
#include "..\..\Boss\Boss.h"
#include "..\..\Enemy\Enemy.h"
#include "..\..\Room\Room.h"

void CheckColisionShotRoom(Shot **Shot, const RoomGameData *room);
void CheckColisionPlayerShots(Player *player, Enemy* enemy, const RoomGameData *room, Boss *boss, const sfBool firstTimeColliding);
void CheckColisionPlayerShotEnemies(Shot **shot, Enemy* enemy, Player* player);
void CheckColisionPlayerShotBoss(Shot **shot, Boss* boss, Player* player);
void CheckCollisionEnemyShot(Shot** firstShot, Player player[], const int nbrOfPlayer, const RoomGameData *room);
sfBool CheckCollisionShotTrap(Shot* shot);
void CheckCollisionEnemyShotPlayer(Player *player, Shot** firstShot);
void CheckCollisionEnemies(Enemy **firstEnemy, Player player[], const int nbrOfPlayer, const RoomGameData *room, Boss *boss);
void CheckCollisionPlayerEnemy(Player *player, Enemy *enemy);
void CheckCollisionMineEnemy(PlayerMineStat* mineStat, Enemy *enemy, Player* player);
void CheckCollisionPlayerConeOfFireEnemy(Player *player, Enemy *enemy);
void CheckCollisionPlayerConeOfFireBoss(Player *player, Boss *boss);
void CheckCollisionPlayerLoots(Player* player, LootData* lootdata);
void CheckCollisionPlayerRoom(Player *player, const RoomGameData *room);
void CheckCollisionPlayerTrap(Player *player);
void CheckCollisionEnemyEnemy(Enemy *firstEnemy, const Enemy* secondEnemy);
void CheckCollisionEnemyRoom(Enemy *enemy, const RoomGameData *room);

#endif
