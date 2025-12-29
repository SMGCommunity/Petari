/**
 * targcont.c
 * Description:
 */

#include "Os/dolphin/targcont.h"

DSError TRKTargetContinue(void) {
    TRKTargetSetStopped(0);
    UnreserveEXI2Port();
    TRKSwapAndGo();
    ReserveEXI2Port();
    return 0;
}
