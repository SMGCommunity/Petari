#include "revolution/os.h"

extern BOOL __OSInIPL;

typedef enum NWC24Err {
    NWC24_OK                    = 0,
} NWC24Err;

NWC24Err NWC24iPrepareShutdown(void);
NWC24Err NWC24iSynchronizeRtcCounter(BOOL);
s32 NWC24SuspendScheduler(void);

void __OSInitNet(void) {
    NWC24Err cur_err;
    s32 res;
    OSIOSRev iosRev;

    __OSGetIOSRev(&iosRev);

    if (iosRev.major <= 4 || iosRev.major == 9) {
        return;
    }

    cur_err = NWC24iPrepareShutdown();

    if (cur_err != NWC24_OK) {
        if (cur_err < NWC24_OK) {
            OSReport("Failed to register network shutdown function. %d\n", cur_err);
        }

        res = NWC24SuspendScheduler();
        if (res < NWC24_OK) {
            OSReport("Failed to suspend the WiiConnect24 scheduler. %d\n", res);
        }
    }

    if (!__OSInIPL) {
        cur_err = NWC24iSynchronizeRtcCounter(0);
        
        if (cur_err != NWC24_OK) {
            OSReport("Failed to synchronize time with network resource managers. %d\n", cur_err);
        }
    }
}