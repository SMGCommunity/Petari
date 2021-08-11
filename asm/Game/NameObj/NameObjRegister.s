.include "macros.inc"

.text

.global func_8026C0FC
func_8026C0FC:
/* 8026C0FC 0026763C  90 83 00 00 */	stw r4, 0(r3)
/* 8026C100 00267640  4E 80 00 20 */	blr 

.global func_8026C104
func_8026C104:
/* 8026C104 00267644  80 63 00 00 */	lwz r3, 0(r3)
/* 8026C108 00267648  4B FF F9 48 */	b lbl_8026BA50

.global func_8026C10C
func_8026C10C:
/* 8026C10C 0026764C  38 00 00 00 */	li r0, 0
/* 8026C110 00267650  90 03 00 00 */	stw r0, 0(r3)
/* 8026C114 00267654  4E 80 00 20 */	blr 
