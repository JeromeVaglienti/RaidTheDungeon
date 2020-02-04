#include "Camera.h"
#include "..\..\Controller\MainSceneController\ShadersController\ShadersController.h"
#include "..\State\State.h"

Camera InitializeCamera(sfVector2f pSize, const sfVector2f pWindowSize, const float pSpeed)
{
	Camera newCamera;

	int ratioX = (int)((pSize.x * pWindowSize.y) / pWindowSize.x);
	int ratioY = (int)((pSize.y * pWindowSize.x) / pWindowSize.y);

	float ratioX2 = pWindowSize.x / 1920.0f;
	float ratioY2 = pWindowSize.y / 1080.0f;
	pSize.x = (float)ratioY;

	newCamera.Angle = 0.0f;
	newCamera.View = sfView_create();
	newCamera.ViewHUD = sfView_create();
	newCamera.ViewMap = sfView_create();
	newCamera.Position = (sfVector2f) { 0.0f, 0.0f };
	newCamera.PositionMiniMap = (sfVector2f) { 0.0f, 0.0f };
	newCamera.Scale = (sfVector2f) { 1.0f, 1.0f };
	newCamera.Speed = pSpeed;
	newCamera.Size = pSize;
	newCamera.FirstSize = pSize;
	newCamera.Timer = 0.0f;
	newCamera.isZoomed = sfTrue;
	sfVector2f sizeMap = { 150, 150 };

	sfView_setSize(newCamera.View, pSize);
	sfView_setSize(newCamera.ViewHUD, pWindowSize);
	sfView_setSize(newCamera.ViewMap, sizeMap);
	sfView_setViewport(newCamera.ViewMap, (sfFloatRect) { 0.83f, 0.695f, 0.15f, 0.275f });
	sfView_setCenter(newCamera.ViewHUD, (sfVector2f) { pWindowSize.x / 2.f, pWindowSize.y / 2.f });
	return newCamera;
}

void ClearCamera(Camera *camera, const int numberOfPlayer)
{
	sfView_destroy(camera->View);
	sfView_destroy(camera->ViewHUD);
	sfView_destroy(camera->ViewMap);
	ClearMiniMap(numberOfPlayer);
}

void FollowObject(Camera* pCamera, Player* pPlayers, const int numberOfPlayer, const RoomGameData room, const float pDeltatime)
{
	int nbPlayer = 0;
	float speed = pCamera->Speed*pDeltatime;
	//Maximum distance of movement camera
	float maxDistance = 80;
	float AverageX = 0;
	float AverageY = 0;

	sfVector2f pPositionObject = { 0.0f,0.0f };

	sfBool moveX = sfFalse;
	sfBool moveY = sfFalse;

	for (int i = 0; i < numberOfPlayer; i++)
	{
		if (!pPlayers[i].States.IsDead)
		{
			if (pPlayers[i].Physics.Rb.Position.x + 32.0f > pCamera->Position.x + pCamera->Size.x / 2.0f)
			{
				pPlayers[i].States.IsStopXr = sfTrue;
			}
			else pPlayers[i].States.IsStopXr = sfFalse;

			if (pPlayers[i].Physics.Rb.Position.x - 32.0f < pCamera->Position.x - pCamera->Size.x / 2.0f)
			{
				pPlayers[i].States.IsStopXl = sfTrue;
			}
			else pPlayers[i].States.IsStopXl = sfFalse;

			if (pPlayers[i].Physics.Rb.Position.y + 32.0f > pCamera->Position.y + pCamera->Size.y / 2.0f)
			{
				pPlayers[i].States.IsStopYd = sfTrue;
			}
			else pPlayers[i].States.IsStopYd = sfFalse;

			if (pPlayers[i].Physics.Rb.Position.y - 32.0f < pCamera->Position.y - pCamera->Size.y / 2.0f)
			{
				pPlayers[i].States.IsStopYu = sfTrue;
			}
			else pPlayers[i].States.IsStopYu = sfFalse;

			pPositionObject.x += pPlayers[i].Physics.Rb.Position.x;
			pPositionObject.y += pPlayers[i].Physics.Rb.Position.y;
			AverageX += (pPlayers[i].Physics.Rb.Position.x + pPlayers[i].Hud.Cursor.Position.x);
			AverageY += (pPlayers[i].Physics.Rb.Position.y + pPlayers[i].Hud.Cursor.Position.y);

			if (fabsf(pCamera->Position.x - pPlayers[i].Physics.Rb.Position.x) < pCamera->Size.x / 2.5f)
				moveX = sfTrue;

			if (fabsf(pCamera->Position.y - pPlayers[i].Physics.Rb.Position.y) < pCamera->Size.y / 2.5f)
				moveY = sfTrue;

			nbPlayer++;
		}
	}
	//Compute the average position of the alive players
	pPositionObject.x /= nbPlayer;
	pPositionObject.y /= nbPlayer;

	AverageX /= (2 * nbPlayer);
	AverageY /= (2 * nbPlayer);

	//Move the position camera thanks to a Lerp 
	if (nbPlayer != 0)
	{
		if(moveX)
			pCamera->Position.x = Lerp(pCamera->Position.x, Clamp(AverageX, pPositionObject.x - maxDistance, pPositionObject.x + maxDistance), speed);
		if(moveY)
			pCamera->Position.y = Lerp(pCamera->Position.y, Clamp(AverageY, pPositionObject.y - maxDistance, pPositionObject.y + maxDistance), speed);
	}

	DungeonId tempId = GetActualDungeonId();
	if (tempId == elemDungeon || tempId == razarDungeon)
	{
		float rm_width = room.Width / 2.0f;
		float rm_height = room.Height / 2.0f;

		pCamera->Position.x = Clamp(pCamera->Position.x, (room.Position.x - rm_width)*64.0f + pCamera->Size.x / 2.0f, (room.Position.x + rm_width)*64.0f - pCamera->Size.x / 2.0f);
		pCamera->Position.y = Clamp(pCamera->Position.y, (room.Position.y - rm_height)*64.0f + pCamera->Size.y / 2.0f, (room.Position.y + rm_height)*64.0f - pCamera->Size.y / 2.0f);
	}
	pCamera->Angle = Lerp(pCamera->Angle, 0.0f, speed);

}

void FollowObjectSpecialBoy(Camera* pCamera, const Player* pPlayers, const sfVector2f posBoy, const int numberOfPlayer, const RoomGameData room, const float pDeltatime)
{
	int nbPlayer = 0;
	float speed = pCamera->Speed*pDeltatime;
	//Maximum distance of movement camera
	float maxDistance = 80;
	float AverageX = 0;
	float AverageY = 0;
	sfVector2f pPositionObject = { 0.0f,0.0f };

	for (int i = 0; i < numberOfPlayer; i++)
	{
		if (!pPlayers[i].States.IsDead)
		{
			pPositionObject.x += pPlayers[i].Physics.Rb.Position.x;
			pPositionObject.y += pPlayers[i].Physics.Rb.Position.y;
			AverageX += (pPlayers[i].Physics.Rb.Position.x + pPlayers[i].Hud.Cursor.Position.x);
			AverageY += (pPlayers[i].Physics.Rb.Position.y + pPlayers[i].Hud.Cursor.Position.y);
			nbPlayer++;
		}
	}
	nbPlayer += 1;

	pPositionObject.x += posBoy.x;
	pPositionObject.y += posBoy.y;
	AverageX += posBoy.x * 3;
	AverageY += posBoy.y * 3;
	//Compute the average position of the alive players
	pPositionObject.x /= nbPlayer;
	pPositionObject.y /= nbPlayer;

	AverageX /= (2 * nbPlayer);
	AverageY /= (2 * nbPlayer);

	//Move the position camera thanks to a Lerp 
	if (nbPlayer != 0)
	{
		pCamera->Position.x = Lerp(pCamera->Position.x, Clamp(AverageX, pPositionObject.x - maxDistance, pPositionObject.x + maxDistance), speed);
		pCamera->Position.y = Lerp(pCamera->Position.y, Clamp(AverageY, pPositionObject.y - maxDistance, pPositionObject.y + maxDistance), speed);
	}

	if (GetActualDungeonId() == elemDungeon)
	{
		float rm_width = room.Width / 2.0f;
		float rm_height = room.Height / 2.0f;
		pCamera->Position.x = Clamp(pCamera->Position.x, (room.Position.x - rm_width)*64.0f + pCamera->Size.x / 2.0f, (room.Position.x + rm_width)*64.0f - pCamera->Size.x / 2.0f);
		pCamera->Position.y = Clamp(pCamera->Position.y, (room.Position.y - rm_height)*64.0f + pCamera->Size.y / 2.0f, (room.Position.y + rm_height)*64.0f - pCamera->Size.y / 2.0f);
	}
	pCamera->Angle = Lerp(pCamera->Angle, 0.0f, speed);

}

void MovementToASpecificPoint(Camera* pCamera, const sfVector2f pointToReach, const float pDeltatime)
{
	float speed = (pCamera->Speed / 2.f)*pDeltatime;
	pCamera->Position.x = Lerp(pCamera->Position.x, pointToReach.x, speed);
	pCamera->Position.y = Lerp(pCamera->Position.y, pointToReach.y, speed);
}

void FollowObjectMiniMap(Camera* pCamera, const Player* pPlayers, const int numberOfPlayer, const float pDeltatime)
{
	float speed = pCamera->Speed*pDeltatime;
	int nbPlayer = 0;
	sfVector2f pPositionObject = { 0.0f,0.0f };
	for (int i = 0; i < numberOfPlayer; i++)
	{
		if (!pPlayers[i].States.IsDead)
		{
			sfVector2f tempPos = GetPlayerCirclePos(i);
			pPositionObject.x += tempPos.x;//sfCircleShape_getPosition(GetCirclePlayer(i)).x;
			pPositionObject.y += tempPos.y; //sfCircleShape_getPosition(GetCirclePlayer(i)).y;
			nbPlayer++;
		}
	}
	pPositionObject.x /= nbPlayer;
	pPositionObject.y /= nbPlayer;

	if (nbPlayer != 0)
	{
		pCamera->PositionMiniMap.x = Lerp(pCamera->PositionMiniMap.x, pPositionObject.x, speed);
		pCamera->PositionMiniMap.y = Lerp(pCamera->PositionMiniMap.y, pPositionObject.y, speed);
	}
}

void SetZoom(Camera* camera, const sfBool haveToUnZoom)
{
	if (haveToUnZoom)
	{
		camera->Size.x *= 1.3f;
		camera->Size.y *= 1.3f;
		sfView_setSize(camera->View, camera->Size);
	}
	else
	{
		camera->Size = camera->FirstSize;
		sfView_setSize(camera->View, camera->Size);
	}
}

void ChangeMiniMap(Camera* camera, const int numberOfPlayer)
{
	if (IsEnableMap())
	{
		sfVector2f center = CalculateCenterDiscoveryMap();
		sfVector2f sizeMap = { 400, 310 };
		sfView_setSize(camera->ViewMap, sizeMap);
		camera->PositionMiniMap.x = center.x;
		camera->PositionMiniMap.y = center.y;
		sfView_setViewport(camera->ViewMap, (sfFloatRect) { 0.f, 0.f, 1.f, 1.f });
	}
	else
	{
		sfVector2f sizeMap = { 150,150 };
		sfView_setSize(camera->ViewMap, sizeMap);
		sfView_setViewport(camera->ViewMap, (sfFloatRect) { 0.83f, 0.695f, 0.15f, 0.275f });
		for (int i = 0; i < numberOfPlayer; i++)
		{
			sfVector2f tempPos = GetPlayerCirclePos(i);
			camera->PositionMiniMap.x = tempPos.x; //sfCircleShape_getPosition(GetCirclePlayer(i)).x;
			camera->PositionMiniMap.y = tempPos.y; //sfCircleShape_getPosition(GetCirclePlayer(i)).y;
		}
	}
}

void DisplayCamera(const Camera* pCamera, sfRenderWindow* pWindow)
{
	sfView_setRotation(pCamera->View, pCamera->Angle);
	sfView_setCenter(pCamera->View, pCamera->Position);
	sfRenderWindow_setView(pWindow, pCamera->View);
	//sfRenderTexture_setView(GetNormalMap(), pCamera->View);
}

void DisplayCameraHud(const Camera* pCamera, sfRenderWindow* pWindow)
{
	sfRenderWindow_setView(pWindow, pCamera->ViewHUD);
}

void DisplayViewMiniMap(const Camera* pCamera, const Player *player, const int numberOfPlayer, sfRenderWindow* pWindow, sfShader* shd)
{
	sfView_setCenter(pCamera->ViewMap, pCamera->PositionMiniMap);
	sfRenderWindow_setView(pWindow, pCamera->ViewMap);
	DisplayMiniMap(player, numberOfPlayer, pWindow, shd);
}

sfBool DrawOnCamera(const Camera pCamera, const sfSprite* pSprite, const sfVector2f pPosition)
{
	sfFloatRect tempFloatRect = sfSprite_getGlobalBounds(pSprite);
	sfVector2f size = { tempFloatRect.width, tempFloatRect.height };
	float divider = 1.98f;
	return (pCamera.Position.x + (pCamera.Size.x + size.x) / divider > pPosition.x && pCamera.Position.x - (pCamera.Size.x + size.x) / divider < pPosition.x &&
		pCamera.Position.y + (pCamera.Size.y + size.y) / divider > pPosition.y && pCamera.Position.y - (pCamera.Size.y + size.y) / divider < pPosition.y);
}

void ActiveLightOnCamera(const Camera pCamera)
{
	float divider = 1.98f;
	PointLight* lights = GetPointLights();
	int NumberPointLights = GetNumberPointLights();
	for (int i = 0; i < NumberPointLights; i++)
	{
		if (pCamera.Position.x + (pCamera.Size.x + 2.0f*lights[i].Data.Size.x) / divider > lights[i].Data.Pos.x && pCamera.Position.x - (pCamera.Size.x + 2.0f*lights[i].Data.Size.x) / divider <  lights[i].Data.Pos.x &&
			pCamera.Position.y + (pCamera.Size.y + 2.0f*lights[i].Data.Size.y) / divider > lights[i].Data.Pos.y && pCamera.Position.y - (pCamera.Size.y + 2.0f*lights[i].Data.Size.y) / divider < lights[i].Data.Pos.y)
		{
			if (!lights[i].Data.enabled)
				ActivatePointLight(&lights[i], 1);
		}
		else
		{
			if (lights[i].Data.enabled)
			{
				ActivatePointLight(&lights[i], 0);
				lights[i].Index = -1;
			}
		}
	}
}

void ShakeCamera(Camera* pCamera, const int pPower, const float pTime, const float pDeltaTime)
{
	pCamera->Timer += pDeltaTime;

	if (pCamera->Timer < pTime)
	{
		pCamera->Position.x += (rand() % pPower - pPower / 2) * 0.1f;
		pCamera->Position.y += (rand() % pPower - pPower / 2) * 0.1f;
		pCamera->Angle += (rand() % pPower - pPower / 2) * 0.01f;
	}
	else
	{
		pCamera->Timer = 0.0f;
	}
}