.include "macros.inc"

.data

.global lbl_805B98F8
lbl_805B98F8:
	.incbin "baserom.dol", 0x5B59F8, 0x20
.global lbl_805B9918
lbl_805B9918:
	.incbin "baserom.dol", 0x5B5A18, 0x10
.global lbl_805B9928
lbl_805B9928:
	.incbin "baserom.dol", 0x5B5A28, 0xA00
.global lbl_805BA328
lbl_805BA328:
	.incbin "baserom.dol", 0x5B6428, 0x20
