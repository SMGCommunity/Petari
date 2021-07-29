.text

.include "macros.inc"

.global abs
abs:
/* 80519C84 005151C4  7C 64 FE 70 */	srawi r4, r3, 0x1f
/* 80519C88 005151C8  7C 80 1A 78 */	xor r0, r4, r3
/* 80519C8C 005151CC  7C 64 00 50 */	subf r3, r4, r0
/* 80519C90 005151D0  4E 80 00 20 */	blr 
