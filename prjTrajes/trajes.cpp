#include "Main.h"

int jmp_addr;
DWORD newMagicIncrement;

void SetValue(UINT32 addr, UINT32 value, size_t size)
{
	DWORD oldProt;
	VirtualProtect((void*)addr, size, PAGE_EXECUTE_READWRITE, &oldProt);
	switch (size)
	{
	case 1:
		*(BYTE*)addr = value;
		break;
	case 2:
		*(WORD*)addr = value;
		break;
	case 4:
		*(DWORD*)addr = value;
		break;
	case 8:
		*(double*)addr = value;
		break;
	default:
		MessageBoxA(NULL, "Size Incorreta: Set value", "Size Incorreta : Set value", MB_OK);
		break;
	}

	VirtualProtect((void*)addr, size, oldProt, &oldProt);
}

void Initialize()
{
	/*JNZ_NEAR(0x04DEB03, HKD_AddIndex);
	JMP_NEAR(0x04DFD06, NKD_CorrectBone, 1);
	JNZ_NEAR(0x04CC2B7, NKD_LoadFile);

	JMP_NEAR(send_call, Send);
	JGE_NEAR(recv_call, Recv);

	JMP_NEAR(0x0040CD6F, NKD_ShowRefine);
	JMP_NEAR(0x0041DCA7, NKD_ItemDescription);

	JMP_NEAR(0x004A50A6, NKD_ChangeD1DColor);*/

	JMP_NEAR(0x0048B2D5, NKD_ChangeCliver_01);
	JMP_NEAR(0x004B8D39, NKD_ChangeCliver_02);

	JMP_NEAR(0x00424C42, NKD_FormatGoldString_01);
	JMP_NEAR(0x0041C15C, NKD_FormatGoldString_02);
	JMP_NEAR(0x004211A6, NKD_FormatGoldString_03);

	////SetValue(0x00476BDC, 0xEB, 1);
	//JMP_NEAR(0x00476BB1, (void*)0x00476BC4);
	//JMP_NEAR(0x0476B99, NKD_FixAddPoint_1, 2);

	////SetValue(0x00476D39, 0xEB, 1);
	//JMP_NEAR(0x00476CE3, (void*)0x00476CF0);
	//JMP_NEAR(0x0476CC5, NKD_FixAddPoint_2, 2);

	////SetValue(0x00476E96, 0xEB, 1);
	//JMP_NEAR(0x00476E40, (void*)0x00476E4D);
	//JMP_NEAR(0x0476E22, NKD_FixAddPoint_3, 2);

	////SetValue(0x00476FF3, 0xEB, 1);
	//JMP_NEAR(0x00476F9D, (void*)0x00476FAA);
	//JMP_NEAR(0x0476F7F, NKD_FixAddPoint_4, 2);
	//JMP_NEAR(0x044C194, NKD_FixAddPoint_CorrectVis, 1);

	//JMP_NEAR(0x04E3D8B, NKD_ChangeTabColor);

	//JMP_NEAR(0x050D331, NKD_CorrectPet, 4);
	//JMP_NEAR(0x050E8F3, NKD_CorrectPet_2, 4);
	//JMP_NEAR(0x050D34B, NKD_CorrectPet_3, 4);
	//JMP_NEAR(0x050E90A, NKD_CorrectPet_4, 4);

	//CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)LoadPage, NULL, 0, 0);
}

__declspec(naked) void NKD_CorrectPet()
{
	__asm
	{
		MOV WORD PTR DS : [EDX + 12Eh], 0h

			MOV ECX, DWORD PTR SS : [EBP - 084h]
			MOV BYTE PTR DS : [ECX + 0146h], 50
			MOV EDX, DWORD PTR SS : [EBP - 084h]
			MOV BYTE PTR DS : [EDX + 0147h], 50
			MOV EAX, DWORD PTR SS : [EBP - 084h]
			MOV BYTE PTR DS : [EAX + 0148h], 20



			MOV EAX, 0x050D3E9
			JMP EAX
	}
}

__declspec(naked) void NKD_CorrectPet_2()
{
	__asm
	{
		MOV WORD PTR DS : [EAX + 12Eh], 0h

			MOV EDX, DWORD PTR SS : [EBP - 028h]
			MOV BYTE PTR DS : [EDX + 0146h], 50
			MOV EAX, DWORD PTR SS : [EBP - 028h]
			MOV BYTE PTR DS : [EAX + 0147h], 50
			MOV ECX, DWORD PTR SS : [EBP - 028h]
			MOV BYTE PTR DS : [ECX + 0148h], 20



			MOV EAX, 0x050E98A
			JMP EAX
	}
}

__declspec(naked) void NKD_CorrectPet_3()
{
	__asm
	{
		MOV WORD PTR DS : [EAX + 12Eh], 0h

			MOV ECX, DWORD PTR SS : [EBP - 084h]
			MOV BYTE PTR DS : [ECX + 0146h], 50
			MOV EDX, [EBP - 084h]
			MOV BYTE PTR DS : [EDX + 147], 50
			MOV EAX, [EBP - 084h]
			MOV BYTE PTR DS : [EAX + 148], 20

			MOV EAX, 0x050D397
			JMP EAX
	}
}

__declspec(naked) void NKD_CorrectPet_4()
{
	__asm
	{
		MOV WORD PTR DS : [EAX + 12Eh], 0h

			MOV EDX, [EBP - 028h]
			MOV BYTE PTR DS : [EDX + 0146h], 50
			MOV EAX, [EBP - 028h]
			MOV BYTE PTR DS : [EAX + 0147h], 50
			MOV ECX, [EBP - 028h]
			MOV BYTE PTR DS : [ECX + 0148h], 20



			MOV EAX, 0x050E947
			JMP EAX
	}
}

int HKD_ChangeTabColor(char* msg)
{
	int color = 0xFFCCCCCC;

	if (!strcmp(msg, "Matador Aprendiz"))
		color = 0xFF00BFFF;

	else if (!strcmp(msg, "Matador Profissional"))
		color = 0xFFD28989;

	else if (!strcmp(msg, "Serial Killer"))
		color = 0xFFFF0000;

	else if (!strcmp(msg, "GRUPO FOGO"))
		color = 0xFFFF0000;

	else if (!strcmp(msg, "GRUPO GELO"))
		color = 0xFF000FFF;

	return color;
}

__declspec(naked) void NKD_ChangeTabColor()
{
	__asm
	{
		MOV EDX, DWORD PTR SS : [EBP - 0x1B4]
			ADD EDX, 0x100
			PUSH EDX

			CALL HKD_ChangeTabColor

			ADD ESP, 04h

			PUSH EAX

			MOV EDX, 0x4E3D90

			JMP EDX
	}
}

__declspec(naked) void NKD_FixAddPoint_CorrectVis()
{
	static DWORD dwContinue = 0x044C1A9;
	__asm
	{
		MOV EAX, DWORD PTR SS : [EBP - 023Ch]
			MOVSX EAX, WORD PTR DS : [EAX + 040h]
			ADD EAX, 1
			IMUL EAX, EAX, 8
			CDQ
			SUB EAX, EDX
			SAR EAX, 1

			JMP dwContinue
	}
}

__declspec(naked) void NKD_FixAddPoint_1()
{
	static DWORD dwContinue = 0x0476BAB;
	__asm
	{
		MOV EAX, DWORD PTR SS : [EBP - 018h]
			MOVSX EAX, WORD PTR DS : [EAX + 040h]
			ADD EAX, 1
			IMUL EAX, EAX, 8
			CDQ
			SUB EAX, EDX
			SAR EAX, 1

			JMP dwContinue
	}
}

__declspec(naked) void NKD_FixAddPoint_2()
{
	static DWORD dwContinue = 0x0476CD7;
	__asm
	{
		MOV EAX, DWORD PTR SS : [EBP - 018h]
			MOVSX EAX, WORD PTR DS : [EAX + 040h]
			ADD EAX, 1
			IMUL EAX, EAX, 8
			CDQ
			SUB EAX, EDX
			SAR EAX, 1

			JMP dwContinue
	}
}

__declspec(naked) void NKD_FixAddPoint_3()
{
	static DWORD dwContinue = 0x0476E34;
	__asm
	{
		MOV EAX, DWORD PTR SS : [EBP - 018h]
			MOVSX EAX, WORD PTR DS : [EAX + 040h]
			ADD EAX, 1
			IMUL EAX, EAX, 8
			CDQ
			SUB EAX, EDX
			SAR EAX, 1

			JMP dwContinue
	}
}

__declspec(naked) void NKD_FixAddPoint_4()
{
	static DWORD dwContinue = 0x0476F91;
	__asm
	{
		MOV EAX, DWORD PTR SS : [EBP - 018h]
			MOVSX EAX, WORD PTR DS : [EAX + 040h]
			ADD EAX, 1
			IMUL EAX, EAX, 8
			CDQ
			SUB EAX, EDX
			SAR EAX, 1

			JMP dwContinue
	}
}


void __stdcall FormatGoldString(char *string, int Gold)
{
	char st[32];
	char result[32];
	memset(result, 0, sizeof result);

	_itoa(Gold, st, 10);

	int len = strlen(st);
	int sum = ((len - 1) / 3);

	for (int i = (len - 1), count = 0, index = (len - 1) + sum; i >= 0; i--, count++)
	{
		if (!(count % 3) && count != 0)
		{
			result[index] = ',';
			index--;
		}

		result[index] = st[i];

		count++;
		index--;
	}

	sprintf(string, "%s Gold", result);
}

__declspec(naked) void NKD_FormatGoldString_03()
{
	static const UINT32 dwContinue = 0x004211BB;

	__asm
	{
		PUSH EDX
			LEA EAX, DWORD PTR SS : [EBP - 1110h]
			PUSH EAX

			CALL FormatGoldString

			JMP dwContinue
	}
}

__declspec(naked) void NKD_FormatGoldString_02()
{
	static const UINT32 dwContinue = 0x0041C171;

	__asm
	{
		PUSH EDX
			LEA EAX, DWORD PTR SS : [EBP - 320h]
			PUSH EAX

			CALL FormatGoldString

			JMP dwContinue
	}
}

__declspec(naked) void NKD_FormatGoldString_01()
{
	static const UINT32 dwContinue = 0x00424C6B;

	__asm
	{
		PUSH ECX
			LEA EDX, DWORD PTR SS : [EBP - 84h]
			PUSH EDX

			CALL FormatGoldString

			JMP dwContinue
	}
}

__declspec(naked) void NKD_ChangeCliver_02()
{
	static const UINT32 dwContinue = 0x004B8D43;

	__asm
	{
		MOV EAX, CLIVER
			MOV DWORD PTR SS : [EBP - 0xA00], EAX
			JMP dwContinue
	}
}

__declspec(naked) void NKD_ChangeCliver_01()
{
	static const UINT32 dwContinue = 0x0048B2DF;

	__asm
	{
		MOV EAX, CLIVER
			MOV DWORD PTR SS : [EBP - 0x2CC], EAX
			JMP dwContinue
	}
}

__declspec(naked) void NKD_ChangeD1DColor()
{
	static const UINT32 dwContinue = 0x004A50B0;

	__asm
	{
		MOV EAX, ColorD1D
			MOV DWORD PTR SS : [EBP - 0x434], EAX
			JMP dwContinue
	}
}

#define f_ptr (__cdecl *)
static int(*BASE_GetItemAmount)(st_Item*) = (int f_ptr(st_Item*)) 0x537721; // Lib: Server.obj Class : (null)

int __stdcall FindRefine(st_Item *Item, char* str)
{
	// O valor de retorno desta função, caso acima de 0 será impresso junto ao item
	// Mostrando refinação, nível de montaria, ou qualquer outro valor q desejarmos

	int Amount = BASE_GetItemAmount(Item);
	if (Amount > 0)
	{
		if (Item->Index == 5155 || Item->Index == 5156)
			sprintf(str, "%dD", Amount);
		else if (Amount > 1)
			sprintf(str, "%d", Amount);

		return TRUE;
	}
	else if (Item->Index >= 2360 && Item->Index <= 2389 && Item->EF2 > 0)
	{
		sprintf(str, "%d", Item->EF2);

		return TRUE;
	}

	int retn = 0;

	for (int i = 0; i < 3; i++)
		if (Item->Effect[i].Index == 43 || (Item->Effect[i].Index >= 116 && Item->Effect[i].Index <= 125))
		{
			retn = Item->Effect[i].Value;
			break;
		}

	if (retn)
	{
		if (retn < 230)
			retn = retn % 10;
		else
			retn = retn - 220;
		if (retn >= 10 && retn <= 35)
		{
			retn = (retn - 10) / 4 + 10;
		}

		sprintf(str, "+%d", retn);
	}

	return retn;
}

__declspec(naked) void NKD_ShowRefine()
{
	static const UINT32 dwShowNumber = 0x0040CD8F;
	static const UINT32 dwJMPFail = 0x0040CDB2;

	__asm
	{
		// Ponteiro para o item e chamada da função
		MOV EAX, DWORD PTR SS : [EBP - 0x20]
			ADD EAX, 0xAC
			PUSH EAX
			MOV ECX, DWORD PTR SS : [EBP + 0xC]
			PUSH ECX
			CALL FindRefine

			// Retorno igual a zero desconsidera a função e permite a execução normal
			CMP EAX, 0
			JE lbl_Fail

			JMP dwShowNumber

		lbl_Fail :
		JMP dwJMPFail
	}
}

__declspec(naked) void Send()
{
	__asm
	{
		PUSH SEND_TYPE
			LEA ECX, DWORD PTR SS : [EBP + 0x0C]
			PUSH ECX
			PUSH DWORD PTR SS : [EBP + 0x08]
			CALL PacketControl
			ADD ESP, 0x0C

			MOV DWORD PTR SS : [EBP - 0x10C], 0x04

			MOV EAX, send_jmp
			JMP EAX
	}
}

__declspec(naked) void Recv()
{
	__asm
	{
		MOV EAX, DWORD PTR SS : [EBP - 0x08]
			AND EAX, 0xFFFF
			MOV DWORD PTR SS : [EBP - 0x08], EAX

			PUSH RECV_TYPE
			LEA ECX, DWORD PTR SS : [EBP - 0x08]
			PUSH ECX
			PUSH DWORD PTR SS : [EBP - 0x18]
			CALL PacketControl
			ADD ESP, 0x0C

			MOV EAX, recv_jmp
			JMP EAX
	}
}

__declspec(naked) void NKD_CorrectBone()
{
	__asm
	{
		MOV EDX, DWORD PTR SS : [EBP - 0358h]
			LEA EDX, DWORD PTR SS : [EDX + 05Ch]
			PUSH EDX

			MOVSX EAX, WORD PTR SS : [EBP - 018h]
			PUSH EAX

			CALL CorrectBone
			ADD ESP, 08h

			MOV EDX, DWORD PTR SS : [EBP - 0358h]
			PUSH 0x4DFD0C
			RETN
	}
}

__declspec(naked) void NKD_LoadFile()
{
	__asm
	{
		PUSH ECX
			PUSH EBP

			CALL LoadFile
			ADD ESP, 08h

			PUSH 0x4CC40D
			RETN
	}
}

__declspec(naked) void HKD_AddIndex()
{
	__asm
	{
		LEA ECX, DWORD PTR SS : [EBP - 018h]
			PUSH ECX;
		PUSH EAX;

		CALL AddIndex
			ADD ESP, 8

			CMP AL, 1

			JNZ lbl_Jnz

			PUSH 0x4DEB0F
			RETN

		lbl_Jnz :
		PUSH 0x4DEBC3
			RETN

	}
}
/*
Função dos Trajes.
Créditos originais: KF4T4.
Adaptação dos addresses para o client versão 7.556
e adição de mais trajes: ~AgateOwnz
*/
enum Costumes
{
	Valquiria = 1,
	Esqueleto,
	Romano,
	Kalintz_M,
	Kalintz_F,
	Draco,
	Bruxa,
	RedSanta,
	WhiteSanta,
	Purple_Rudolph = 0xD,
	Blue_Rudolph,
	White_Police,
	Black_Police,
	Blindness,
	Deafness,
	Hanbok,
	Rabbit_Girl,
	ST_Set,
	Kingdom_Red,
	Kingdom_Blue,
	Gandi,
	Odin,
	Rabbit,
	Dancing_Girl,
	Feudal,
	Oriental,
	Plant,
	Succubus,
	Oriental_F,
	Elegant,
	Dosa,
	Feudal_F,
};

void CorrectBone(INT32 id, INT32* bone)
{

	switch (id)
	{
		//Coloque um id que quiser, caso bugue com 0x1, coloque ele no 0x0.
	case Valquiria:
	case Bruxa:
	case Romano:
	case Kalintz_F:
	case Purple_Rudolph:
	case Blue_Rudolph:
	case White_Police:
	case Black_Police:
	case Deafness:
	case Rabbit_Girl:
	case Rabbit:
	case Dancing_Girl:
	case Oriental:
	case Plant:
	case Succubus:
	case Oriental_F:
	case Elegant:
	case Hanbok:
	case Feudal_F:
	case RedSanta:
		*bone = 0x1;
		break;

	case Draco:
	case Esqueleto:
	case Kalintz_M:
	case Feudal:
	case Kingdom_Blue:
	case Kingdom_Red:
	case Dosa:
	case Blindness:
	case ST_Set:
	case Gandi:
	case Odin:
		*bone = 0x0;
		break;

	default:
		break;
	}

}

void AddIndex(int id, int *value)
{
	__asm
	{
		MOV AL, 1
	}
	switch (id)
	{
		// Id do traje, e o value é o mesmo do correct bone
	case 4188: *value = RedSanta;
		break;
	case 4161:
	case 4187: *value = Purple_Rudolph;
		break;

	case 4162:  *value = Blue_Rudolph;
		break;

	case 4163: *value = White_Police;
		break;

	case 4164: *value = Black_Police;
		break;

	case 4165: *value = Blindness;
		break;

	case 4166: *value = Deafness;
		break;

	case 4167: *value = Hanbok;
		break;

	case 4168: *value = Rabbit_Girl;
		break;

	case 4169:
	case 4170: *value = ST_Set;
		break;

	case 4171: *value = Kingdom_Red;
		break;
	case 4172: *value = Kingdom_Blue;
		break;

	case 4173:
	case 4186: *value = Gandi;
		break;

	case 4174:
	case 4185: *value = Odin;
		break;

	case 4175: *value = Dancing_Girl;
		break;

	case 4176: *value = Rabbit;
		break;

	case 4177: *value = Oriental;
		break;

	case 4178: *value = Feudal;
		break;

	case 4179: *value = Feudal_F;
		break;

	case 4183: *value = Elegant;
		break;

	case 4180: *value = Plant;
		break;

	case 4181: *value = Succubus;
		break;

	case 4182: *value = Oriental_F;
		break;

	case 4184:
	case 4150: *value = Dosa;
		break;

	case 4159: *value = RedSanta;
		break;

	default:
		__asm{	XOR AL, AL	};
		break;
	}

}

void LoadFile(INT32 ebp, INT32 id)// 004CC2B4  |. 83F9 07        |CMP ECX,7
{
	switch (id)
	{
		//id do addIndex e do correct bone.
		//pegue o meshtexturelist do over e cole no seu client
		//a itemlist tbm, converta e taque no server, e coloque a .bin no client que quiser.
	case Purple_Rudolph:
		LoadCostume(ebp, "mesh\\PurpleRudol.wyt", "mesh\\ch020194.msh", "mesh\\ch020294.msh", "mesh\\ch020394.msh", "mesh\\ch020494.msh", "mesh\\ch020594.msh", "mesh\\ch020694.msh");
		break;
	case Blue_Rudolph:
		LoadCostume(ebp, "mesh\\BlueRudol.wyt", "mesh\\ch020194.msh", "mesh\\ch020294.msh", "mesh\\ch020394.msh", "mesh\\ch020494.msh", "mesh\\ch020594.msh", "mesh\\ch020694.msh");
		break;
	case White_Police:
		LoadCostume(ebp, "mesh\\WhitePolice.wyt", "mesh\\ch020195.msh", "mesh\\ch020295.msh", "mesh\\ch020395.msh", "mesh\\ch020495.msh", "mesh\\ch020595.msh", "mesh\\ch020695.msh");
		break;
	case Black_Police:
		LoadCostume(ebp, "mesh\\BlackPolice.wyt", "mesh\\ch020195.msh", "mesh\\ch020295.msh", "mesh\\ch020395.msh", "mesh\\ch020495.msh", "mesh\\ch020595.msh", "mesh\\ch020695.msh");
		break;
	case Blindness:
		LoadCostume(ebp, "mesh\\DeathCos2.wyt", "mesh\\ch010189.msh", "mesh\\ch010289.msh", "mesh\\ch010389.msh", "mesh\\ch010489.msh", "mesh\\ch010589.msh", "mesh\\ch010689.msh");
		break;
	case Deafness:
		LoadCostume(ebp, "mesh\\DeathCos.wyt", "mesh\\ch020189.msh", "mesh\\ch020289.msh", "mesh\\ch020389.msh", "mesh\\ch020489.msh", "mesh\\ch020589.msh", "mesh\\ch020689.msh");
		break;
	case Dosa:
		LoadCostume(ebp, "mesh\\ch010199.wyt", "mesh\\ch010199.msh", "mesh\\ch010299.msh", "mesh\\ch010399.msh", "mesh\\ch010499.msh", "mesh\\ch010599.msh", "mesh\\ch010699.msh");
		break;
	case Hanbok:
		LoadCostume(ebp, "mesh\\ch0101100.wys", "mesh\\ch0101100.msh", "mesh\\ch0102100.msh", "mesh\\ch0103100.msh", "mesh\\ch0104100.msh", "mesh\\ch0105100.msh", "mesh\\ch0106100.msh");
		break;
	case Rabbit_Girl:
		LoadCostume(ebp, "mesh\\ch0101101.wys", "mesh\\ch0101101.msh", "mesh\\ch0102101.msh", "mesh\\ch0103101.msh", "mesh\\ch0104101.msh", "mesh\\ch0105101.msh", "mesh\\ch0106101.msh");
		break;
	case ST_Set:
		LoadCostume(ebp, "mesh\\ch0101102.wyt", "mesh\\ch0101102.msh", "mesh\\ch0102102.msh", "mesh\\ch0103102.msh", "mesh\\ch0104102.msh", "mesh\\ch0105102.msh", "mesh\\ch0106102.msh");
		break;
	case Kingdom_Red:
		LoadCostume(ebp, "mesh\\ch0101103.wyt", "mesh\\ch0101103.msh", "mesh\\ch0102103.msh", "mesh\\ch0103103.msh", "mesh\\ch0104103.msh", "mesh\\ch0105103.msh", "mesh\\ch0105103.msh");
		break;
	case Kingdom_Blue:
		LoadCostume(ebp, "mesh\\ch0102103.wyt", "mesh\\ch0101103.msh", "mesh\\ch0102103.msh", "mesh\\ch0103103.msh", "mesh\\ch0104103.msh", "mesh\\ch0105103.msh", "mesh\\ch0105103.msh");
		break;
	case Gandi:
		LoadCostume(ebp, "mesh\\ch0101104.wyt", "mesh\\ch0101104.msh", "mesh\\ch0102104.msh", "mesh\\ch0103104.msh", "mesh\\ch0104104.msh", "mesh\\ch0105104.msh", "mesh\\ch0106104.msh");
		break;
	case Odin:
		LoadCostume(ebp, "mesh\\ch0101106.wyt", "mesh\\ch0101106.msh", "mesh\\ch0102106.msh", "mesh\\ch0103106.msh", "mesh\\ch0104106.msh", "mesh\\ch0105106.msh", "mesh\\ch0106106.msh");
		break;
	case Rabbit:
		LoadCostume(ebp, "mesh\\ch0101107.wyt", "mesh\\ch0101107.msh", "mesh\\ch0102107.msh", "mesh\\ch0103107.msh", "mesh\\ch0104107.msh", "mesh\\ch0105107.msh", "mesh\\ch0106107.msh");
		break;
	case Oriental:
		LoadCostume(ebp, "mesh\\ch0101108.wyt", "mesh\\ch0101108.msh", "mesh\\ch0102108.msh", "mesh\\ch0103108.msh", "mesh\\ch0104108.msh", "mesh\\ch0105108.msh", "mesh\\ch0106108.msh");
		break;
	case Feudal:
		LoadCostume(ebp, "mesh\\ch0101109.wyt", "mesh\\ch0101109.msh", "mesh\\ch0102109.msh", "mesh\\ch0103109.msh", "mesh\\ch0104109.msh", "mesh\\ch0105109.msh", "mesh\\ch0106109.msh");
		break;
	case Dancing_Girl:
		LoadCostume(ebp, "mesh\\ch0101105.wyt", "mesh\\ch0101105.msh", "mesh\\ch0102105.msh", "mesh\\ch0103105.msh", "mesh\\ch0104105.msh", "mesh\\ch0105105.msh", "mesh\\ch0106105.msh");
		break;
	case Feudal_F:
		LoadCostume(ebp, "mesh\\ch0101110.wyt", "mesh\\ch0101110.msh", "mesh\\ch0102110.msh", "mesh\\ch0103110.msh", "mesh\\ch0104110.msh", "mesh\\ch0105110.msh", "mesh\\ch0106110.msh");
		break;
	case Plant:
		LoadCostume(ebp, "mesh\\ch0101111.wyt", "mesh\\ch0101111.msh", "mesh\\ch0102111.msh", "mesh\\ch0103111.msh", "mesh\\ch0104111.msh", "mesh\\ch0105111.msh", "mesh\\ch0106111.msh");
		break;
	case Succubus:
		LoadCostume(ebp, "mesh\\ch0101112.wyt", "mesh\\ch0101112.msh", "mesh\\ch0102112.msh", "mesh\\ch0103112.msh", "mesh\\ch0104112.msh", "mesh\\ch0105112.msh", "mesh\\ch0106112.msh");
		break;
	case Oriental_F:
		LoadCostume(ebp, "mesh\\ch0101113.wyt", "mesh\\ch0101113.msh", "mesh\\ch0102113.msh", "mesh\\ch0103113.msh", "mesh\\ch0104113.msh", "mesh\\ch0105113.msh", "mesh\\ch0106113.msh");
		break;
	case Elegant:
		LoadCostume(ebp, "mesh\\ch0101114.wyt", "mesh\\ch0101114.msh", "mesh\\ch0102114.msh", "mesh\\ch0103114.msh", "mesh\\ch0104114.msh", "mesh\\ch0105114.msh", "mesh\\ch0106114.msh");
		break;
	case RedSanta:
		LoadCostume(ebp, "mesh\\RedSanta.wys", "mesh\\ch020170.msh", "mesh\\ch020270.msh", "mesh\\ch020370.msh", "mesh\\ch020470.msh", "mesh\\ch020570.msh", "mesh\\ch020670.msh");
		break;
	default:
		break;
	}
}

void LoadCostume(INT32 ebp, char* texture, char* msh1, char* msh2, char* msh3, char* msh4, char* msh5, char* msh6)
{
	int* contador = (int*)0x05EDE98;

	char *currentMesh = (char*)(ebp - 0x30);
	char *currentTexture = (char*)(ebp - 0x80);

	strcpy(currentTexture, texture);
	switch (*contador)
	{
	case 1:
		strcpy(currentMesh, msh1);
		*contador = 2;
		break;
	case 2:
		strcpy(currentMesh, msh2);
		*contador = 3;
		break;
	case 3:
		strcpy(currentMesh, msh3);
		*contador = 4;
		break;
	case 4:
		strcpy(currentMesh, msh4);
		*contador = 5;
		break;
	case 5:
		strcpy(currentMesh, msh5);
		*contador = 6;
		break;
	case 6:
		strcpy(currentMesh, msh6);
		*contador = 1;
		break;
	default:

		break;
	}

}

