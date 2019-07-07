#include "Main.h"

INT32 ColorDesc = 0xFFFFFFFF;

__inline void AddLine()
{
	__asm
	{
		MOV EAX, DWORD PTR SS : [EBP - 010h]
			MOV ECX, DWORD PTR SS : [EBP - 018h]
			MOV DWORD PTR DS : [EAX + 027680h], ECX

			PUSH 0
			LEA ECX, DWORD PTR SS : [EBP - 0A8h]
			PUSH ECX
			MOV EDX, DWORD PTR SS : [EBP - 028h]
			MOV EAX, DWORD PTR SS : [EBP - 010h]
			MOV ECX, DWORD PTR DS : [EAX + EDX * 04h + 0278C0h]
			MOV EDX, DWORD PTR SS : [EBP - 028h]
			MOV EAX, DWORD PTR SS : [EBP - 010h]
			MOV EDX, DWORD PTR DS : [EAX + EDX * 04h + 0278C0h]
			MOV EAX, DWORD PTR DS : [EDX]
			CALL DWORD PTR DS : [EAX + 080h]

			PUSH ColorDesc

			MOV ECX, DWORD PTR SS : [EBP - 028h]
			MOV EDX, DWORD PTR SS : [EBP - 010h]
			MOV ECX, DWORD PTR DS : [EDX + ECX * 04h + 0278C0h]
			MOV EAX, DWORD PTR SS : [EBP - 028h]
			MOV EDX, DWORD PTR SS : [EBP - 010h]
			MOV EAX, DWORD PTR DS : [EDX + EAX * 04h + 0278C0h]
			MOV EDX, DWORD PTR DS : [EAX]
			CALL DWORD PTR DS : [EDX + 084h]

			MOV EAX, DWORD PTR SS : [EBP - 028h]
			ADD EAX, 01h
			MOV DWORD PTR SS : [EBP - 028h], EAX
	}
}

/*bool HKD_ItemPrice_FormatDecimal(char *gold)
{
        int *pointer;
        int value;
        int a;
        __asm
        {
                MOV pointer,ECX
 
                MOV EAX,DWORD PTR SS:[pointer]
                MOV ECX,DWORD PTR DS:[EAX + 058Ch]
                MOV DWORD PTR SS:[value],ECX
        }
 
        char st[32];
        char result[32];
        memset(result, 0, sizeof result);
 
        itoa(value, st, 10);
 
        int len = strlen(st);
        int sum = ((len - 1) / 3);
       
        for(int i = (len - 1), count = 0, index = (len - 1) + sum; i >= 0; i--, count++)
        {
                if(!(count % 3) && count != 0)
                {
                        result[index] = ',';
                        index--;
                }
 
                result[index] = st[i];
 
                count++;
                index--;
        }
 
        sprintf(gold, "%s Gold", result);
        return true;
}*/

__declspec(naked) void NKD_ItemDescription()
{
	static char *szName;
	static st_Item *item;
	__asm
	{
		MOV ECX, DWORD PTR SS : [EBP - 08h]
		MOV EDX, DWORD PTR DS : [ECX + 0670h]

		MOVSX EAX, WORD PTR DS : [EDX]

		CMP EAX, 0CFCh
		JE lbl_continuePesa

		LEA EDX, DWORD PTR SS : [EBP - 0A8h]
		MOV szName, EDX

		MOV ECX, DWORD PTR SS : [EBP - 08h]
		MOV EDX, DWORD PTR DS : [ECX + 0670h]

		MOV DWORD PTR DS : [item], EDX
	}

	ColorDesc = 0xFFFFFFFF;

	if (item->Index == 5155)
	{
		ColorDesc = 0xFFFF0000;
		sprintf(szName, "Item para a ativação do Membro Fogo.");
		AddLine();

		ColorDesc = 0xFFFFFF00;
		sprintf(szName, "Período (Dias): %d", item->EFV1);
		AddLine();

		ColorDesc = 0xFFFFFFFF;
		sprintf(szName, "Membros Fogo recebem as seguintes vantagens:");
		AddLine();

		sprintf(szName, "Bônus de Experiência: 4%%");
		AddLine();

		sprintf(szName, "Bônus de Drop: 4%%");
		AddLine();

		sprintf(szName, "Bônus em composições: 4%%");
		AddLine();

		sprintf(szName, "Bônus no gold dropado: 50%%");
		AddLine();

		sprintf(szName, "Filtro de drop de até 3 itens");
		AddLine();

		sprintf(szName, "Gritos gratuitos por minuto: 5");
		AddLine();

		sprintf(szName, "Acesso a loja de fogo exclusiva");
		AddLine();

		ColorDesc = 0xFFFFFF00;
		sprintf(szName, "Seu efeito é cumulativo", item->EFV1);
		AddLine();
	}
	else if (item->Index == 508 || item->Index == 509 || (item->Index >= 526 && item->Index <= 528))
	{
		sprintf(szName, "Nome: %s", Guild.GuildName);
		AddLine();

		sprintf(szName, "Lider: %s", Guild.LiderName);
		AddLine();

		sprintf(szName, "Numero de membros: %d", Guild.Members);
		AddLine();

		if (Guild.SubLiderName[0][0] != '\0')
		{
			sprintf(szName, "SubLider I: %s", Guild.SubLiderName[0]);
			AddLine();
		}

		if (Guild.SubLiderName[1][0] != '\0')
		{
			sprintf(szName, "SubLider II: %s", Guild.SubLiderName[1]);
			AddLine();
		}

		if (Guild.SubLiderName[2][0] != '\0')
		{
			sprintf(szName, "SubLider III: %s", Guild.SubLiderName[2]);
			AddLine();
		}

		if (item->Index != 508)
		{
			sprintf(szName, "GuildID: %d", Guild.GuildID);
			AddLine();

			sprintf(szName, "Pontuação: %d", Guild.Points);
			AddLine();
		}
	}
	else if (item->Index == 5156)
	{
		ColorDesc = 0xFF00FFFF;
		sprintf(szName, "Item para a ativação do Membro Gelo.");
		AddLine();

		ColorDesc = 0xFFFFFF00;
		sprintf(szName, "Período (Dias): %d", item->EFV1);
		AddLine();

		ColorDesc = 0xFFFFFFFF;
		sprintf(szName, "Membros Gelo recebem as seguintes vantagens:");
		AddLine();

		sprintf(szName, "Bônus de Experiência: 8%%");
		AddLine();

		sprintf(szName, "Bônus de Drop: 8%%");
		AddLine();

		sprintf(szName, "Bônus em composições: 8%%");
		AddLine();

		sprintf(szName, "Bônus no gold dropado: 100%%");
		AddLine();

		sprintf(szName, "Filtro de drop de até 7 itens");
		AddLine();

		sprintf(szName, "Gritos gratuitos por minuto: 10");
		AddLine();

		sprintf(szName, "Acesso a loja de gelo exclusiva");
		AddLine();

		ColorDesc = 0xFFFFFF00;
		sprintf(szName, "Seu efeito é cumulativo", item->EFV1);
		AddLine();
	}
	else if (item->Index >= 0x938 && item->Index <= 0x956)
	{
		if (item->EF2 <= 50)
			sprintf(szName, "[Normal]");
		else if (item->EF2 < 120)
		{
			ColorDesc = 0xFFA020F0;
			sprintf(szName, "[Místico]");
		}
		else
		{
			ColorDesc = 0xFFFFD700;
			sprintf(szName, "[Arcano]");
		}

		AddLine();
	}
	else
	{
		for (int i = 0; i < 3; i++)
		{
			if (item->Effect[i].Index > 115 && item->Effect[i].Index < 126)
			{
				ColorDesc = 0xFFFFFF00;

				if (item->Effect[i].Index == 116)
					sprintf(szName, "Adicional de 2%% de resistências");
				else if (item->Effect[i].Index == 117)
					sprintf(szName, "Aumento de 1%% da regeneração de HP");
				else if (item->Effect[i].Index == 118)
					sprintf(szName, "Aumento de 2%% no drop"); // Feito
				else if (item->Effect[i].Index == 119)
					sprintf(szName, "Aumento de 2%% do HP total"); // Feito
				else if (item->Effect[i].Index == 120)
					sprintf(szName, "Aumento de 2%% no dano PvM"); // Feito
				else if (item->Effect[i].Index == 121)
					sprintf(szName, "Índice de 2%% de reflexão de dano recebido");
				else if (item->Effect[i].Index == 122)
					sprintf(szName, "Índice de 2%% de absorção de dano infligido"); // Feito
				else if (item->Effect[i].Index == 123)
					sprintf(szName, "Ganho de 100 de exp a cada 20 monstros mortos"); // Feito
				else if (item->Effect[i].Index == 124)
					sprintf(szName, "Aumento de 1%% da regeneração de MP");
				else
					sprintf(szName, "Aumento de 2%% do MP total"); // Feito

				AddLine();

				break;
			}
		}

		__asm JMP lbl_nextCMP
	}

	__asm
	{
	lbl_nextCMP:
		MOV EAX, 0041DCBEh
			JMP EAX

	lbl_continuePesa :
		MOV EAX, 0041DD47h
			JMP EAX
	}
}
