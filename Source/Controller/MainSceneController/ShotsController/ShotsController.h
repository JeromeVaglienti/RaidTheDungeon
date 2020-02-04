#ifndef _SHOTSCONTROLLER_H
#define _SHOTSCONTROLLER_H
#include "..\..\..\Model\Physics\Projectile\Shot.h"
#include "..\..\Camera\Camera.h"

typedef struct ShotsController
{
	sfSprite* SheetBasicAttack;
	sfVector2f Ratio;
	sfSprite* SheetRangeBall;
	sfSprite* MiniBossAtack;
	sfSprite* BossAtack;
	sfSprite* SheetKamikazeProjectile;
	sfSprite* SheetFireElemProjectile;
	sfSprite* SheetLightFireElemProjectile;
	sfSprite* SheetWaterElemProjectile;
	sfSprite* SheetLightWaterElemProjectile;
	sfSprite* SheetWindElemProjectile;
	sfSprite* SheetLightWindElemProjectile;
	sfSprite* SheetIceElemProjectile;
	sfSprite* SheetIceShardProjectile[3];
	sfSprite* SheetLightIceElemProjectile;
	sfSprite* SheetLightIceShardProjectile[3];
	sfSprite* SheetDirtElemProjectile;
	sfSprite* SheetRazarShot;
	sfSprite* SheetVoidBalls;
	sfSprite* SheetVoidGround;
	sfSprite* SheetRazarBigFrag;
	sfSprite* SheetRazarMediumFrag;
	sfSprite* SheetRazarLittleFrag;
	sfSprite* SheetLightRazarBigFrag;
	sfSprite* SheetLightRazarMediumFrag;
	sfSprite* SheetLightRazarLittleFrag;
}ShotsController;

void InitShotsGraphics(const sfVideoMode mode);

void SetShots(Shot* firstElement, sfRenderWindow* window, const Camera pCam);
void DisplayShotVertexArray(sfRenderWindow* window);
void ClearShotVertexArray();
void UnloadShotsGraphicsSlime();
void UnloadShotsGraphicsElem();
#endif // !_SHOTSCONTROLLER_H
