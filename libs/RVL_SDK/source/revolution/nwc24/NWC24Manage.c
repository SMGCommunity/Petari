#include <revolution/nwc24.h>
#include <revolution/os.h>

static int Opened = 0;
static int Registered;

const char* __NWC24Version = "<< RVL_SDK - NWC24 \trelease build: Dec 10 2007 10:02:25 (0x4199_60831) >>";

void NWC24iRegister(void) {
    if (Registered == 0) {
        OSRegisterVersion(__NWC24Version);
        Registered = 1;
    }
}

int NWC24IsMsgLibOpened(void) {
    return Opened == 1;
}

int NWC24IsMsgLibOpenedByTool(void) {
    return Opened == 2;
}

int NWC24IsMsgLibOpenBlocking(void) {
    return Opened == 3;
}