.text

.include "macros.inc"

.global TRKTargetContinue
TRKTargetContinue:
/* 8052C198 005276D8  94 21 FF F0 */	stwu r1, -0x10(r1)
/* 8052C19C 005276DC  7C 08 02 A6 */	mflr r0
/* 8052C1A0 005276E0  38 60 00 00 */	li r3, 0
/* 8052C1A4 005276E4  90 01 00 14 */	stw r0, 0x14(r1)
/* 8052C1A8 005276E8  4B FF DC B1 */	bl TRKTargetSetStopped
/* 8052C1AC 005276EC  4B FF FC 41 */	bl UnreserveEXI2Port
/* 8052C1B0 005276F0  4B FF DB 21 */	bl TRKSwapAndGo
/* 8052C1B4 005276F4  4B FF FC 69 */	bl ReserveEXI2Port
/* 8052C1B8 005276F8  80 01 00 14 */	lwz r0, 0x14(r1)
/* 8052C1BC 005276FC  38 60 00 00 */	li r3, 0
/* 8052C1C0 00527700  7C 08 03 A6 */	mtlr r0
/* 8052C1C4 00527704  38 21 00 10 */	addi r1, r1, 0x10
/* 8052C1C8 00527708  4E 80 00 20 */	blr
