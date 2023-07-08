#include <revolution/nwc24.h>

static int Opened = 0;

int NWC24IsMsgLibOpened(void) {
    return Opened == 1;
}

int NWC24IsMsgLibOpenedByTool(void) {
    return Opened == 2;
}

int NWC24IsMsgLibOpenBlocking(void) {
    return Opened == 3;
}