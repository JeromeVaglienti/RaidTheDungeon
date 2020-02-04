#include "MapKey.h"

MapKey mapKey[11];

void InitMapKey()
{
	FILE* f;
	if (!(f = fopen("Ressources/Configuration/Controls.b", "rb")))
	{
		mapKey[MoveUp] = sfKeyZ;
		mapKey[MoveLeft] = sfKeyQ;
		mapKey[MoveDown] = sfKeyS;
		mapKey[MoveRight] = sfKeyD;
		mapKey[SelectFireBall] = sfKeyNum1;
		mapKey[SelectConeOfFire] = sfKeyNum2;
		mapKey[SelectFireTrap] = sfKeyNum3;
		mapKey[ShowMiniMap] = sfKeyM;
		mapKey[UseDash] = sfKeySpace;
		mapKey[BasicAttack] = sfMouseLeft;
		mapKey[UseSpell] = sfMouseRight;
	}
	else
	{
		fread(mapKey, 11, sizeof(MapKey), f);
		fclose(f);
	}
	SaveMapKey();
}

sfKeyCode GetMapKey(const MapKey mapKeyId)
{
	return mapKey[mapKeyId];
}

void SetMapKey(const MapKey mapKayId, const sfKeyCode newBind)
{
	mapKey[mapKayId] = newBind;
	if (mapKayId == BasicAttack || mapKayId == UseSpell)
	{
		for (int i = 9; i < 11; i++)
		{
			if (i != mapKayId && mapKey[i] == newBind)
			{
				mapKey[i] = sfKeyUnknown;
			}
		}
	}
	else
	{
		for (int i = 0; i < 9; i++)
		{
			if (i != mapKayId && mapKey[i] == newBind)
			{
				mapKey[i] = sfKeyUnknown;
			}
		}
	}
}

sfBool IfCanUseThisKey(const sfKeyCode key)
{
	return (key != sfKeyEscape && key != sfKeyLSystem && key != sfKeyRSystem &&  key != sfKeyMenu &&
		key != sfKeyReturn && key != sfKeyBack && key != sfKeyPageUp && key != sfKeyPageDown && key != sfKeyEnd
		&& key != sfKeyHome && key != sfKeyInsert && key != sfKeyDelete && key != sfKeyPause);
}

void SaveMapKey()
{
	FILE* f;
	f = fopen("Ressources/Configuration/Controls.b", "w+");
	fwrite(mapKey, 11, sizeof(MapKey), f);
	fclose(f);
}