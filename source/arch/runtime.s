.text

.global _savegpr_29
_savegpr_29:
    stw r29, -0xC(r11)

.global _savegpr_30
_savegpr_30:
    stw r30, -0x8(r11)

.global _savegpr_31
_savegpr_31:
    stw r31, -0x4(r11)

.global _restgpr_29
_restgpr_29:
    lwz r29, -0xC(r11)