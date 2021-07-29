.text

.include "macros.inc"

.global TRK_strlen
TRK_strlen:
/* 80529A38 00524F78  38 83 FF FF */	addi r4, r3, -1
/* 80529A3C 00524F7C  38 60 FF FF */	li r3, -1
lbl_80529A40:
/* 80529A40 00524F80  8C 04 00 01 */	lbzu r0, 1(r4)
/* 80529A44 00524F84  38 63 00 01 */	addi r3, r3, 1
/* 80529A48 00524F88  28 00 00 00 */	cmplwi r0, 0
/* 80529A4C 00524F8C  40 82 FF F4 */	bne lbl_80529A40
/* 80529A50 00524F90  4E 80 00 20 */	blr 
