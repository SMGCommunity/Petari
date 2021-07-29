.section .ctors  # 0x8052D280 - 0x8052DEE0

.global lbl_8052D280
lbl_8052D280:
	.incbin "baserom.dol", 0x529380, 0xC60
