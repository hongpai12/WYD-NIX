#include "PE_Patch.h"
#include "Naked.h"
#include "Hook.h"
#include "Functions.h"
#include "Send.h"
#include "MobKilled.h"

int jmp_addr;
static INT32 reg_aux;
static INT32 st_newmob_addr;
static INT32 st_newmob_size;

void DoIndirections()
{
	memset(&mBuffer, 0, sizeof(mBuffer));

	//PacketControl
	SetJMP(0x004199B6, (UINT32)&NKD_ProcessSend, 1);
	//Recv Patch Base
	SetJMP(0x004195C4, (UINT32)&NKD_ProcessRecv, 3);

	//JNZ_NEAR(0x0407CC2, NKD_GetCurScore_ControlAffects);

	//pControl
	JMP_NEAR(0x0040124E, NKD_ReadMessage);
	JMP_NEAR(0x00401221, NKD_AddMessage);

	// Decryptação
	JMP_NEAR(0x00419555, NKD_ReadMessage_DecKey_1, 2);
	JMP_NEAR(0x00419578, NKD_ReadMessage_DecKey_2, 2);

	// Encryptação
	JMP_NEAR(0x00419B89, NKD_AddMessage_EncKey_1, 2);

	// MobKilled Indirections
	JMP_NEAR(0x0045D58F, NKD_MobKilled_DropGold, 1);
	JMP_NEAR(0x0045DCC1, &NKD_MobKilled_ControlDropItem, 2);
	JMP_NEAR(0x0045AC7C, &NKD_MobKilled_EventDead, 2);
	//JMP_NEAR(0x0045C29E, &NKD_MobKilled_ControlExp, 1);

	// Experiência acumulada
	JMP_NEAR(0x0045C5AE, &NKD_ExpInPesaAllArea, 1);

	// CheckPacket Indirections
	CALL_NEAR(0x0040FB6B, NKD_CheckPacket_GetCreateMobSize); // Altera o tamanho de checagem do packet de CreateMob (0x364). Padrão: 108
	CALL_NEAR(0x0040FF95, NKD_CheckPacket_SendTradeSize); // Altera o tamanho de checagem do packet de Trade (0x383). Padrão : 92

	// fix
	SetJMP(0x00459465, (INT32)&HKD_CloseUser, 7);

	// Limite de buffs
	JGE_NEAR(0x0044D80E, &FixAffects);
	JMP_NEAR(0x00401505, &CheckAffects);
	JMP_NEAR(0x00412543, &GetBuffer, 5);
	JMP_NEAR(0x00412580, &AfterAffects, 1);
	JMP_NEAR(0x00452467, &RemoveAffects);
	st_newmob_addr = (int)mBuffer[0].Affects;
	st_newmob_size = sizeof(stBuffer);
	JMP_NEAR(0x00410EF7, NKD_StandingByProcessor_VerifyAbility_1, 5);
	JMP_NEAR(0x00412543, NKD_GetCurrentScore_VerifyAbility_1, 4);
	JMP_NEAR(0x00446273, &NKD_GenerateSummon_CanSummon, 4);
	// Limite de Buffs

	// Limite de crítico até 102%, e também acaba protegendo contra critical hack do whook
	CALL_NEAR(0x0040C3B8, &NKD_GetDoubleCritical_SetNewCriticalRate, 3);
	CALL_NEAR(0x00412567, &NKD_GetGetCurrentScore_FixCriticalRate, 5);

	// Retira o crash da divisão por 0 
	CALL_NEAR(0x00427BA1, &Fix_IntegerDivisionByZero, 3);

	// Faz um HOOK na skill espectral, dando +1 de distância
	JMP_NEAR(0x0040549F, &NKD_GetMobAbility_Spectral);
	//JMP_NEAR(0x00427485, &NKD_FixSpectralVisual, 4);

	// Correções nas refinações		
	JMP_NEAR(0x00404CEE, &NKD_CorrectRefs, 2);

	// Correção na Jóia da Armazenagem
	JMP_NEAR(0x00433633, (void*)0x004337CC); // Eric Santos
	// Desativa a função nativa do controle de mana
	JMP_NEAR(0x00407B3C, (void*)0x00407B99);
	// Remoção dos logs nativos da tm
	JMP_NEAR(0x00401406, (void*)0x00459039);
	// Desativa guerras originais
	JMP_NEAR(0x004013D9, (void*)0x0045792A);
	JMP_NEAR(0x00457A20, (void*)0x00458921);

	// Desativa a skill possuido.
	JMP_NEAR(0x0407429, (void*)0x0407488);

	// Load/Save
	CALL_NEAR(0x0044E248, &NKD_ProcessDBMessage_LoadBuffs, 1);
	JMP_NEAR(0x0040158C, HKD_SaveUser);

	JMP_NEAR(0x0044BA1D, &NKD_CorrectSleip, 3);

	// Funções reescritas
	// BASE_GetHpMp
	JMP_NEAR(0x00401307, BASE_GetHpMp);
	// BASE_RemoveParty
	JMP_NEAR(0x00401366, RemoveParty);
	// Sistema de trocas
	// GetParryRate
	JMP_NEAR(0x0040113B, GetParryRate);
	// Reescreve a função BASE_GetBonusScorePoint
	JMP_NEAR(0x0040133E, BASE_GetBonusScorePoint);
	// Reescreve a função BASE_GetBonusSkillPoint
	JMP_NEAR(0x00401456, BASE_GetBonusSkillPoint);
	// CheckGetLevel
	JMP_NEAR(0x0040143D, &HKD_CheckGetLevel);
	// GetExpApply
	//JMP_NEAR(0x00401104, &HKD_GetExpApply);
	// Controle de Status e Bloqueios
    JMP_NEAR(0x00412792, &NKD_GetCurScore);
	
	// Correção no login na cidade
	JMP_NEAR(0x0044DFF4, &NKD_CorrectPositionGoWorld, 1);
	JMP_NEAR(0x0044DF8C, (void*)0x0044DFF4, 3);

	// Controle dos teleportes
	JMP_NEAR(0x428905, &NKD_ControlTeleportPosition, 1);

	// Uso de rações
	JMP_NEAR(0x0042FA12, &NKD_ProcessClientMessage_UseMountFeed, 1);

	//Fix Eden
	JMP_NEAR(0x00407605, &NKD_Base_GetCurrentScore_FixTrans, 1);
	JMP_NEAR(0x00406F77, &NKD_Base_GetCurrentScore_FixFaceArch, 2);
	JMP_NEAR(0x0040774A, &NKD_Base_GetCurrentScore_FixEdenStatus, 2);
	JMP_NEAR(0x00407765, &NKD_GetCurrentScore_SancTransform, 2);

	// Controle total do Campo de Treino
	//JMP_NEAR(0x00423D31, &NKD_FixTrainningCamp, 4);

	// Controle de Dano
	JMP_NEAR(0x0041D90A, &NKD_BASEGetDamage_01, 2);
	JMP_NEAR(0x004255F6, &NKD_BASEGetDamage_02, 1);
	JMP_NEAR(0x00425B41, &NKD_BASEGetSkillDamage, 1);

	JMP_NEAR(0x0041D8D8, NKD_GetAttack_CriticalHit, 3); // by shepher

	JMP_NEAR(0x00425E31, NKD_ControllBossDamage_01, 1);
	JMP_NEAR(0x00427D8A, NKD_ControllBossDamage_02, 1);
	JMP_NEAR(0x004518C9, NKD_ControllBossDamage_03, 1);
	JMP_NEAR(0x00454D97, NKD_ControllBossDamage_04, 1);
	
	JMP_NEAR(0x00454841, &NKD_GetParryRate_JewelOfPrecision_2, 1);
	JMP_NEAR(0x004278DF, &NKD_GetParryRate_JewelOfPrecision, 1);

	JMP_NEAR(0x0040F1D3, &NKD_BASE_GetSkillDamage_OL2_FixLevel);
	JMP_NEAR(0x00407765, &NKD_BASE_GetCurScore_FixLevel);
	JMP_NEAR(0x00462E99, &NKD_ApplyHp_FixLevel);
	JMP_NEAR(0x00462FD9, &NKD_ApplyMp_FixLevel);

	JMP_NEAR(0x00408178, &NKD_BASE_GetCur_FixNewMontsSpeed);

	JMP_NEAR(0x004365E0, &NKD_ProcessClientMessage_ApplyBonus, 4);

	//Class Info
	JMP_NEAR(0x409113, NKD_BASE_CanEquip_VerifClassInfo, 2);

	//Class Arch
	CALL_NEAR(0x409248, NKD_BASE_CanEquip_REQ_SIDC, 1);
	CALL_NEAR(0x409258, NKD_BASE_CanEquip_REQ_SIDC, 1);
	CALL_NEAR(0x409268, NKD_BASE_CanEquip_REQ_SIDC, 1);
	CALL_NEAR(0x409278, NKD_BASE_CanEquip_REQ_SIDC, 1);
	CALL_NEAR(0x409288, NKD_BASE_CanEquip_REQ_SIDC, 1);

	// Controle de Evasão
	JMP_NEAR(0x004278DF, &NKD_GetParryRate_FixEvasion, 1);
	JMP_NEAR(0x00454841, &NKD_GetParryRate_FixEvasion_2, 1);

	JMP_NEAR(0x004365AB, &NKD_FixApplyBonusLevel, 4);
	
	// Remove o limite de BonusExp
	JMP_NEAR(0x0045C2C0, (void*)0x0045C2D2);

	JMP_NEAR(0x004323F4, &NKD_NewTrade, 2);

	JMP_NEAR(0x0408EAB, NKD_StoreInNewCity_Fix);

	JMP_NEAR(0x00427BA9, &NKD_GetExpApply_01, 1);
	JMP_NEAR(0x00454C3E, &NKD_GetExpApply_02, 1);
	JMP_NEAR(0x0045C264, &NKD_GetExpApply_03);

	JMP_NEAR(0x00423E9B, &FixPossuido);
	JMP_NEAR(0x0042566F, NKD_Correct_Damage_HT, 2);

	//Constantes Fix
	SetDefaultConstants();

	JMP_NEAR(0x004282BF, (void*)0x004282D5);
	JMP_NEAR(0x0406FC5, NKD_FixSkillsGetCurrentScore, 1);
}


__declspec(naked) void NKD_FixSkillsGetCurrentScore()
{
	__asm
	{
		PUSH[EBP + 0xC]
		PUSH[EBP + 0x8]
		PUSH DWORD PTR SS : [EBP - 0x5C]
		CALL FixSkillsGetCurrentScore

		TEST AL, AL
		JNZ lblContinueLoop
		MOV EAX, DWORD PTR SS : [EBP - 0x5C]
		MOV ECX, DWORD PTR SS : [EBP + 0xC]

		PUSH 0x0406FCB
		RETN

	lblContinueLoop :
		PUSH 0x0406FAA
		RETN
	}
}

__declspec(naked) void NKD_FixSpectralVisual()
{
	static short effect;
	static int mobId;
	__asm
	{
		MOV ECX, DWORD PTR SS : [EBP - 0230h]
		MOV AX, WORD PTR DS : [ECX + 01Fh]
		MOV effect, AX

		MOV EAX, DWORD PTR SS : [EBP + 08h]
		MOV mobId, EAX
	}

	static st_Mob *mob = (st_Mob*)0x0015C69B8;

	if (mob[mobId].Learn & LEARN_29)
	{
		effect++; //teste
		__asm
		{
			MOV ECX, DWORD PTR SS : [EBP - 0230h]
			MOV AX, effect
			MOV WORD PTR DS : [ECX + 01Fh], AX
		}
	}

	__asm
	{
		MOV EAX, DWORD PTR SS : [EBP + 0x8]
		IMUL EAX, EAX, 0x410

		MOV jmp_addr, 0x042748E
		JMP jmp_addr
	}
}

void __stdcall DoRemovePossuido(int cId)
{
	if (cId < 0 || cId > 30000)
		return;

	for (int i = 0; i < MAX_AFFECTS; i++)
	{
		if (mBuffer[cId].Affects[i].Index == 24)
		{
			memset(&mBuffer[cId].Affects[i], 0, sizeof stAffects);

			GetCurrentScore(cId);
			SendAffect(cId);
			SendScore(cId);

			break;
		}
	}
}

__declspec(naked) void FixPossuido()
{
	static const UINT32 dwContinue = 0x00423EAC;
	__asm
	{
		CMP EDX, 0
		JNZ withoutBuff

		MOV EAX, DWORD PTR SS : [EBP + 8]
		PUSH EAX
		CALL DoRemovePossuido
		JMP dwContinue

	withoutBuff:
		JMP dwContinue
	}
}

__declspec(naked) void NKD_Correct_Damage_HT()
{
	static int dex;
	static DWORD dwContinue = 0x0425676;

	__asm
	{
		MOVSX ECX, WORD PTR DS:[EAX + 0x015C6A0C]
		MOV dex, ECX
	}

	dex /= 4;

	__asm
	{
		MOV EAX, dex

		JMP dwContinue
	}
}

__declspec(naked) void NKD_DoRemovePossuido()
{
	static DWORD dwFail = 0x00461D43,
		       dwSucess = 0x00461D07;
	__asm
	{
		CMP EAX, 0x1C
		JE Sucess
		CMP EAX, 0x18
		JE Sucess

		JMP dwFail
	Sucess:
		JMP dwSucess
	}
}

__declspec(naked) void NKD_GetExpApply_03()
{
	static DWORD dwContinue = 0x0045C29E;
	__asm
	{
		PUSH DWORD PTR SS : [EBP + 8]
		PUSH DWORD PTR SS : [EBP + 0xC]
		
		MOV EAX, DWORD PTR SS : [EBP + 8]
		IMUL EAX, EAX, 0x410
		MOV ECX, DWORD PTR DS : [EAX + 0x0015C69D4]
		PUSH ECX

		CALL HKD_GetExpApply
		JMP dwContinue

	}
}

__declspec(naked) void NKD_GetExpApply_02()
{
	static DWORD dwContinue = 0x00454C75;
	__asm
	{
		MOV EAX,DWORD PTR SS:[EBP - 0x290]
		AND EAX,07FFFh
		PUSH EAX
		
		PUSH DWORD PTR SS:[EBP - 0x1B8]
		PUSH DWORD PTR SS:[EBP - 0x2F8]
		CALL HKD_GetExpApply
		JMP dwContinue
	}
}

__declspec(naked) void NKD_GetExpApply_01()
{
	static DWORD dwContinue = 0x00427BDD;
	__asm
	{
		PUSH DWORD PTR SS:[EBP - 0x2CC]
		PUSH DWORD PTR SS:[EBP + 8]
		PUSH DWORD PTR SS:[EBP - 0x544]
		CALL HKD_GetExpApply
		JMP dwContinue
	}
}

__declspec(naked) void NKD_StoreInNewCity_Fix()
{
	int posX, posY;
	static DWORD dwContinue = 0x0408EB0;
	__asm
	{

		MOV EAX, DWORD PTR SS : [EBP+08h]
		MOV posX, EAX

		MOV EDX, DWORD PTR SS : [EBP+0Ch]
		MOV posY, EDX

	}

	if (posX >= 2640 && posY >= 1948
		&& posX <= 2671 && posY <= 2007)
		__asm { MOV EAX, 0} // 0 = ARMIA
	else
		__asm { MOV EAX, 5}

	__asm
	{
		JMP dwContinue
	}
}

__declspec(naked) void NKD_FixApplyBonusLevel()
{
	static const DWORD dwContinue = 0x04365C6;

	__asm
	{
		MOV EDX, DWORD PTR SS : [EBP + 08h]
		IMUL EDX, EDX, 0x410
		MOVSX EAX, WORD PTR DS : [EDX + 0x15C69DC]
		ADD EAX, 0x1
		IMUL EAX, EAX, 0x8
		CDQ
		SUB EAX, EDX
		SAR EAX, 1

		JMP dwContinue
	}
}

__declspec(naked) void NKD_GetParryRate_FixEvasion()
{
	static const DWORD mContinue = 0x004278E5;
	_asm
	{
		PUSH EAX
			PUSH DWORD PTR SS : [EBP + 08h]
			PUSH DWORD PTR SS : [EBP - 02CCh]

			CALL HKD_GetParryRate_FixEvasion

			MOV DWORD PTR SS : [EBP - 0528h], EAX

			JMP mContinue
	}
}

__declspec(naked) void NKD_GetParryRate_FixEvasion_2()
{
	static const DWORD mContinue = 0x00454847;
	_asm
	{
		PUSH EAX
			PUSH DWORD PTR SS : [EBP - 01B8h]
			PUSH DWORD PTR SS : [EBP - 0290h]

			CALL HKD_GetParryRate_FixEvasion

			MOV DWORD PTR SS : [EBP - 02D0h], EAX

			JMP mContinue
	}
}

void __stdcall NKD_CloseUser(INT32 ClientID){
	
	SaveAccount(ClientID);
	if (Trade[ClientID].isTrading)
	{
		CloseTrade(ClientID);
		CloseTrade(Trade[ClientID].ClientID);
	}

}

void __declspec(naked) HKD_CloseUser(){
	static const DWORD dwContinue = 0x0045946C;
	__asm{
		PUSH DWORD PTR SS : [EBP + 08h]
		CALL NKD_CloseUser
		MOV DWORD PTR SS : [EBP - 08h], 00h
		JMP dwContinue
	}
}

bool HKD_BASE_CanEquip(st_Mob *player)
{
	INT8 mCLASS = player->Equip[0].EFV2;

	bool retn = true;

	if (mCLASS <= MORTAL)
		retn = false;

	else if (mCLASS > SHARDCORE)
		retn = false;

	return retn;
}

__declspec(naked) void NKD_BASE_CanEquip_VerifClassInfo()
{
	__asm
	{
		MOV EAX, DWORD PTR SS : [EBP + 0xC]
		SUB EAX, 64
		PUSH EAX
		CALL HKD_BASE_CanEquip
		ADD ESP, 4

		CMP EAX, 0
		JNZ lbl_ContinueFunc

	lbl_ExitFunc :
		XOR EAX, EAX
		PUSH 0x409293
		RETN

	lbl_ContinueFunc :
		PUSH 0x40911A
		RETN
	}
}

__declspec(naked) void NKD_BASE_CanEquip_REQ_SIDC()
{
	__asm
	{
		JLE lbl_ContinueFunc

		MOV EAX, DWORD PTR SS : [EBP + 0xC]
		SUB EAX, 64
		PUSH EAX
		CALL HKD_BASE_CanEquip
		ADD ESP, 4

		CMP EAX, 0
		JNZ lbl_ContinueFunc

	lbl_ExitFunc :
		POP EAX
		XOR EAX, EAX
		PUSH 0x409293
		RETN

	lbl_ContinueFunc :
		RETN
	}
}

__declspec(naked) void NKD_ProcessClientMessage_ApplyBonus()
{
	__asm{
		PUSH DWORD PTR SS : [EBP + 08h]
		PUSH DWORD PTR SS : [EBP - 1EE4h]

		CALL HKD_ProcessClientMessage_ApplyBonus
		ADD ESP, 08h

		TEST EAX, EAX
		JE lbl_cantAdd

		MOV EAX, 0043661Ah
		JMP EAX

	lbl_cantAdd :
		MOV EAX, 00436604h
		JMP EAX
	}
}

__declspec(naked) void NKD_BASE_GetCur_FixNewMontsSpeed()
{
	static const INT32 dwContinue = 0x00408192;

	__asm
	{
		PUSH DWORD PTR SS:[EBP+0x8]
		PUSH DWORD PTR SS:[EBP-0x24]
		CALL GetNewSpeed
		MOV DWORD PTR SS:[EBP-0x24], EAX
		JMP dwContinue
	}
}

__declspec(naked) void NKD_ApplyMp_FixLevel()
{
	static const INT32 dwContinue = 0x00462FEC;

	__asm
	{
		PUSH DWORD PTR SS : [EBP + 0x8]
		CALL HKD_FixLevelBonusFromCid
		JMP dwContinue
	}
}

__declspec(naked) void NKD_ApplyHp_FixLevel()
{
	static const INT32 dwContinue = 0x00462EAC;

	__asm
	{
		PUSH DWORD PTR SS:[EBP + 0x8]
		CALL HKD_FixLevelBonusFromCid
		JMP dwContinue
	}
}

__declspec(naked) void NKD_BASE_GetCurScore_FixLevel()
{
	static const INT32 dwContinue = 0x0040776C;

	__asm
	{
		PUSH DWORD PTR SS:[EBP+8]
		CALL HKD_FixLevelBonus
		JMP dwContinue
	}
}

__declspec(naked) void NKD_BASE_GetSkillDamage_OL2_FixLevel()
{
	static const INT32 dwContinue = 0x0040F1FA;

	__asm
	{
		PUSH DWORD PTR SS:[EBP+0xC]
		CALL HKD_FixLevelBonus
		MOV DWORD PTR SS:[EBP-0xC], EAX
		JMP dwContinue
	}
}

__declspec(naked) void NKD_GetParryRate_JewelOfPrecision()
{
	static const DWORD dwContinue = 0x004278E5;
	_asm
	{
		PUSH EAX
		PUSH DWORD PTR SS:[EBP + 08h]

		CALL HKD_GetParryRate_JewelOfPrecision
		ADD ESP, 08h

		MOV DWORD PTR SS:[EBP - 0528h], EAX

		JMP dwContinue
	}
}

__declspec(naked) void NKD_GetParryRate_JewelOfPrecision_2()
{
	static const DWORD dwContinue = 0x00454847;
	_asm
	{
		PUSH EAX
		PUSH DWORD PTR SS:[EBP - 01B8h]

		CALL HKD_GetParryRate_JewelOfPrecision
		ADD ESP, 08h

		MOV DWORD PTR SS:[EBP - 02D0h], EAX

		JMP dwContinue
	}
}

__declspec(naked) void NKD_ControllBossDamage_04()
{
	static const INT32 dwContinue = 0x00454E1B;

	__asm
	{
		PUSH DWORD PTR SS : [EBP - 292h]
			PUSH DWORD PTR SS : [EBP - 290h]

			CALL ControllBossDamage

			CMP AX, 0h
			JE lbl_Continue

			MOV ECX, 0x00454E1D
			JMP ECX

		lbl_Continue :
		JMP dwContinue
	}
}

__declspec(naked) void NKD_ControllBossDamage_03()
{
	static const INT32 dwContinue = 0x0045194D;

	__asm
	{
		PUSH DWORD PTR SS : [EBP - 152h] // Damage
			PUSH DWORD PTR SS : [EBP - 18Ch] // mobId

			CALL ControllBossDamage

			CMP AX, 0h
			JE lbl_Continue

			MOV ECX, 0x0045194F
			JMP ECX

		lbl_Continue :
		JMP dwContinue
	}
}

__declspec(naked) void NKD_ControllBossDamage_02()
{
	static const INT32 dwContinue = 0x00427E4F;
	__asm
	{
		PUSH DWORD PTR SS : [EBP - 548h] // Damage
			PUSH DWORD PTR SS : [EBP - 2CCh] // mobId

			CALL ControllBossDamage

			CMP AX, 0h
			JE lbl_Continue

			MOV ECX, 0x00427E51
			JMP ECX

		lbl_Continue :
		JMP dwContinue
	}
}

__declspec(naked) void NKD_ControllBossDamage_01()
{
	static const INT32 dwContinue = 0x0425EB5;

	__asm
	{
		PUSH DWORD PTR SS : [EBP - 2D0h] // Damage
			PUSH DWORD PTR SS : [EBP - 2CCh] // mobId

			CALL ControllBossDamage

			CMP AX, 0h
			JE lbl_Continue

			MOV ECX, 0x00425EB7
			JMP ECX

		lbl_Continue :
		JMP dwContinue
	}
}

__declspec(naked) void NKD_GetAttack_CriticalHit()
{
	__asm
	{
		MOV DWORD PTR SS : [EBP - 050h], 00h // dano atual

			PUSH DWORD PTR SS : [EBP + 010h] // pacote
			PUSH DWORD PTR SS : [EBP + 08h]  // clientId de quem ataca

			CALL HKD_GetAttack_CriticalHit
			ADD ESP, 08h

			MOV DWORD PTR SS : [EBP - 050h], EAX

			MOV EAX, 0041D8EBh
			JMP EAX
	}
}

__declspec(naked) void NKD_GetCurScore()
{
	static const INT32 dwContinue = 0x0041286E;

	__asm
	{
		PUSH DWORD PTR SS : [EBP + 8]
		PUSH DWORD PTR SS:[EBP - 4] // Mob

		CALL HKD_GetCurrentScore
		JMP dwContinue
	}
}

__declspec(naked) void NKD_BASEGetSkillDamage()
{
	static const INT32 dwContinue = 0x00425B47;

	__asm
	{
		PUSH 2
		PUSH EAX
		PUSH DWORD PTR SS : [EBP - 0x2CC]
		PUSH DWORD PTR SS:[EBP + 0x8]

		CALL ControllDamage

		MOV DWORD PTR SS:[EBP - 0x2D0], EAX

		JMP dwContinue
	}
}

__declspec(naked) void NKD_BASEGetDamage_02()
{
	static const INT32 dwContinue = 0x004255FC;

	__asm
	{
		PUSH 1 // TYpe
		PUSH EAX
		PUSH DWORD PTR SS:[EBP - 0x2CC]
		PUSH DWORD PTR SS:[EBP + 8]

		CALL ControllDamage

		MOV DWORD PTR SS:[EBP - 0x2D0], EAX

		JMP dwContinue
	}
}

__declspec(naked) void NKD_BASEGetDamage_01()
{
	static const INT32 dwContinue = 0x0041D913;
	static const INT32 dwJL = 0x0041D945;

	__asm
	{
		PUSH 0 // Type
		PUSH EAX // Damage
		PUSH DWORD PTR SS:[EBP + 0xC] // Target
		PUSH DWORD PTR SS:[EBP + 8] // Attacker

		CALL ControllDamage

		MOV DWORD PTR SS:[EBP - 0x50], EAX

		CMP DWORD PTR SS:[EBP - 4], 0
		JL label_01

		JMP dwContinue

	label_01:
		JMP dwJL
	}
}

__declspec(naked) void NKD_ExpInPesaAllArea()
{
	static const DWORD dwFail = 0x0045C5B4;
	static const DWORD dwContinue = 0x0045C627;
	__asm
	{
		PUSH DWORD PTR SS : [EBP - 0208h] // Y
		PUSH DWORD PTR SS : [EBP - 0204h] // X
		PUSH DWORD PTR SS : [EBP - 020Ch] // otherClientId

		CALL HKD_ExpInPesaAllArea
		ADD ESP, 0Ch

		TEST EAX, EAX
		JE Fail

		JMP dwContinue

	Fail :
		MOV EAX, DWORD PTR SS : [EBP - 020Ch]
		JMP dwFail
	}
}

void __declspec(naked) NKD_FixTrainningCamp(){
	static const INT32 jmpFail = 0x00438D57;
	static const INT32 mContinue = 0x00423DDD;

	__asm{
		PUSH DWORD PTR SS : [EBP + 08h]
		CALL HKD_FixTrainningCamp

		CMP EAX, TRUE
		JE lbl_Continue

		JMP jmpFail

	lbl_Continue:
		JMP mContinue
	}
}

__declspec(naked) void NKD_MobKilled_ControlExp()
{
	static const UINT32 dwContinue = 0x0045C2A4;

	__asm
	{
		PUSH DWORD PTR SS : [EBP - 08h] // Mob
		PUSH DWORD PTR SS : [EBP - 0Ch] // Player
		PUSH EAX // exp

		CALL ControlExp

		MOV DWORD PTR SS : [EBP - 01F0h], EAX
		JMP dwContinue
	}
}

__declspec(naked) void NKD_BASE_GetCurrentScore_BlockStatus()
{
	static const INT32 mContinue = 0x0041286E;
	__asm
	{
		PUSH DWORD PTR SS : [EBP + 08h]
			CALL HKD_BASE_GetCurrentScore_BlockStatus
			JMP mContinue
	}
}

void __declspec(naked) NKD_GenerateSummon_CanSummon()
{
	static const DWORD dwCanSummon = 0x0044630E;
	static const DWORD dwContinue = 0x0044627C;
	_asm
	{
		PUSH DWORD PTR SS : [EBP - 0Ch]
		PUSH EAX
		CALL HKD_GenerateSummon_CanSummon

		ADD ESP, 08h

		TEST EAX, EAX
		JNZ lbl_canSummon

		JMP dwContinue
	lbl_canSummon :
		JMP dwCanSummon
	}
}

__declspec(naked) void NKD_ProcessClientMessage_UseMountFeed()
{
	__asm
	{
		PUSH EAX
		PUSH EDX
		PUSH ECX

		PUSH EAX
		MOV EAX, EBP
		SUB EAX, 0x1010
		PUSH EAX
		CALL HKD_ProcessClientMessage_UseMountFeed
		ADD ESP, 0x8

		POP ECX
		POP EDX
		POP EAX

		CDQ
		MOV ECX, 0x1E

		MOV jmp_addr, 0x42FA18
		JMP jmp_addr
	}
}

__declspec(naked) void NKD_ControlTeleportPosition()
{ //by ptr0x
	__asm
	{
		LEA EDX, DWORD PTR SS : [EBP - 0x810]
		PUSH EDX
		LEA EAX, DWORD PTR SS : [EBP - 0x80C]
		PUSH EAX
		PUSH DWORD PTR SS : [EBP + 0x8]
		CALL HKD_ControlTeleportPosition
		ADD ESP, 0xC

		CMP EAX, -1
		JNZ 0x42891B

		LEA EDX, DWORD PTR SS : [EBP - 0x810]
		MOV EAX, 0x042890B
		JMP EAX
	}
}

__declspec(naked) void NKD_CorrectPositionGoWorld()
{
	static const DWORD dwJNZ = 0x0044E05A;
	static const DWORD dwContinue = 0x0044E01D;
	_asm
	{
		LEA EDX, DWORD PTR SS : [EBP - 380h]
		PUSH EDX
		LEA EDX, DWORD PTR SS : [EBP - 37Ch]
		PUSH EDX
		PUSH DWORD PTR SS : [EBP - 0Ch]
		CALL HKD_CorrectPositionGoWorld

		ADD ESP, 0Ch

		MOV DWORD PTR SS : [EBP - 0394h], EAX
		CMP DWORD PTR SS : [EBP - 0394h], 0

		JNZ Sucess
		JMP dwContinue
	Sucess :
		JMP dwJNZ
	}
}

__declspec(naked) void NKD_AddMessage()
{
	__asm
	{
		POP reg_aux

			PUSH ECX

			PUSH reg_aux

			JMP HKD_AddMessage
	}
}

__declspec(naked) void NKD_ReadMessage()
{
	__asm
	{
		POP reg_aux

			PUSH ECX

			PUSH reg_aux

			JMP HKD_ReadMessage
	}
}

void __declspec(naked) NKD_NewTrade()
{
	static const int dwContinue = 0x00438D57;
	__asm
	{
		PUSH[EBP + 08h]
		PUSH[EBP + 0Ch]
		CALL NKD_SystemTrade
		JMP dwContinue
	}
}

__declspec(naked) void NKD_MobKilled_EventDead()
{
	static const INT32 dwRessucitar = 0x0045AC39;
	static const INT32 dwContinue = 0x0045ACD6;

	__asm
	{
		PUSH DWORD PTR SS : [EBP + 0xC] // Killer
		PUSH DWORD PTR SS : [EBP + 8] // Killed 

		CALL HKD_MobKilled_EventDead

		TEST EAX, EAX
		JNZ lbl_Continue
		JMP dwRessucitar

	lbl_Continue:
		JMP dwContinue
	}
}

__declspec(naked) void NKD_MobKilled_ControlDropItem()
{
	__asm
	{
		PUSH DWORD PTR SS : [EBP - 033Ch]
			PUSH DWORD PTR SS : [EBP + 0Ch]

			CALL HKD_MobKilled_ControlDropItem

			MOV jmp_addr, 0045DCF2h
			JMP jmp_addr
	}
}

__declspec(naked) void NKD_CorrectSleip()
{
	static const DWORD dwContinue = 0x044BA25;
	_asm {
		MOVSX EAX, WORD PTR DS : [EDX]
			CMP EAX, 954h
			JNZ dwCorrect
			SUB EAX, 939h
			JMP dwContinue
		dwCorrect :
		SUB EAX, 938h
			JMP dwContinue
	}
}

__declspec(naked) void NKD_ProcessDBMessage_LoadBuffs()
{
	__asm{
		PUSH DWORD PTR SS : [EBP - 0Ch]
			CALL HKD_ProcessDBMessage_LoadBuffs
			MOV EDX, DWORD PTR SS : [EBP - 0390h]
			RETN
	}
}

__declspec(naked) void NKD_CorrectRefs()
{
	static const DWORD dwContinue = 0x00404DD2;
	_asm
	{
		PUSH DWORD PTR SS : [EBP + 08h]
			CALL HKD_GetItemSanc_FixSanc
			JMP dwContinue
	}
}

__declspec(naked) void NKD_GetMobAbility_Spectral()
{
	static const INT32 dwContinue = 0x00405733;
	__asm
	{
		MOV ECX, DWORD PTR SS : [EBP + 08h]

			TEST DWORD PTR DS : [ECX + 02DCh], 20000000h
			JE lbl_NoIncrease

			INC EAX

		lbl_NoIncrease :

		JMP dwContinue
	}
}

__declspec(naked) void Fix_IntegerDivisionByZero()
{
	__asm
	{
		MOVSX ECX, WORD PTR DS : [EDX + 015C6A00h]
			CDQ

			TEST ECX, ECX
			JNZ Normal

			MOV ECX, 01h
		Normal:
		IDIV ECX

			MOV DWORD PTR SS : [EBP - 544h], EAX
			RETN
	}
}

__declspec(naked) void NKD_GetDoubleCritical_SetNewCriticalRate()
{
	__asm
	{
		CALL Rand
			CDQ

			MOV ECX, 255

			IDIV ECX
			MOV ECX, EDX
			RETN
	}
}

__declspec(naked) void NKD_GetGetCurrentScore_FixCriticalRate()
{
	__asm
	{
		CMP EAX, 00h
			JE lbl_ContinueNormalExec

			CDQ
			MOV ECX, 04h
			IDIV ECX

		lbl_ContinueNormalExec :
		MOV DWORD PTR SS : [EBP - 0Ch], EAX
			CMP DWORD PTR SS : [EBP - 0Ch], 0FFh
			RETN
	}
}

__declspec(naked) void NKD_GetCurrentScore_VerifyAbility_1()
{
	__asm
	{
		MOV EAX, DWORD PTR SS : [EBP - 4]
			SUB EAX, 0x15C69B8
			MOV ECX, 0x410
			XOR EDX, EDX
			DIV ECX

			MOV EDX, st_newmob_size
			IMUL EDX, EAX
			ADD EDX, st_newmob_addr

			PUSH EDX

			MOV jmp_addr, 0x41254D
			JMP jmp_addr
	}
}

__declspec(naked) void NKD_StandingByProcessor_VerifyAbility_1()
{
	__asm
	{
		MOV EAX, DWORD PTR SS : [EBP - 0x04]
			SUB EAX, 0x015C69B8
			CDQ
			MOV ECX, 0x410
			IDIV ECX

			MOV ECX, st_newmob_size
			IMUL EAX, ECX
			ADD EAX, st_newmob_addr
			MOVSX ECX, WORD PTR DS : [EAX]

			MOV jmp_addr, 0x0410F01
			JMP jmp_addr
	}
}

void __stdcall ZeroAffect(UINT16 ClientID)
{
	if (ClientID >= 1 && ClientID < MAX_PLAYER)
		for (UINT8 i = 0; i <= 15; i++)
			memset(&mBuffer[ClientID].Affects[i], 0, sizeof stAffects);
}

void __declspec(naked) CheckAffects()
{
	__asm{
		PUSH EBP
			MOV EBP, ESP
			MOV EDX, DWORD PTR SS : [EBP + 08h]
			CMP EDX, 00h
			JLE mERROR
			CMP EDX, 65h
			JGE mERROR
			LEA EAX, DWORD PTR DS : [mBuffer]
			IMUL EDX, EDX, 410h
			ADD EAX, EDX
			MOV ECX, 00h
		mLOOP :
			  CMP ECX, 10h
			  JGE mERROR
			  CMP WORD PTR DS : [ECX * 08h + EAX], 01Ch
			  JE mSUCESSFUL
			  INC ECX
			  JMP mLOOP
		  mERROR :
		XOR EAX, EAX
			JMP mRETURN
		mSUCESSFUL :
		MOV EAX, 01h
		mRETURN :
				MOV ESP, EBP
				POP EBP
				RETN
	}
}

void __declspec(naked) FixAffects(){
	static const UINT32 dwContinue = 0x0044D940;
	__asm{
		PUSH DWORD PTR SS : [EBP - 0Ch]
			CALL ZeroAffect
			JMP dwContinue
	}
}

void __declspec(naked) RemoveAffects(){
	static const UINT32 dwContinue = 0x004505E2;
	__asm{
		LEA EAX, DWORD PTR DS : [mBuffer]
			MOV EDX, DWORD PTR SS : [EBP + 08h]
			IMUL EDX, EDX, 410h
			MOV ECX, DWORD PTR SS : [EBP - 70h]
			IMUL ECX, ECX, 08h
			ADD ECX, EDX
			MOV DWORD PTR DS : [ECX + EAX], 00h
			MOV WORD PTR DS : [ECX + EAX + 04h], 00h
			PUSH DWORD PTR SS : [EBP + 08h]
			CALL SendAffect
			MOV DWORD PTR SS : [EBP - 04h], 01h
			JMP dwContinue
	}
}

void __declspec(naked) GetBuffer(){
	static const UINT32 dwContinue = 0x0041254D;
	__asm{
		MOV EAX, DWORD PTR SS : [EBP - 04h]
			SUB EAX, 015C69B8h
			LEA EDX, DWORD PTR DS : [mBuffer]
			ADD EAX, EDX
			PUSH EAX
			JMP dwContinue
	}
}

void __declspec(naked) AfterAffects(){
	static const UINT32 dwContinue = 0x00412586;
	__asm{
		MOV BYTE PTR DS : [EDX + 2E6h], AL
			MOV EAX, DWORD PTR SS : [EBP - 04h]
			SUB EAX, 015C69B8h
			MOV ECX, 410h
			XOR EDX, EDX
			IDIV ECX
			PUSH EAX
			CALL SendAffect
			PUSH DWORD PTR SS : [EBP + 08h]
			CALL SendAffect
			JMP dwContinue
	}
}

void __declspec(naked) NKD_ProcessRecv()
{
	static const UINT32 dwContinue = 0x004195CC;

	__asm
	{
		MOV EAX, DWORD PTR SS : [EBP - 28h]
			MOVSX EAX, WORD PTR DS : [EAX]

			PUSH 00h					// typeControl
			PUSH EAX					// packetSize
			PUSH DWORD PTR SS : [EBP - 28h] // pBuffer
			PUSH DWORD PTR SS : [EBP - 04h] // socketAddress
			CALL HKD_PacketControl
			ADD ESP, 10h

			MOV DWORD PTR SS : [EBP - 28h], EAX // move m_PacketBuffer[clientId][0]

			MOV EAX, DWORD PTR SS : [EBP - 30h]
			AND EAX, 0FFh

			JMP dwContinue
	}
}

void __declspec(naked) NKD_ProcessSend()
{
	static const UINT32 dwContinue = 0x004199BC;

	__asm
	{
		MOV BYTE PTR SS : [EBP - 10Ch], AL

			PUSH 01h					// typeControl
			PUSH DWORD PTR SS : [EBP + 0Ch] // packetSize
			PUSH DWORD PTR SS : [EBP + 08h] // pBuffer
			PUSH DWORD PTR SS : [EBP - 04h] // socketAddress
			CALL HKD_PacketControl
			ADD ESP, 10h

			MOV DWORD PTR SS : [EBP + 08h], EAX // move m_PacketBuffer[clientId][1]
			MOV DWORD PTR SS : [EBP - 108h], EAX // move m_PacketBuffer[clientId][1]

			JMP dwContinue
	}
}

__declspec(naked) void NKD_MobKilled_DropGold()
{
	__asm
	{
		PUSH EAX
		PUSH [EBP + 0xC]
		CALL HKD_MobKilled_DropGold
		ADD ESP, 8h
		MOV EAX, 0x045D646
		JMP EAX
	}
}

__declspec(naked) void NKD_CheckPacket_SendTradeSize()
{
	__asm
	{
		CMP EAX, 156
		JE Correct_Size

		RETN

Correct_Size:
		MOV jmp_addr, 0x40FFA1
		JMP jmp_addr
	}
}

__declspec(naked) void NKD_CheckPacket_GetCreateMobSize()
{
	__asm
	{
		CMP ECX, 176
		JE Correct_Size

		RETN

Correct_Size:
		MOV jmp_addr, 0x40FB77
		JMP jmp_addr
	}
}

__declspec(naked) void NKD_AddMessage_EncKey_1()
{
	__asm
	{
		SAR ECX, 0x3
		SUB EAX, ECX
		MOV EDX, DWORD PTR SS : [EBP - 0x4]

		MOV jmp_addr, 0x419B90
		JMP jmp_addr
	}
}

__declspec(naked) void NKD_ReadMessage_DecKey_1()
{
	__asm
	{
		SAR EDX, 0x3
		ADD ECX, EDX
		MOV EAX, DWORD PTR SS:[EBP - 0x28]
		
		MOV jmp_addr, 0x41955C
		JMP jmp_addr
	}
}

__declspec(naked) void NKD_ReadMessage_DecKey_2()
{
	__asm
	{
		SHL EAX, 0x2
		SUB EDX, EAX
		MOV ECX, DWORD PTR SS:[EBP - 0x28]

		MOV jmp_addr, 0x41957F
		JMP jmp_addr
	}
}

__declspec(naked) void NKD_GetCurrentScore_SancTransform()
{
	__asm {
		MOV EDX, DWORD PTR SS : [EBP + 08h]
			MOVSX EAX, BYTE PTR DS : [EDX + 061h]
			CMP EAX, 03h
			JGE lbl_noCalc
			MOV EDX, DWORD PTR SS : [EBP + 08h]
			MOVSX EAX, WORD PTR DS : [EDX + 040h]
			MOV ECX, 0040776Ch
			JMP ECX
		lbl_noCalc :
		MOV DWORD PTR SS : [EBP - 0ECh], 09h
			MOV EAX, 004077A3h
			JMP EAX
	}
}

__declspec(naked) void NKD_Base_GetCurrentScore_FixEdenStatus()
{
	static const INT32 mContinue = 0x00407765;
	static const INT32 foundEden = 0x0040770C;
	static const INT32 foundTita = 0x00407756;
	__asm{
		MOV EDX, DWORD PTR SS : [EBP + 08h]
		MOVSX EAX, WORD PTR DS : [EDX + 05Ch]

		CMP EAX, 19h
		JE lbl_Tita

		CMP EAX, 20h
		JE lbl_Eden

		JMP mContinue

	lbl_Tita :
		JMP foundTita

	lbl_Eden :
		CMP DWORD PTR SS : [EBP - 0E4h], 0
		JNZ lbl_FixStatus
		JMP mContinue

	lbl_FixStatus :
		JMP foundEden
	}
}

void __stdcall HKD_Base_GetCurrentScore_FixFaceArch(st_Mob &player)
{
	if ((player.Equip[0].Index >= 22 && player.Equip[0].Index <= 25) || player.Equip[0].Index == 32)
		player.Equip[0].Index = player.Equip[0].EFV3;
}

void __declspec(naked) NKD_Base_GetCurrentScore_FixFaceArch()
{
	static const INT32 mContinue = 0x00406F8C;
	__asm{
		PUSH DWORD PTR SS : [EBP + 08h]
			CALL HKD_Base_GetCurrentScore_FixFaceArch
			JMP mContinue
	}
}

void __stdcall HKD_Base_GetCurrentScore_FixTrans(st_Mob &player, INT32 transId)
{
	INT32 value = transId + 22;
	if (transId == 4)
		value = 32;

	INT32 clientid = GetUserFromMob((int)&player);

	if (!(player.Equip[0].Index >= 22 && player.Equip[0].Index <= 25 || player.Equip[0].Index == 32))
		player.Equip[0].EFV3 = player.Equip[0].Index;

	player.Equip[0].Index = value;

	if (player.Equip[0].Index == 32)
	{
		player.Equip[0].EF1 = 43;

		INT32 sanc = (unsigned char)(player.Status.Mastery[3] / 22);

		if (sanc > 9)
			sanc = 9;

		else if (player.Equip[0].EFV2 >= 3)
			sanc = 9;

		player.Equip[0].EFV1 = sanc;

		SendEquip(clientid);

		BYTE packet[108];
		BASE_GetCreateMob(clientid, (BYTE*)&packet);
		GridMulticast(clientid, player.TargetX, player.TargetY, (void*)&packet);
	}
}

void __declspec(naked) NKD_Base_GetCurrentScore_FixTrans()
{
	static const INT32 mContinue = 0x00407615;
	__asm
	{
		PUSH DWORD PTR SS : [EBP - 0C8h]
			PUSH DWORD PTR SS : [EBP + 08h]
			CALL HKD_Base_GetCurrentScore_FixTrans
			JMP mContinue
	}
}
