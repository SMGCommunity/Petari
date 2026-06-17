/**
 * target_options.c
 * Description:
 */

#include "Os/dolphin/target_options.h"

static u8 bUseSerialIO;

void SetUseSerialIO(u8 serialIO) {
    bUseSerialIO = serialIO;
}

u8 GetUseSerialIO(void) {
    return bUseSerialIO;
}
