.include "macros.inc"

.text

.global func_80195A6C
func_80195A6C:
/* 80195A6C 00190FAC  94 21 FF F0 */	stwu r1, -0x10(r1)
/* 80195A70 00190FB0  7C 08 02 A6 */	mflr r0
/* 80195A74 00190FB4  90 01 00 14 */	stw r0, 0x14(r1)
/* 80195A78 00190FB8  93 E1 00 0C */	stw r31, 0xc(r1)
/* 80195A7C 00190FBC  7C 7F 1B 78 */	mr r31, r3
/* 80195A80 00190FC0  48 06 06 B9 */	bl func_801F6138
/* 80195A84 00190FC4  3C 80 80 57 */	lis r4, lbl_8057740C@ha
/* 80195A88 00190FC8  7F E3 FB 78 */	mr r3, r31
/* 80195A8C 00190FCC  38 84 74 0C */	addi r4, r4, lbl_8057740C@l
/* 80195A90 00190FD0  90 9F 00 00 */	stw r4, 0(r31)
/* 80195A94 00190FD4  83 E1 00 0C */	lwz r31, 0xc(r1)
/* 80195A98 00190FD8  80 01 00 14 */	lwz r0, 0x14(r1)
/* 80195A9C 00190FDC  7C 08 03 A6 */	mtlr r0
/* 80195AA0 00190FE0  38 21 00 10 */	addi r1, r1, 0x10
/* 80195AA4 00190FE4  4E 80 00 20 */	blr 
/* 80195AA8 00190FE8  48 25 97 7C */	b func_803EF224
/* 80195AAC 00190FEC  94 21 FF F0 */	stwu r1, -0x10(r1)
/* 80195AB0 00190FF0  7C 08 02 A6 */	mflr r0
/* 80195AB4 00190FF4  2C 03 00 00 */	cmpwi r3, 0
/* 80195AB8 00190FF8  90 01 00 14 */	stw r0, 0x14(r1)
/* 80195ABC 00190FFC  93 E1 00 0C */	stw r31, 0xc(r1)
/* 80195AC0 00191000  7C 9F 23 78 */	mr r31, r4
/* 80195AC4 00191004  93 C1 00 08 */	stw r30, 8(r1)
/* 80195AC8 00191008  7C 7E 1B 78 */	mr r30, r3
/* 80195ACC 0019100C  41 82 00 1C */	beq lbl_80195AE8
/* 80195AD0 00191010  38 80 00 00 */	li r4, 0
/* 80195AD4 00191014  4B FE C0 99 */	bl func_80181B6C
/* 80195AD8 00191018  2C 1F 00 00 */	cmpwi r31, 0
/* 80195ADC 0019101C  40 81 00 0C */	ble lbl_80195AE8
/* 80195AE0 00191020  7F C3 F3 78 */	mr r3, r30
/* 80195AE4 00191024  48 27 40 5D */	bl __dl__FPv
lbl_80195AE8:
/* 80195AE8 00191028  7F C3 F3 78 */	mr r3, r30
/* 80195AEC 0019102C  83 E1 00 0C */	lwz r31, 0xc(r1)
/* 80195AF0 00191030  83 C1 00 08 */	lwz r30, 8(r1)
/* 80195AF4 00191034  80 01 00 14 */	lwz r0, 0x14(r1)
/* 80195AF8 00191038  7C 08 03 A6 */	mtlr r0
/* 80195AFC 0019103C  38 21 00 10 */	addi r1, r1, 0x10
/* 80195B00 00191040  4E 80 00 20 */	blr 
/* 80195B04 00191044  94 21 FE 60 */	stwu r1, -0x1a0(r1)
/* 80195B08 00191048  7C 08 02 A6 */	mflr r0
/* 80195B0C 0019104C  90 01 01 A4 */	stw r0, 0x1a4(r1)
/* 80195B10 00191050  93 E1 01 9C */	stw r31, 0x19c(r1)
/* 80195B14 00191054  7C 9F 23 78 */	mr r31, r4
/* 80195B18 00191058  93 C1 01 98 */	stw r30, 0x198(r1)
/* 80195B1C 0019105C  7C 7E 1B 78 */	mr r30, r3
/* 80195B20 00191060  48 06 07 39 */	bl func_801F6258
/* 80195B24 00191064  38 61 01 08 */	addi r3, r1, 0x108
/* 80195B28 00191068  48 06 22 D1 */	bl func_801F7DF8
/* 80195B2C 0019106C  3C 80 80 57 */	lis r4, lbl_805773E8@ha
/* 80195B30 00191070  38 61 01 08 */	addi r3, r1, 0x108
/* 80195B34 00191074  38 84 73 E8 */	addi r4, r4, lbl_805773E8@l
/* 80195B38 00191078  48 06 23 A9 */	bl func_801F7EE0
/* 80195B3C 0019107C  38 61 01 08 */	addi r3, r1, 0x108
/* 80195B40 00191080  48 06 23 A9 */	bl func_801F7EE8
/* 80195B44 00191084  38 61 01 08 */	addi r3, r1, 0x108
/* 80195B48 00191088  48 06 23 B5 */	bl func_801F7EFC
/* 80195B4C 0019108C  38 61 01 08 */	addi r3, r1, 0x108
/* 80195B50 00191090  38 80 00 00 */	li r4, 0
/* 80195B54 00191094  48 06 23 C9 */	bl func_801F7F1C
/* 80195B58 00191098  C0 22 C7 18 */	lfs f1, lbl_806A7998-_SDA2_BASE_(r2)
/* 80195B5C 0019109C  38 61 01 08 */	addi r3, r1, 0x108
/* 80195B60 001910A0  48 06 24 51 */	bl func_801F7FB0
/* 80195B64 001910A4  3C A0 80 57 */	lis r5, lbl_80577405@ha
/* 80195B68 001910A8  80 DE 00 8C */	lwz r6, 0x8c(r30)
/* 80195B6C 001910AC  38 61 00 08 */	addi r3, r1, 8
/* 80195B70 001910B0  38 80 01 00 */	li r4, 0x100
/* 80195B74 001910B4  38 A5 74 05 */	addi r5, r5, lbl_80577405@l
/* 80195B78 001910B8  4C C6 31 82 */	crclr 6
/* 80195B7C 001910BC  48 38 73 C1 */	bl snprintf
/* 80195B80 001910C0  38 61 01 08 */	addi r3, r1, 0x108
/* 80195B84 001910C4  38 81 00 08 */	addi r4, r1, 8
/* 80195B88 001910C8  48 06 23 6D */	bl func_801F7EF4
/* 80195B8C 001910CC  7F C3 F3 78 */	mr r3, r30
/* 80195B90 001910D0  7F E4 FB 78 */	mr r4, r31
/* 80195B94 001910D4  38 A1 01 08 */	addi r5, r1, 0x108
/* 80195B98 001910D8  48 06 08 59 */	bl func_801F63F0
/* 80195B9C 001910DC  80 01 01 A4 */	lwz r0, 0x1a4(r1)
/* 80195BA0 001910E0  83 E1 01 9C */	lwz r31, 0x19c(r1)
/* 80195BA4 001910E4  83 C1 01 98 */	lwz r30, 0x198(r1)
/* 80195BA8 001910E8  7C 08 03 A6 */	mtlr r0
/* 80195BAC 001910EC  38 21 01 A0 */	addi r1, r1, 0x1a0
/* 80195BB0 001910F0  4E 80 00 20 */	blr 