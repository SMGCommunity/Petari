.include "macros.inc"

.data

.global lbl_8054EE20
lbl_8054EE20:
	.incbin "baserom.dol", 0x54AF20, 0x14
.global lbl_8054EE34
lbl_8054EE34:
	.incbin "baserom.dol", 0x54AF34, 0x14
.global lbl_8054EE48
lbl_8054EE48:
	.incbin "baserom.dol", 0x54AF48, 0x30
.global lbl_8054EE78
lbl_8054EE78:
	.incbin "baserom.dol", 0x54AF78, 0x6C
.global lbl_8054EEE4
lbl_8054EEE4:
	.incbin "baserom.dol", 0x54AFE4, 0x5C
.global lbl_8054EF40
lbl_8054EF40:
	.incbin "baserom.dol", 0x54B040, 0x60


.global lbl_8054EFA0
lbl_8054EFA0:
	.incbin "baserom.dol", 0x54B0A0, 0x60
.global lbl_8054F000
lbl_8054F000:
	.incbin "baserom.dol", 0x54B100, 0x20


# note, some of this stuff is wrong
.global lbl_8054F020
lbl_8054F020:
	.incbin "baserom.dol", 0x54B120, 0xFE4
.global lbl_80550004
lbl_80550004:
	.incbin "baserom.dol", 0x54C104, 0x74
.global lbl_80550078
lbl_80550078:
	.incbin "baserom.dol", 0x54C178, 0x68
.global lbl_805500E0
lbl_805500E0:
	.incbin "baserom.dol", 0x54C1E0, 0x10
.global lbl_805500F0
lbl_805500F0:
	.incbin "baserom.dol", 0x54C1F0, 0x40
.global lbl_80550130
lbl_80550130:
	.incbin "baserom.dol", 0x54C230, 0x68
.global lbl_80550198
lbl_80550198:
	.incbin "baserom.dol", 0x54C298, 0x78
.global lbl_80550210
lbl_80550210:
	.incbin "baserom.dol", 0x54C310, 0x3C
.global lbl_8055024C
lbl_8055024C:
	.incbin "baserom.dol", 0x54C34C, 0x7C
.global lbl_805502C8
lbl_805502C8:
	.incbin "baserom.dol", 0x54C3C8, 0x68
.global lbl_80550330
lbl_80550330:
	.incbin "baserom.dol", 0x54C430, 0x20
.global lbl_80550350
lbl_80550350:
	.incbin "baserom.dol", 0x54C450, 0x20
.global lbl_80550370
lbl_80550370:
	.incbin "baserom.dol", 0x54C470, 0x28
.global lbl_80550398
lbl_80550398:
	.incbin "baserom.dol", 0x54C498, 0x10
.global lbl_805503A8
lbl_805503A8:
	.incbin "baserom.dol", 0x54C4A8, 0x28
.global lbl_805503D0
lbl_805503D0:
	.incbin "baserom.dol", 0x54C4D0, 0x18
