#include "Game/Util/SchedulerUtil.hpp"
#include <revolution/os.h>

MR::ProhibitSchedulerAndInterrupts::ProhibitSchedulerAndInterrupts(bool unused) {
    mInterruptsStatus = OSDisableInterrupts();
    OSDisableScheduler();
}

MR::ProhibitSchedulerAndInterrupts::~ProhibitSchedulerAndInterrupts() {
    OSEnableScheduler();
    OSYieldThread();
    OSRestoreInterrupts(mInterruptsStatus);
}
