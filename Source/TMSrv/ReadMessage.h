#ifndef __READ_MESSAGE_H__
#define __READ_MESSAGE_H__

#include <Windows.h>
#include <stdio.h>

#include "PacketControl.h"

bool Recv(UINT32 recvFrom, UINT8* m_PacketBuffer, UINT8* pBuffer, UINT32 packetSize);

#endif // __READ_MESSAGE_H__