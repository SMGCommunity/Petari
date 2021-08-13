.text

.include "macros.inc"

.global func_8041C854
func_8041C854:
/* 8041C854 00417D94  C0 43 00 00 */	lfs f2, 0(r3)
/* 8041C858 00417D98  C0 24 00 00 */	lfs f1, 0(r4)
/* 8041C85C 00417D9C  C0 03 00 08 */	lfs f0, 8(r3)
/* 8041C860 00417DA0  EC 82 08 2A */	fadds f4, f2, f1
/* 8041C864 00417DA4  C0 43 00 04 */	lfs f2, 4(r3)
/* 8041C868 00417DA8  EC 20 08 2A */	fadds f1, f0, f1
/* 8041C86C 00417DAC  C0 64 00 04 */	lfs f3, 4(r4)
/* 8041C870 00417DB0  C0 03 00 0C */	lfs f0, 0xc(r3)
/* 8041C874 00417DB4  EC 42 18 2A */	fadds f2, f2, f3
/* 8041C878 00417DB8  EC 00 18 2A */	fadds f0, f0, f3
/* 8041C87C 00417DBC  D0 83 00 00 */	stfs f4, 0(r3)
/* 8041C880 00417DC0  D0 43 00 04 */	stfs f2, 4(r3)
/* 8041C884 00417DC4  D0 23 00 08 */	stfs f1, 8(r3)
/* 8041C888 00417DC8  D0 03 00 0C */	stfs f0, 0xc(r3)
/* 8041C88C 00417DCC  4E 80 00 20 */	blr 

