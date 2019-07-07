#ifndef CONST_DEFINES_H
#define CONST_DEFINES_H

#define CLIVER 7556
#define GetUserFromMob(mob) (((int)mob - 0x15C69B8) / 0x410)

#define StartThread(thread) (CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)thread, NULL, 0, 0))
#define MAX_MAINREFINELEVEL 15
#define MAX_SKILLLIST 151

#define m_HP 0
#define m_MP 1

#define REPLATION_A_TYPE 1
#define REPLATION_B_TYPE 2
#define REPLATION_C_TYPE 3
#define REPLATION_D_TYPE 4
#define REPLATION_E_TYPE 5
#define REPLATION_ERROR  0

#define MOB_EMPTY       0
#define MOB_USERDOCK    1
#define MOB_USER        2
#define MOB_IDLE        3
#define MOB_PEACE		4
#define MOB_COMBAT      5
#define MOB_RETURN		6
#define MOB_FLEE		7
#define MOB_ROAM		8

#define	FACE 0
#define	HELMET 1
#define	ARMOR 2
#define	PANTS 3
#define	GLOVES 4
#define	BOOTS 5
#define	WEAPON 6
#define	SHIELD 7
#define	MOUNT 14
#define	CAPE 15

#define	MAX_EQUIP 16
#define	MAX_INVEN 60
#define	MAX_CARGO 128

#define LEARN_00 0x00000001
#define LEARN_01 0x00000002
#define LEARN_02 0x00000004
#define LEARN_03 0x00000008
#define LEARN_04 0x00000010
#define LEARN_05 0x00000020
#define LEARN_06 0x00000040
#define LEARN_07 0x00000080
#define LEARN_08 0x00000100
#define LEARN_09 0x00000200
#define LEARN_10 0x00000400
#define LEARN_11 0x00000800
#define LEARN_12 0x00001000
#define LEARN_13 0x00002000
#define LEARN_14 0x00004000
#define LEARN_15 0x00008000
#define LEARN_16 0x00010000
#define LEARN_17 0x00020000
#define LEARN_18 0x00040000
#define LEARN_19 0x00080000
#define LEARN_20 0x00100000
#define LEARN_21 0x00200000
#define LEARN_22 0x00400000
#define LEARN_23 0x00800000
#define LEARN_24 0x01000000
#define LEARN_25 0x02000000
#define LEARN_26 0x04000000
#define LEARN_27 0x08000000
#define LEARN_28 0x10000000
#define LEARN_29 0x20000000
#define LEARN_30 0x40000000

#endif