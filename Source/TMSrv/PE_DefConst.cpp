#include "PE_DefConst.h"

void ReadAllData()
{
	ReadDonateList();
	ReadQuestBonusItensList();
	ReadResetList();
	ReadTabList();
	ReadGuildFile();
	ReadChannel();
	ReadNpcsDeTroca();
	ReadBossQuests();
	ReadServerFile();
}

void SetDefaultConstants()
{
	ULONG OldProt;
	VirtualProtect((void*)0x00401000, 0x00BF000, PAGE_EXECUTE_READWRITE, &OldProt);

	SetMobIndex();
	SetSkillIndex();
	SetEncDec();
	SetVersion();
	SetSockAddrPorts();
	SetPacketSize();
	SetSkillDataString();

	ChangeItemlistStructPointer();

	SetMaxLevel_TrainningCamp();

	//New
	TranslateBaseNPCFiles();
	ChangeLimitAffects();
	RedirectCUserBuffer();
	LimitPlayers();
	CorrectSomeSizes();
	ChangeMountStatus();
	LimiteHpMp();
	ChangeTransBonusStructurePointer();
	ChangeSkillDataStructurePointer();
	NPCGenerLimit();
	
	ReadAllData();

	//Delay dos pots
	*(INT32*)(0x00453663 + 1) = 1;

	// regen de hp e mp
	*(INT32*)(0x0044FC79 + 1) = 300;
	*(INT32*)(0x0044FC94 + 1) = 200;
	//mp
	*(INT32*)(0x0044FD0F + 1) = 500;
	*(INT32*)(0x0044FD2A + 1) = 300;
}

void NPCGenerLimit()
{
	for (UINT32 dwTMSrv = 0x00401000; dwTMSrv <= 0x004C0000; dwTMSrv++)
	{
		UINT32 Read = *(UINT32*)dwTMSrv;
		if (Read >= 0xA169B0 && Read <= (0xA169B0 + 0xBAC))
			*(UINT32*)dwTMSrv = (UINT32)&Genner[0] + (Read - 0xA169B0);
	}

	*(INT8*)(0x00413E40 + 1) = 5;
	*(INT32*)(0x00413E40 + 2) = 0x015C69B0;

	*(INT8*)0x0041529B = 0xA3;
	*(INT32*)(0x0041529B + 1) = 0x015C69B0;
	*(INT8*)(0x0041529B + 5) = 0x90;

	*(INT32*)0x00413E4B = sizeof Genner;

	// M�ximo de NPCs gener na struct
	*(DWORD*)0x0045AA22 = 10000;
	*(DWORD*)0x004124A3 = 10000;
	*(DWORD*)0x00454055 = 10000;
	*(DWORD*)0x00459D3D = 10000;
}

void ChangeSkillDataStructurePointer()
{
	for (UINT32 dwTMSrv = 0x00401000; dwTMSrv <= 0x004C0000; dwTMSrv++)
	{
		UINT32 Read = *(UINT32*)dwTMSrv;
		if (Read >= 0x007DCDD8 && Read <= (0x007DCDD8 + 0x54))
			*(UINT32*)dwTMSrv = (UINT32)&g_pSpell[0] + (Read - 0x007DCDD8);
	}
}

void ChangeTransBonusStructurePointer()
{
	for (UINT32 dwTMSrv = 0x00401000; dwTMSrv <= 0x004C0000; dwTMSrv++){
		UINT32 Read = *(UINT32*)dwTMSrv;
		if (Read >= 0x004CAD50 && Read <= 0x004CAD94) 
			*(UINT32*)dwTMSrv = (UINT32)&TransBonus[0] + (Read - 0x004CAD50);
	}

	BYTE buffer[sizeof TransBonus];
	FILE* pFile = NULL;
	pFile = fopen("C://StoredData/transData.bin", "rb");

	if (pFile == NULL)
	{
		MessageBox(NULL, "File [transData.bin] not found.", "Error : fopen", MB_OK);
		exit(1);
	}

	fread(&buffer[0], 1, sizeof TransBonus, pFile);
	fclose(pFile);
	memcpy(&TransBonus[0], &buffer[0], sizeof TransBonus);

	for (int i = 0; i < 5; i++)
	{
		TransBonus[i].MaxAtkSpeed += TransBonus[i].MaxAtkSpeed / 4;
		TransBonus[i].MaxDamage += TransBonus[i].MaxDamage / 4;
		TransBonus[i].MaxDefense += TransBonus[i].MaxDefense / 4;
		TransBonus[i].MaxHP += TransBonus[i].MaxHP / 4;
		TransBonus[i].MinAtkSpeed += TransBonus[i].MinAtkSpeed / 4;
		TransBonus[i].MinDamage += TransBonus[i].MinDamage / 4;
		TransBonus[i].MinDefense += TransBonus[i].MinDefense / 4;
		TransBonus[i].MinHP += TransBonus[i].MinHP / 4;
	}

	TransBonus[4].MaxDamage = TransBonus[0].MaxDamage;
	TransBonus[4].MinDamage = TransBonus[0].MinDamage;
	TransBonus[4].MaxAtkSpeed = TransBonus[0].MaxAtkSpeed;
	TransBonus[4].MinAtkSpeed = TransBonus[0].MinAtkSpeed;
	TransBonus[4].MaxHP = TransBonus[1].MaxHP;
	TransBonus[4].MinHP = TransBonus[1].MinHP;
	TransBonus[4].MinDefense = TransBonus[3].MinDefense;
	TransBonus[4].MaxDefense = TransBonus[3].MaxDefense;
}

st_MountData g_pMountBonus[30] = {
	//Fisico - Magico - Evasao - Resist - Speed //4C92B8
		{ 10, 1, 0, 0, 4 }, //Porco
		{ 10, 1, 0, 0, 4 }, //Javali
		{ 50, 1, 0, 0, 5 }, //Lobo
		{ 80, 15, 0, 0, 5 }, //Dragao Menor
		{ 100, 20, 0, 0, 4 }, //Urso
		{ 150, 22, 0, 0, 5 }, //Dente de Sabre
		{ 250, 50, 40, 0, 6 }, //s/ sela N
		{ 300, 60, 50, 0, 6 }, //fantasma N
		{ 350, 65, 60, 0, 6 }, //leve N
		{ 400, 70, 70, 0, 6 }, //equipado N
		{ 500, 85, 80, 0, 6 }, //andaluz N
		{ 250, 50, 0, 16, 6 }, //s/ sela B
		{ 300, 60, 0, 20, 6 }, //fantasma B
		{ 350, 65, 0, 24, 6 }, //leve B
		{ 400, 70, 0, 28, 6 }, //equipado B
		{ 500, 85, 0, 32, 6 }, //andaluz B
		{ 550, 90, 0, 0, 6 }, //Fenrir
		{ 600, 90, 0, 0, 6 }, //Dragao
		{ 550, 90, 0, 0, 6 }, //Fenrir das Sombras
		{ 800, 90, 70, 32, 6 }, //Tigre de Fogo
		{ 900, 90, 70, 32, 6 }, //Dragao Vermelho
		{ 488, 76, 26, 16, 6 }, //Unicornio
		{ 514, 78, 66, 16, 6 }, //Pegasus
		{ 488, 73, 53, 12, 6 }, //Unisus
		{ 505, 79, 40, 20, 6 }, //Grifo
		{ 514, 79, 66, 16, 6 }, //Hipo Grifo
		{ 600, 85, 70, 28, 6 }, //Grifo Sangrento
		{ 700, 34, 80, 28, 6 }, //Svadilfari
		{ 257, 90, 80, 28, 6 }, //Sleipnir
		{ 250, 35, 20, 12, 5 }  //Pantera Negra
};

void ChangeMountStatus()
{
	// Altera os endere�os est�ticos da estrutura do mob (STRUCT_MOB) para o buffer da dll.
	for (DWORD i = 0x401000, addrBase = 0x4C92B8, addrFinal = 0x4C92CC; i < 0x4C0000; i++) //addrFinal = 0x4C92CA;
	{
		if (*(DWORD*)i >= addrBase && *(DWORD*)i <= addrFinal)
		{
			int diff = *(DWORD*)i - addrBase;

			*(DWORD*)i = (DWORD)&g_pMountBonus[0] + diff;
		}
	}
}

void CorrectSomeSizes()
{
	// BackDoor nativo e crash do ataque
	*(BYTE*)0x425053 = 48;
	*(BYTE*)0x42506A = 52;
	*(INT8*)0x00421D00 = 0xC3;

	// Desativa os logs nativos da tmsrv
	*(INT8*)0x00401406 = 0xC3;

	// Corrige a skill nativa Escudo Dourado - HT
	*(INT8*)0x00424D75 = 0x55;

	// Corrige a skill nativa Lamina - HT
	//*(BYTE*)0x00425672 = 0x0C;

	// Ataque do personagem TK M�gico aumenta pela Intelig�ncia e n�o pela For�a como era na 613
	*(BYTE*)0x040F38D = 0x52;

	// Quando o player morrer a montaria n�o morre junto
	*(BYTE*)0x45C13B = 0xEB;

	//Fix MAX_TRANS_BONUS
	*(INT8*)(0x004075EA + 6) = MAX_TRANS_BONUS;

	// Fix no DeleteChar
	*(INT8*)(0x00422BFA + 1) = 44;

	// Remove bug n�o tomar dano m�gico de mobs quando tem 100 de imu
	*(INT8*)(0x0041D92D + 1) = 0x6E;

	// Retira o bonus de imunidade dos elmos
	*(INT8*)0x00407D97 = 0;
	*(INT8*)0x00407DCC = 0;
	*(INT8*)0x00407E01 = 0;
	*(INT8*)0x00407E36 = 0;

	*(INT8*)(0x0045C545 + 6) = 12; //corrige o loop da party	

	// Diminui��o por membros do grupo
	*(DWORD*)0x0045C51E = 25;

	// Corre��o Buffs 2hrs+ Kevin Boketsu
	*(DWORD*)0x004523FD = 0x7FFF;

	// Altera o jump para os players n�o levarem mais disconect.
	*(BYTE*)0x004438D2 = 0xEB;

}

void RedirectCUserBuffer()
{
	for (UINT32 dwTMSrv = 0x00401000; dwTMSrv <= 0x004C0000; dwTMSrv++){
		UINT32 Read = *(UINT32*)dwTMSrv;
		if (Read >= 0x7B318C8 && Read <= 0x7B32514)
			*(UINT32*)dwTMSrv = (UINT32)&Users[0] + (Read - 0x7B318C8);
	}
}

void ChangeLimitAffects()
{
	for (UINT32 dwTMSrv = 0x00401000; dwTMSrv <= 0x004C0000; dwTMSrv++){
		UINT32 Read = *(UINT32*)dwTMSrv;
		if (Read >= 0x015C6CAC && Read <= 0x015C6CCA)
			*(UINT32*)dwTMSrv = (UINT32)&mBuffer[0] + (Read - 0x015C6CAC);
	}

	*(UINT8*)0x0041AB1D = MAX_AFFECTS;
	*(UINT8*)0x0041AB55 = MAX_AFFECTS;
	*(UINT8*)0x004260E6 = MAX_AFFECTS;
	*(UINT8*)0x00406FB6 = MAX_AFFECTS;
	*(UINT8*)0x004505EE = MAX_AFFECTS;
	*(UINT8*)0x00461CEB = MAX_AFFECTS;
	*(UINT8*)0x00461FCA = MAX_AFFECTS;
	*(UINT8*)0x0046226A = MAX_AFFECTS;
}


void TranslateBaseNPCFiles()
{
	static const int npc_address[36] = {
		0x0413F49, 0x0413F94, 0x0413FF1, 0x041404E,
		0x04140AA, 0x0414107, 0x0414164, 0x04141C0,
		0x041421D, 0x041427A, 0x04142D6, 0x0414333,
		0x0414390, 0x04143EC, 0x0414449, 0x04144A6,
		0x0414502, 0x041455F, 0x04145BC, 0x0414618,
		0x0414675, 0x04146D2, 0x041472E, 0x041478B,
		0x04147E8, 0x0414844, 0x04148AE, 0x041490A,
		0x0414967, 0x04149C4, 0x0414A20, 0x0414A7D,
		0x0414ADA, 0x0414B36, 0x0414B93, 0x0414BF0
	};

	static const char* npc_name[37] = {
		"Condor", "Javali", "Lobo", "Urso", "Tigre",
		"Gorila", "Dragao_Negro", "Succubus", "Porco",
		"Javali_", "Lobo_", "Dragao_Menor", "Urso_",
		"Dente_de_Sabre", "Sem_Sela", "Fantasma", "Leve",
		"Equipado", "Andaluz", "Sem_Sela_", "Fantasma_",
		"Leve_", "Equipado_", "Andaluz_", "Fenrir", "Dragao",
		"Grande_Fenrir", "Tigre_de_Fogo", "Dragao_Vermelho",
		"Unicornio", "Pegasus", "Unisus", "Grifo", "Hippo_Grifo",
		"Grifo_Sangrento", "Sleipnir", "Svadilfire"
	};

	// Escreve o nome dos arquivos que possuem as
	// informa��es sobre as montarias e as crias.
	for (int i = 0; i < 36; i++)
		*(int*)(npc_address[i] + 1) = (int)npc_name[i];
	// Modifica a string da pasta das montarias
	strcpy((char*)0x4C0C64, "npc_base");
}


void ChangeItemlistStructPointer()
{
	//8E4C00
	// Altera os endere�os est�ticos da estrutura do mob (STRUCT_MOB) para o buffer da dll.
	for(DWORD i = 0x401000, addrBase = 0x8E4C00, addrFinal = 0x8E4C8C; i < 0x4C0000; i++)
	{
		if(*(DWORD*)i >= addrBase && *(DWORD*)i <= addrFinal)
		{
			int diff = *(DWORD*)i - addrBase;

			*(DWORD*)i = (DWORD)&itemlistBuffer[0] + diff;
		}
	}

	// Modifica todas as checagens de �ndices do itemlist
	// para poder adicionar novos itens no servidor
	*(DWORD*)0x0401C7A = 6500;
	*(DWORD*)0x0403021 = 6500;
	*(DWORD*)0x0403731 = 6500;
	*(DWORD*)0x0404041 = 6500;
	*(DWORD*)0x0404793 = 6500;
	*(DWORD*)0x0404B03 = 6500;
	*(DWORD*)0x04053DB = 6500;
	*(DWORD*)0x04055DD = 6500;
	*(DWORD*)0x0405603 = 6500;
	*(DWORD*)0x0406730 = 6500;
	*(DWORD*)0x0406ECB = 6500;
	*(DWORD*)0x0407D39 = 6500;
	*(DWORD*)0x0408FDF = 6500;
	*(DWORD*)0x040907E = 6500;
	*(DWORD*)0x040AEBA = 6500;
	*(DWORD*)0x04126AE = 6500;
	*(DWORD*)0x0412719 = 6500;
	*(DWORD*)0x04127C5 = 6500;
	*(DWORD*)0x041A501 = 6500;
	*(DWORD*)0x041A854 = 6500;
	*(DWORD*)0x041A86A = 6500;
	*(DWORD*)0x041A8E4 = 6500;
	*(DWORD*)0x0424C39 = 6500;
	*(DWORD*)0x0429221 = 6500;
	*(DWORD*)0x04293C7 = 6500;
	*(DWORD*)0x042CC5F = 6500;
	*(DWORD*)0x042D82F = 6500;
	*(DWORD*)0x0430707 = 6500;
	*(DWORD*)0x0432174 = 6500;
	*(DWORD*)0x0436C65 = 6500;
	*(DWORD*)0x0437DF7 = 6500;
	*(DWORD*)0x04499D5 = 6500;
	*(DWORD*)0x044D033 = 6500;
	*(DWORD*)0x044D739 = 6500;
	*(DWORD*)0x044D845 = 6500;
	*(DWORD*)0x0455E21 = 6500;
	*(DWORD*)0x0456E0D = 6500;
	*(DWORD*)0x0456F83 = 6500;
	*(DWORD*)0x04572EC = 6500;
	*(DWORD*)0x0457467 = 6500;
	*(DWORD*)0x045819B = 6500;
	*(DWORD*)0x045B92C = 6500;
	*(DWORD*)0x045BC02 = 6500;
	*(DWORD*)0x045DC21 = 6500;
	*(DWORD*)0x045EEA9 = 6500;
	*(DWORD*)0x04065F1 = sizeof(st_ItemList);
    *(DWORD*)0x0406649 = sizeof(st_ItemList);
}

void SetMobIndex()
{
	// Altera as constantes de �nicializa��o dos �ndices dos mobs para se adequarem ao client 754
	*(DWORD*)0x4CBBE4 = 1000;
	*(DWORD*)0x04591FA = 1000;
	*(DWORD*)0x045923C = 1000;
	*(DWORD*)0x0459201 = 29000;
}

void SetSkillIndex()
{
	// Modifica o indice base das skill que ficam a venda.
	*(DWORD*)0x04366DF = 5000;
	*(DWORD*)0x04366FC = 5000;
	*(DWORD*)0x043672C = 5000;
	*(DWORD*)0x043673E = 5150;

	// Modifica o merchant que � usado para venda de skills para coincidir com o merchant do client.
	*(BYTE*)0x0437A3B = 19;
}

void SetSkillDataString()
{
	// Modifica a string de formata��o do SkillData.csv para n�o ler a informa��o Act123_2
	strcpy((char*)0x4C059E, "%*s %d %d %d %d %d %d %d");
}

void SetEncDec()
{
	// Altera a 'keys hash table' para a vers�o atual do client.
	memcpy((BYTE*)0x4CB920, &keys[0], 512);

	// Altera as 'Enc Keys'
	*(BYTE*)0x419BFE = 0x5;
	*(BYTE*)0x419B52 = 0x51;
	*(BYTE*)0x419BC5 = 0x91;
	
	// Altera as 'Dec Keys'
	*(WORD*)0x419530 = 0xE1D1;
	*(BYTE*)0x419532 = 0x90;
	*(BYTE*)0x41959E = 0x5;
}

void SetVersion()
{
	// Altera o 'CliVer'
	*(DWORD*)0x422C18 = CLIVER;
}

void SetSockAddrPorts()
{
	// Modifica as portas de comunica��o via tcp com a database e o client.
	*(DWORD*)0x44A48C = 8281;
	*(DWORD*)0x44A1FE = 7514;
}

void SetPacketSize()
{
	/* Default CliVer : 7.54 */

	// Altera o tamanho de montagem e checagem do packet GoToWorld (0x114). Padr�o: 788
	*(WORD*)0x0044E147 = 1244;
	*(DWORD*)0x0040F98B = 1244;

	// Altera o tamanho de montagem e checagem do packet RequestLogin (0x20D). Padr�o: 64
	*(BYTE*)0x0040F883 = 116;
	*(BYTE*)0x00422C26 = 116;
	*(BYTE*)0x00422D8E = 116;

	// Altera o tamanho de checagem do packet unknow (0x213). Padr�o: 20
	*(BYTE*)0x0040F967 = 36;

	// Altera o tamanho de montagem e checagem do packet SendScore (0x336). Padr�o: 64
	*(WORD*)0x004413A6 = 92;
	*(BYTE*)0x0040FAFF = 92;

	// Altera o tamanho de montagem e checagem do packet SendEtc (0x337). Padr�o : 32
	*(WORD*)0x00441607 = 36;
	*(BYTE*)0x0040FB23 = 36;

	// Altera o tamanho de montagem e checagem do packet AddExp (0x338). Padr�o : 20
	*(WORD*)0x0045AF0E = 24;

	// Altera o tamanho de montagem do packet de AutoTrade (0x363). Padr�o : 132
	*(WORD*)0x0041CBA1 = 200;

	// Altera o tamanho de montagem do packet de CreateMob (0x364). Padr�o: 108
	*(WORD*)0x0041C62D = 176;

	// Altera o tamanho de montagem e checagem do packet de SendAffect (0x36C). Padr�o: 92
	*(BYTE*)0x0040FC49 = 96;
	*(WORD*)0x0045142F = 96;
	*(WORD*)0x00452380 = 96;

	// Altera o tamanho de montagem do packet de NormalAtack (0x39D). Padr�o : 44
	*(WORD*)0x0041D280 = 48;
	*(WORD*)0x0045055A = 48; // Desativar caso aparecer algum bug.

	// Altera o tamanho de checagem do packet unnamed (0x803). Padr�o : 64
	*(BYTE*)0x004105FA = 116;

	// Altera o tamanho de montagem e checagem do packet unnamed (0x804). Padr�o : 20
	*(BYTE*)0x0041061F = 36;
	*(BYTE*)0x004231E2 = 36;

	// Altera o tamanho de montagem e checagem do packet ItemVisuality (0x36B). Padr�o : 44
	*(WORD*)0x004410C2 = 60;
	*(BYTE*)0x0040FC24 = 60;

	// Altera o tamanho de checagem do pacote DeleteChar (0x809). Padr�o : 0x20
	*(BYTE*)0x004106B7 = 0x2C;

	// Evita verifica��o do cliver, liberando assim qualquer vers�o de client se conectar ao servidor (0x20D).
	//*(BYTE*)0x422C40 = 0xEB;
	//*(WORD*)0x422C32 = 0x9090;
}

void SetMaxLevel_TrainningCamp()
{
	*(BYTE*)0x423D43 = 35;
}

void __declspec(naked) NKD_ProcessClientMessage_CorrectHpMpLimit_6()
{
	static DWORD dwContinue = 0x0042366C;
	// 00423665  |. 66:8B82 066A5C>MOV AX,WORD PTR DS:[EDX+15C6A06]
	__asm
	{
		MOV AX, WORD PTR DS : [EAX + 15C6A06h]
			MOV EDX, DWORD PTR SS : [EBP - 0A0h]

			CMP EAX, EDX
			JL lbl_setZero

			JMP dwContinue

		lbl_setZero :
		MOV EAX, DWORD PTR SS : [EBP + 8]
			IMUL EAX, EAX, 0410h
			MOV WORD PTR DS : [EAX + 015C6A06h], 0

			MOV EAX, 00423683h
			JMP EAX
	}
}

void __declspec(naked) NKD_ProcessClientMessage_CorrectHpMpLimit_5()
{
	static DWORD dwContinue = 0x00425ECA;
	//00425EC3  |. 0FBF88 046A5C0>|MOVSX ECX,WORD PTR DS:[EAX+15C6A04]
	__asm
	{
		MOVZX ECX, WORD PTR DS : [EAX + 15C6A04h]
			MOV EDX, DWORD PTR SS : [EBP - 2D0h]

			CMP ECX, EDX
			JL lbl_setZero

			JMP dwContinue

		lbl_setZero :
		MOV EAX, DWORD PTR SS : [EBP - 2CCh]
			IMUL EAX, EAX, 0410h
			MOV WORD PTR DS : [EAX + 015C6A04h], 0

			MOV EAX, 00425EE3h
			JMP EAX
	}
}

void __declspec(naked) NKD_ProcessClientMessage_CorrectHpMpLimit_4()
{
	static DWORD dwContinue = 0x00425E99;
	// 00425E8C  |. 0FBF88 046A5C0>|MOVSX ECX,WORD PTR DS:[EAX+15C6A04]
	__asm
	{
		MOVZX ECX, WORD PTR DS : [EAX + 15C6A04h]
			MOV EAX, DWORD PTR SS : [EBP - 2D0h]

			CMP ECX, EAX
			JL lbl_setZero

			JMP dwContinue

		lbl_setZero :
		MOV EAX, DWORD PTR SS : [EBP - 2CCh]
			IMUL EAX, EAX, 0410h
			MOV WORD PTR DS : [EAX + 015C6A04h], 0

			MOV EAX, 00425EB5h
			JMP EAX
	}
}

void __declspec(naked) NKD_ProcessClientMessage_CorrectHpMpLimit_3()
{
	static DWORD dwContinue = 0x00424E81;
	//00424E7A  |. 0FBF88 066A5C01 MOVSX ECX,WORD PTR DS:[EAX+15C6A06]
	__asm
	{
		MOVZX ECX, WORD PTR DS : [EAX + 15C6A06h]
			MOVSX EDX, WORD PTR SS : [EBP - 2A8h]

			CMP ECX, EDX
			JL lbl_setZero

			JMP dwContinue

		lbl_setZero :
		MOV EAX, DWORD PTR SS : [EBP + 8h]
			IMUL EAX, EAX, 0410h
			MOV WORD PTR DS : [EAX + 015C6A06h], 0

			MOV EAX, 00424E97h
			JMP EAX
	}
}

void __declspec(naked) NKD_ProcessSecTimer_CorrectHpMpLimit_2()
{
	static DWORD dwContinue = 0x00454DFF;
	//00454DF1  |. 0FBF89 046A5C0>|MOVSX ECX,WORD PTR DS:[ECX+15C6A04]
	__asm
	{
		MOVZX ECX, WORD PTR DS : [ECX + 015C6A04h]
			MOVSX EDX, WORD PTR SS : [EBP - 0292h]

			CMP ECX, EDX
			JL lbl_setZero

			JMP dwContinue

		lbl_setZero :
		MOV EAX, DWORD PTR SS : [EBP - 0290h]
			IMUL EAX, EAX, 0410h
			MOV WORD PTR DS : [EAX + 015C6A04h], 0

			MOV EAX, 00454E1Bh
			JMP EAX
	}
}

void __declspec(naked) NKD_ProcessSecTimer_CorrectHpMpLimit_1()
{
	static DWORD dwContinue = 0x00454E37;
	//00454E29  |. 0FB788 046A5C0>|MOVZX ECX,WORD PTR DS:[EAX+15C6A04]
	__asm
	{
		MOVZX ECX, WORD PTR DS : [EAX + 015C6A04h]
			MOVSX EDX, WORD PTR SS : [EBP - 0292h]

			CMP ECX, EDX
			JL lbl_setZero

			JMP dwContinue

		lbl_setZero :
		MOV EAX, DWORD PTR SS : [EBP - 0290h]
			IMUL EAX, EAX, 0410h
			MOV WORD PTR DS : [EAX + 015C6A04h], 0

			MOV EAX, 00454E4Ch
			JMP EAX
	}
}

void LimiteHpMp()
{
	// Altera os JMPS e MOVS
	
	for (UINT32 dwTMSrv = 0x00401000; dwTMSrv <= 0x004C0000; dwTMSrv++) 
	{ 
		if (*(DWORD*)dwTMSrv == 0x15C6A06 || *(DWORD*)dwTMSrv == 0x15C69E4 || *(DWORD*)dwTMSrv == 0x15C69E6 ||
			*(DWORD*)dwTMSrv == 0x15C69E8 || *(DWORD*)dwTMSrv == 0x15C69EA || *(DWORD*)dwTMSrv == 0x15C6A00 ||
			*(DWORD*)dwTMSrv == 0x15C6A02 || *(DWORD*)dwTMSrv == 0x15C6A04)
		{
			if (*(BYTE*)(dwTMSrv - 2) == 0xBF)
				*(BYTE*)(dwTMSrv - 2) = 0xB7;
			else if (*(BYTE*)(dwTMSrv - 2) == 0x7D)
				*(BYTE*)(dwTMSrv - 2) = 0x73;
			else if (*(BYTE*)(dwTMSrv - 2) == 0x7E)
				*(BYTE*)(dwTMSrv - 2) = 0x76;
			else if (*(BYTE*)(dwTMSrv - 2) == 0x7F)
				*(BYTE*)(dwTMSrv - 2) = 0x77;
			else if (*(BYTE*)(dwTMSrv - 2) == 0x8E)
				*(BYTE*)(dwTMSrv - 2) = 0x86;
		}
	}
	
	
	// Was, verificar as linhas abaixo
	// MOVZX tem de virar MOVSX
	// Trocar 0x7E para 0x76 (JLE to JBE)
	// Trocar 0x7F para 0x77 (JG to JA)
	// Trocar 0x7D para 0x73 (JGE to JAE)
	// Trocar 0x8E para 0x86 (JLE to JBE)  2BYTES
	/*
	// bStatus
	// MOVS
	// MOVSX EAX,WORD PTR DS:[EDX+15C69E4]
	*(INT8*)(0x00446568 + 1) = 0xB7;
	// MOVSX EAX,WORD PTR DS:[EDX+15C69E6]
	*(INT8*)(0x00447558 + 1) = 0xB7;
	//JMPS

	// Status
	// MOVS
	// MOVSX ECX,WORD PTR DS:[EDX+15C6A00]
	*(INT8*)(0x0041D71F + 1) = 0xB7;
	// MOVSX ECX,WORD PTR DS:[EDX+15C6A00]
	*(INT8*)(0x0041D754 + 1) = 0xB7;
	// MOVSX EAX,WORD PTR DS:[ECX+15C6A00]
	*(INT8*)(0x0041D79E + 1) = 0xB7;
	// MOVSX EAX,WORD PTR DS:[EDX+15C6A00]
	*(INT8*)(0x00425F02 + 1) = 0xB7;
	// MOVSX EAX,WORD PTR DS:[EDX+15C6A00]
	*(INT8*)(0x004265D8 + 1) = 0xB7;
	// MOVSX ECX,WORD PTR DS:[EDX+15C6A00]
	*(INT8*)(0x00427B99 + 1) = 0xB7;
	// MOVSX EAX,WORD PTR DS:[EDX+15C6A00]
	*(INT8*)(0x0042DE71 + 1) = 0xB7;
	// MOVSX EAX,WORD PTR DS:[EDX+15C6A00]
	*(INT8*)(0x0042DE89 + 1) = 0xB7;
	// MOVSX EAX,WORD PTR DS:[EDX+15C6A00]
	*(INT8*)(0X00446AE7 + 1) = 0xB7;
	// MOVSX EDX,WORD PTR DS:[ECX+15C6A00]
	*(INT8*)(0x0044FC05 + 1) = 0xB7;
	// MOVSX EDX,WORD PTR DS:[ECX+15C6A00]
	*(INT8*)(0x0045061D + 1) = 0xB7;
	// MOVSX ECX,WORD PTR DS:[ECX+15C6A00]
	*(INT8*)(0x00454C00 + 1) = 0xB7;
	// MOVSX EDX,WORD PTR DS:[ECX+15C6A00]
	*(INT8*)(0x0045C1B5 + 1) = 0xB7;
	// MOVSX ECX,WORD PTR DS:[EAX+15C6A00]
	*(INT8*)(0x0045DF42 + 1) = 0xB7;
	// MOVSX ECX,WORD PTR DS:[EAX+15C6A00]
	*(INT8*)(0x0045DFD6 + 1) = 0xB7;
	// MOVSX EDX,WORD PTR DS:[ECX+15C6A00]
	*(INT8*)(0x00462E3A + 1) = 0xB7;
	// MOVSX ECX,WORD PTR DS:[EAX+15C6A00]
	*(INT8*)(0x00462E52 + 1) = 0xB7;
	// MOVSX EAX,WORD PTR DS:[EDX+15C6A00]
	*(INT8*)(0x004630D2 + 1) = 0xB7;
	// MOVSX ECX,WORD PTR DS:[EAX+15C6A02]
	*(INT8*)(0x0042DEFB + 1) = 0xB7;
	// MOVSX EAX,WORD PTR DS:[EDX+15C6A02]
	*(INT8*)(0x0042DF13 + 1) = 0xB7;
	// MOVSX ECX,WORD PTR DS:[EAX+15C6A02]
	*(INT8*)(0x0044FC1B + 1) = 0xB7;
	// MOVSX EDX,WORD PTR DS:[ECX+15C6A02]
	*(INT8*)(0x00462F7A + 1) = 0xB7;
	// MOVSX ECX,WORD PTR DS:[EAX+15C6A02]
	*(INT8*)(0x00462F92 + 1) = 0xB7;
	// MOVSX EAX,WORD PTR DS:[EDX+15C6A02]
	*(INT8*)(0x004631C2 + 1) = 0xB7;
	// MOVSX EDX,WORD PTR DS:[ECX+15C6A04]
	*(INT8*)(0x00411C01 + 1) = 0xB7;
	// MOVSX EAX,WORD PTR DS:[EDX+15C6A04]
	*(INT8*)(0x00413CCA + 1) = 0xB7;
	// MOVSX EAX,WORD PTR DS:[EDX+15C6A04]
	*(INT8*)(0x0041D703 + 1) = 0xB7;
	// MOVSX EAX,WORD PTR DS:[EDX+15C6A04]
	*(INT8*)(0x0041D738 + 1) = 0xB7;
	// MOVSX EAX,WORD PTR DS:[EDX+15C6A04]
	*(INT8*)(0x00423293 + 1) = 0xB7;
	// MOVSX ECX,WORD PTR DS:[EAX+15C6A04]
	*(INT8*)(0x004233D2 + 1) = 0xB7;
	// MOVSX ECX,WORD PTR DS:[EAX+15C6A04]
	*(INT8*)(0x0042354F + 1) = 0xB7;
	// MOVSX ECX,WORD PTR DS:[EAX+15C6A04]
	*(INT8*)(0x0042444A + 1) = 0xB7;
	// MOVSX EAX,WORD PTR DS:[EDX+15C6A04]
	*(INT8*)(0x00424536 + 1) = 0xB7;
	// MOVSX ECX,WORD PTR DS:[EAX+15C6A04]
	*(INT8*)(0x00425005 + 1) = 0xB7;
	// MOVSX ECX,WORD PTR DS:[EAX+15C6A04]
	*(INT8*)(0x004250DF + 1) = 0xB7;
	// MOVSX EDX,WORD PTR DS:[ECX+15C6A04]
	*(INT8*)(0x00425E24 + 1) = 0xB7;
	// MOVSX ECX,WORD PTR DS:[EAX+15C6A04]
	//*(INT8*)(0x00425E8C + 1) = 0xB7;
	// MOVSX ECX,WORD PTR DS:[EAX+15C6A04]
	//*(INT8*)(0x00425EC3 + 1) = 0xB7;
	// MOVSX ECX,WORD PTR DS:[EAX+15C6A04]
	*(INT8*)(0x00425EEF + 1) = 0xB7;
	// MOVSX EAX,WORD PTR DS:[EDX+15C6A04]
	*(INT8*)(0x00425F60 + 1) = 0xB7;
	// MOVSX ECX,WORD PTR DS:[EAX+15C6A04]
	*(INT8*)(0x004261CB + 1) = 0xB7;
	// MOVSX ECX,WORD PTR DS:[EAX+15C6A04]
	*(INT8*)(0x004265C5 + 1) = 0xB7;
	// MOVSX EAX,WORD PTR DS:[ECX+15C6A04]
	*(INT8*)(0x00426EEF + 1) = 0xB7;
	// MOVSX ECX,WORD PTR DS:[EAX+15C6A04]
	*(INT8*)(0x00426F19 + 1) = 0xB7;
	// MOVSX EDX,WORD PTR DS:[ECX+15C6A04]
	*(INT8*)(0x00427B32 + 1) = 0xB7;
	// MOVSX ECX,WORD PTR DS:[EAX+15C6A04]
	*(INT8*)(0x00427B4D + 1) = 0xB7;
	// MOVSX EAX,WORD PTR DS:[EDX+15C6A04]
	*(INT8*)(0x00427DFB + 1) = 0xB7;
	// MOVSX EDX,WORD PTR DS:[ECX+15C6A04]
	*(INT8*)(0x00427E16 + 1) = 0xB7;
	// MOVSX ECX,WORD PTR DS:[EAX+15C6A04]
	*(INT8*)(0x00427E2F + 1) = 0xB7;
	// MOVSX ECX,WORD PTR DS:[EAX+15C6A04]
	*(INT8*)(0x00427E5D + 1) = 0xB7;
	// MOVSX EAX,WORD PTR DS:[EDX+15C6A04]
	*(INT8*)(0x00427E78 + 1) = 0xB7;
	// MOVSX EDX,WORD PTR DS:[ECX+15C6A04]
	*(INT8*)(0x00427E91 + 1) = 0xB7;
	// MOVSX ECX,WORD PTR DS:[EAX+15C6A04]
	*(INT8*)(0x00427EE5 + 1) = 0xB7;
	// MOVSX EDX,WORD PTR DS:[ECX+15C6A04]
	*(INT8*)(0x00427FBB + 1) = 0xB7;
	// MOVSX EDX,WORD PTR DS:[ECX+15C6A04]
	*(INT8*)(0x0042801A + 1) = 0xB7;
	// MOVSX ECX,WORD PTR DS:[EAX+15C6A04]
	*(INT8*)(0x004280EE + 1) = 0xB7;
	// MOVSX EDX,WORD PTR DS:[ECX+15C6A04]
	*(INT8*)(0x004281FC + 1) = 0xB7;
	// MOVSX EDX,WORD PTR DS:[ECX+15C6A04]
	*(INT8*)(0x0042C971 + 1) = 0xB7;
	// MOVSX EDX,WORD PTR DS:[ECX+15C6A04]
	*(INT8*)(0x0042D496 + 1) = 0xB7;
	// MOVSX ECX,WORD PTR DS:[EAX+15C6A04]
	*(INT8*)(0x0042DBDF + 1) = 0xB7;
	// MOVSX ECX,WORD PTR DS:[EAX+15C6A04]
	*(INT8*)(0x00430C93 + 1) = 0xB7;
	// MOVSX EAX,WORD PTR DS:[EDX+15C6A04]
	*(INT8*)(0x00430EE1 + 1) = 0xB7;
	// MOVSX ECX,WORD PTR DS:[EAX+15C6A04]
	*(INT8*)(0x00430F69 + 1) = 0xB7;
	// MOVSX EAX,WORD PTR DS:[EDX+15C6A04]
	*(INT8*)(0x00431893 + 1) = 0xB7;
	// MOVSX ECX,WORD PTR DS:[EAX+15C6A04]
	*(INT8*)(0x00431A1D + 1) = 0xB7;
	// MOVSX ECX,WORD PTR DS:[EAX+15C6A04]
	*(INT8*)(0x00432406 + 1) = 0xB7;
	// MOVSX ECX,WORD PTR DS:[EAX+15C6A04]
	*(INT8*)(0x00433586 + 1) = 0xB7;
	// MOVSX EDX,WORD PTR DS:[ECX+15C6A04]
	*(INT8*)(0x00434879 + 1) = 0xB7;
	// MOVSX ECX,WORD PTR DS:[EAX+15C6A04]
	*(INT8*)(0x004349D7 + 1) = 0xB7;
	// MOVSX ECX,WORD PTR DS:[EAX+15C6A04]
	*(INT8*)(0x00434E2A + 1) = 0xB7;
	// MOVSX ECX,WORD PTR DS:[EAX+15C6A04]
	*(INT8*)(0x00436275 + 1) = 0xB7;
	// MOVSX EDX,WORD PTR DS:[ECX+15C6A04]
	*(INT8*)(0x00436ABD + 1) = 0xB7;
	// MOVSX ECX,WORD PTR DS:[EAX+15C6A04]
	*(INT8*)(0x0043790D + 1) = 0xB7;
	// MOVSX EAX,WORD PTR DS:[EDX+15C6A04]
	*(INT8*)(0x00437A6B + 1) = 0xB7;
	// MOVSX EAX,WORD PTR DS:[EAX+15C6A04]
	*(INT8*)(0x00447E4B + 1) = 0xB7;
	// MOVSX EAX,WORD PTR DS:[EDX+15C6A04]
	*(INT8*)(0x00448543 + 1) = 0xB7;
	// MOVSX ECX,WORD PTR DS:[EAX+15C6A04]
	*(INT8*)(0x0044DA88 + 1) = 0xB7;
	// MOVSX EDX,WORD PTR DS:[ECX+15C6A04]
	*(INT8*)(0x0044E181 + 1) = 0xB7;
	// MOVSX EDX,WORD PTR DS:[ECX+15C6A04]
	*(INT8*)(0x0044FBCF + 1) = 0xB7;
	// MOVSX EAX,WORD PTR DS:[EDX+15C6A04]
	*(INT8*)(0x0044FC31 + 1) = 0xB7;
	// MOVSX ECX,WORD PTR DS:[EAX+15C6A04]
	*(INT8*)(0x00450630 + 1) = 0xB7;
	// MOVSX EAX,WORD PTR DS:[EDX+15C6A04]
	*(INT8*)(0x004506D1 + 1) = 0xB7;
	// MOVSX EDX,WORD PTR DS:[ECX+15C6A04]
	*(INT8*)(0x004506ED + 1) = 0xB7;
	// MOVSX EAX,WORD PTR DS:[EDX+15C6A04]
	*(INT8*)(0x004507FE + 1) = 0xB7;
	// MOVSX EDX,WORD PTR DS:[ECX+15C6A04]
	*(INT8*)(0x0045081A + 1) = 0xB7;
	// MOVSX EAX,WORD PTR DS:[EDX+15C6A04]
	*(INT8*)(0x004508F8 + 1) = 0xB7;
	// MOVSX EDX,WORD PTR DS:[ECX+15C6A04]
	*(INT8*)(0x00450914 + 1) = 0xB7;
	// MOVSX ECX,WORD PTR DS:[EAX+15C6A04]
	*(INT8*)(0x00450B29 + 1) = 0xB7;
	// MOVSX EDX,WORD PTR DS:[ECX+15C6A04]
	*(INT8*)(0x00451031 + 1) = 0xB7;
	// MOVSX ECX,WORD PTR DS:[EAX+15C6A04]
	*(INT8*)(0x0045177C + 1) = 0xB7;
	// MOVSX ECX,WORD PTR DS:[ECX+15C6A04]
	*(INT8*)(0x00451923 + 1) = 0xB7;
	// MOVSX ECX,WORD PTR DS:[EAX+15C6A04]
	*(INT8*)(0x0045195B + 1) = 0xB7;
	// MOVSX EAX,WORD PTR DS:[EDX+15C6A04]
	*(INT8*)(0x00451A27 + 1) = 0xB7;
	// MOVSX EAX,WORD PTR DS:[EDX+15C6A04]
	*(INT8*)(0x00452F51 + 1) = 0xB7;
	// MOVSX ECX,WORD PTR DS:[EAX+15C6A04]
	*(INT8*)(0x00452FD5 + 1) = 0xB7;
	// MOVSX ECX,WORD PTR DS:[EAX+15C6A04]
	*(INT8*)(0x0045301D + 1) = 0xB7;
	// MOVSX EDX,WORD PTR DS:[ECX+15C6A04]
	*(INT8*)(0x004536BD + 1) = 0xB7;
	// MOVSX EAX,WORD PTR DS:[EDX+15C6A04]
	*(INT8*)(0x0045383C + 1) = 0xB7;
	// MOVSX EAX,WORD PTR DS:[EDX+15C6A04]
	*(INT8*)(0x00453B9A + 1) = 0xB7;
	// MOVSX ECX,WORD PTR DS:[EAX+15C6A04]
	*(INT8*)(0x00453C2D + 1) = 0xB7;
	// MOVSX EDX,WORD PTR DS:[ECX+15C6A04]
	*(INT8*)(0x00453DB5 + 1) = 0xB7;
	// MOVSX EAX,WORD PTR DS:[EDX+15C6A04]
	*(INT8*)(0x00453ED0 + 1) = 0xB7;
	// MOVSX EAX,WORD PTR DS:[EDX+15C6A04]
	*(INT8*)(0x004543D2 + 1) = 0xB7;
	// MOVSX EAX,WORD PTR DS:[EDX+15C6A04]
	*(INT8*)(0x004549C5 + 1) = 0xB7;
	// MOVSX EAX,WORD PTR DS:[EDX+15C6A04]
	*(INT8*)(0x00454B92 + 1) = 0xB7;
	// MOVSX ECX,WORD PTR DS:[ECX+15C6A04]
	//*(INT8*)(0x00454DF1 + 1) = 0xB7;
	// MOVSX ECX,WORD PTR DS:[EAX+15C6A04]
	//*(INT8*)(0x00454E29 + 1) = 0xB7;
	// MOVSX EDX,WORD PTR DS:[ECX+15C6A04]
	*(INT8*)(0x00454F2C + 1) = 0xB7;
	// MOVSX EAX,WORD PTR DS:[EDX+15C6A04]
	*(INT8*)(0x00456BEA + 1) = 0xB7;
	// MOVSX EAX,WORD PTR DS:[EDX+15C6A04]
	*(INT8*)(0x00457172 + 1) = 0xB7;
	// MOVSX ECX,WORD PTR DS:[EAX+15C6A04]
	*(INT8*)(0x0045770D + 1) = 0xB7;
	// MOVSX EAX,WORD PTR DS:[EDX+15C6A04]
	*(INT8*)(0x0045C3B7 + 1) = 0xB7;
	// MOVSX EDX,WORD PTR DS:[ECX+15C6A04]
	*(INT8*)(0x0045C5A3 + 1) = 0xB7;
	// MOVSX EAX,WORD PTR DS:[EDX+15C6A04]
	*(INT8*)(0x0045DF89 + 1) = 0xB7;
	// MOVSX EDX,WORD PTR DS:[ECX+15C6A04]
	*(INT8*)(0x0045DFC6 + 1) = 0xB7;
	// MOVSX EAX,WORD PTR DS:[EDX+15C6A04]
	*(INT8*)(0x0045E00A + 1) = 0xB7;
	// MOVSX EDX,WORD PTR DS:[ECX+15C6A04]
	*(INT8*)(0x00460AA4 + 1) = 0xB7;
	// MOVSX EAX,WORD PTR DS:[EDX+15C6A04]
	*(INT8*)(0x00460FCA + 1) = 0xB7;
	// MOVSX ECX,WORD PTR DS:[EAX+15C6A04]
	*(INT8*)(0x00462E71 + 1) = 0xB7;
	// MOVSX EDX,WORD PTR DS:[ECX+15C6A04]
	*(INT8*)(0x00462EB8 + 1) = 0xB7;
	// MOVSX EDX,WORD PTR DS:[ECX+15C6A04]
	*(INT8*)(0x00462EDB + 1) = 0xB7;
	// MOVSX ECX,WORD PTR DS:[EAX+15C6A04]
	*(INT8*)(0x004630C2 + 1) = 0xB7;
	// MOVSX EAX,WORD PTR DS:[EDX+15C6A04]
	*(INT8*)(0x0046310F + 1) = 0xB7;
	// MOVSX EDX,WORD PTR DS:[ECX+15C6A04]
	*(INT8*)(0x00463127 + 1) = 0xB7;
	// MOVSX EAX,WORD PTR DS:[EDX+15C6A06]
	*(INT8*)(0x0042363C + 1) = 0xB7;
	// MOVSX EDX,WORD PTR DS:[ECX+15C6A06]
	*(INT8*)(0x00424DDC + 1) = 0xB7;
	// MOVSX ECX,WORD PTR DS:[EAX+15C6A06]
	*(INT8*)(0x00424E4F + 1) = 0xB7;
	// MOVSX ECX,WORD PTR DS:[EAX+15C6A06]
	//*(INT8*)(0x00424E7A + 1) = 0xB7;
	// MOVSX EDX,WORD PTR DS:[ECX+15C6A06]
	*(INT8*)(0x00426C9D + 1) = 0xB7;
	// MOVSX EDX,WORD PTR DS:[ECX+15C6A06]
	*(INT8*)(0x0044E1A0 + 1) = 0xB7;
	// MOVSX EDX,WORD PTR DS:[ECX+15C6A06]
	*(INT8*)(0x0044FC47 + 1) = 0xB7;
	// MOVSX ECX,WORD PTR DS:[EAX+15C6A06]
	*(INT8*)(0x00462FB1 + 1) = 0xB7;
	// MOVSX EDX,WORD PTR DS:[ECX+15C6A06]
	*(INT8*)(0x00462FF8 + 1) = 0xB7;
	// MOVSX EDX,WORD PTR DS:[ECX+15C6A06]
	*(INT8*)(0x0046301B + 1) = 0xB7;
	// MOVSX ECX,WORD PTR DS:[EAX+15C6A06]
	*(INT8*)(0x004631B2 + 1) = 0xB7;
	// MOVSX EAX,WORD PTR DS:[EDX+15C6A06]
	*(INT8*)(0x004631FF + 1) = 0xB7;
	// MOVSX EDX,WORD PTR DS:[ECX+15C6A06]
	*(INT8*)(0x00463217 + 1) = 0xB7;

	// JMPS
	// JLE SHORT TMSRV.00425F33
	*(INT8*)0x00425F0B = 0x76;
	// JLE SHORT TMSRV.0042DE96
	*(INT8*)0x0042DE7E = 0x76;
	// JLE SHORT TMSRV.00446B09
	*(INT8*)0x00446AF0 = 0x76;
	// JLE SHORT TMSRV.0045E001
	*(INT8*)0x0045DFDF = 0x76;
	// JLE SHORT TMSRV.00462E68
	*(INT8*)0x00462E47 = 0x76;
	// JLE SHORT TMSRV.004630FD
	*(INT8*)0x004630DB = 0x76;
	// JLE SHORT TMSRV.0042DF20
	*(INT8*)0x0042DF08 = 0x76;
	// JLE SHORT TMSRV.00462FA8
	*(INT8*)0x00462F87 = 0x76;
	// JLE SHORT TMSRV.004631ED
	*(INT8*)0x004631CB = 0x76;
	// JLE SHORT TMSRV.00427E23
	*(INT8*)0x00427E08 = 0x76;
	// JLE SHORT TMSRV.00427E85
	*(INT8*)0x00427E6A = 0x76;
	// JLE TMSRV.00428266
	*(INT8*)(0x00428023 + 1) = 0x86;
	// JLE SHORT TMSRV.004538889
	*(INT8*)0x00453845 = 0x76;
	// JLE SHORT TMSRV.0045C436
	*(INT8*)0x0045C3C0 = 0x76;
	// JLE SHORT TMSRV.0045C622
	*(INT8*)0x0045C5AC = 0x76;
	// JLE SHORT TMSRV.00462EFB
	*(INT8*)0x00462EE5 = 0x76;
	// JLE SHORT TMSRV.0046303B
	*(INT8*)0x00463025 = 0x76;

	// JG SHORT TMSRV.00411C21
	*(INT8*)0x00411C0A = 0x77;
	// JG SHORT TMSRV.00413CDA
	*(INT8*)0x00413CD3 = 0x77;
	// JG SHORT TMSRV.OO42512C
	*(INT8*)0x004250E8 = 0x77;
	// JG SHORT TMSRV.004261EC
	*(INT8*)0x004261D4 = 0x77;
	// JG SHORT TMSRV.00427FF5
	*(INT8*)0x00427FC4 = 0x77;
	// JG SHORT TMSRV.00428140
	*(INT8*)0x004280F7 = 0x77;
	// JG SHORT TMSRV.0042824E
	*(INT8*)0x00428205 = 0x77;
	// JG SHORT TMSRV.0043489A
	*(INT8*)0x00434882 = 0x77;
	// JG SHORT TMSRV.00434A01
	*(INT8*)0x004349E0 = 0x77;
	// JG SHORT TMSRV.00434E4B
	*(INT8*)0x00434E33 = 0x77;
	// JG SHORT TMSRV.00450B39
	*(INT8*)0x00450B32 = 0x77;
	// JG SHORT TMSRV.00451041
	*(INT8*)0x0045103A = 0x77;
	// JG SHORT TMSRV.004517CE
	*(INT8*)0x00451785 = 0x77;
	// JG SHORT TMSRV.00451A5E
	*(INT8*)0x00451A30 = 0x77;
	// JG SHORT TMSRV.00452F5E
	*(INT8*)0x00452F5A = 0x77;
	// JG SHORT TMSRV.004536CA
	*(INT8*)0x004536C6 = 0x77;
	// JG SHORT TMSRV.00453BCF
	*(INT8*)0x00453BA3 = 0x77;
	// JG SHORT TMSRV.00453C62
	*(INT8*)0x00453C36 = 0x77;
	// JG SHORT TMSRV.00453E07
	*(INT8*)0x00453DBE = 0x77;
	// JG SHORT TMSRV.00453F22
	*(INT8*)0x00453ED9 = 0x77;
	// JG SHORT TMSRV.0045445B
	*(INT8*)0x004543DB = 0x77;
	// JG SHORT TMSRV.00454A17
	*(INT8*)0x004549CE = 0x77;
	// JG SHORT TMSRV.00454F66
	*(INT8*)0x00454F35 = 0x77;
	// JG SHORT TMSRV.00456C13
	*(INT8*)0x00456BF3 = 0x77;
	// JG SHORT TMSRV.0045717F
	*(INT8*)0x0045717B = 0x77;
	// JG SHORT TMSRV.0045771D
	*(INT8*)0x00457716 = 0x77;
	// JG SHORT TMSRV.00460AB1
	*(INT8*)0x00460AAD = 0x77;
	// JG SHORT TMSRV.00460FD7
	*(INT8*)0x00460FD3 = 0x77;

	// JGE SHORT TMSRV.00427B5C
	*(INT8*)0x00427B3F = 0x73;
	// JGE SHORT TMSRV.004265E8
	*(INT8*)0x004265E1 = 0x73;
	// JGE SHORT TMSRV.0042365C
	*(INT8*)0x00423649 = 0x73;
	// JGE SHORT TMSRV.00424E71
	*(INT8*)0x00424E5E = 0x73;
	// JGE SHORT TMSRV.0046322D                                     
	*(INT8*)0x0046320C = 0x73;
	*/
	// ProcessSecTimer, ajeita o limite de hp/mp
	JMP_NEAR(0x00454E29, NKD_ProcessSecTimer_CorrectHpMpLimit_1, 2);
	JMP_NEAR(0x00454DF1, NKD_ProcessSecTimer_CorrectHpMpLimit_2, 2);
	// ProcessClientMessage, ajeita o limite de hp/mp
	JMP_NEAR(0x00424E7A, NKD_ProcessClientMessage_CorrectHpMpLimit_3, 2);
	JMP_NEAR(0x00425E8C, NKD_ProcessClientMessage_CorrectHpMpLimit_4, 2);
	JMP_NEAR(0x00425EC3, NKD_ProcessClientMessage_CorrectHpMpLimit_5, 2);
	JMP_NEAR(0x00423665, NKD_ProcessClientMessage_CorrectHpMpLimit_6, 2);
}

