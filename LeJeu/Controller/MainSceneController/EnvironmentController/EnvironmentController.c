#include <time.h>
#include "EnvironmentController.h"
#include "..\..\GraphicsUtilities\Utilities.h"
#include "..\..\..\Model\Room\Room.h"
#include "..\..\..\Controller\MainSceneController\ShadersController\ShadersController.h"
#include "..\..\..\Model\Math\Vector\VectorUtilities.h"
EnvironmentDungeon* DungeonContainer = NULL;
sfVertexArray** corridorVertexArray = NULL;
float lightTimer = 0.0f;


void InitEnviDungeonSlime()
{
	DungeonContainer = (EnvironmentDungeon*)calloc(1, sizeof(EnvironmentDungeon));
	corridorVertexArray = calloc(8, sizeof(sfVertexArray*));
#if _DEBUG
	printf("Loading Environment Slime Dungeon\n");
#endif

	for (int i = 0; i < 8; i++)
	{
		corridorVertexArray[i] = sfVertexArray_create();
		sfVertexArray_setPrimitiveType(corridorVertexArray[i], sfQuads);
	}

	DungeonContainer->Room = (RoomDungeon*)calloc(11, sizeof(RoomDungeon));

	for (int i = 0; i < 11; i++)
	{
		DungeonContainer->Room[i].Room = (sfSprite**)calloc(1, sizeof(sfSprite*));
		if (i >= 1 && i <= 8)
		{
			DungeonContainer->Room[i].FrontDisplay = (sfSprite**)calloc(1, sizeof(sfSprite*));
		}

	}

	if (GetActualDungeonId() == beginningDungeon)
	{
		*DungeonContainer->Room[0].Room = LoadSprite("Ressources/Sprites/Environment/Dungeon1/Rooms/ID_0/room spawn_Tuto.png", 1);
	}
	else
	{
		*DungeonContainer->Room[0].Room = LoadSprite("Ressources/Sprites/Environment/Dungeon1/Rooms/ID_0/room spawn.png", 1);
	}

	*DungeonContainer->Room[1].Room = LoadSprite("Ressources/Sprites/Environment/Dungeon1/Rooms/ID_1/Dortoir_Sprite.png", 1);
	*DungeonContainer->Room[1].FrontDisplay = LoadSprite("Ressources/Sprites/Environment/Dungeon1/Rooms/ID_1/Dortoir_Sprite_L1.png", 1);

	*DungeonContainer->Room[2].Room = LoadSprite("Ressources/Sprites/Environment/Dungeon1/Rooms/ID_2/Dortoir_Sprite_L0.png", 1);
	*DungeonContainer->Room[2].FrontDisplay = LoadSprite("Ressources/Sprites/Environment/Dungeon1/Rooms/ID_2/Dortoir_Sprite_L1.png", 1);

	*DungeonContainer->Room[3].Room = LoadSprite("Ressources/Sprites/Environment/Dungeon1/Rooms/ID_3/Entrepot.png", 1);
	*DungeonContainer->Room[3].FrontDisplay = LoadSprite("Ressources/Sprites/Environment/Dungeon1/Rooms/ID_3/Entrepot_Sprite_L1.png", 1);

	*DungeonContainer->Room[4].Room = LoadSprite("Ressources/Sprites/Environment/Dungeon1/Rooms/ID_4/entrepot_Sprite_L0.png", 1);
	*DungeonContainer->Room[4].FrontDisplay = LoadSprite("Ressources/Sprites/Environment/Dungeon1/Rooms/ID_4/entrepot_Sprite_L1.png", 1);

	*DungeonContainer->Room[5].Room = LoadSprite("Ressources/Sprites/Environment/Dungeon1/Rooms/ID_5/Cantine_Sprite_L0.png", 1);
	*DungeonContainer->Room[5].FrontDisplay = LoadSprite("Ressources/Sprites/Environment/Dungeon1/Rooms/ID_5/Cantine_Sprite_L1.png", 1);

	*DungeonContainer->Room[6].Room = LoadSprite("Ressources/Sprites/Environment/Dungeon1/Rooms/ID_6/Cantine_02_Sprite_L0.png", 1);
	*DungeonContainer->Room[6].FrontDisplay = LoadSprite("Ressources/Sprites/Environment/Dungeon1/Rooms/ID_6/Cantine_02_Sprite_L1.png", 1);

	*DungeonContainer->Room[7].Room = LoadSprite("Ressources/Sprites/Environment/Dungeon1/Rooms/ID_7/Bibliotheque_Sprite_L0.png", 1);
	*DungeonContainer->Room[7].FrontDisplay = LoadSprite("Ressources/Sprites/Environment/Dungeon1/Rooms/ID_7/Bibliotheque_Sprite_L1.png", 1);

	*DungeonContainer->Room[8].Room = LoadSprite("Ressources/Sprites/Environment/Dungeon1/Rooms/ID_8/bibliotheque_sprite_L0.png", 1);
	*DungeonContainer->Room[8].FrontDisplay = LoadSprite("Ressources/Sprites/Environment/Dungeon1/Rooms/ID_8/bibliotheque_sprite_L1.png", 1);

	*DungeonContainer->Room[9].Room = LoadSprite("Ressources/Sprites/Environment/Dungeon1/Rooms/ID_9/MiniBoss_Sprite_L0.png", 1);
	*DungeonContainer->Room[10].Room = LoadSprite("Ressources/Sprites/Environment/Dungeon1/Rooms/ID_10/BossRoom_Sprite_L0.png", 1);

	DungeonContainer->DoorsLayersNorth = (sfSprite**)calloc(3, sizeof(sfSprite*));
	DungeonContainer->DoorsLayerSouth = (sfSprite**)calloc(2, sizeof(sfSprite*));
	DungeonContainer->DoorsLayerWest = (sfSprite**)calloc(2, sizeof(sfSprite*));
	DungeonContainer->DoorsLayerEast = (sfSprite**)calloc(2, sizeof(sfSprite*));
	DungeonContainer->DoorsLayerAny = NULL;
	DungeonContainer->Corridor = (sfSprite**)calloc(8, sizeof(sfSprite*));

	DungeonContainer->DoorsLayersNorth[0] = LoadSprite("Ressources/Sprites/Environment/Dungeon1/Doors/North/jonction_dalle_sombre.png", 1);
	DungeonContainer->DoorsLayersNorth[1] = LoadSprite("Ressources/Sprites/Environment/Dungeon1/Doors/North/2.png", 1);
	DungeonContainer->DoorsLayersNorth[2] = LoadSprite("Ressources/Sprites/Environment/Dungeon1/Doors/North/1.png", 1);
	DungeonContainer->DoorsLayerSouth[0] = LoadSprite("Ressources/Sprites/Environment/Dungeon1/Doors/South/0.png", 1);
	DungeonContainer->DoorsLayerSouth[1] = LoadSprite("Ressources/Sprites/Environment/Dungeon1/Doors/South/1.png", 1);
	DungeonContainer->DoorsLayerWest[0] = LoadSprite("Ressources/Sprites/Environment/Dungeon1/Doors/West/0.png", 1);
	DungeonContainer->DoorsLayerWest[1] = LoadSprite("Ressources/Sprites/Environment/Dungeon1/Doors/West/1.png", 1);
	DungeonContainer->DoorsLayerEast[0] = LoadSprite("Ressources/Sprites/Environment/Dungeon1/Doors/East/0.png", 1);
	DungeonContainer->DoorsLayerEast[1] = LoadSprite("Ressources/Sprites/Environment/Dungeon1/Doors/East/1.png", 1);
	DungeonContainer->Corridor[0] = LoadSprite("Ressources/Sprites/Environment/Dungeon1/Corridor/couloir horizontal.png", 1);
	DungeonContainer->Corridor[1] = LoadSprite("Ressources/Sprites/Environment/Dungeon1/Corridor/Couloir vertical.png", 1);
	DungeonContainer->Corridor[2] = LoadSprite("Ressources/Sprites/Environment/Dungeon1/Corridor/EST-NORD.png", 1);
	DungeonContainer->Corridor[3] = LoadSprite("Ressources/Sprites/Environment/Dungeon1/Corridor/NORD-OUEST.png", 1);
	DungeonContainer->Corridor[4] = LoadSprite("Ressources/Sprites/Environment/Dungeon1/Corridor/OUEST-SUD.png", 1);
	DungeonContainer->Corridor[5] = LoadSprite("Ressources/Sprites/Environment/Dungeon1/Corridor/EST-SUD.png", 1);
	DungeonContainer->Corridor[6] = LoadSprite("Ressources/Sprites/Environment/Dungeon1/Corridor/couloir horizontalSlice.png", 1);
	DungeonContainer->Corridor[7] = LoadSprite("Ressources/Sprites/Environment/Dungeon1/Corridor/Couloir verticalSlice.png", 1);


	DungeonContainer->BigTeleport = (Animation*)calloc(1, sizeof(Animation));

	*DungeonContainer->BigTeleport = LoadAnimation(12, (12.f / 12.f), "Ressources/Sprites/Environment/Dungeon_Transition/Anim_TP_InterDj/TP_InterDj_", "png", (sfVector2f) { 0.0f, 0.0f }, sfFalse, 1);
}

void UnLoadEnviDungeonSlime()
{

#if _DEBUG
	printf("Unloading Environment Slime Dungeon\n");
#endif

	for (int i = 0; i < 11; i++)
	{
		DestroySfSprite(DungeonContainer->Room[i].Room);
		if (DungeonContainer->Room[i].FrontDisplay != NULL)
		{
			DestroySfSprite(DungeonContainer->Room[i].FrontDisplay);
			free(DungeonContainer->Room[i].FrontDisplay);
		}
		free(DungeonContainer->Room[i].Room);
	}

	for (int i = 0; i < 8; i++)
	{
		DestroySfSprite(&DungeonContainer->Corridor[i]);
	}

	for (int i = 0; i < 3; i++)
	{
		DestroySfSprite(&DungeonContainer->DoorsLayersNorth[i]);
	}

	for (int i = 0; i < 2; i++)
	{
		DestroySfSprite(&DungeonContainer->DoorsLayerSouth[i]);
		DestroySfSprite(&DungeonContainer->DoorsLayerWest[i]);
		DestroySfSprite(&DungeonContainer->DoorsLayerEast[i]);
	}

	UnLoadAnimation(DungeonContainer->BigTeleport);
	free(corridorVertexArray);
	free(DungeonContainer->BigTeleport);
	free(DungeonContainer->Room);
	free(DungeonContainer->Corridor);
	free(DungeonContainer->DoorsLayersNorth);
	free(DungeonContainer->DoorsLayerEast);
	free(DungeonContainer->DoorsLayerSouth);
	free(DungeonContainer->DoorsLayerWest);
	free(DungeonContainer);
	DungeonContainer = NULL;
}

void InitEnviDungeonElem()
{
	DungeonContainer = (EnvironmentDungeon*)calloc(1, sizeof(EnvironmentDungeon));

#if _DEBUG
	printf("Loading Environment Elementary Dungeon\n");
#endif

	lightTimer = 0;

	DungeonContainer->Room = (RoomDungeon*)calloc(11, sizeof(RoomDungeon));
	DungeonContainer->RoomLight = (sfSprite**)calloc(4, sizeof(sfSprite*));
	DungeonContainer->LevitateRockSprite = (sfSprite**)calloc(6, sizeof(sfSprite*));

	for (int i = 0; i < 11; i++)
	{
		DungeonContainer->Room[i].Room = (sfSprite**)calloc(1, sizeof(sfSprite*));
		if (i >= 1 && i <= 2 || i == 4)
		{
			DungeonContainer->Room[i].FrontDisplay = (sfSprite**)calloc(1, sizeof(sfSprite*));
		}

		if (i == 0 || i == 4 || i == 9)
		{
			DungeonContainer->Room[i].BackGroundDisplay = (sfSprite**)calloc(1, sizeof(sfSprite*));
			DungeonContainer->Room[i].Scroller = (ScrollerBackGround*)calloc(1, sizeof(ScrollerBackGround));
		}

	}

	srand((unsigned int)time(NULL));
	DungeonContainer->LevitateRockSprite[0] = LoadSprite("Ressources/Sprites/Environment/Dungeon2/Rooms/ID_4/Rocher_01_L2.png", 1);
	DungeonContainer->LevitateRockSprite[1] = LoadSprite("Ressources/Sprites/Environment/Dungeon2/Rooms/ID_4/Rocher_02_L2.png", 1);
	DungeonContainer->LevitateRockSprite[2] = LoadSprite("Ressources/Sprites/Environment/Dungeon2/Rooms/ID_4/Rocher_03_L2.png", 1);
	DungeonContainer->LevitateRockSprite[3] = LoadSprite("Ressources/Sprites/Environment/Dungeon2/Rooms/ID_4/Rocher_04_L2.png", 1);
	DungeonContainer->LevitateRockSprite[4] = LoadSprite("Ressources/Sprites/Environment/Dungeon2/Rooms/ID_4/Rocher_05_L2.png", 1);
	DungeonContainer->LevitateRockSprite[5] = LoadSprite("Ressources/Sprites/Environment/Dungeon2/Rooms/ID_4/Rocher_06_L2.png", 1);

	DungeonContainer->nbRocks = rand() % 8 + 10;
	DungeonContainer->LevitateRock = calloc(DungeonContainer->nbRocks, sizeof(LevitateRock));
	for (int i = 0; i < DungeonContainer->nbRocks; i++)
	{
		DungeonContainer->LevitateRock[i].Pos.x = (float)(rand() % 2560 - 1280);
		DungeonContainer->LevitateRock[i].Pos.y = (float)(rand() % 2304 - 1152);
		DungeonContainer->LevitateRock[i].index = rand() % 6;
		DungeonContainer->LevitateRock[i].Timer = (float)(rand() % 360);
		DungeonContainer->LevitateRock[i].Height = (float)(rand() % 30 + 20);
		float scale = (rand() % 90 + 40)*0.01f;
		int X = 1;
		if (rand() % 1)
		{
			X = -1;
		}
		DungeonContainer->LevitateRock[i].scale.x = scale * X;
		DungeonContainer->LevitateRock[i].scale.y = scale;
	}
	DungeonContainer->RoomLight[0] = LoadSprite("Ressources/Sprites/Environment/Dungeon2/Rooms/ID_10/Map_Boss_elementaire_LightMap.png", 1);
	*DungeonContainer->Room[0].Room = LoadSprite("Ressources/Sprites/Environment/Dungeon2/Rooms/ID_0/Spawn_Sprite_L0.png", 1);
	*DungeonContainer->Room[0].BackGroundDisplay = LoadSprite("Ressources/Sprites/Environment/Dungeon2/Rooms/ID_9/Wind_02_Sprite_L-1.png", 1);
	sfVector2u size = sfTexture_getSize(sfSprite_getTexture(*DungeonContainer->Room[0].BackGroundDisplay));
	DungeonContainer->Room[0].Scroller->speed = 50.0f;
	DungeonContainer->Room[0].Scroller->Start.x = -(float)size.x + 2.0f;
	DungeonContainer->Room[0].Scroller->Start.y = 0.0f;
	DungeonContainer->Room[0].Scroller->End.x = (float)+(size.x);
	DungeonContainer->Room[0].Scroller->End.y = 0.0f;
	DungeonContainer->Room[0].Scroller->Pos_1 = DungeonContainer->Room[0].Scroller->Start;
	DungeonContainer->Room[0].Scroller->Pos_2.y = 0.0f;
	DungeonContainer->Room[0].Scroller->Pos_2.x = 0.0f;

	*DungeonContainer->Room[1].Room = LoadSprite("Ressources/Sprites/Environment/Dungeon2/Rooms/ID_1/Lava_01_Sprite_L0.png", 1);
	DungeonContainer->RoomLight[1] = LoadSprite("Ressources/Sprites/Environment/Dungeon2/Rooms/ID_1/Lava_01_LightMap.png", 1);
	*DungeonContainer->Room[1].FrontDisplay = LoadSprite("Ressources/Sprites/Environment/Dungeon2/Rooms/ID_1/Lava_01_Sprite_L1.png", 1);

	*DungeonContainer->Room[2].Room = LoadSprite("Ressources/Sprites/Environment/Dungeon2/Rooms/ID_2/Lava_02_Sprite_L0.png", 1);
	DungeonContainer->RoomLight[2] = LoadSprite("Ressources/Sprites/Environment/Dungeon2/Rooms/ID_2/Lava_02_LightMap.png", 1);
	*DungeonContainer->Room[2].FrontDisplay = LoadSprite("Ressources/Sprites/Environment/Dungeon2/Rooms/ID_2/Lava_02_Sprite_L1.png", 1);

	*DungeonContainer->Room[3].Room = LoadSprite("Ressources/Sprites/Environment/Dungeon2/Rooms/ID_3/Lava_03_Sprite_L0.png", 1);
	DungeonContainer->RoomLight[3] = LoadSprite("Ressources/Sprites/Environment/Dungeon2/Rooms/ID_3/Lava_03_LightMap.png", 1);

	*DungeonContainer->Room[4].Room = LoadSprite("Ressources/Sprites/Environment/Dungeon2/Rooms/ID_4/Wind_01_Sprite_L0.png", 1);
	*DungeonContainer->Room[4].FrontDisplay = LoadSprite("Ressources/Sprites/Environment/Dungeon2/Rooms/ID_4/Wind_01_Sprite_L1.png", 1);
	*DungeonContainer->Room[4].BackGroundDisplay = LoadSprite("Ressources/Sprites/Environment/Dungeon2/Rooms/ID_4/Wind_01_Sprite_L-1.png", 1);
	size = sfTexture_getSize(sfSprite_getTexture(*DungeonContainer->Room[4].BackGroundDisplay));
	DungeonContainer->Room[4].Scroller->speed = 50.0f;
	DungeonContainer->Room[4].Scroller->Start.x = -((float)(size.x)) + 2.0f;
	DungeonContainer->Room[4].Scroller->Start.y = 0.0f;
	DungeonContainer->Room[4].Scroller->End.x = (float)+(size.x);
	DungeonContainer->Room[4].Scroller->End.y = 0.0f;
	DungeonContainer->Room[4].Scroller->Pos_1 = DungeonContainer->Room[4].Scroller->Start;
	DungeonContainer->Room[4].Scroller->Pos_2.y = 0.0f;
	DungeonContainer->Room[4].Scroller->Pos_2.x = 0.0f;

	*DungeonContainer->Room[5].Room = LoadSprite("Ressources/Sprites/Environment/Dungeon2/Rooms/ID_5/ice_01_Sprite_L0.png", 1);
	*DungeonContainer->Room[6].Room = LoadSprite("Ressources/Sprites/Environment/Dungeon2/Rooms/ID_6/ice_02_Sprite_L0.png", 1);
	*DungeonContainer->Room[7].Room = LoadSprite("Ressources/Sprites/Environment/Dungeon2/Rooms/ID_7/ice_03_Sprite_L0.png", 1);
	*DungeonContainer->Room[8].Room = LoadSprite("Ressources/Sprites/Environment/Dungeon2/Rooms/ID_8/dirt_01_Sprite_L0.png", 1);
	*DungeonContainer->Room[9].Room = LoadSprite("Ressources/Sprites/Environment/Dungeon2/Rooms/ID_9/Wind_02_Sprite_L0.png", 1);
	*DungeonContainer->Room[9].BackGroundDisplay = LoadSprite("Ressources/Sprites/Environment/Dungeon2/Rooms/ID_9/Wind_02_Sprite_L-1.png", 1);
	size = sfTexture_getSize(sfSprite_getTexture(*DungeonContainer->Room[9].BackGroundDisplay));
	DungeonContainer->Room[9].Scroller->speed = 50.0f;
	DungeonContainer->Room[9].Scroller->Start.x = -((float)(size.x)) + 2.0f;
	DungeonContainer->Room[9].Scroller->Start.y = 0.0f;
	DungeonContainer->Room[9].Scroller->End.x = (float)+(size.x);
	DungeonContainer->Room[9].Scroller->End.y = 0.0f;
	DungeonContainer->Room[9].Scroller->Pos_1 = DungeonContainer->Room[9].Scroller->Start;
	DungeonContainer->Room[9].Scroller->Pos_2.y = 0.0f;
	DungeonContainer->Room[9].Scroller->Pos_2.x = 0.0f;

	*DungeonContainer->Room[10].Room = LoadSprite("Ressources/Sprites/Environment/Dungeon2/Rooms/ID_10/Map_Boss_elementaire.png", 1);
	/*DungeonContainer->Room[9] = LoadSprite("Ressources/Sprites/Environment/Dungeon1/Rooms/ID_9/MiniBoss_Sprite_L0.png", "Ressources/Sprites/Environment/Dungeon1/Rooms/ID_9/MiniBoss_Normal_L0.png", 1, 1);
	DungeonContainer->Room[10] = LoadSprite("Ressources/Sprites/Environment/Dungeon1/Rooms/ID_10/BossRoom_Sprite_L0.png", "Ressources/Sprites/Environment/Dungeon1/Rooms/ID_10/BossRoom_NormalMap_L0.png", 1, 1);*/

	//DungeonContainer->FrontDisplay[2] = LoadSprite("Ressources/Sprites/Environment/Dungeon2/Rooms/ID_3/Lava_03_Sprite_L1.png", "Ressources/Sprites/Environment/Dungeon2/Rooms/ID_3/Lava_03_NormalMap_L1.png", 1, 1);


	DungeonContainer->PortalClosedStatic = (sfSprite**)calloc(1, sizeof(sfSprite*));

	*(DungeonContainer->PortalClosedStatic) = LoadSprite("Ressources/Sprites/Environment/Dungeon2/Portal/Inactive/0000.png", 1);

	DungeonContainer->PortalActive = (Animation*)calloc(1, sizeof(Animation));
	DungeonContainer->PortalTeleportingAnimation = (Animation*)calloc(1, sizeof(Animation));
	DungeonContainer->PortalReceivingTeleport = (Animation*)calloc(1, sizeof(Animation));

	*(DungeonContainer->PortalActive) = LoadAnimation(4, (4.0f / 12.0f), "Ressources/Sprites/Environment/Dungeon2/Portal/Active/", "png", (sfVector2f) { 0.0f, 0.0f }, sfTrue, 1);
	*(DungeonContainer->PortalTeleportingAnimation) = LoadAnimation(22, (31.0f / 12.0f), "Ressources/Sprites/Environment/Dungeon2/Portal/Teleporting/", "png", (sfVector2f) { 0.0f, 0.0f }, sfTrue, 1);
	*(DungeonContainer->PortalReceivingTeleport) = LoadAnimation(9, (9.0f / 12.0f), "Ressources/Sprites/Environment/Dungeon2/Portal/Teleporting/Arriving/", "png", (sfVector2f) { 0.0f, 0.0f }, sfTrue, 1);

	DungeonContainer->BigTeleport = (Animation*)calloc(1, sizeof(Animation));

	*DungeonContainer->BigTeleport = LoadAnimation(12, (12.f / 12.f), "Ressources/Sprites/Environment/Dungeon_Transition/Anim_TP_InterDj/TP_InterDj_", "png", (sfVector2f) { 0.0f, 0.0f }, sfFalse, 1);
}

void UnLoadEnviDungeonElem()
{
#if _DEBUG
	printf("Unloading Environment Elementary Dungeon\n");
#endif
	for (int i = 0; i < 3; i++)
	{
		DestroySfSprite(&DungeonContainer->LevitateRockSprite[i]);
	}

	for (int i = 0; i < 4; i++)
	{
		DestroySfSprite(&DungeonContainer->RoomLight[i]);
	}

	for (int i = 0; i < 11; i++)
	{
		DestroySfSprite(DungeonContainer->Room[i].Room);
		if (DungeonContainer->Room[i].FrontDisplay != NULL)
		{
			DestroySfSprite(DungeonContainer->Room[i].FrontDisplay);
			free(DungeonContainer->Room[i].FrontDisplay);
		}
		if (DungeonContainer->Room[i].BackGroundDisplay != NULL)
		{
			DestroySfSprite(DungeonContainer->Room[i].BackGroundDisplay);
			free(DungeonContainer->Room[i].BackGroundDisplay);
		}
		if (DungeonContainer->Room[i].Scroller != NULL)
		{
			free(DungeonContainer->Room[i].Scroller);
		}
		free(DungeonContainer->Room[i].Room);
	}

	UnLoadAnimation(DungeonContainer->PortalActive);
	UnLoadAnimation(DungeonContainer->PortalTeleportingAnimation);
	UnLoadAnimation(DungeonContainer->PortalReceivingTeleport);
	UnLoadAnimation(DungeonContainer->BigTeleport);

	DestroySfSprite(DungeonContainer->PortalClosedStatic);

	free(DungeonContainer->PortalActive);
	free(DungeonContainer->PortalTeleportingAnimation);
	free(DungeonContainer->PortalReceivingTeleport);

	free(DungeonContainer->BigTeleport);

	free(DungeonContainer->LevitateRockSprite);
	free(DungeonContainer->Room);
	free(DungeonContainer->Corridor);
	free(DungeonContainer->PortalClosedStatic);

	free(DungeonContainer->LevitateRock);
	free(DungeonContainer);
	DungeonContainer = NULL;
}

void InitEnviDungeonRazar()
{
	DungeonContainer = (EnvironmentDungeon*)calloc(1, sizeof(EnvironmentDungeon));

	DungeonContainer->Room = (RoomDungeon*)calloc(1, sizeof(RoomDungeon));

	DungeonContainer->RoomLight = (sfSprite**)calloc(1, sizeof(sfSprite*));

	DungeonContainer->Room[0].Room = (sfSprite**)calloc(1, sizeof(sfSprite*));

	*DungeonContainer->Room[0].Room = LoadSprite("Ressources/Sprites/Environment/Dungeon3/ID_0/RazarRoom_Sprite_L0.png", 1);

	DungeonContainer->RoomLight[0] = LoadSprite("Ressources/Sprites/Environment/Dungeon3/ID_0/RazarRoom_LightMap.png", 1);

#if _DEBUG
	printf("Loading Environment Razar Dungeon\n");
#endif

	DungeonContainer->BigTeleport = (Animation*)calloc(1, sizeof(Animation));

	*DungeonContainer->BigTeleport = LoadAnimation(12, (12.f / 12.f), "Ressources/Sprites/Environment/Dungeon_Transition/Anim_TP_InterDj/TP_InterDj_", "png", (sfVector2f) { 0.0f, 0.0f }, sfFalse, 1);
}

void UnLoadEnviDungeonRazar()
{

#if _DEBUG
	printf("UnLoading Environment Razar Dungeon\n");
#endif

	DestroySfSprite(DungeonContainer->Room[0].Room);
	DestroySfSprite(DungeonContainer->RoomLight);
	UnLoadAnimation(DungeonContainer->BigTeleport);

	free(DungeonContainer->BigTeleport);
	free(DungeonContainer->Room[0].Room);
	free(DungeonContainer->Room);
	free(DungeonContainer->RoomLight);
	free(DungeonContainer);
	DungeonContainer = NULL;
}

void UpdateLayerLight(const float deltatime)
{
	lightTimer += deltatime * 0.5f;
}

sfBool UpdateEndTeleporting(const float deltaTime)
{
	UpdateAnimation(DungeonContainer->BigTeleport, deltaTime);

	if (DungeonContainer->BigTeleport->actualFrameToShow == DungeonContainer->BigTeleport->numberOfFrame - 1)
	{
		DungeonContainer->BigTeleport->actualFrameToShow = 0;
		return sfTrue;
	}
	else
	{
		return sfFalse;
	}
}

void UpdateScroller(const int RoomID, const float deltaTime)
{
	if (RoomID == 9 || RoomID == 4 || RoomID == 0)
	{
		int id = RoomID;

		if (DungeonContainer->Room[id].Scroller->Pos_1.x >= DungeonContainer->Room[id].Scroller->End.x)
		{
			DungeonContainer->Room[id].Scroller->Pos_1.x = DungeonContainer->Room[id].Scroller->Pos_2.x + DungeonContainer->Room[id].Scroller->Start.x;
		}
		if (DungeonContainer->Room[id].Scroller->Pos_2.x >= DungeonContainer->Room[id].Scroller->End.x)
		{
			DungeonContainer->Room[id].Scroller->Pos_2.x = DungeonContainer->Room[id].Scroller->Pos_1.x + DungeonContainer->Room[id].Scroller->Start.x;
		}

		DungeonContainer->Room[id].Scroller->Pos_1.x += DungeonContainer->Room[id].Scroller->speed * deltaTime;
		DungeonContainer->Room[id].Scroller->Pos_2.x += DungeonContainer->Room[id].Scroller->speed * deltaTime;
	}
}

void UpdateLevitateRock(const float deltaTime)
{
	for (int i = 0; i < DungeonContainer->nbRocks; i++)
	{
		if (DungeonContainer->LevitateRock[i].Timer >= 360)
		{
			DungeonContainer->LevitateRock[i].Timer = 0.f;
		}
		else
		{
			DungeonContainer->LevitateRock[i].Timer += deltaTime * 50.0f;
		}
	}
}

void DisplayLevitateRock(sfRenderWindow* window, const DungeonGraphicData dungeonGameData, const sfVector2f vec)
{
	sfVector2f* DirectonalArray = GetDirectionalArray();
	for (int i = 0; i < DungeonContainer->nbRocks; i++)
	{
		sfSprite_setScale(DungeonContainer->LevitateRockSprite[DungeonContainer->LevitateRock[i].index], DungeonContainer->LevitateRock[i].scale);
		BlitSprite(window, DungeonContainer->LevitateRockSprite[DungeonContainer->LevitateRock[i].index], (sfVector2f) { DungeonContainer->LevitateRock[i].Pos.x + vec.x, DungeonContainer->LevitateRock[i].Pos.y + vec.y + DirectonalArray[(int)DungeonContainer->LevitateRock[i].Timer].y*DungeonContainer->LevitateRock[i].Height }, NULL);
	}
}

void DisplayScroller(sfRenderWindow* window, sfSprite* ScrollerSprite,const ScrollerBackGround Scroller,const sfVector2f Position)
{
	sfVector2f temp = Position;
	temp.x += Scroller.Pos_1.x;
	BlitSprite(window, ScrollerSprite, temp, NULL);

	temp = Position;
	temp.x += Scroller.Pos_2.x;
	BlitSprite(window, ScrollerSprite, temp, NULL);
}

void DisplayEnviDungeonLayer0(sfRenderWindow* window, const DungeonGraphicData dungeonGraphicData, const DungeonGameData dungeonGameData, const Camera Cam)
{
	SetAmbient(DEFAULTABIENT);
	DungeonId dungeonId = GetActualDungeonId();
	for (unsigned short i = 0; i < dungeonGraphicData.numberOfRoom; i++)
	{
		sfVector2f temp = { dungeonGraphicData.room[i].Pos.x * 64.f, dungeonGraphicData.room[i].Pos.y * 64.f };

		if (DrawOnCamera(Cam, *DungeonContainer->Room[dungeonGraphicData.room[i].uniqueID].Room, temp))
		{
			if (DungeonContainer->Room[dungeonGraphicData.room[i].uniqueID].BackGroundDisplay != NULL)
			{
				if (DungeonContainer->Room[dungeonGraphicData.room[i].uniqueID].Scroller != NULL)
				{
					DisplayScroller(window, *DungeonContainer->Room[dungeonGraphicData.room[i].uniqueID].BackGroundDisplay, *DungeonContainer->Room[dungeonGraphicData.room[i].uniqueID].Scroller, temp);
				}
				else
				{
					BlitSprite(window, *DungeonContainer->Room[dungeonGraphicData.room[i].uniqueID].BackGroundDisplay, temp, NULL);
				}
			}

			if (dungeonId == elemDungeon)
			{
				if (dungeonGraphicData.room[i].uniqueID == 10)
				{
					sfColor colLava = sfWhite;
					colLava.a = (sfUint8)(125 + fabsf(130 * sinf(lightTimer)));
					sfSprite_setColor(DungeonContainer->RoomLight[0], colLava);
					BlitRenderTexture(window, GetLavaLightTexture(), DungeonContainer->RoomLight[0], temp);
				}

				switch (dungeonGraphicData.room[i].uniqueID)
				{
				case 0:
					SetAmbient(SPAWNABIENT);
					break;
				case 1:
				case 2:
				case 3:
					SetAmbient(LAVAABIENT);
					sfColor colLava = sfWhite;
					colLava.a = (sfUint8)(125 + fabsf(130 * sinf(lightTimer)));
					sfSprite_setColor(DungeonContainer->RoomLight[dungeonGraphicData.room[i].uniqueID], colLava);
					BlitRenderTexture(window, GetLavaLightTexture(), DungeonContainer->RoomLight[dungeonGraphicData.room[i].uniqueID], temp);
					break;
				case 4:
				case 9:
					SetAmbient(WINDABIENT);
					DisplayLevitateRock(window, dungeonGraphicData, temp);
					break;
				case 5:
				case 6:
				case 7:
					SetAmbient(ICEABIENT);
					break;
				case 8:
					SetAmbient(DIRTABIENT);
					break;
				case 10:
					SetAmbient(BOSSELEMABIENT);
					break;
				default:
					SetAmbient(DEFAULTABIENT);
					break;
				}
			}

			if (dungeonId == razarDungeon)
			{
				sfColor light = sfWhite;
				light.a = (sfUint8)(125 + fabsf(130 * sinf(lightTimer)));
				sfSprite_setColor(DungeonContainer->RoomLight[dungeonGraphicData.room[i].uniqueID], light);
				BlitRenderTexture(window, GetEnemiLightTexture(), DungeonContainer->RoomLight[dungeonGraphicData.room[i].uniqueID], temp);
			}

			BlitSprite(window, *DungeonContainer->Room[dungeonGraphicData.room[i].uniqueID].Room, temp, NULL);


			if (GetTypeOfDungeon() == dungeonCorridor)
			{
				for (int j = 0; j < dungeonGraphicData.room[i].doorCount; j++)
				{
					sfBool doorClosed = dungeonGameData.room[i].doors[j].isClosed;
					if (dungeonGraphicData.room[i].doors[j].Dir == (CardinalDirection)N)
					{
						sfVector2f globalPos = { (dungeonGraphicData.room[i].Pos.x + dungeonGraphicData.room[i].doors[j].LocalPos.x) * 64.0f, (dungeonGraphicData.room[i].Pos.y + dungeonGraphicData.room[i].doors[j].LocalPos.y) * 64.0f };
						if (DrawOnCamera(Cam, DungeonContainer->DoorsLayersNorth[0], globalPos))
						{
							for (int i = 0; i < 1 + doorClosed; i++)
							{
								BlitSprite(window, DungeonContainer->DoorsLayersNorth[i], globalPos, NULL);
							}
						}
					}
					else if (dungeonGraphicData.room[i].doors[j].Dir == (CardinalDirection)S)
					{
						sfVector2f globalPos = { (dungeonGraphicData.room[i].Pos.x + dungeonGraphicData.room[i].doors[j].LocalPos.x) * 64.0f, (dungeonGraphicData.room[i].Pos.y + dungeonGraphicData.room[i].doors[j].LocalPos.y) * 64.0f };
						if (DrawOnCamera(Cam, DungeonContainer->DoorsLayersNorth[0], globalPos))
						{
							for (int i = 0; i < 1 + doorClosed; i++)
							{
								BlitSprite(window, DungeonContainer->DoorsLayerSouth[i], globalPos, NULL);
							}
						}
					}
					else if (dungeonGraphicData.room[i].doors[j].Dir == (CardinalDirection)W)
					{
						sfVector2f globalPos = { (dungeonGraphicData.room[i].Pos.x + dungeonGraphicData.room[i].doors[j].LocalPos.x) * 64.0f, (dungeonGraphicData.room[i].Pos.y + dungeonGraphicData.room[i].doors[j].LocalPos.y) * 64.0f };
						if (DrawOnCamera(Cam, DungeonContainer->DoorsLayerWest[0], globalPos))
						{
							for (int i = 0; i < 1 + doorClosed; i++)
							{
								BlitSprite(window, DungeonContainer->DoorsLayerWest[i], globalPos, NULL);
							}
						}
					}
					else if (dungeonGraphicData.room[i].doors[j].Dir == (CardinalDirection)E)
					{
						sfVector2f globalPos = { (dungeonGraphicData.room[i].Pos.x + dungeonGraphicData.room[i].doors[j].LocalPos.x) * 64.0f, (dungeonGraphicData.room[i].Pos.y + dungeonGraphicData.room[i].doors[j].LocalPos.y) * 64.0f };
						if (DrawOnCamera(Cam, DungeonContainer->DoorsLayerEast[0], globalPos))
						{
							for (int i = 0; i < 1 + doorClosed; i++)
							{
								BlitSprite(window, DungeonContainer->DoorsLayerEast[i], globalPos, NULL);
							}
						}
					}
				}
			}
		}

		for (int j = 0; j < dungeonGraphicData.room[i].doorCount; j++)
		{
			if (dungeonGraphicData.room[i].doors[j].isDebutOfCorridor)
			{
				for (Vector2fList *list = dungeonGraphicData.room[i].doors[j].corridorRef->firstElementPos; list != NULL; list = list->next)
				{
					if (DrawOnCamera(Cam, DungeonContainer->Corridor[list->id], list->Position))
					{
						AppendSprite(corridorVertexArray[list->id], DungeonContainer->Corridor[list->id], list->Position, (sfVector2f) { 1.0f, 1.0f }, sfWhite, 0.0f, 0, 0, 0);
					}
				}
			}
		}
	}
}

void DisplayEnviDungeonLayer1(sfRenderWindow* window, const DungeonGraphicData dungeonGraphicData, const DungeonGameData dungeonGameData, const Camera Cam)
{
	for (unsigned short i = 0; i < dungeonGraphicData.numberOfRoom; i++)
	{
		sfVector2f temp = { dungeonGraphicData.room[i].Pos.x * 64.f, dungeonGraphicData.room[i].Pos.y * 64.f };

		if (DungeonContainer->Room[dungeonGraphicData.room[i].uniqueID].FrontDisplay != NULL)
		{
			if (DrawOnCamera(Cam, *DungeonContainer->Room[dungeonGraphicData.room[i].uniqueID].FrontDisplay, temp))
			{
				BlitSprite(window, *DungeonContainer->Room[dungeonGraphicData.room[i].uniqueID].FrontDisplay, temp, NULL);
			}
		}

		if (GetTypeOfDungeon() == dungeonCorridor)
		{
			for (int j = 0; j < dungeonGraphicData.room[i].doorCount; j++)
			{
				//draw arch over the player
				if (dungeonGraphicData.room[i].doors[j].Dir == (CardinalDirection)N)
				{
					sfVector2f globalPos = { (dungeonGraphicData.room[i].Pos.x + dungeonGraphicData.room[i].doors[j].LocalPos.x) * 64.0f, (dungeonGraphicData.room[i].Pos.y + dungeonGraphicData.room[i].doors[j].LocalPos.y) * 64.0f };
					if (DrawOnCamera(Cam, DungeonContainer->DoorsLayersNorth[2], globalPos))
					{
						BlitSprite(window, DungeonContainer->DoorsLayersNorth[2], globalPos, NULL);
					}
				}
			}
		}
	}
}

void DisplayCorridor(sfRenderWindow* mainWindow)
{
	for (int i = 0; i < 8; i++)
	{
		BlitVertexArray(mainWindow, corridorVertexArray[i], DungeonContainer->Corridor[i]);
	}
}

void DisplayPortalLayer0(sfRenderWindow* mainWindow, const ROOMGAMEDATA* ToDisplay, const Camera Cam)
{
	for (int i = 0; i < ToDisplay->nbPortals; i++)
	{
		sfVector2f globalPos = { (ToDisplay->portals[i].PosCenter.x) * 64.0f, (ToDisplay->portals[i].PosCenter.y) * 64.0f };

		if (!ToDisplay->portals[i].IsNOTActive)
		{
			if (ToDisplay->portals[i].IsTransfering && !ToDisplay->portals[i].IsReceivingTransfert)
			{
				globalPos.y -= 308.f;
				BlitSprite(mainWindow, DungeonContainer->PortalTeleportingAnimation->spriteArray[ToDisplay->portals[i].TelePortAnimKey.actualFrameToShow], globalPos, NULL);
				globalPos.y += 308.f;
			}

			BlitSprite(mainWindow, DungeonContainer->PortalActive->spriteArray[ToDisplay->portals[i].OpenAnimKey.actualFrameToShow], globalPos, NULL);
		}
		else
		{
			BlitSprite(mainWindow, *DungeonContainer->PortalClosedStatic, globalPos, NULL);
		}
	}
}

void DisplayPortals(sfRenderWindow* mainWindow, const ROOMGAMEDATA* ToDisplay, const Camera Cam)
{
	for (int i = 0; i < ToDisplay->nbPortals; i++)
	{
		sfVector2f globalPos = { (ToDisplay->portals[i].PosCenter.x) * 64.0f, (ToDisplay->portals[i].PosCenter.y) * 64.0f };
		if (DrawOnCamera(Cam, *DungeonContainer->PortalClosedStatic, globalPos))
		{
			if (ToDisplay->portals[i].IsReceivingTransfert)
			{
				globalPos.y -= 308.f;
				BlitSprite(mainWindow, DungeonContainer->PortalReceivingTeleport->spriteArray[ToDisplay->portals[i].ReceivingAnimKey.actualFrameToShow], globalPos, NULL);
				globalPos.y += 308.f;
			}
		}
	}
}

void DisplayEndTeleporting(sfRenderWindow * mainWindow, const GameData ToDisplay, const Camera Cam)
{
	for (int i = 0; i < ToDisplay.numberPlayer; i++)
	{
		if (DrawOnCamera(Cam, DungeonContainer->BigTeleport->spriteArray[DungeonContainer->BigTeleport->actualFrameToShow], ToDisplay.Player[i].Physics.Rb.Position))
		{
			sfVector2f pos = ToDisplay.Player[i].Physics.Rb.Position;
			pos.y -= 133.f;
			BlitSprite(mainWindow, DungeonContainer->BigTeleport->spriteArray[DungeonContainer->BigTeleport->actualFrameToShow], pos, NULL);
		}
	}
}