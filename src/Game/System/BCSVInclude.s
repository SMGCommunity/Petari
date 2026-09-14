.section .rodata

.global StoryEventBCSV   # 0x8053DC20 - 0x8053DDFF
StoryEventBCSV:
	.incbin "src/Game/System/StoryEvent.bcsv"

.global GalaxyIDBCSV     # 0x8053DE00 - 0x8053EB1F
GalaxyIDBCSV:
	.incbin "src/Game/System/GalaxyID.bcsv"
