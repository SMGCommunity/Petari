.text

.include "macros.inc"

.global TRK_main
TRK_main:
/* 8052BC94 005271D4  94 21 FF F0 */	stwu r1, -0x10(r1)
/* 8052BC98 005271D8  7C 08 02 A6 */	mflr r0
/* 8052BC9C 005271DC  90 01 00 14 */	stw r0, 0x14(r1)
/* 8052BCA0 005271E0  4B FF B7 49 */	bl TRKInitializeNub
/* 8052BCA4 005271E4  3C 80 80 6A */	lis r4, TRK_mainError@ha
/* 8052BCA8 005271E8  2C 03 00 00 */	cmpwi r3, 0
/* 8052BCAC 005271EC  90 64 C7 68 */	stw r3, TRK_mainError@l(r4)
/* 8052BCB0 005271F0  40 82 00 0C */	bne lbl_8052BCBC
/* 8052BCB4 005271F4  4B FF B6 E9 */	bl TRKNubWelcome
/* 8052BCB8 005271F8  4B FF B3 C5 */	bl TRKNubMainLoop
lbl_8052BCBC:
/* 8052BCBC 005271FC  4B FF B7 09 */	bl TRKTerminateNub
/* 8052BCC0 00527200  3C 80 80 6A */	lis r4, TRK_mainError@ha
/* 8052BCC4 00527204  90 64 C7 68 */	stw r3, TRK_mainError@l(r4)
/* 8052BCC8 00527208  80 01 00 14 */	lwz r0, 0x14(r1)
/* 8052BCCC 0052720C  7C 08 03 A6 */	mtlr r0
/* 8052BCD0 00527210  38 21 00 10 */	addi r1, r1, 0x10
/* 8052BCD4 00527214  4E 80 00 20 */	blr
