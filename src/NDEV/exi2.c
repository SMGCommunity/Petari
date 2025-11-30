/* slight modified from https://github.com/doldecomp/ogws/blob/master/src/revolution/NdevExi2AD/exi2.c */

#include <revolution/exi.h>
#include <revolution/os.h>

static BOOL __EXI2Select(void) {
    u32 cpr = EXI_CHAN_PARAMS[2].cpr;
    EXI_CHAN_PARAMS[2].cpr = ((cpr & (EXI_CPR_EXIINTMASK | EXI_CPR_TCINTMASK | EXI_CPR_EXTINTMASK)) | 0x40 | EXI_CPR_CS0B);

    return TRUE;
}

static BOOL __EXI2Deselect(void) {
    u32 cpr = EXI_CHAN_PARAMS[2].cpr;

    EXI_CHAN_PARAMS[2].cpr = cpr & (EXI_CPR_EXIINTMASK | EXI_CPR_TCINTMASK | EXI_CPR_EXTINTMASK);

    return TRUE;
}

static BOOL __EXI2Sync(void) {
    while (EXI_CHAN_PARAMS[2].cr & 1) {
        ;
    }

    return TRUE;
}

BOOL __EXI2Imm(void* mem, s32 size, u32 type) {
    int i;

    if (type != 0) {
        u32 imm = 0;

        for (i = 0; i < size; i++) {
            const u8* bmem = (const u8*)mem;
            imm |= bmem[i] << (3 - i) * 8;
        }

        EXI_CHAN_PARAMS[2].data = imm;
    }

    EXI_CHAN_PARAMS[2].cr = type << 2 | EXI_CR_TSTART | (size - 1) * 16;

    __EXI2Sync();

    if (type == 0) {
        u32 imm = EXI_CHAN_PARAMS[2].data;

        u8* bmem = (u8*)mem;
        for (i = 0; i < size; i++, bmem++) {
            *bmem = imm >> (3 - i) * 8;
        }
    }

    return TRUE;
}

void __DBEXIInit(void) {
    u32 val0, val1;

    __OSMaskInterrupts(0x18000);
    while ((EXI_CHAN_PARAMS[2].cr & 1) == 1) {
    }
    EXI_CHAN_PARAMS[2].cpr = 0;

    val0 = 0xB4000000;
    val1 = 0xD4000000;

    __EXI2Select();
    __EXI2Imm(&val0, 4, 1);
    __EXI2Sync();

    __EXI2Imm(&val1, 4, 1);
    __EXI2Sync();
    __EXI2Deselect();
}

BOOL __DBEXIReadReg(u32 cmd, void* mem, s32 size) {
    BOOL error = FALSE;
    u32 read_val = 0;

    error = error | !__EXI2Select();
    error = error | !__EXI2Imm(&cmd, sizeof(cmd), 1);
    error = error | !__EXI2Sync();
    error = error | !__EXI2Imm(&read_val, sizeof(read_val), 0);
    error = error | !__EXI2Sync();
    error = error | !__EXI2Deselect();

    switch (size) {
    case 1:
        *(u8*)mem = read_val >> 24;
        break;
    case 2:
        *(u16*)mem = read_val >> 24 | read_val >> 8 & 0xFF00;
        break;
    default:
        *(u32*)mem = __EXISwap32(read_val);
        break;
    }

    return error == FALSE;
}

BOOL __DBEXIWriteReg(u32 cmd, const void* mem, s32 size) {
    BOOL error = FALSE;
    u32 write_val = 0;

    switch (size) {
    case 1:
        write_val = *(u8*)mem << 24;
        break;
    case 2:
        write_val = *(u16*)mem << 24 | (*(u16*)mem & 0xFF00) << 8;
        break;
    default:
        write_val = __EXISwap32(*(u32*)mem);
        break;
    }

    error = error | !__EXI2Select();
    error = error | !__EXI2Imm(&cmd, sizeof(cmd), 1);
    error = error | !__EXI2Sync();
    error = error | !__EXI2Imm(&write_val, sizeof(write_val), 1);
    error = error | !__EXI2Sync();
    error = error | !__EXI2Deselect();

    return error == FALSE;
}

BOOL __DBEXIReadRam(u32 cmd, void* mem, s32 size) {
    BOOL error = FALSE;
    u32 read_val;
    u32* lmem = (u32*)mem;

    error = error | !__EXI2Select();
    error = error | !__EXI2Imm(&cmd, sizeof(cmd), 1);
    error = error | !__EXI2Sync();

    for (; size > 0; size -= 4) {
        error = error | !__EXI2Imm(&read_val, sizeof(read_val), 0);
        error = error | !__EXI2Sync();
        *lmem++ = read_val;
    }

    error = error | !__EXI2Deselect();

    return error == FALSE;
}

BOOL __DBEXIWriteRam(u32 cmd, const void* mem, s32 size) {
    BOOL error = FALSE;
    u32* lmem = (u32*)mem;

    error = error | !__EXI2Select();
    error = error | !__EXI2Imm(&cmd, sizeof(cmd), 1);
    error = error | !__EXI2Sync();

    for (; size > 0; size -= 4) {
        u32 write_val = *lmem++;
        error = error | !__EXI2Imm(&write_val, sizeof(write_val), 1);
        error = error | !__EXI2Sync();
    }

    error = error | !__EXI2Deselect();

    return error == FALSE;
}
