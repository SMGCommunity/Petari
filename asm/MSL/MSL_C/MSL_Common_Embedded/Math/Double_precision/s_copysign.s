.text

.include "macros.inc"

.global copysign
copysign:
/* 8052688C 00521DCC  94 21 FF E0 */	stwu r1, -0x20(r1)
/* 80526890 00521DD0  D8 21 00 08 */	stfd f1, 8(r1)
/* 80526894 00521DD4  D8 41 00 10 */	stfd f2, 0x10(r1)
/* 80526898 00521DD8  80 61 00 08 */	lwz r3, 8(r1)
/* 8052689C 00521DDC  80 01 00 10 */	lwz r0, 0x10(r1)
/* 805268A0 00521DE0  54 00 00 00 */	rlwinm r0, r0, 0, 0, 0
/* 805268A4 00521DE4  50 60 00 7E */	rlwimi r0, r3, 0, 1, 0x1f
/* 805268A8 00521DE8  90 01 00 08 */	stw r0, 8(r1)
/* 805268AC 00521DEC  C8 21 00 08 */	lfd f1, 8(r1)
/* 805268B0 00521DF0  38 21 00 20 */	addi r1, r1, 0x20
/* 805268B4 00521DF4  4E 80 00 20 */	blr 
