#include <revolution.h>
#include <revolution/os.h>
#include <__ppc_eabi_init.h>
#include <__ppc_eabi_linker.h>

#pragma section code_type ".init"

#ifdef __cplusplus
extern "C" {
#endif

u16 Pad3Button : 0x800030E4;
static u8 Debug_BBA = 0;

static void __init_registers(void);

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

#ifdef __cplusplus
}
#endif