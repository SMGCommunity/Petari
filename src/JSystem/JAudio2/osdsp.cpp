#include "JSystem/JAudio2/osdsp.hpp"
#include "JSystem/JAudio2/osdsp_task.hpp"
#include <revolution/dsp.h>
#include <revolution/os.h>

extern "C" void __DSP_boot_task(DSPTaskInfo*);

void DSPAddPriorTask(DSPTaskStruct* pTask) {
    if (DSP_prior_task != nullptr) {
        OSReport("Already inited prior DSP task\n");
        return;
    }

    BOOL status = OSDisableInterrupts();
    DSP_prior_task = pTask;
    pTask->state = 0;
    pTask->flags = 1;
    __DSP_boot_task(pTask);
    OSRestoreInterrupts(status);
}
