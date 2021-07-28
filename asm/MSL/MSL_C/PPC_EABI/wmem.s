.text

.include "macros.inc"

.global wmemcpy
wmemcpy:
/* 805208C4 0051BE04  54 A5 08 3C */	slwi r5, r5, 1
/* 805208C8 0051BE08  4B AE 3A 70 */	b memcpy

.global wmemchr
wmemchr:
/* 805208CC 0051BE0C  7C A9 03 A6 */	mtctr r5
/* 805208D0 0051BE10  2C 05 00 00 */	cmpwi r5, 0
/* 805208D4 0051BE14  41 82 00 18 */	beq lbl_805208EC
lbl_805208D8:
/* 805208D8 0051BE18  A0 03 00 00 */	lhz r0, 0(r3)
/* 805208DC 0051BE1C  7C 00 20 40 */	cmplw r0, r4
/* 805208E0 0051BE20  4D 82 00 20 */	beqlr 
/* 805208E4 0051BE24  38 63 00 02 */	addi r3, r3, 2
/* 805208E8 0051BE28  42 00 FF F0 */	bdnz lbl_805208D8
lbl_805208EC:
/* 805208EC 0051BE2C  38 60 00 00 */	li r3, 0
/* 805208F0 0051BE30  4E 80 00 20 */	blr 
