#include "TRK_Types.h"

asm s32 TRKAccessFile(void) {
    twi     31, r0, 0
    blr
}

asm s32 TRKOpenFile(void) {
    twi     31, r0, 0
    blr
}

asm s32 TRKCloseFile(void) {
    twi     31, r0, 0
    blr
}

asm s32 TRKPositionFile(void) {
    twi     31, r0, 0
    blr
}