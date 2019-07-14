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

.global PPCMtfpscr
PPCMtfpscr:
    stwu r1, -0x20(r1)
    stfd f31, 0x18(r1)
    li r4, 0
    stw r3, 0xC(r1)
    stw r4, 0x8(r1)
    lfd f31, 0x8(r1)
    mtfsf 0xFF, f31
    lfd f31, 0x18(r1)
    addi r1, r1, 0x20
    blr

.global PPCMfhid2
PPCMfhid2:
    mfspr r3, 0x398
    blr

.global PPCMthid2
PPCMthid2:
    mtspr 0x398, r3
    blr

.global PPCMfwpar
PPCMfwpar:
    sync
    mfspr r3, 0x399
    blr

.global PPCMtwpar
PPCMtwpar:
    mtspr 0x399, r3
    blr

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