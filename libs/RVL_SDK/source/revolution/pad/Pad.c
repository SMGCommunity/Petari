#include <revolution/types.h>
#include <revolution/os.h>

u8  GameChoice : (OS_BASE_CACHED | 0x30E3);

u32 __PADSpec;

BOOL __PADDisableRecalibration(BOOL disable) {
    BOOL enabled, prev;

    enabled = OSDisableInterrupts();
    prev = (GameChoice & 0x40) ? TRUE : FALSE;
    GameChoice &= ~0x40;

    if (disable) {
        GameChoice |= 0x40;
    }

    OSRestoreInterrupts(enabled);
    return prev;
}