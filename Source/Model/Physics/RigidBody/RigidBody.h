#ifndef _RIGIDBODY_H
#define _RIGIDBODY_H

#include <stdio.h>
#include <stdlib.h>
#include <SFML/System.h>

typedef struct RIGIDBODY
{
	sfVector2f Position;
	sfVector2f Velocity;
	sfVector2f KnockBackForce;
	sfVector2f Friction;
	float Mass;
	//sfBool IsGravityEnabled;

}RigidBody;

void InitializeRigidBody(RigidBody* pRigidBody);
void UpdateRigidBody(RigidBody* pRigidBody, const float pDeltaTime);

#endif