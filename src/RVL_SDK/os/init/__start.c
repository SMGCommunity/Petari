#include <revolution.h>
#include <revolution/os.h>
#include <__ppc_eabi_init.h>
#include <__ppc_eabi_linker.h>

#pragma section code_type ".init"

u16 Pad3Button : 0x800030E4;
static u8 Debug_BBA = 0;

static void __init_registers(void);
static void __init_data(void);
extern void OSInit(void);
extern void DBInit(void);
extern void main(void);
extern void exit(void);
extern void InitMetroTRK(void);
extern void InitMetroTRK_BBA(void);

static void __check_pad3(void) {
    if ((Pad3Button & 0xEEF) == 0xEEF) {
        OSResetSystem(0, 0, FALSE);
    }
}

static void __set_debug_bba(void) {
    Debug_BBA = 1;
}

static u8 __get_debug_bba(void) {
    return Debug_BBA;
}

__declspec(weak) asm void __start(void) {
    nofralloc
    bl __init_registers
    bl __init_hardware
    li r0, 0xFFFF
    stwu r1, -8(r1)
    stw r0, 4(r1)
    stw r0, 0(r1)
    bl __init_data

    li r0, 0
    lis r6, 0x80000044@ha
    addi r6, r6, 0x80000044@l
    stw r0, 0(r6)

check_trk:
    lis r6, 0x800000F4@ha
    addi r6, r6, 0x800000F4@l
    lwz r6, 0(r6)
    cmplwi r6, 0
    beq load_debug_flag
    lwz r7, 0xC(r6)
    b check_debug

load_debug_flag:
    lis r5, 0x80000034@ha
    addi r5, r5, 0x80000034@l
    lwz r5, 0(r5)
    cmplwi r5, 0
    beq go_to_main

    lis r7, 0x800030E8@ha
    addi r7, r7, 0x800030E8@l
    lwz r7, 0(r7)

check_debug:
    li r5, 0
    cmplwi r7, 2
    beq go_to_init_trk
    cmplwi r7, 3
    li r5, 1
    beq go_to_init_trk
    cmplwi r7, 4
    bne go_to_main
    li r5, 2
    bl __set_debug_bba
    b go_to_main

go_to_init_trk:
    lis r6, InitMetroTRK@ha
    addi r6, r6, InitMetroTRK@l
    mtlr r6
    blrl

go_to_main:
    lis r6, 0x800000F4@ha
    addi r6, r6, 0x800000F4@l
    lwz r5, 0(r6)
    cmplwi r5, 0
    beq+ no_args
    lwz r6, 8(r5)
    cmplwi r6, 0
    beq+ no_args
    add r6, r5, r6

    lwz r14, 0(r6)
    cmplwi r14, 0
    beq- no_args
    addi r15, r6, 4
    mtctr r14

loop:
    addi r6, r6, 4
    lwz r7, 0(r6)
    add r7, r7, r5
    stw r7, 0(r6)
    bdnz loop

    lis r5, 0x80000034@ha
    addi r5, r5, 0x80000034@l
    clrrwi r7, r15, 5 
    stw r7, 0(r5)

    lis r5, 0x80003110@ha
    addi r5, r5, 0x80003110@l
    clrrwi r7, r15, 5
    stw r7, 0(r5)

    b end_args

no_args:
    li r14, 0
    li r15, 0

end_args:
    bl DBInit
    bl OSInit

    lis r4, 0x800030E6@ha
    addi r4, r4, 0x800030E6@l
    lhz r3, 0(r4)
    andi. r5, r3, 0x8000
    beq check_pad
    andi. r3, r3, 0x7FFF
    cmplwi r3, 1
    bne skip_crc

check_pad:
    bl __check_pad3

skip_crc:
    bl __get_debug_bba
    cmplwi r3, 1
    bne skip_init
    bl InitMetroTRK_BBA

skip_init:
    bl __init_user

    mr r3, r14
    mr r4, r15
    bl main
    b exit
}

asm static void __init_registers(void) {
    nofralloc
    li      r0, 0x0
    li      r3, 0x0
    li      r4, 0x0
    li      r5, 0x0
    li      r6, 0x0
    li      r7, 0x0
    li      r8, 0x0
    li      r9, 0x0
    li      r10, 0x0
    li      r11, 0x0
    li      r12, 0x0
    li      r14, 0x0
    li      r15, 0x0
    li      r16, 0x0
    li      r17, 0x0
    li      r18, 0x0
    li      r19, 0x0
    li      r20, 0x0
    li      r21, 0x0
    li      r22, 0x0
    li      r23, 0x0
    li      r24, 0x0
    li      r25, 0x0
    li      r26, 0x0
    li      r27, 0x0
    li      r28, 0x0
    li      r29, 0x0
    li      r30, 0x0
    li      r31, 0x0
    lis     r1, _stack_addr@h 
    ori     r1, r1, _stack_addr@l
    lis     r2, _SDA2_BASE_@h
    ori     r2, r2, _SDA2_BASE_@l
    lis     r13, _SDA_BASE_@h
    ori     r13, r13, _SDA_BASE_@l
    blr
}

static void __copy_rom_section(void *pDest, const void *pSrc, u32 sectionSize) {
    if (sectionSize && (pDest != pSrc)) {
        memcpy(pDest, pSrc, sectionSize);
        __flush_cache(pDest, sectionSize);
    }
}

static void __init_bss_section(void *pDest, u32 size) {
    if (size != 0) {
        memset(pDest, 0, size);
    }
}

static void __init_data(void) {
    __rom_copy_info* romInfo;
    __bss_init_info* bssInfo;

    romInfo = _rom_copy_info;
    while (1) {
        if (romInfo->size == 0) {
            break;
        }

        __copy_rom_section(romInfo->addr, romInfo->rom, romInfo->size);
        romInfo++;
    }

    bssInfo = _bss_init_info;
    while (1) {
        if (bssInfo->size == 0) {
            break;
        }

        __init_bss_section(bssInfo->addr, bssInfo->size);
        bssInfo++;
    }
}
