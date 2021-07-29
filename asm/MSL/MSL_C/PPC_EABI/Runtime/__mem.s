.text

.include "macros.inc"

.global strlen
strlen:
/* 80516E80 005123C0  38 83 FF FF */	addi r4, r3, -1
/* 80516E84 005123C4  38 60 FF FF */	li r3, -1
lbl_80516E88:
/* 80516E88 005123C8  8C 04 00 01 */	lbzu r0, 1(r4)
/* 80516E8C 005123CC  38 63 00 01 */	addi r3, r3, 1
/* 80516E90 005123D0  2C 00 00 00 */	cmpwi r0, 0
/* 80516E94 005123D4  40 82 FF F4 */	bne lbl_80516E88
/* 80516E98 005123D8  4E 80 00 20 */	blr 
