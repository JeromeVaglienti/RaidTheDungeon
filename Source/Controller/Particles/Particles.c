#include "Particles.h"
#include <stdio.h>
#include <stdlib.h>
#include "..\GraphicsUtilities\Utilities.h"
#include "..\..\Model\Math\MathUtilities.h"
#include "..\HUD\Text\Text.h"

// Modeled after the line y = x
float LinearInterpolation(const float p)
{
	return p;
}

// Modeled after the parabola y = x^2
float EaseIn(const float p)
{
	return p * p;
}

// Modeled after the parabola y = -x^2 + 2x
float EaseOut(const float p)
{
	return -(p * (p - 2.f));
}

// Modeled after the piecewise quadratic
// y = (1/2)((2x)^2)             ; [0, 0.5)
// y = -(1/2)((2x-1)*(2x-3) - 1) ; [0.5, 1]
float EaseInOut(const float p)
{
	if (p < 0.5f)
	{
		return 2.f * p * p;
	}
	else
	{
		return (-2.f * p * p) + (4.f * p) - 1.f;
	}
}

// Modeled after the cubic y = x^3
float CubicEaseIn(const float p)
{
	return p * p * p;
}

// Modeled after the cubic y = (x - 1)^3 + 1
float CubicEaseOut(const float p)
{
	float f = (p - 1.f);
	return f * f * f + 1.f;
}

// Modeled after the piecewise cubic
// y = (1/2)((2x)^3)       ; [0, 0.5)
// y = (1/2)((2x-2)^3 + 2) ; [0.5, 1]
float CubicEaseInOut(const float p)
{
	if (p < 0.5f)
	{
		return 4.f * p * p * p;
	}
	else
	{
		float f = ((2.f * p) - 2.f);
		return 0.5f * f * f * f + 1.f;
	}
}

// Modeled after the quartic x^4
float QuarticEaseIn(const float p)
{
	return p * p * p * p;
}

// Modeled after the quartic y = 1 - (x - 1)^4
float QuarticEaseOut(const float p)
{
	float f = (p - 1.f);
	return f * f * f * (1.f - p) + 1.f;
}

// Modeled after the piecewise quartic
// y = (1/2)((2x)^4)        ; [0, 0.5)
// y = -(1/2)((2x-2)^4 - 2) ; [0.5, 1]
float QuarticEaseInOut(const float p)
{
	if (p < 0.5f)
	{
		return 8.f * p * p * p * p;
	}
	else
	{
		float f = (p - 1.f);
		return -8.f * f * f * f * f + 1.f;
	}
}

// Modeled after the quintic y = x^5
float QuinticEaseIn(const float p)
{
	return p * p * p * p * p;
}

// Modeled after the quintic y = (x - 1)^5 + 1
float QuinticEaseOut(const float p)
{
	float f = (p - 1.f);
	return f * f * f * f * f + 1.f;
}

// Modeled after the piecewise quintic
// y = (1/2)((2x)^5)       ; [0, 0.5)
// y = (1/2)((2x-2)^5 + 2) ; [0.5, 1]
float QuinticEaseInOut(const float p)
{
	if (p < 0.5f)
	{
		return 16.f * p * p * p * p * p;
	}
	else
	{
		float f = ((2.f * p) - 2.f);
		return  0.5f * f * f * f * f * f + 1.f;
	}
}

// Modeled after quarter-cycle of sine wave
float SineEaseIn(const float p)
{
	return sinf((p - 1.f) * M_PI_2) + 1.f;
}

// Modeled after quarter-cycle of sine wave (different phase)
float SineEaseOut(const float p)
{
	return sinf(p * M_PI_2);
}

// Modeled after half sine wave
float SineEaseInOut(const float p)
{
	return 0.5f * (1.f - cosf(p * M_PI));
}

// Modeled after shifted quadrant IV of unit circle
float CircularEaseIn(const float p)
{
	return 1.f - sqrtf(1.f - (p * p));
}

// Modeled after shifted quadrant II of unit circle
float CircularEaseOut(const float p)
{
	return sqrtf((2.f - p) * p);
}

// Modeled after the piecewise circular function
// y = (1/2)(1 - sqrt(1 - 4x^2))           ; [0, 0.5)
// y = (1/2)(sqrt(-(2x - 3)*(2x - 1)) + 1) ; [0.5, 1]
float CircularEaseInOut(const float p)
{
	if (p < 0.5f)
	{
		return 0.5f * (1.f - sqrtf(1.f - 4.f * (p * p)));
	}
	else
	{
		return 0.5f * (sqrtf(-((2.f * p) - 3.f) * ((2.f * p) - 1.f)) + 1.f);
	}
}

// Modeled after the exponential function y = 2^(10(x - 1))
float ExponentialEaseIn(const float p)
{
	return (p == 0.0f) ? p : powf(2.f, 10.f * (p - 1.f));
}

// Modeled after the exponential function y = -2^(-10x) + 1
float ExponentialEaseOut(const float p)
{
	return (p == 1.0f) ? p : 1.f - powf(2.f, -10.f * p);
}

// Modeled after the piecewise exponential
// y = (1/2)2^(10(2x - 1))         ; [0,0.5)
// y = -(1/2)*2^(-10(2x - 1))) + 1 ; [0.5,1]
float ExponentialEaseInOut(const float p)
{
	if (p == 0.0f || p == 1.0f) return p;

	if (p < 0.5f)
	{
		return 0.5f * powf(2.f, (20.f * p) - 10.f);
	}
	else
	{
		return -0.5f * powf(2.f, (-20.f * p) + 10.f) + 1.f;
	}
}

// Modeled after the damped sine wave y = sin(13pi/2*x)*pow(2, 10 * (x - 1))
float ElasticEaseIn(const float p)
{
	return sinf(13.f * M_PI_2 * p) * powf(2.f, 10.f * (p - 1.f));
}

// Modeled after the damped sine wave y = sin(-13pi/2*(x + 1))*pow(2, -10x) + 1
float ElasticEaseOut(const float p)
{
	return sinf(-13.f * M_PI_2 * (p + 1.f)) * powf(2.f, -10.f * p) + 1.f;
}

// Modeled after the piecewise exponentially-damped sine wave:
// y = (1/2)*sin(13pi/2*(2*x))*pow(2, 10 * ((2*x) - 1))      ; [0,0.5)
// y = (1/2)*(sin(-13pi/2*((2x-1)+1))*pow(2,-10(2*x-1)) + 2) ; [0.5, 1]
float ElasticEaseInOut(const float p)
{
	if (p < 0.5f)
	{
		return 0.5f * sinf(13.f * M_PI_2 * (2.f * p)) * powf(2.f, 10.f * ((2.f * p) - 1.f));
	}
	else
	{
		return 0.5f * (sinf(-13.f * M_PI_2 * ((2.f * p - 1.f) + 1.f)) * powf(2.f, -10.f * (2.f * p - 1.f)) + 2.f);
	}
}

// Modeled after the overshooting cubic y = x^3-x*sin(x*pi)
float BackEaseIn(const float p)
{
	return p * p * p - p * sinf(p * M_PI);
}

// Modeled after overshooting cubic y = 1-((1-x)^3-(1-x)*sin((1-x)*pi))
float BackEaseOut(const float p)
{
	float f = (1.f - p);
	return 1.f - (f * f * f - f * sinf(f * M_PI));
}

// Modeled after the piecewise overshooting cubic function:
// y = (1/2)*((2x)^3-(2x)*sin(2*x*pi))           ; [0, 0.5)
// y = (1/2)*(1-((1-x)^3-(1-x)*sin((1-x)*pi))+1) ; [0.5, 1]
float BackEaseInOut(const float p)
{
	if (p < 0.5f)
	{
		float f = 2.f * p;
		return 0.5f * (f * f * f - f * sinf(f * M_PI));
	}
	else
	{
		float f = (1.f - (2.f * p - 1.f));
		return 0.5f * (1.f - (f * f * f - f * sinf(f * M_PI))) + 0.5f;
	}
}

float BounceEaseOut(const float p)
{
	if (p < 4.f / 11.0f)
	{
		return (121.f * p * p) / 16.0f;
	}
	else if (p < 8.f / 11.0f)
	{
		return (363.f / 40.0f * p * p) - (99.f / 10.0f * p) + 17.f / 5.0f;
	}
	else if (p < 9.f / 10.0f)
	{
		return (4356.f / 361.0f * p * p) - (35442.f / 1805.0f * p) + 16061.f / 1805.0f;
	}
	else
	{
		return (54.f / 5.0f * p * p) - (513.f / 25.0f * p) + 268.f / 25.0f;
	}
}

float BounceEaseIn(const float p)
{
	return 1.f - BounceEaseOut(1.f - p);
}


float BounceEaseInOut(const float p)
{
	if (p < 0.5f)
	{
		return 0.5f * BounceEaseIn(p * 2.f);
	}
	else
	{
		return 0.5f * BounceEaseOut(p * 2.f - 1.f) + 0.5f;
	}
}

void SetParticleFunction(Particles* particle, const Fct function)
{
	particle->Func = function;
}

void SetParticlePosition(Particles* particle, const sfVector2f position)
{
	particle->Position.x = position.x;
	particle->Position.y = position.y;
}

void SetParticleDirection(Particles* particle, const sfVector2f direction)
{
	particle->Direction.x = direction.x;
	particle->Direction.y = direction.y;
}

void SetParticleDelta(Particles* particle, const sfVector2f delta)
{
	particle->Delta.x = delta.x;
	particle->Delta.y = delta.y;
}

void SetParticleScale(Particles* particle, const sfVector2f scaleStart, const sfVector2f scaleEnd)
{
	particle->Scale = scaleStart;
	particle->ScaleStart = scaleStart;
	particle->ScaleEnd = scaleEnd;
}

void SetParticleSpeed(Particles* particle, const float speed)
{
	particle->Speed = speed;
}

void SetParticleRotation(Particles* particle, const float rotation)
{
	particle->Rotation = rotation;
	particle->RotationEnd = rotation;
}

void SetParticleColor(Particles* particle, const sfColor colorStart, const sfColor colorEnd)
{
	particle->Color = colorStart;
	particle->ColorStart = colorStart;
	particle->ColorEnd = colorEnd;
}

void SetParticleTTL(Particles* particle, const float timeLife)
{
	particle->TTL = timeLife;
	particle->TTLStart = timeLife;
}

void SetImageIndex(Particles* particle, const int index)
{
	particle->FixedImageIndex = index;
}

void SetFixedRotation(Particles* particle, const float rotation)
{
	particle->FixedRotation = rotation;
}

void SetParticleGravity(Particles* particle, const sfVector2f gravity)
{
	particle->Gravity = gravity;
}

void AddParticles(Particles** firstParticle)
{
	Particles* tempParticle = (Particles*)calloc(1, sizeof(Particles));
	tempParticle->Next = *firstParticle;

	tempParticle->Color = sfWhite;
	tempParticle->ColorEnd = sfWhite;

	tempParticle->Scale = (sfVector2f) { 1.0f, 1.0f };
	tempParticle->ScaleEnd = (sfVector2f) { 1.0f, 1.0f };

	tempParticle->FixedRotation = -1.0f;
	tempParticle->FixedImageIndex = -1;

	tempParticle->Gravity.x = 0.0f;
	tempParticle->Gravity.y = 0.0f;
	tempParticle->TimerStopGravity = (float)rand() / (float)RAND_MAX;

	tempParticle->Func = linear;

	tempParticle->TextToDisplay = NULL;

	SetParticleTTL(tempParticle, 1.0f);
	*firstParticle = tempParticle;

}

void SetParticleText(Particles* particle, const char* msg, const int data, const char* msg2)
{
	char message[20];
	sprintf(message, "%s %d %s", msg, data, msg2);
	particle->TextToDisplay = InitText(message, particle->Position, sfWhite, 20);
}

void DeleteParticle(Particles **firstParticle, Particles *current)
{
	Particles* tempParticle = *firstParticle;

	if (tempParticle == current)
	{
		*firstParticle = current->Next;
		free(current);
	}
	else
	{
		while (tempParticle != NULL)
		{
			if (tempParticle->Next == current)
			{
				tempParticle->Next = current->Next;
				free(current);
				return;
			}
			tempParticle = tempParticle->Next;
		}
	}
}

void ClearParticle(Particles** firstParticle)
{
	if (firstParticle == NULL || *firstParticle == NULL)
	{
		return;
	}
	Particles* tempParticle = *firstParticle;
	while (tempParticle->Next != NULL)
	{
		Particles* delParticle = tempParticle->Next;
		tempParticle->Next = delParticle->Next;
		free(delParticle);
	}
	free(tempParticle);
	free(*firstParticle);
	*firstParticle = NULL;
}

void UpdateParticles(Particles **firstParticle, const float deltaTime)
{
	if (firstParticle == NULL || *firstParticle == NULL)
	{
		return;
	}

	float t = 0.0f;
	Particles *tempParticle = *firstParticle;
	while (tempParticle != NULL)
	{
		tempParticle->TTL -= deltaTime;
		t = tempParticle->TTL / tempParticle->TTLStart;

		switch (tempParticle->Func)
		{
		case linear:
			t = LinearInterpolation(t);
			break;
		case easeIn:
			t = EaseIn(t);
			break;
		case easeOut:
			t = EaseOut(t);
			break;
		case easeInOut:
			t = EaseInOut(t);
			break;
		}

		tempParticle->Rotation += deltaTime * tempParticle->RotationEnd;
		tempParticle->RotationEnd = fmodf(tempParticle->RotationEnd, 360);

		tempParticle->Speed *= t;

		if (tempParticle->Scale.x < tempParticle->ScaleEnd.x)
		{
			tempParticle->Scale.x = (1.f - t) * tempParticle->ScaleEnd.x;
		}

		if (tempParticle->Scale.x > tempParticle->ScaleEnd.x)
		{
			tempParticle->Scale.x = t * tempParticle->ScaleStart.x;
		}

		if (tempParticle->Scale.y < tempParticle->ScaleEnd.y)
		{
			tempParticle->Scale.y = (1.f - t) * tempParticle->ScaleEnd.y;
		}

		if (tempParticle->Scale.y > tempParticle->ScaleEnd.y)
		{
			tempParticle->Scale.y = t * tempParticle->ScaleStart.y;
		}




		if (tempParticle->Color.r < tempParticle->ColorEnd.r)
		{
			tempParticle->Color.r = (sfUint8)((1.f - t) * tempParticle->ColorEnd.r);
		}

		if (tempParticle->Color.r > tempParticle->ColorEnd.r)
		{
			tempParticle->Color.r = (sfUint8)(t * tempParticle->ColorStart.r);
		}

		if (tempParticle->Color.g < tempParticle->ColorEnd.g)
		{
			tempParticle->Color.g = (sfUint8)((1.f - t) * tempParticle->ColorEnd.g);
		}

		if (tempParticle->Color.g > tempParticle->ColorEnd.g)
		{
			tempParticle->Color.g = (sfUint8)(t * tempParticle->ColorStart.g);
		}

		if (tempParticle->Color.b < tempParticle->ColorEnd.b)
		{
			tempParticle->Color.b = (sfUint8)((1.f - t) * tempParticle->ColorEnd.b);
		}

		if (tempParticle->Color.b > tempParticle->ColorEnd.b)
		{
			tempParticle->Color.b = (sfUint8)(t * tempParticle->ColorStart.b);
		}


		if (tempParticle->Color.a < tempParticle->ColorEnd.a)
		{
			tempParticle->Color.a = (sfUint8)((1.f - t) * tempParticle->ColorEnd.a);
		}

		if (tempParticle->Color.a > tempParticle->ColorEnd.a)
		{
			tempParticle->Color.a = (sfUint8)(t * tempParticle->ColorStart.a);
		}

		tempParticle->Position.x += tempParticle->Direction.x * tempParticle->Speed * deltaTime;
		tempParticle->Position.y += tempParticle->Direction.y * tempParticle->Speed * deltaTime;
		tempParticle->Position.x += tempParticle->Delta.x * deltaTime;
		tempParticle->Position.y += tempParticle->Delta.y * deltaTime;
		if (tempParticle->TextToDisplay != NULL)
			sfText_setPosition(tempParticle->TextToDisplay, tempParticle->Position);


		if (tempParticle->Speed >= tempParticle->TimerStopGravity)
		{
			tempParticle->Gravity.x += tempParticle->Gravity.x * tempParticle->Speed * deltaTime;
			tempParticle->Gravity.y += tempParticle->Gravity.y * tempParticle->Speed * deltaTime;

			tempParticle->Position.x += tempParticle->Gravity.x;
			tempParticle->Position.y += tempParticle->Gravity.y;
		}


		if (t <= 0.01f)
		{
			Particles *delParticle = tempParticle;
			tempParticle = tempParticle->Next;
			DeleteParticle(firstParticle, delParticle);
		}
		else
		{
			tempParticle = tempParticle->Next;
		}
	}
}

void DisplayParticles(Particles *firstParticle, sfSprite* sprite, sfRenderWindow *window, sfShader* shd, sfSprite** arrays)
{
	sfSprite* temp = sprite;

	Particles* tempParticle = firstParticle;
	while (tempParticle != NULL)
	{
		if (arrays)
		{
			temp = arrays[tempParticle->FixedImageIndex];
		}
		if (temp != NULL)
		{
			sfSprite_setScale(temp, tempParticle->Scale);
			sfSprite_setColor(temp, tempParticle->Color);


			if (tempParticle->FixedRotation == -1.0f)
			{
				sfSprite_setRotation(temp, tempParticle->Rotation);
			}
			else
			{
				sfSprite_setRotation(temp, tempParticle->FixedRotation);
			}
		}

		if (shd != NULL)
		{
			sfShader_setFloatUniform(shd, "alpha", (float)(tempParticle->Color.a) / 255);

		}
		if (temp != NULL)
		{
			BlitSprite(window, temp, tempParticle->Position, shd);
		}
		if (tempParticle->TextToDisplay != NULL)
		{
			sfRenderWindow_drawText(window, tempParticle->TextToDisplay, NULL);
		}
		tempParticle = tempParticle->Next;

	}
}