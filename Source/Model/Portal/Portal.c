#include "Portal.h"
#include "..\Physics\FunctionsCollisions\Colision.h"

void InitPortal(Portal* ToInit, const sfVector2f Pos, const sfVector2f Dimension, const float TimeForTransfert, const float TimeForEndOfTransfer)
{
	static int ID = 0;
	ToInit->PosCenter = Pos;
	ToInit->TimeForTransfert = TimeForTransfert;
	ToInit->Timer = 0.0f;
	ToInit->Dimension.x = Dimension.x * 64.f;
	ToInit->Dimension.y = Dimension.y * 64.f;
	ToInit->IsTransfering = sfFalse;
	ToInit->IsNOTActive = sfFalse;
	ToInit->IsReceivingTransfert = sfFalse;
	ToInit->TimerEndOfTransfer = 0.0f;
	ToInit->TimeForEndOfTransfer = TimeForEndOfTransfer;
	ToInit->OpenAnimKey = InitAnimKey(4, (4.0f / 6.0f), sfTrue);
	ToInit->TelePortAnimKey = InitAnimKey(22, (22.0f / 12.0f), sfFalse);
	ToInit->ReceivingAnimKey = InitAnimKey(9, (9.0f / 12.0f), sfFalse);
	ToInit->IsNOTActive = sfFalse;
	ToInit->ID = ID;

	ToInit->LinkPortal = NULL;
	ID++;
}

sfBool UpdatePortals(Portal* ToUpdate, Player* ArrayOfPlayer, const int nbPlayer, const float deltaTime)
{	
	if (ToUpdate->IsReceivingTransfert)
	{
		if (ToUpdate->ReceivingAnimKey.actualFrameToShow == 5)
		{
			for (int i = 0; i < nbPlayer; i++)
			{
				ArrayOfPlayer[i].States.IsTeleporting = sfFalse;
			}

		}
		
		ToUpdate->TimerEndOfTransfer += deltaTime;
		UpdateAnimationKey(&ToUpdate->ReceivingAnimKey, deltaTime);
		if (ToUpdate->TimerEndOfTransfer >= ToUpdate->TimeForEndOfTransfer)
		{
			ToUpdate->TimerEndOfTransfer = 0.0f;
			ToUpdate->ReceivingAnimKey.actualFrameToShow = 0;
			ToUpdate->ReceivingAnimKey.actualDuration = 0.0f;
			ToUpdate->IsReceivingTransfert = sfFalse;
		}
		return sfFalse;
	}
	
	if (!ToUpdate->IsNOTActive)
	{
		if (ToUpdate->IsTransfering && !ToUpdate->IsReceivingTransfert)
		{
			UpdateAnimationKey(&ToUpdate->TelePortAnimKey, deltaTime);
			if (ToUpdate->TelePortAnimKey.actualFrameToShow == 13)
			{
				for (int i = 0; i < nbPlayer; i++)
				{
					ArrayOfPlayer[i].States.IsTeleporting = sfTrue;
				}
				
			}
			if (ToUpdate->Timer >= ToUpdate->TimeForTransfert)
			{
				ToUpdate->Timer = 0.0f;
				ToUpdate->TelePortAnimKey.actualFrameToShow = 0;
				ToUpdate->TelePortAnimKey.actualDuration = 0.0f;
				for (int i = 0; i < nbPlayer; i++)
				{
					SetPlayerPos(&ArrayOfPlayer[i], (sfVector2f) { ToUpdate->LinkPortal->PosCenter.x * 64.f, ToUpdate->LinkPortal->PosCenter.y * 64.f });
				}
				ToUpdate->LinkPortal->IsReceivingTransfert = sfTrue;
				ToUpdate->IsTransfering = sfFalse;

				return sfTrue;
			}

			sfBool temp = sfTrue;
			for (int i = 0; i < nbPlayer; i++)
			{
				if (!IsCollidingCircleSquare(ArrayOfPlayer[i].Physics.Collider, (sfVector2f) { ToUpdate->PosCenter.x * 64.f, ToUpdate->PosCenter.y * 64.f }, ToUpdate->Dimension).IsCollided && !ArrayOfPlayer[i].States.IsDead)
				{
					ToUpdate->Timer = 0.0f;
					ToUpdate->IsTransfering = sfFalse;
					ToUpdate->TelePortAnimKey.actualFrameToShow = 0;
					ToUpdate->TelePortAnimKey.actualDuration = 0.0f;
				}
			}

			if (temp)
			{
				ToUpdate->Timer += deltaTime;
			}
		}
		else
		{
			UpdateAnimationKey(&ToUpdate->OpenAnimKey, deltaTime);
			
			for (int i = 0; i < nbPlayer; i++)
			{
				if (!IsCollidingCircleSquare(ArrayOfPlayer[i].Physics.Collider, (sfVector2f) { ToUpdate->PosCenter.x * 64.f, ToUpdate->PosCenter.y * 64.f }, ToUpdate->Dimension).IsCollided && !ArrayOfPlayer[i].States.IsDead)
				{
					ToUpdate->IsTransfering = sfFalse;
					return sfFalse;
				}
			}

			ToUpdate->IsTransfering = sfTrue;
		}
	}

	return sfFalse;
}