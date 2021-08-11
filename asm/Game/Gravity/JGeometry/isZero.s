.include "macros.inc"

.text

.global func_80156CB0
func_80156CB0:
/* 80156CB0 001521F0  E0 23 00 00 */	psq_l f1, 0(r3), 0, 0
/* 80156CB4 001521F4  C0 43 00 08 */	lfs f2, 8(r3)
/* 80156CB8 001521F8  10 21 00 72 */	ps_mul f1, f1, f1
/* 80156CBC 001521FC  C0 02 BE E8 */	lfs f0, lbl_806A7168-_SDA2_BASE_(r2)
/* 80156CC0 00152200  10 42 08 BA */	ps_madd f2, f2, f2, f1
/* 80156CC4 00152204  10 42 08 54 */	ps_sum0 f2, f2, f1, f1
/* 80156CC8 00152208  FC 02 00 40 */	fcmpo cr0, f2, f0
/* 80156CCC 0015220C  4C 40 13 82 */	cror 2, 0, 2
/* 80156CD0 00152210  7C 60 00 26 */	mfcr r3
/* 80156CD4 00152214  54 63 1F FE */	rlwinm r3, r3, 3, 0x1f, 0x1f
/* 80156CD8 00152218  4E 80 00 20 */	blr 

