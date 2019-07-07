#ifndef __FIX_SEND_EXP_H__
#define __FIX_SEND_EXP_H__

#include "../Common/st_Base.h"
#include "DLLMain.h"

void FixSendExp_SERVER(BYTE *m_PacketBuffer, BYTE *packetBuffer);
int __stdcall ControlExp(DWORD exp, DWORD Killer, DWORD Killed);
INT32 HKD_ExpInPesaAllArea(UINT32 ClientID, INT32 X, INT32 Y);
int __stdcall HKD_GetExpApply(int Exp, int attackerId, int mobId);

#endif // __FIX_SEND_EXP_H__