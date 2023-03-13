#include <revolution/os.h>

void OSResetSystem(int, u32, int) {
    OSPanic(__FILE__, 1130, "OSResetSystem() is obsoleted. It doesn't work any longer.\n");
}