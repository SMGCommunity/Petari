.extern __OSPSInit
.extern __OSFPRInit
.extern __OSCacheInit
.extern InitMetroTRK

.text

.global start
start:
    bl __init_registers
    bl __init_sys
    li r0, -1
    stwu r1, -0x8(r1)
    stw r0, 0x4(r1)
    stw r0, 0(r1)
    bl sub_80004238
    li r0, 0
    lis r6, -0x8000
    addi r6, r6, 0x44
    stw r0, 0(r6)
    lis r6, -0x8000
    addi r6, r6, 0xF4
    lwz r6, 0(r6)
    cmplwi r6, 0
    beq loc_80004084
    lwz r7, 0xC(r6)
    b loc_800040A4

loc_80004084:
    lis r5, -0x8000
    addi r5, r5, 0x34
    lwz r5, 0(r5)
    cmplwi r5, 0
    beq loc_800040E0
loc_800040A4:
    blr
loc_800040D0:
    blr
loc_800040E0:
    blr

.global sub_80004238
sub_80004238:
    blr

.global __init_registers
__init_registers:
    li r0, 0
    li r3, 0
    li r4, 0
    li r5, 0
    li r6, 0
    li r7, 0
    li r8, 0
    li r9, 0
    li r10, 0
    li r11, 0
    li r12, 0
    li r14, 0
    li r15, 0
    li r16, 0
    li r17, 0
    li r18, 0
    li r19, 0
    li r20, 0
    li r21, 0
    li r22, 0
    li r23, 0
    li r24, 0
    li r25, 0
    li r26, 0
    li r27, 0
    li r28, 0
    li r29, 0
    li r30, 0
    li r31, 0
    lis r1, -0x7F93
    ori r1, r1, 0x2930
    lis r2, -0x7F95
    ori r2, r2, 0xFC20
    lis r13, -0x7F95
    ori r13, 13, 0x9620
    blr

.global __init_sys
__init_sys:
    mfmsr r0
    ori r0, r0, 0x2000
    mtmsr r0
    mflr r31
    bl __OSPSInit
    bl __OSFPRInit
    bl __OSCacheInit
    mtlr r31
    blr

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

.align 4