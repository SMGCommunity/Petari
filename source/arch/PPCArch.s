.text

.global PPCMfmsr
PPCMfmsr:
    mfmsr r3
    blr

.global PPCMtmsr
PPCMtmsr:
    mtmsr r3
    blr

.global PPCMfhid0
PPCMfhid0:
    mfspr r3, hid0
    blr

.global PPCMthid0
PPCMthid0:
    mtspr hid0, r3
    blr

.global PPCSync
PPCSync:
    sc
    blr

.global PPCHalt
PPCHalt:
    sync

loop:
    nop
    li r3, 0
    nop
    b loop

.global PPCDisableSpeculation
PPCDisableSpeculation:
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    bl PPCMfhid0
    ori r3, r3, 0x200
    bl PPCMthid0
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

.global PPCSetFpNonIEEEMode
PPCSetFpNonIEEEMode:
    mtfsb1 4*cr7+gt
    blr