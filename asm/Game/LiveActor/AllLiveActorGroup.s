.include "macros.inc"

.text
.global func_8015CCD8
func_8015CCD8:
/* 8015CCD8 00158218  94 21 FF F0 */	stwu r1, -0x10(r1)
/* 8015CCDC 0015821C  7C 08 02 A6 */	mflr r0
/* 8015CCE0 00158220  3C 80 80 57 */	lis r4, lbl_80573268@ha
/* 8015CCE4 00158224  38 A0 0A 00 */	li r5, 0xa00
/* 8015CCE8 00158228  90 01 00 14 */	stw r0, 0x14(r1)
/* 8015CCEC 0015822C  38 84 32 68 */	addi r4, r4, lbl_80573268@l
/* 8015CCF0 00158230  93 E1 00 0C */	stw r31, 0xc(r1)
/* 8015CCF4 00158234  7C 7F 1B 78 */	mr r31, r3
/* 8015CCF8 00158238  48 00 93 D9 */	bl func_801660D0
/* 8015CCFC 0015823C  3C 80 80 57 */	lis r4, lbl_8057327C@ha
/* 8015CD00 00158240  38 00 00 00 */	li r0, 0
/* 8015CD04 00158244  38 84 32 7C */	addi r4, r4, lbl_8057327C@l
/* 8015CD08 00158248  90 1F 00 18 */	stw r0, 0x18(r31)
/* 8015CD0C 0015824C  7F E3 FB 78 */	mr r3, r31
/* 8015CD10 00158250  90 9F 00 00 */	stw r4, 0(r31)
/* 8015CD14 00158254  83 E1 00 0C */	lwz r31, 0xc(r1)
/* 8015CD18 00158258  80 01 00 14 */	lwz r0, 0x14(r1)
/* 8015CD1C 0015825C  7C 08 03 A6 */	mtlr r0
/* 8015CD20 00158260  38 21 00 10 */	addi r1, r1, 0x10
/* 8015CD24 00158264  4E 80 00 20 */	blr 

.global func_8015CD28
func_8015CD28:
/* 8015CD28 00158268  94 21 FF E0 */	stwu r1, -0x20(r1)
/* 8015CD2C 0015826C  7C 08 02 A6 */	mflr r0
/* 8015CD30 00158270  90 01 00 24 */	stw r0, 0x24(r1)
/* 8015CD34 00158274  39 61 00 20 */	addi r11, r1, 0x20
/* 8015CD38 00158278  48 3B A7 FD */	bl func_80517534
/* 8015CD3C 0015827C  80 03 00 10 */	lwz r0, 0x10(r3)
/* 8015CD40 00158280  7C 7C 1B 78 */	mr r28, r3
/* 8015CD44 00158284  7C 9D 23 78 */	mr r29, r4
/* 8015CD48 00158288  2C 00 00 00 */	cmpwi r0, 0
/* 8015CD4C 0015828C  40 81 00 54 */	ble lbl_8015CDA0
/* 8015CD50 00158290  83 E3 00 18 */	lwz r31, 0x18(r3)
/* 8015CD54 00158294  7C 1F 00 00 */	cmpw r31, r0
/* 8015CD58 00158298  40 82 00 3C */	bne lbl_8015CD94
/* 8015CD5C 0015829C  48 00 00 44 */	b lbl_8015CDA0
/* 8015CD60 001582A0  48 00 00 34 */	b lbl_8015CD94
lbl_8015CD64:
/* 8015CD64 001582A4  80 9C 00 18 */	lwz r4, 0x18(r28)
/* 8015CD68 001582A8  7F 83 E3 78 */	mr r3, r28
/* 8015CD6C 001582AC  48 00 93 A5 */	bl func_80166110
/* 8015CD70 001582B0  7C 7E 1B 78 */	mr r30, r3
/* 8015CD74 001582B4  48 00 27 45 */	bl func_8015F4B8
/* 8015CD78 001582B8  7F C4 F3 78 */	mr r4, r30
/* 8015CD7C 001582BC  7F A5 EB 78 */	mr r5, r29
/* 8015CD80 001582C0  48 00 26 A9 */	bl func_8015F428
/* 8015CD84 001582C4  80 7C 00 18 */	lwz r3, 0x18(r28)
/* 8015CD88 001582C8  3B FF 00 01 */	addi r31, r31, 1
/* 8015CD8C 001582CC  38 03 00 01 */	addi r0, r3, 1
/* 8015CD90 001582D0  90 1C 00 18 */	stw r0, 0x18(r28)
lbl_8015CD94:
/* 8015CD94 001582D4  80 1C 00 10 */	lwz r0, 0x10(r28)
/* 8015CD98 001582D8  7C 1F 00 00 */	cmpw r31, r0
/* 8015CD9C 001582DC  41 80 FF C8 */	blt lbl_8015CD64
lbl_8015CDA0:
/* 8015CDA0 001582E0  39 61 00 20 */	addi r11, r1, 0x20
/* 8015CDA4 001582E4  48 3B A7 DD */	bl func_80517580
/* 8015CDA8 001582E8  80 01 00 24 */	lwz r0, 0x24(r1)
/* 8015CDAC 001582EC  7C 08 03 A6 */	mtlr r0
/* 8015CDB0 001582F0  38 21 00 20 */	addi r1, r1, 0x20
/* 8015CDB4 001582F4  4E 80 00 20 */	blr 

.global func_8015CDB8
func_8015CDB8:
/* 8015CDB8 001582F8  94 21 FF F0 */	stwu r1, -0x10(r1)
/* 8015CDBC 001582FC  7C 08 02 A6 */	mflr r0
/* 8015CDC0 00158300  90 01 00 14 */	stw r0, 0x14(r1)
/* 8015CDC4 00158304  48 1E 7C E5 */	bl func_80344AA8
/* 8015CDC8 00158308  38 80 00 30 */	li r4, 0x30
/* 8015CDCC 0015830C  48 1E 7C 89 */	bl func_80344A54
/* 8015CDD0 00158310  80 01 00 14 */	lwz r0, 0x14(r1)
/* 8015CDD4 00158314  7C 08 03 A6 */	mtlr r0
/* 8015CDD8 00158318  38 21 00 10 */	addi r1, r1, 0x10
/* 8015CDDC 0015831C  4E 80 00 20 */	blr 

.global func_8015CDE0
func_8015CDE0:
/* 8015CDE0 00158320  94 21 FF F0 */	stwu r1, -0x10(r1)
/* 8015CDE4 00158324  7C 08 02 A6 */	mflr r0
/* 8015CDE8 00158328  90 01 00 14 */	stw r0, 0x14(r1)
/* 8015CDEC 0015832C  48 1E 7C BD */	bl func_80344AA8
/* 8015CDF0 00158330  38 80 00 30 */	li r4, 0x30
/* 8015CDF4 00158334  48 1E 7C 61 */	bl func_80344A54
/* 8015CDF8 00158338  80 03 00 10 */	lwz r0, 0x10(r3)
/* 8015CDFC 0015833C  90 03 00 18 */	stw r0, 0x18(r3)
/* 8015CE00 00158340  80 01 00 14 */	lwz r0, 0x14(r1)
/* 8015CE04 00158344  7C 08 03 A6 */	mtlr r0
/* 8015CE08 00158348  38 21 00 10 */	addi r1, r1, 0x10
/* 8015CE0C 0015834C  4E 80 00 20 */	blr 

.global func_8015CE10
func_8015CE10:
/* 8015CE10 00158350  94 21 FF F0 */	stwu r1, -0x10(r1)
/* 8015CE14 00158354  7C 08 02 A6 */	mflr r0
/* 8015CE18 00158358  90 01 00 14 */	stw r0, 0x14(r1)
/* 8015CE1C 0015835C  93 E1 00 0C */	stw r31, 0xc(r1)
/* 8015CE20 00158360  7C 7F 1B 78 */	mr r31, r3
/* 8015CE24 00158364  48 1E 7C 85 */	bl func_80344AA8
/* 8015CE28 00158368  38 80 00 30 */	li r4, 0x30
/* 8015CE2C 0015836C  48 1E 7C 29 */	bl func_80344A54
/* 8015CE30 00158370  7F E4 FB 78 */	mr r4, r31
/* 8015CE34 00158374  4B FF FE F5 */	bl func_8015CD28
/* 8015CE38 00158378  80 01 00 14 */	lwz r0, 0x14(r1)
/* 8015CE3C 0015837C  83 E1 00 0C */	lwz r31, 0xc(r1)
/* 8015CE40 00158380  7C 08 03 A6 */	mtlr r0
/* 8015CE44 00158384  38 21 00 10 */	addi r1, r1, 0x10
/* 8015CE48 00158388  4E 80 00 20 */	blr 

.global func_8015CE4C
func_8015CE4C:
/* 8015CE4C 0015838C  94 21 FF F0 */	stwu r1, -0x10(r1)
/* 8015CE50 00158390  7C 08 02 A6 */	mflr r0
/* 8015CE54 00158394  90 01 00 14 */	stw r0, 0x14(r1)
/* 8015CE58 00158398  48 1E 7C 51 */	bl func_80344AA8
/* 8015CE5C 0015839C  38 80 00 30 */	li r4, 0x30
/* 8015CE60 001583A0  48 1E 7B F5 */	bl func_80344A54
/* 8015CE64 001583A4  48 00 26 55 */	bl func_8015F4B8
/* 8015CE68 001583A8  48 00 25 4D */	bl func_8015F3B4
/* 8015CE6C 001583AC  80 01 00 14 */	lwz r0, 0x14(r1)
/* 8015CE70 001583B0  7C 08 03 A6 */	mtlr r0
/* 8015CE74 001583B4  38 21 00 10 */	addi r1, r1, 0x10
/* 8015CE78 001583B8  4E 80 00 20 */	blr 
/* 8015CE7C 001583BC  94 21 FF F0 */	stwu r1, -0x10(r1)
/* 8015CE80 001583C0  7C 08 02 A6 */	mflr r0
/* 8015CE84 001583C4  2C 03 00 00 */	cmpwi r3, 0
/* 8015CE88 001583C8  90 01 00 14 */	stw r0, 0x14(r1)
/* 8015CE8C 001583CC  93 E1 00 0C */	stw r31, 0xc(r1)
/* 8015CE90 001583D0  7C 9F 23 78 */	mr r31, r4
/* 8015CE94 001583D4  93 C1 00 08 */	stw r30, 8(r1)
/* 8015CE98 001583D8  7C 7E 1B 78 */	mr r30, r3
/* 8015CE9C 001583DC  41 82 00 20 */	beq lbl_8015CEBC
/* 8015CEA0 001583E0  41 82 00 0C */	beq lbl_8015CEAC
/* 8015CEA4 001583E4  38 80 00 00 */	li r4, 0
/* 8015CEA8 001583E8  48 10 E9 FD */	bl func_8026B8A4
lbl_8015CEAC:
/* 8015CEAC 001583EC  2C 1F 00 00 */	cmpwi r31, 0
/* 8015CEB0 001583F0  40 81 00 0C */	ble lbl_8015CEBC
/* 8015CEB4 001583F4  7F C3 F3 78 */	mr r3, r30
/* 8015CEB8 001583F8  48 2A CC 89 */	bl __dl__FPv
lbl_8015CEBC:
/* 8015CEBC 001583FC  7F C3 F3 78 */	mr r3, r30
/* 8015CEC0 00158400  83 E1 00 0C */	lwz r31, 0xc(r1)
/* 8015CEC4 00158404  83 C1 00 08 */	lwz r30, 8(r1)
/* 8015CEC8 00158408  80 01 00 14 */	lwz r0, 0x14(r1)
/* 8015CECC 0015840C  7C 08 03 A6 */	mtlr r0
/* 8015CED0 00158410  38 21 00 10 */	addi r1, r1, 0x10
/* 8015CED4 00158414  4E 80 00 20 */	blr 