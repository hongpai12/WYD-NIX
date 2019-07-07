#include "GlobalData.h"

st_ItemListCSV itemlistBuffer[6500];

sTeleport TeleportList[256];

Surprise sup;

WarInfos          War[4];
GuildInfos		  Guilds[0xFFFF];
stBuffer          mBuffer[30000];
CUser             Users[1000];
nInfo             nBuffer[1000];
sInfos            Servidor;
Aguas             Agua[3];
Pesas             Pesa[3];
Donate            DonateList[300];
QuestItensOnEnter QuestItensEntered[5];
st_Trade          Trade[1000];
Resets            ResetList[50];
Tabs              TabList[50];
Quests            QuestList[5];
STRUCT_SPELL      g_pSpell[MAX_SKILLLIST];
DWORD             MagicIncrementBonus[30000];
stMOBGener		  Genner[10000];
Events            Eventos;
//MySqlServerInfos  SqlInfos;

st_ItemListCSV *GetItemlistBuffer()
{
	return &itemlistBuffer[0];
}

CUser* GetUserFromIndex(INT16 index)
{
	if(index < 0 || index > 740)
		return NULL;

	return &Users[index];
}

st_Mob* _GetMobFromIndex(INT16 index)
{
	st_Mob* user = (st_Mob*)0x15C69B8;

	return &user[index];
}

Surprise *Get_supv()
{
	return &sup;
}

sTeleport *Getteleport()
{
	return &TeleportList[0];
}

void LoadTeleport()
{
	FILE *fp = fopen("teleport.txt", "rt");
	if(fp == NULL)
	{
		printf("Arquivo teleport.txt não encontrado.\n");
        return ;
	}

	int TeleportCount = 0, ret;
	sTeleport *pTel = TeleportList;
	char tmp[8192];
    memset(TeleportList, 0, sizeof(TeleportList));

	while(fgets(tmp, sizeof(tmp), fp) && TeleportCount < 256)
	{
		if(*tmp == '\n' || *tmp == '#')
            continue;

		ret = sscanf(tmp, "%04hu, %04hu, %04hu, %04hu, %u", &pTel->Src.X,
			&pTel->Src.Y, &pTel->Dst.X, &pTel->Dst.Y, &pTel->Price);

		pTel->Func = 0;

		if(ret == 5)
		{
			pTel++;
			TeleportCount++;
		}
	}

    fclose(fp);
	/*
    if(TeleportCount < 36)
        memset(pTel, 0, sizeof(sTeleport));
	*/
	printf("Teleport.txt carregado.\n");
}