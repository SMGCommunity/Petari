#include "TRK_Types.h"
#include "portable/dserror.h"

static DSError TRK_mainError;

void TRK_main(void) {
    TRK_mainError = TRK_InitializeNub();

    if (TRK_mainError == 0) {
        TRK_NubWelcome();
        TRK_NubMainLoop();
    }

    TRK_mainError = TRK_TerminateNub();
}