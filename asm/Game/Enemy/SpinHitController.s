.include "macros.inc"

.text

.global func_8013E8F0
func_8013E8F0:
/* 8013E8F0 00139E30  94 21 FF F0 */	stwu r1, -0x10(r1)
/* 8013E8F4 00139E34  7C 08 02 A6 */	mflr r0
/* 8013E8F8 00139E38  7C A8 2B 78 */	mr r8, r5
/* 8013E8FC 00139E3C  7C C9 33 78 */	mr r9, r6
/* 8013E900 00139E40  90 01 00 14 */	stw r0, 0x14(r1)
/* 8013E904 00139E44  38 A0 00 0F */	li r5, 0xf
/* 8013E908 00139E48  C0 22 B9 B8 */	lfs f1, lbl_806A6C38-_SDA2_BASE_(r2)
/* 8013E90C 00139E4C  38 C0 00 0A */	li r6, 0xa
/* 8013E910 00139E50  93 E1 00 0C */	stw r31, 0xc(r1)
/* 8013E914 00139E54  7C 7F 1B 78 */	mr r31, r3
/* 8013E918 00139E58  C0 42 B9 BC */	lfs f2, lbl_806A6C3C-_SDA2_BASE_(r2)
/* 8013E91C 00139E5C  38 E0 00 03 */	li r7, 3
/* 8013E920 00139E60  C0 62 B9 C0 */	lfs f3, lbl_806A6C40-_SDA2_BASE_(r2)
/* 8013E924 00139E64  48 00 00 4D */	bl func_8013E970
/* 8013E928 00139E68  7F E3 FB 78 */	mr r3, r31
/* 8013E92C 00139E6C  83 E1 00 0C */	lwz r31, 0xc(r1)
/* 8013E930 00139E70  80 01 00 14 */	lwz r0, 0x14(r1)
/* 8013E934 00139E74  7C 08 03 A6 */	mtlr r0
/* 8013E938 00139E78  38 21 00 10 */	addi r1, r1, 0x10
/* 8013E93C 00139E7C  4E 80 00 20 */	blr 

.global func_8013E940
func_8013E940:
/* 8013E940 00139E80  94 21 FF F0 */	stwu r1, -0x10(r1)
/* 8013E944 00139E84  7C 08 02 A6 */	mflr r0
/* 8013E948 00139E88  90 01 00 14 */	stw r0, 0x14(r1)
/* 8013E94C 00139E8C  93 E1 00 0C */	stw r31, 0xc(r1)
/* 8013E950 00139E90  7C 7F 1B 78 */	mr r31, r3
/* 8013E954 00139E94  48 00 00 1D */	bl func_8013E970
/* 8013E958 00139E98  7F E3 FB 78 */	mr r3, r31
/* 8013E95C 00139E9C  83 E1 00 0C */	lwz r31, 0xc(r1)
/* 8013E960 00139EA0  80 01 00 14 */	lwz r0, 0x14(r1)
/* 8013E964 00139EA4  7C 08 03 A6 */	mtlr r0
/* 8013E968 00139EA8  38 21 00 10 */	addi r1, r1, 0x10
/* 8013E96C 00139EAC  4E 80 00 20 */	blr 

.global func_8013E970
func_8013E970:
/* 8013E970 00139EB0  90 A3 00 00 */	stw r5, 0(r3)
/* 8013E974 00139EB4  90 C3 00 04 */	stw r6, 4(r3)
/* 8013E978 00139EB8  90 E3 00 08 */	stw r7, 8(r3)
/* 8013E97C 00139EBC  D0 23 00 0C */	stfs f1, 0xc(r3)
/* 8013E980 00139EC0  D0 43 00 10 */	stfs f2, 0x10(r3)
/* 8013E984 00139EC4  D0 63 00 14 */	stfs f3, 0x14(r3)
/* 8013E988 00139EC8  99 23 00 19 */	stb r9, 0x19(r3)
/* 8013E98C 00139ECC  91 03 00 1C */	stw r8, 0x1c(r3)
/* 8013E990 00139ED0  4E 80 00 20 */	blr 

.global func_8013E994
func_8013E994:
/* 8013E994 00139ED4  94 21 FF C0 */	stwu r1, -0x40(r1)
/* 8013E998 00139ED8  7C 08 02 A6 */	mflr r0
/* 8013E99C 00139EDC  90 01 00 44 */	stw r0, 0x44(r1)
/* 8013E9A0 00139EE0  DB E1 00 30 */	stfd f31, 0x30(r1)
/* 8013E9A4 00139EE4  F3 E1 00 38 */	psq_st f31, 56(r1), 0, 0
/* 8013E9A8 00139EE8  39 61 00 30 */	addi r11, r1, 0x30
/* 8013E9AC 00139EEC  48 3D 8B 8D */	bl func_80517538
/* 8013E9B0 00139EF0  7C 7D 1B 78 */	mr r29, r3
/* 8013E9B4 00139EF4  7C 9E 23 78 */	mr r30, r4
/* 8013E9B8 00139EF8  7C BF 2B 78 */	mr r31, r5
/* 8013E9BC 00139EFC  7C C4 33 78 */	mr r4, r6
/* 8013E9C0 00139F00  38 61 00 14 */	addi r3, r1, 0x14
/* 8013E9C4 00139F04  4B ED A1 C9 */	bl func_80018B8C
/* 8013E9C8 00139F08  7F E4 FB 78 */	mr r4, r31
/* 8013E9CC 00139F0C  38 61 00 14 */	addi r3, r1, 0x14
/* 8013E9D0 00139F10  4B EE 13 C5 */	bl func_8001FD94
/* 8013E9D4 00139F14  38 61 00 14 */	addi r3, r1, 0x14
/* 8013E9D8 00139F18  38 9E 00 3C */	addi r4, r30, 0x3c
/* 8013E9DC 00139F1C  7C 65 1B 78 */	mr r5, r3
/* 8013E9E0 00139F20  48 2A 74 C9 */	bl func_803E5EA8
/* 8013E9E4 00139F24  38 61 00 14 */	addi r3, r1, 0x14
/* 8013E9E8 00139F28  48 2A 64 C9 */	bl func_803E4EB0
/* 8013E9EC 00139F2C  C0 3D 00 10 */	lfs f1, 0x10(r29)
/* 8013E9F0 00139F30  38 61 00 14 */	addi r3, r1, 0x14
/* 8013E9F4 00139F34  4B EE 4A 11 */	bl func_80023404
/* 8013E9F8 00139F38  C3 FD 00 14 */	lfs f31, 0x14(r29)
/* 8013E9FC 00139F3C  38 61 00 08 */	addi r3, r1, 8
/* 8013EA00 00139F40  38 9E 00 3C */	addi r4, r30, 0x3c
/* 8013EA04 00139F44  4B ED A1 89 */	bl func_80018B8C
/* 8013EA08 00139F48  FC 20 F8 90 */	fmr f1, f31
/* 8013EA0C 00139F4C  38 61 00 08 */	addi r3, r1, 8
/* 8013EA10 00139F50  4B EE 49 F5 */	bl func_80023404
/* 8013EA14 00139F54  38 61 00 14 */	addi r3, r1, 0x14
/* 8013EA18 00139F58  38 81 00 08 */	addi r4, r1, 8
/* 8013EA1C 00139F5C  4B EE 13 79 */	bl func_8001FD94
/* 8013EA20 00139F60  C0 01 00 14 */	lfs f0, 0x14(r1)
/* 8013EA24 00139F64  38 00 00 01 */	li r0, 1
/* 8013EA28 00139F68  D0 1E 00 30 */	stfs f0, 0x30(r30)
/* 8013EA2C 00139F6C  C0 01 00 18 */	lfs f0, 0x18(r1)
/* 8013EA30 00139F70  D0 1E 00 34 */	stfs f0, 0x34(r30)
/* 8013EA34 00139F74  C0 01 00 1C */	lfs f0, 0x1c(r1)
/* 8013EA38 00139F78  D0 1E 00 38 */	stfs f0, 0x38(r30)
/* 8013EA3C 00139F7C  98 1D 00 18 */	stb r0, 0x18(r29)
/* 8013EA40 00139F80  E3 E1 00 38 */	psq_l f31, 56(r1), 0, 0
/* 8013EA44 00139F84  CB E1 00 30 */	lfd f31, 0x30(r1)
/* 8013EA48 00139F88  39 61 00 30 */	addi r11, r1, 0x30
/* 8013EA4C 00139F8C  48 3D 8B 39 */	bl func_80517584
/* 8013EA50 00139F90  80 01 00 44 */	lwz r0, 0x44(r1)
/* 8013EA54 00139F94  7C 08 03 A6 */	mtlr r0
/* 8013EA58 00139F98  38 21 00 40 */	addi r1, r1, 0x40
/* 8013EA5C 00139F9C  4E 80 00 20 */	blr 

.global func_8013EA60
func_8013EA60:
/* 8013EA60 00139FA0  94 21 FF F0 */	stwu r1, -0x10(r1)
/* 8013EA64 00139FA4  7C 08 02 A6 */	mflr r0
/* 8013EA68 00139FA8  90 01 00 14 */	stw r0, 0x14(r1)
/* 8013EA6C 00139FAC  93 E1 00 0C */	stw r31, 0xc(r1)
/* 8013EA70 00139FB0  7C 7F 1B 78 */	mr r31, r3
/* 8013EA74 00139FB4  4B FF FF 21 */	bl func_8013E994
/* 8013EA78 00139FB8  38 00 00 00 */	li r0, 0
/* 8013EA7C 00139FBC  98 1F 00 18 */	stb r0, 0x18(r31)
/* 8013EA80 00139FC0  83 E1 00 0C */	lwz r31, 0xc(r1)
/* 8013EA84 00139FC4  80 01 00 14 */	lwz r0, 0x14(r1)
/* 8013EA88 00139FC8  7C 08 03 A6 */	mtlr r0
/* 8013EA8C 00139FCC  38 21 00 10 */	addi r1, r1, 0x10
/* 8013EA90 00139FD0  4E 80 00 20 */	blr 

.global func_8013EA94
func_8013EA94:
/* 8013EA94 00139FD4  94 21 FF D0 */	stwu r1, -0x30(r1)
/* 8013EA98 00139FD8  7C 08 02 A6 */	mflr r0
/* 8013EA9C 00139FDC  90 01 00 34 */	stw r0, 0x34(r1)
/* 8013EAA0 00139FE0  DB E1 00 20 */	stfd f31, 0x20(r1)
/* 8013EAA4 00139FE4  F3 E1 00 28 */	psq_st f31, 40(r1), 0, 0
/* 8013EAA8 00139FE8  93 E1 00 1C */	stw r31, 0x1c(r1)
/* 8013EAAC 00139FEC  7C 9F 23 78 */	mr r31, r4
/* 8013EAB0 00139FF0  93 C1 00 18 */	stw r30, 0x18(r1)
/* 8013EAB4 00139FF4  7C 7E 1B 78 */	mr r30, r3
/* 8013EAB8 00139FF8  7F E3 FB 78 */	mr r3, r31
/* 8013EABC 00139FFC  48 29 DC 31 */	bl func_803DC6EC
/* 8013EAC0 0013A000  2C 03 00 00 */	cmpwi r3, 0
/* 8013EAC4 0013A004  41 82 00 20 */	beq lbl_8013EAE4
/* 8013EAC8 0013A008  7F E3 FB 78 */	mr r3, r31
/* 8013EACC 0013A00C  48 29 B6 25 */	bl func_803DA0F0
/* 8013EAD0 0013A010  88 1E 00 18 */	lbz r0, 0x18(r30)
/* 8013EAD4 0013A014  2C 00 00 00 */	cmpwi r0, 0
/* 8013EAD8 0013A018  41 82 00 0C */	beq lbl_8013EAE4
/* 8013EADC 0013A01C  80 7E 00 08 */	lwz r3, 8(r30)
/* 8013EAE0 0013A020  48 2B 26 69 */	bl func_803F1148
lbl_8013EAE4:
/* 8013EAE4 0013A024  C3 FE 00 0C */	lfs f31, 0xc(r30)
/* 8013EAE8 0013A028  38 61 00 08 */	addi r3, r1, 8
/* 8013EAEC 0013A02C  38 9F 00 3C */	addi r4, r31, 0x3c
/* 8013EAF0 0013A030  4B ED A0 9D */	bl func_80018B8C
/* 8013EAF4 0013A034  FC 20 F8 90 */	fmr f1, f31
/* 8013EAF8 0013A038  38 61 00 08 */	addi r3, r1, 8
/* 8013EAFC 0013A03C  4B EE 49 09 */	bl func_80023404
/* 8013EB00 0013A040  E0 1F 00 30 */	psq_l f0, 48(r31), 0, 0
/* 8013EB04 0013A044  7F E3 FB 78 */	mr r3, r31
/* 8013EB08 0013A048  E0 21 00 08 */	psq_l f1, 8(r1), 0, 0
/* 8013EB0C 0013A04C  E0 5F 80 38 */	psq_l f2, 56(r31), 1, 0
/* 8013EB10 0013A050  10 00 08 2A */	ps_add f0, f0, f1
/* 8013EB14 0013A054  F0 1F 00 30 */	psq_st f0, 48(r31), 0, 0
/* 8013EB18 0013A058  E0 01 80 10 */	psq_l f0, 16(r1), 1, 0
/* 8013EB1C 0013A05C  10 02 00 2A */	ps_add f0, f2, f0
/* 8013EB20 0013A060  F0 1F 80 38 */	psq_st f0, 56(r31), 1, 0
/* 8013EB24 0013A064  80 9E 00 04 */	lwz r4, 4(r30)
/* 8013EB28 0013A068  48 29 DB 8D */	bl func_803DC6B4
/* 8013EB2C 0013A06C  2C 03 00 00 */	cmpwi r3, 0
/* 8013EB30 0013A070  41 82 00 10 */	beq lbl_8013EB40
/* 8013EB34 0013A074  7F C3 F3 78 */	mr r3, r30
/* 8013EB38 0013A078  7F E4 FB 78 */	mr r4, r31
/* 8013EB3C 0013A07C  48 00 00 A5 */	bl func_8013EBE0
lbl_8013EB40:
/* 8013EB40 0013A080  7F E3 FB 78 */	mr r3, r31
/* 8013EB44 0013A084  48 29 AC 19 */	bl func_803D975C
/* 8013EB48 0013A088  2C 03 00 00 */	cmpwi r3, 0
/* 8013EB4C 0013A08C  41 82 00 18 */	beq lbl_8013EB64
/* 8013EB50 0013A090  7F E3 FB 78 */	mr r3, r31
/* 8013EB54 0013A094  38 80 00 01 */	li r4, 1
/* 8013EB58 0013A098  48 29 DC 19 */	bl func_803DC770
/* 8013EB5C 0013A09C  2C 03 00 00 */	cmpwi r3, 0
/* 8013EB60 0013A0A0  40 82 00 18 */	bne lbl_8013EB78
lbl_8013EB64:
/* 8013EB64 0013A0A4  80 9E 00 00 */	lwz r4, 0(r30)
/* 8013EB68 0013A0A8  7F E3 FB 78 */	mr r3, r31
/* 8013EB6C 0013A0AC  48 29 DC 05 */	bl func_803DC770
/* 8013EB70 0013A0B0  2C 03 00 00 */	cmpwi r3, 0
/* 8013EB74 0013A0B4  41 82 00 48 */	beq lbl_8013EBBC
lbl_8013EB78:
/* 8013EB78 0013A0B8  7F E3 FB 78 */	mr r3, r31
/* 8013EB7C 0013A0BC  48 29 B5 31 */	bl func_803DA0AC
/* 8013EB80 0013A0C0  81 9F 00 00 */	lwz r12, 0(r31)
/* 8013EB84 0013A0C4  7F E3 FB 78 */	mr r3, r31
/* 8013EB88 0013A0C8  81 8C 00 2C */	lwz r12, 0x2c(r12)
/* 8013EB8C 0013A0CC  7D 89 03 A6 */	mtctr r12
/* 8013EB90 0013A0D0  4E 80 04 21 */	bctrl 
/* 8013EB94 0013A0D4  80 9E 00 04 */	lwz r4, 4(r30)
/* 8013EB98 0013A0D8  7F E3 FB 78 */	mr r3, r31
/* 8013EB9C 0013A0DC  48 29 DB 59 */	bl func_803DC6F4
/* 8013EBA0 0013A0E0  2C 03 00 00 */	cmpwi r3, 0
/* 8013EBA4 0013A0E4  41 82 00 10 */	beq lbl_8013EBB4
/* 8013EBA8 0013A0E8  7F C3 F3 78 */	mr r3, r30
/* 8013EBAC 0013A0EC  7F E4 FB 78 */	mr r4, r31
/* 8013EBB0 0013A0F0  48 00 00 31 */	bl func_8013EBE0
lbl_8013EBB4:
/* 8013EBB4 0013A0F4  38 60 00 01 */	li r3, 1
/* 8013EBB8 0013A0F8  48 00 00 08 */	b lbl_8013EBC0
lbl_8013EBBC:
/* 8013EBBC 0013A0FC  38 60 00 00 */	li r3, 0
lbl_8013EBC0:
/* 8013EBC0 0013A100  E3 E1 00 28 */	psq_l f31, 40(r1), 0, 0
/* 8013EBC4 0013A104  80 01 00 34 */	lwz r0, 0x34(r1)
/* 8013EBC8 0013A108  CB E1 00 20 */	lfd f31, 0x20(r1)
/* 8013EBCC 0013A10C  83 E1 00 1C */	lwz r31, 0x1c(r1)
/* 8013EBD0 0013A110  83 C1 00 18 */	lwz r30, 0x18(r1)
/* 8013EBD4 0013A114  7C 08 03 A6 */	mtlr r0
/* 8013EBD8 0013A118  38 21 00 30 */	addi r1, r1, 0x30
/* 8013EBDC 0013A11C  4E 80 00 20 */	blr 

.global func_8013EBE0
func_8013EBE0:
/* 8013EBE0 0013A120  94 21 FF F0 */	stwu r1, -0x10(r1)
/* 8013EBE4 0013A124  7C 08 02 A6 */	mflr r0
/* 8013EBE8 0013A128  90 01 00 14 */	stw r0, 0x14(r1)
/* 8013EBEC 0013A12C  93 E1 00 0C */	stw r31, 0xc(r1)
/* 8013EBF0 0013A130  7C 9F 23 78 */	mr r31, r4
/* 8013EBF4 0013A134  93 C1 00 08 */	stw r30, 8(r1)
/* 8013EBF8 0013A138  7C 7E 1B 78 */	mr r30, r3
/* 8013EBFC 0013A13C  80 A3 00 1C */	lwz r5, 0x1c(r3)
/* 8013EC00 0013A140  2C 05 00 00 */	cmpwi r5, 0
/* 8013EC04 0013A144  41 82 00 64 */	beq lbl_8013EC68
/* 8013EC08 0013A148  88 03 00 19 */	lbz r0, 0x19(r3)
/* 8013EC0C 0013A14C  2C 00 00 00 */	cmpwi r0, 0
/* 8013EC10 0013A150  41 82 00 14 */	beq lbl_8013EC24
/* 8013EC14 0013A154  7F E3 FB 78 */	mr r3, r31
/* 8013EC18 0013A158  38 84 00 0C */	addi r4, r4, 0xc
/* 8013EC1C 0013A15C  48 2B 1E 75 */	bl func_803F0A90
/* 8013EC20 0013A160  48 00 00 40 */	b lbl_8013EC60
lbl_8013EC24:
/* 8013EC24 0013A164  C0 22 B9 C4 */	lfs f1, lbl_806A6C44-_SDA2_BASE_(r2)
/* 8013EC28 0013A168  7F E3 FB 78 */	mr r3, r31
/* 8013EC2C 0013A16C  C0 42 B9 BC */	lfs f2, lbl_806A6C3C-_SDA2_BASE_(r2)
/* 8013EC30 0013A170  38 C0 00 00 */	li r6, 0
/* 8013EC34 0013A174  38 84 00 0C */	addi r4, r4, 0xc
/* 8013EC38 0013A178  48 2B 21 09 */	bl func_803F0D40
/* 8013EC3C 0013A17C  80 1F 00 64 */	lwz r0, 0x64(r31)
/* 8013EC40 0013A180  2C 00 00 00 */	cmpwi r0, 0
/* 8013EC44 0013A184  41 82 00 1C */	beq lbl_8013EC60
/* 8013EC48 0013A188  3C 80 80 57 */	lis r4, lbl_8056FFB0@ha
/* 8013EC4C 0013A18C  7F E3 FB 78 */	mr r3, r31
/* 8013EC50 0013A190  38 84 FF B0 */	addi r4, r4, lbl_8056FFB0@l
/* 8013EC54 0013A194  38 A0 FF FF */	li r5, -1
/* 8013EC58 0013A198  38 C0 FF FF */	li r6, -1
/* 8013EC5C 0013A19C  48 2B 9D E1 */	bl func_803F8A3C
lbl_8013EC60:
/* 8013EC60 0013A1A0  38 00 00 00 */	li r0, 0
/* 8013EC64 0013A1A4  90 1E 00 1C */	stw r0, 0x1c(r30)
lbl_8013EC68:
/* 8013EC68 0013A1A8  80 01 00 14 */	lwz r0, 0x14(r1)
/* 8013EC6C 0013A1AC  83 E1 00 0C */	lwz r31, 0xc(r1)
/* 8013EC70 0013A1B0  83 C1 00 08 */	lwz r30, 8(r1)
/* 8013EC74 0013A1B4  7C 08 03 A6 */	mtlr r0
/* 8013EC78 0013A1B8  38 21 00 10 */	addi r1, r1, 0x10
/* 8013EC7C 0013A1BC  4E 80 00 20 */	blr 