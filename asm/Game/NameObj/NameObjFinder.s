.include "macros.inc"

.text

.global func_8026B82C
func_8026B82C:
/* 8026B82C 00266D6C  80 AD C5 88 */	lwz r5, lbl_806A1228-_SDA_BASE_(r13)
/* 8026B830 00266D70  7C 64 1B 78 */	mr r4, r3
/* 8026B834 00266D74  80 65 00 24 */	lwz r3, 0x24(r5)
/* 8026B838 00266D78  80 63 00 A8 */	lwz r3, 0xa8(r3)
/* 8026B83C 00266D7C  48 00 03 B8 */	b lbl_8026BBF4
