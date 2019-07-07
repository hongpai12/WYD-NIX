#include "Main.h"
#pragma comment(lib, "Rpcrt4.lib")
#include <Rpc.h>

DWORD ColorD1D;

void PacketControl(char *Data, int *Lenght, int Type)
{
	PacketHeader *p = (PacketHeader*)Data;

	bool delPacket = false;

	switch (Type)
	{
		case SEND_TYPE:
			delPacket = HKD_SendPacket(p);
			break;
		case RECV_TYPE:
			delPacket = HKD_ReadPacket(p);
			break;
	}
}

struct pD1D
{
	PacketHeader Header;
	char message[92];
	DWORD color;
};

bool HKD_ReadPacket(PacketHeader* packet)
{
	bool retn = false;

	DWORD color;

	switch (packet->PacketId)
	{
		case 0xD1D:
		{
			pD1D* p = (pD1D*)packet;

			if (p->color)
				ColorD1D = p->color;

			break;
		}


		case 0xAAA:
		{
			pAAA* p = (pAAA*)packet;

			memcpy(&Guild, &p->Guild, sizeof GuildInfo);

			break;
		}
	}

	return retn;
}

typedef struct 
{
	PacketHeader Header;

	char Login[16]; // 12 - 27
	char Password[12]; // 28 - 39

	INT32 Version; // 40 - 43
	INT32 Unknows; // 44- 47

	char Keys[16]; // 48 - 63
	char IPAddress[16]; // 64 - 79
} p_20D;

void GetMAC(char *string)
{
	unsigned char MACData[6];

	UUID uuid;
	UuidCreateSequential(&uuid);    // Ask OS to create UUID

	sprintf(string, "%02d%02d%02d%02d%02d%02d", uuid.Data4[2], uuid.Data4[3], uuid.Data4[4], uuid.Data4[5], uuid.Data4[6], uuid.Data4[7]);
}

bool HKD_SendPacket(PacketHeader* packet)
{
	bool retn = false;

	int packet20D = packet->PacketId + 500;

	if (packet20D == 1025)
	{
		p_20D *p = (p_20D*)packet;

		char MacAdress[17];
		GetMAC(MacAdress);

		strncpy(p->IPAddress, MacAdress, strlen(MacAdress));

		p->Unknows = 0xFAB;
	}

	return retn;
}