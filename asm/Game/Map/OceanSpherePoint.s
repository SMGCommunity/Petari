.include "macros.inc"

.text

.global func_80194080
func_80194080:
/* 80194080 0018F5C0  94 21 FF C0 */	stwu r1, -0x40(r1)
/* 80194084 0018F5C4  7C 08 02 A6 */	mflr r0
/* 80194088 0018F5C8  90 01 00 44 */	stw r0, 0x44(r1)
/* 8019408C 0018F5CC  DB E1 00 30 */	stfd f31, 0x30(r1)
/* 80194090 0018F5D0  F3 E1 00 38 */	psq_st f31, 56(r1), 0, 0
/* 80194094 0018F5D4  DB C1 00 20 */	stfd f30, 0x20(r1)
/* 80194098 0018F5D8  F3 C1 00 28 */	psq_st f30, 40(r1), 0, 0
/* 8019409C 0018F5DC  39 61 00 20 */	addi r11, r1, 0x20
/* 801940A0 0018F5E0  48 38 34 95 */	bl func_80517534
/* 801940A4 0018F5E4  FF C0 08 90 */	fmr f30, f1
/* 801940A8 0018F5E8  7C 7C 1B 78 */	mr r28, r3
/* 801940AC 0018F5EC  FF E0 10 90 */	fmr f31, f2
/* 801940B0 0018F5F0  7C 9D 23 78 */	mr r29, r4
/* 801940B4 0018F5F4  7C BE 2B 78 */	mr r30, r5
/* 801940B8 0018F5F8  7C DF 33 78 */	mr r31, r6
/* 801940BC 0018F5FC  4B E8 4A D1 */	bl func_80018B8C
/* 801940C0 0018F600  93 BC 00 0C */	stw r29, 0xc(r28)
/* 801940C4 0018F604  7F C4 F3 78 */	mr r4, r30
/* 801940C8 0018F608  38 7C 00 10 */	addi r3, r28, 0x10
/* 801940CC 0018F60C  4B E8 4A C1 */	bl func_80018B8C
/* 801940D0 0018F610  C0 3F 00 00 */	lfs f1, 0(r31)
/* 801940D4 0018F614  7F 83 E3 78 */	mr r3, r28
/* 801940D8 0018F618  C0 1F 00 04 */	lfs f0, 4(r31)
/* 801940DC 0018F61C  D3 DC 00 1C */	stfs f30, 0x1c(r28)
/* 801940E0 0018F620  D3 FC 00 20 */	stfs f31, 0x20(r28)
/* 801940E4 0018F624  D0 3C 00 24 */	stfs f1, 0x24(r28)
/* 801940E8 0018F628  D0 1C 00 28 */	stfs f0, 0x28(r28)
/* 801940EC 0018F62C  E3 E1 00 38 */	psq_l f31, 56(r1), 0, 0
/* 801940F0 0018F630  CB E1 00 30 */	lfd f31, 0x30(r1)
/* 801940F4 0018F634  E3 C1 00 28 */	psq_l f30, 40(r1), 0, 0
/* 801940F8 0018F638  CB C1 00 20 */	lfd f30, 0x20(r1)
/* 801940FC 0018F63C  39 61 00 20 */	addi r11, r1, 0x20
/* 80194100 0018F640  48 38 34 81 */	bl func_80517580
/* 80194104 0018F644  80 01 00 44 */	lwz r0, 0x44(r1)
/* 80194108 0018F648  7C 08 03 A6 */	mtlr r0
/* 8019410C 0018F64C  38 21 00 40 */	addi r1, r1, 0x40
/* 80194110 0018F650  4E 80 00 20 */	blr 

.global func_80194114
func_80194114:
/* 80194114 0018F654  94 21 FF E0 */	stwu r1, -0x20(r1)
/* 80194118 0018F658  7C 08 02 A6 */	mflr r0
/* 8019411C 0018F65C  90 01 00 24 */	stw r0, 0x24(r1)
/* 80194120 0018F660  DB E1 00 10 */	stfd f31, 0x10(r1)
/* 80194124 0018F664  F3 E1 00 18 */	psq_st f31, 24(r1), 0, 0
/* 80194128 0018F668  FC 00 18 90 */	fmr f0, f3
/* 8019412C 0018F66C  FF E0 08 90 */	fmr f31, f1
/* 80194130 0018F670  93 E1 00 0C */	stw r31, 0xc(r1)
/* 80194134 0018F674  FC 20 10 90 */	fmr f1, f2
/* 80194138 0018F678  FC 40 00 90 */	fmr f2, f0
/* 8019413C 0018F67C  7C 7F 1B 78 */	mr r31, r3
/* 80194140 0018F680  C0 63 00 1C */	lfs f3, 0x1c(r3)
/* 80194144 0018F684  C0 83 00 20 */	lfs f4, 0x20(r3)
/* 80194148 0018F688  48 00 00 6D */	bl func_801941B4
/* 8019414C 0018F68C  EC 01 F8 2A */	fadds f0, f1, f31
/* 80194150 0018F690  C0 5F 00 10 */	lfs f2, 0x10(r31)
/* 80194154 0018F694  C0 3F 00 14 */	lfs f1, 0x14(r31)
/* 80194158 0018F698  C0 7F 00 18 */	lfs f3, 0x18(r31)
/* 8019415C 0018F69C  EC 42 00 32 */	fmuls f2, f2, f0
/* 80194160 0018F6A0  80 7F 00 0C */	lwz r3, 0xc(r31)
/* 80194164 0018F6A4  EC 21 00 32 */	fmuls f1, f1, f0
/* 80194168 0018F6A8  EC 03 00 32 */	fmuls f0, f3, f0
/* 8019416C 0018F6AC  D0 5F 00 00 */	stfs f2, 0(r31)
/* 80194170 0018F6B0  D0 1F 00 08 */	stfs f0, 8(r31)
/* 80194174 0018F6B4  D0 3F 00 04 */	stfs f1, 4(r31)
/* 80194178 0018F6B8  E0 5F 80 08 */	psq_l f2, 8(r31), 1, 0
/* 8019417C 0018F6BC  E0 1F 00 00 */	psq_l f0, 0(r31), 0, 0
/* 80194180 0018F6C0  E0 23 00 00 */	psq_l f1, 0(r3), 0, 0
/* 80194184 0018F6C4  10 00 08 2A */	ps_add f0, f0, f1
/* 80194188 0018F6C8  F0 1F 00 00 */	psq_st f0, 0(r31), 0, 0
/* 8019418C 0018F6CC  E0 03 80 08 */	psq_l f0, 8(r3), 1, 0
/* 80194190 0018F6D0  10 02 00 2A */	ps_add f0, f2, f0
/* 80194194 0018F6D4  F0 1F 80 08 */	psq_st f0, 8(r31), 1, 0
/* 80194198 0018F6D8  E3 E1 00 18 */	psq_l f31, 24(r1), 0, 0
/* 8019419C 0018F6DC  CB E1 00 10 */	lfd f31, 0x10(r1)
/* 801941A0 0018F6E0  83 E1 00 0C */	lwz r31, 0xc(r1)
/* 801941A4 0018F6E4  80 01 00 24 */	lwz r0, 0x24(r1)
/* 801941A8 0018F6E8  7C 08 03 A6 */	mtlr r0
/* 801941AC 0018F6EC  38 21 00 20 */	addi r1, r1, 0x20
/* 801941B0 0018F6F0  4E 80 00 20 */	blr 

.global func_801941B4
func_801941B4:
/* 801941B4 0018F6F4  94 21 FF D0 */	stwu r1, -0x30(r1)
/* 801941B8 0018F6F8  7C 08 02 A6 */	mflr r0
/* 801941BC 0018F6FC  90 01 00 34 */	stw r0, 0x34(r1)
/* 801941C0 0018F700  DB E1 00 20 */	stfd f31, 0x20(r1)
/* 801941C4 0018F704  F3 E1 00 28 */	psq_st f31, 40(r1), 0, 0
/* 801941C8 0018F708  DB C1 00 10 */	stfd f30, 0x10(r1)
/* 801941CC 0018F70C  F3 C1 00 18 */	psq_st f30, 24(r1), 0, 0
/* 801941D0 0018F710  FF C0 08 90 */	fmr f30, f1
/* 801941D4 0018F714  FF E0 18 90 */	fmr f31, f3
/* 801941D8 0018F718  EC 22 20 2A */	fadds f1, f2, f4
/* 801941DC 0018F71C  4B E9 02 7D */	bl func_80024458
/* 801941E0 0018F720  C0 02 C6 EC */	lfs f0, lbl_806A796C-_SDA2_BASE_(r2)
/* 801941E4 0018F724  C0 42 C6 F0 */	lfs f2, lbl_806A7970-_SDA2_BASE_(r2)
/* 801941E8 0018F728  EC 00 07 F2 */	fmuls f0, f0, f31
/* 801941EC 0018F72C  EF E2 00 72 */	fmuls f31, f2, f1
/* 801941F0 0018F730  EC 3E 00 2A */	fadds f1, f30, f0
/* 801941F4 0018F734  4B E9 02 65 */	bl func_80024458
/* 801941F8 0018F738  C0 02 C6 E8 */	lfs f0, lbl_806A7968-_SDA2_BASE_(r2)
/* 801941FC 0018F73C  EC 00 00 72 */	fmuls f0, f0, f1
/* 80194200 0018F740  EC 20 F8 2A */	fadds f1, f0, f31
/* 80194204 0018F744  E3 E1 00 28 */	psq_l f31, 40(r1), 0, 0
/* 80194208 0018F748  CB E1 00 20 */	lfd f31, 0x20(r1)
/* 8019420C 0018F74C  E3 C1 00 18 */	psq_l f30, 24(r1), 0, 0
/* 80194210 0018F750  CB C1 00 10 */	lfd f30, 0x10(r1)
/* 80194214 0018F754  80 01 00 34 */	lwz r0, 0x34(r1)
/* 80194218 0018F758  7C 08 03 A6 */	mtlr r0
/* 8019421C 0018F75C  38 21 00 30 */	addi r1, r1, 0x30
/* 80194220 0018F760  4E 80 00 20 */	blr 