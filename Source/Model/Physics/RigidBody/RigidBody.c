#include "RigidBody.h"
#include "..\..\Math\MathUtilities.h"
//init all variable in a rigidbody at 0 and set the mass to 1
void InitializeRigidBody(RigidBody* pRigidBody)
{
	RigidBody tempRB = { 0 };
	*pRigidBody = tempRB;
	pRigidBody->Mass = 1.f;
}

void UpdateRigidBody(RigidBody* pRigidBody, const float pDeltaTime)
{
	/*pRigidBody->Acceleration.x = pRigidBody->Force.x / pRigidBody->Mass;
	pRigidBody->Acceleration.y = pRigidBody->Force.y / pRigidBody->Mass;*/

	/*if (pRigidBody->IsGravityEnabled)
	{
		pRigidBody->Acceleration.y += GRAVITY;
	}*/

	pRigidBody->Position.x += pRigidBody->Velocity.x * pDeltaTime;
	pRigidBody->Position.y += pRigidBody->Velocity.y * pDeltaTime;

	pRigidBody->Position.x += pRigidBody->Friction.x*pDeltaTime;
	pRigidBody->Position.y += pRigidBody->Friction.y*pDeltaTime;

	
	pRigidBody->Position.x += pRigidBody->KnockBackForce.x*pDeltaTime;
	pRigidBody->KnockBackForce.x *= 0.96f;
	
	pRigidBody->Position.y += pRigidBody->KnockBackForce.y*pDeltaTime;
	pRigidBody->KnockBackForce.y *= 0.96f;
	

	pRigidBody->Friction.x *= 0.99f;
	pRigidBody->Friction.y *= 0.99f;


}