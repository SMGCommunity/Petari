#include <revolution.h>

static s32 Reschedule;

#ifdef __cplusplus
extern "C" {
#endif

BOOL OSIsThreadSuspended(OSThread *pThread) {
    return (-pThread->suspend & ~pThread->suspend) >> 31;
}

BOOL OSIsThreadTerminated(OSThread *pThread) {
    OS_THREAD_STATE state = (OS_THREAD_STATE)pThread->state;
    BOOL ret = true;
    if (state != OS_THREAD_STATE_MORIBUND && state) {
        ret = false;
    }

    return (-ret | ret) >> 31;
}

s32 OSDisableScheduler() {
    BOOL level = OSDisableInterrupts();
    s32 rescheduleCount = Reschedule++;
    OSRestoreInterrupts(level);
    return rescheduleCount;
}

s32 OSEnableScheduler() {
    BOOL level = OSDisableInterrupts();
    s32 rescheduleCount = Reschedule--;
    OSRestoreInterrupts(level);
    return rescheduleCount;
}

#ifdef __cplusplus
}
#endif