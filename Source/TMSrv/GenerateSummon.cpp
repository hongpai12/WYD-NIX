#include "DLLMain.h"

int HKD_GenerateSummon_CanSummon(int mobId, int faceId)
{
	// by shepher 
	// Retorne 0 caso n�o deseje que seja summonado outro mob igual 
	// Retorne 1 para summonar 

	// Evocs de bm
	for (INT8 i = 0; i < sizeof(MobFaces) / sizeof(INT16); i++)
		if (faceId == MobFaces[i])
			return 1;

	return 0;
}