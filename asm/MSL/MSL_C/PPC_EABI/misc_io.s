.text

.include "macros.inc"

.global __stdio_atexit
__stdio_atexit:
/* 8051AC48 00516188  3C 60 80 52 */	lis r3, __close_all@ha
/* 8051AC4C 0051618C  38 63 82 04 */	addi r3, r3, __close_all@l
/* 8051AC50 00516190  90 6D E5 94 */	stw r3, lbl_806A3234-_SDA_BASE_(r13)
/* 8051AC54 00516194  4E 80 00 20 */	blr
