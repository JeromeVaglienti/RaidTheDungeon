#ifndef PARTICLES_H
#define PARTICLES_H
#include <SFML\Graphics.h>
#include "..\GraphicsUtilities\CustomSprite\CustomSprite.h"

typedef enum fct
{
	linear,
	easeIn,
	easeOut,
	easeInOut
}Fct;

typedef struct Particles
{
	sfVector2f Position;
	sfVector2f Direction;
	sfVector2f Delta;
	sfVector2f Gravity;
	sfVector2f Scale;
	sfVector2f ScaleStart;
	sfVector2f ScaleEnd;

	Fct Func;
	float Speed;
	float TTL;
	float TTLStart;
	float Rotation;
	float RotationEnd;
	float FixedRotation;
	float TimerStopGravity;
	int FixedImageIndex;

	sfColor Color;
	sfColor ColorStart;
	sfColor ColorEnd;

	sfText* TextToDisplay;

	struct Particles* Next;

}Particles;

void SetParticleFunction(Particles* particle, Fct function);

//Add new particle default value, white color, scale 1.0f, time life 1.0f
//Example
//AddParticles(&newParticle);
//SetParticlePosition(newParticle, position);
//SetParticleColor(newParticle, startcolor, endcolor);
//SetParticleTTL(newParticle, 5.0f);
//SetParticleRotation(newParticle, rotation);
//SetParticleDirection(newParticle, directionVector);
//SetParticleSpeed(newParticle, Speed);
//SetParticleDelta(newParticle, deltaVector);
void AddParticles(Particles** firstParticle);

void SetParticlePosition(Particles* particle, const sfVector2f position);

void SetParticleDirection(Particles* particle, const sfVector2f direction);

void SetParticleDelta(Particles* particle, const sfVector2f delta);

void SetParticleScale(Particles* particle, const sfVector2f scaleStart, const sfVector2f scaleEnd);

void SetParticleSpeed(Particles* particle, const float speed);

void SetParticleRotation(Particles* particle, const float rotation);

void SetParticleColor(Particles* particle, const sfColor colorStart, const sfColor colorEnd);

void SetParticleTTL(Particles* particle, const float timeLife);

void SetImageIndex(Particles* particle, const int index);

void SetFixedRotation(Particles* particle, const float rotation);

void SetParticleGravity(Particles* particle, const sfVector2f gravity);

void SetParticleText(Particles* particle, const char* msg, const int data, const char* msg2);

void DeleteParticle(Particles **firstParticle, Particles *current);

void ClearParticle(Particles** firstParticle);

void UpdateParticles(Particles **firstParticle, const float deltaTime);

void DisplayParticles(Particles *firstParticle, sfSprite* sprite, sfRenderWindow *window, sfShader* shd, sfSprite** array);

#endif
