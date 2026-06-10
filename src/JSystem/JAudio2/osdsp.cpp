#include "JSystem/JAudio2/osdsp.hpp"
#include "JSystem/JAudio2/osdsp_task.hpp"
#include <revolution/dsp.h>
#include <revolution/os.h>

// NOTE: unfinished

extern "C" void __DSP_boot_task(DSPTaskInfo*);

void DSPAddPriorTask(DSPTaskStruct* task) {
    if (DSP_prior_task != nullptr) {
        OSReport("Already inited prior DSP task\n");
        return;
    }
    BOOL status = OSDisableInterrupts();
    DSP_prior_task = (DSPTaskInfo*)task;
    task->state = 0;
    task->flags = 1;
    __DSP_boot_task((DSPTaskInfo*)task);
    OSRestoreInterrupts(status);
}
