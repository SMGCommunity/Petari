.text

.include "macros.inc"

.global MWExitCriticalSection
MWExitCriticalSection:
/* 8052C758 00527C98  94 21 FF F0 */	stwu r1, -0x10(r1)
/* 8052C75C 00527C9C  7C 08 02 A6 */	mflr r0
/* 8052C760 00527CA0  90 01 00 14 */	stw r0, 0x14(r1)
/* 8052C764 00527CA4  80 63 00 00 */	lwz r3, 0(r3)
/* 8052C768 00527CA8  4B F7 B9 A9 */	bl OSRestoreInterrupts
/* 8052C76C 00527CAC  80 01 00 14 */	lwz r0, 0x14(r1)
/* 8052C770 00527CB0  7C 08 03 A6 */	mtlr r0
/* 8052C774 00527CB4  38 21 00 10 */	addi r1, r1, 0x10
/* 8052C778 00527CB8  4E 80 00 20 */	blr 

.global MWEnterCriticalSection
MWEnterCriticalSection:
/* 8052C77C 00527CBC  94 21 FF F0 */	stwu r1, -0x10(r1)
/* 8052C780 00527CC0  7C 08 02 A6 */	mflr r0
/* 8052C784 00527CC4  90 01 00 14 */	stw r0, 0x14(r1)
/* 8052C788 00527CC8  93 E1 00 0C */	stw r31, 0xc(r1)
/* 8052C78C 00527CCC  7C 7F 1B 78 */	mr r31, r3
/* 8052C790 00527CD0  4B F7 B9 59 */	bl OSDisableInterrupts
/* 8052C794 00527CD4  90 7F 00 00 */	stw r3, 0(r31)
/* 8052C798 00527CD8  80 01 00 14 */	lwz r0, 0x14(r1)
/* 8052C79C 00527CDC  83 E1 00 0C */	lwz r31, 0xc(r1)
/* 8052C7A0 00527CE0  7C 08 03 A6 */	mtlr r0
/* 8052C7A4 00527CE4  38 21 00 10 */	addi r1, r1, 0x10
/* 8052C7A8 00527CE8  4E 80 00 20 */	blr 

.global MWInitializeCriticalSection
MWInitializeCriticalSection:
/* 8052C7AC 00527CEC  4E 80 00 20 */	blr
