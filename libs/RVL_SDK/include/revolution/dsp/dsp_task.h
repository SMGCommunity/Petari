#ifndef DSP_TASK_H
#define DSP_TASK_H

#ifdef __cplusplus
extern "C" {
#endif

#include <revolution/dsp.h>
#include <revolution/os.h>

void __DSPHandler(__OSInterrupt, OSContext *);

void __DSP_exec_task(DSPTaskInfo *, DSPTaskInfo *);
void __DSP_boot_task(DSPTaskInfo *);
void __DSP_remove_task(DSPTaskInfo *);

#ifdef __cplusplus
}
#endif

#endif // DSP_TASK_H