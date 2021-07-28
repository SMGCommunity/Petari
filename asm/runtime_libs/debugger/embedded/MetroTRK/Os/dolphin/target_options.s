.text

.include "macros.inc"

.global GetUseSerialIO
GetUseSerialIO:
/* 8052C1CC 0052770C  3C 60 80 6A */	lis r3, bUseSerialIO@ha
/* 8052C1D0 00527710  38 63 C7 78 */	addi r3, r3, bUseSerialIO@l
/* 8052C1D4 00527714  88 63 00 00 */	lbz r3, 0(r3)
/* 8052C1D8 00527718  4E 80 00 20 */	blr 

.global SetUseSerialIO
SetUseSerialIO:
/* 8052C1DC 0052771C  3C 80 80 6A */	lis r4, bUseSerialIO@ha
/* 8052C1E0 00527720  98 64 C7 78 */	stb r3, bUseSerialIO@l(r4)
/* 8052C1E4 00527724  4E 80 00 20 */	blr 
