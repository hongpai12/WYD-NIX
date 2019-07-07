#ifndef __ADD_MESSAGE_H__
#define __ADD_MESSAGE_H__

#include <Windows.h>
#include <stdio.h>

#include "PacketControl.h"

bool pSend(UINT32 sendTo, UINT8* m_PacketBuffer, UINT8* pBuffer, UINT32 packetSize);

#endif // __ADD_MESSAGE_H__