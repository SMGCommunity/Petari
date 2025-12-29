/**
 * main_TRK.c
 * Description:
 */

#include "MetroTRK/Portable/main_TRK.h"

static DSError TRK_mainError;

DSError TRK_main(void) {
    TRK_mainError = TRKInitializeNub();

    if (TRK_mainError == DS_NoError) {
        TRKNubWelcome();
        TRKNubMainLoop();
    }

    TRK_mainError = TRKTerminateNub();
    return TRK_mainError;
}
