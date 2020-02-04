#include "Trap.h"
TrapData trapData = { 0 };


void AddTrap(const sfVector2f pos, const sfVector2f direction, const IdTrap id, const float radius, const sfBool canSlow, const float percentageSlow, const float lifeTime, const unsigned short damage, const float speed)
{
	if (trapData.NbrTrap < 100)
	{
		trapData.Traps[trapData.NbrTrap].Id = id;
		trapData.Traps[trapData.NbrTrap].Pos = pos;
		trapData.Traps[trapData.NbrTrap].Direction = direction;
		trapData.Traps[trapData.NbrTrap].CanSlow = canSlow;
		trapData.Traps[trapData.NbrTrap].PercentageSpeed = percentageSlow;
		trapData.Traps[trapData.NbrTrap].Collider.center = pos;
		trapData.Traps[trapData.NbrTrap].Collider.radius = radius;
		trapData.Traps[trapData.NbrTrap].Damage = damage;
		trapData.Traps[trapData.NbrTrap].TimerDestroy = lifeTime;
		trapData.Traps[trapData.NbrTrap].Speed = speed;
		if (id == flask)
		{
			trapData.Traps[trapData.NbrTrap].MustDamage = sfTrue;
			trapData.Traps[trapData.NbrTrap].CanMove = sfFalse;
		}
		{
			trapData.Traps[trapData.NbrTrap].MustDamage = sfFalse;
			trapData.Traps[trapData.NbrTrap].CanMove = sfFalse;
		}

		if (id == flask)
		{
			trapData.Traps[trapData.NbrTrap].KeyAnimation = InitAnimKey(12, .5f, true);
		}
		else if (id == lightning)
		{
			trapData.Traps[trapData.NbrTrap].KeyAnimation = InitAnimKey(24, 2.f, false);
		}
		else if (id == rockSpike || id == rockSpikeBoss)
		{
			trapData.Traps[trapData.NbrTrap].KeyAnimation = InitAnimKey(25, (25.0f/12.0f), false);
		}
#if _DEBUG
		trapData.Traps[trapData.NbrTrap].Shape = sfCircleShape_create();
		sfCircleShape_setOutlineThickness(trapData.Traps[trapData.NbrTrap].Shape, 1.f);
		sfCircleShape_setOutlineColor(trapData.Traps[trapData.NbrTrap].Shape, sfRed);
		sfCircleShape_setFillColor(trapData.Traps[trapData.NbrTrap].Shape, sfTransparent);
		sfCircleShape_setRadius(trapData.Traps[trapData.NbrTrap].Shape, trapData.Traps[trapData.NbrTrap].Collider.radius);
		sfCircleShape_setOrigin(trapData.Traps[trapData.NbrTrap].Shape, (sfVector2f) { trapData.Traps[trapData.NbrTrap].Collider.radius, trapData.Traps[trapData.NbrTrap].Collider.radius });
#endif
		trapData.NbrTrap++;
	}
}

void UpdateTrap(const float deltaTime)
{
	for (int i = 0; i < (int)trapData.NbrTrap; i++)
	{
		UpdateAnimationKey(&trapData.Traps[i].KeyAnimation, deltaTime);
		if (trapData.Traps[i].CanMove)
		{
			trapData.Traps[i].Pos.x += trapData.Traps[i].Speed * trapData.Traps[i].Direction.x * deltaTime;
			trapData.Traps[i].Pos.y += trapData.Traps[i].Speed * trapData.Traps[i].Direction.y * deltaTime;
			trapData.Traps[i].Collider.center = trapData.Traps[i].Pos;
		}
		trapData.Traps[i].MustDamage = sfFalse;
		if (trapData.Traps[i].Id == flask)
		{
			trapData.Traps[i].MustDamage = sfTrue;
		}
		else if (trapData.Traps[i].Id == lightning && trapData.Traps[i].KeyAnimation.actualFrameToShow == 15)
		{
			trapData.Traps[i].MustDamage = sfTrue;
		}
		else if (trapData.Traps[i].Id == rockSpike && trapData.Traps[i].KeyAnimation.actualFrameToShow > 14 && trapData.Traps[i].KeyAnimation.actualFrameToShow < 18)
		{
			trapData.Traps[i].MustDamage = sfTrue;
		}
		else if (trapData.Traps[i].Id == rockSpikeBoss && trapData.Traps[i].KeyAnimation.actualFrameToShow > 14 && trapData.Traps[i].KeyAnimation.actualFrameToShow < 18)
		{
			trapData.Traps[i].MustDamage = sfTrue;
		}
		trapData.Traps[i].TimerDestroy -= deltaTime;
		if (trapData.Traps[i].TimerDestroy <= 0)
		{
			RemoveTrap(i);
		}
	}
}

void RemoveTrap(const unsigned int index)
{
	trapData.NbrTrap--;
	trapData.Traps[index] = trapData.Traps[trapData.NbrTrap];
}

TrapData* GetPointerToTrapData()
{
	return &trapData;
}

TrapData GetTrapData()
{
	return trapData;
}

