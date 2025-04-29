#include "TRK_Types.h"
#include "portable/dserror.h"

static DSError TRK_mainError;

void TRK_main(void) {
    TRK_mainError = TRKInitializeNub();

    if (TRK_mainError == 0) {
        TRKNubWelcome();
        TRKNubMainLoop();
    }

    TRK_mainError = TRKTerminateNub();
}
