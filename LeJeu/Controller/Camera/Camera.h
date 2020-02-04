#ifndef _CAMERA_H
#define _CAMERA_H

#include "..\..\Model\Math\MathUtilities.h"
#include "..\..\Model\Player\Player.h"
#include <SFML\Graphics.h>
#include "..\HUD\MiniMap\MiniMap.h"
#include "..\MainSceneController\LightController\PointLight.h"
#include "..\..\LeJeu\Model\Room\Room.h"
typedef struct CAMERA
{
	sfView* View;
	sfView* ViewHUD;
	sfView* ViewMap;
	sfVector2f Position;
	sfVector2f Scale;
	sfVector2f Size;
	sfVector2f FirstSize;
	sfVector2f PositionMiniMap;
	sfBool isZoomed;
	float Angle;
	float Timer;
	float Speed;
}Camera;

//Set The camera with the camera size in-game, the window size and this movement speed  
Camera InitializeCamera(sfVector2f pSize, const sfVector2f pWindowSize, const float pSpeed);
//destroy view and shape
void ClearCamera(Camera *camera, const int numberOfPlayer);
//Follow the average position between players 
void FollowObject(Camera* pCamera, Player* pPlayers, const int numberOfPlayer, const RoomGameData room, const float pDeltatime);
//same as the one above, but with a third position
void FollowObjectSpecialBoy(Camera* pCamera, const Player* pPlayers, const sfVector2f posBoy, const int numberOfPlayer, const RoomGameData room, const float pDeltatime);
//move the camera to a specific point 
void MovementToASpecificPoint(Camera* pCamera, const sfVector2f pointToReach, const float pDeltatime);
//Follow the average position between players on the miniMap
void FollowObjectMiniMap(Camera* pCamera, const Player* pPlayers, const int numberOfPlayer, const float pDeltatime);

void SetZoom(Camera* camera, const sfBool haveToUnZoom);

void ChangeMiniMap(Camera* camera, const int numberOfPlayer);

void ActiveLightOnCamera(const Camera pCamera);
//Shake the camera with the movement force during with giving time
void ShakeCamera(Camera* pCamera, const int pPower, const float pTime, const float pDeltaTime);
//Return true if the sprite is in the bound camera and false if the sprite is out of bound
sfBool DrawOnCamera(const Camera pCamera, const sfSprite* pSprite, const sfVector2f pPosition);
//Set position Camera and draw the view 
void DisplayCamera(const Camera* pCamera, sfRenderWindow* pWindow);
//Set position hud and draw the hud
void DisplayCameraHud(const Camera* pCamera, sfRenderWindow* pWindow);
//set position miniMap and draw it
void DisplayViewMiniMap(const Camera* pCamera, const Player *player, const int numberOfPlayer, sfRenderWindow* pWindow, sfShader* shd);

#endif
