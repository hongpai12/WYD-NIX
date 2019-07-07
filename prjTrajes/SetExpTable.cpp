#include "ExpTable.h"

void SetNewExp_Addr(){
	for (int dwExp = 0; dwExp < MAXLVL_M; dwExp++)
	{
		*(DWORD*)(0x05F495C + (dwExp * 0x04)) = *(DWORD*)&newexp[dwExp];
	}
	for (int dwExpPlus = 0; dwExpPlus < MAXLVL_M; dwExpPlus++)
	{
		*(DWORD*)(0x05F4FA8 + (dwExpPlus * 0x04)) = *(DWORD*)&newexp[dwExpPlus];
	}
}