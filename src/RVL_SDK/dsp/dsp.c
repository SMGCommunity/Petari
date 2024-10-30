#include <revolution/dsp.h>
#include <revolution/dsp/dsp_debug.h>
#include <revolution/dsp/dsp_task.h>
#include <private/flipper.h>

static BOOL __DSP_init_flag = 0;
extern DSPTaskInfo *__DSP_curr_task;
extern DSPTaskInfo *__DSP_first_task;
extern DSPTaskInfo *__DSP_last_task;
extern DSPTaskInfo *__DSP_tmp_task;

const char* __DSPVersion = "<< RVL_SDK - DSP \trelease build: Aug  8 2007 01:59:06 (0x4199_60831) >>";

u32 DSPCheckMailToDSP(void) {
    return ((__DSPRegs[0] & 0x8000) >> 15);
}

u32 DSPCheckMailFromDSP(void) {
    return ((__DSPRegs[2] & 0x8000) >> 15);
}

u32 DSPReadMailFromDSP(void) {
    return ((__DSPRegs[2]) << 16) | __DSPRegs[3];
}

void DSPSendMailToDSP(u32 mail) {
    __DSPRegs[0] = (mail >> 16);
    __DSPRegs[1] = mail;
}

void DSPAssertInt(void) {
    BOOL intr;
    u16 reg;

    intr = OSDisableInterrupts();
    reg = __DSPRegs[5];
    reg = (reg & ~0xA8 | 0x2);
    __DSPRegs[5] = reg;
    OSRestoreInterrupts(intr); 
}

void DSPInit(void) {
    BOOL intr;
    u16 reg;

    __DSP_debug_printf("DSPInit(): Build Date: %s %s\n", "Aug  8 2007", "01:59:06");

    if (__DSP_init_flag == 1) {
        return;
    }

    OSRegisterVersion(__DSPVersion);
    intr = OSDisableInterrupts();
    __OSSetInterruptHandler(7, __DSPHandler);
    __OSUnmaskInterrupts(0x1000000);

    reg = __DSPRegs[5];
    reg = ((reg & ~(0x20 | 0x8 | 0x80)) | 0x800);
    __DSPRegs[5] = reg;

    reg = __DSPRegs[5];
    reg = (reg & ~(0x20 | 0x8 | 0x80 | 0x4));
    __DSPRegs[5] = reg; 

    __DSP_first_task = __DSP_last_task = __DSP_curr_task = __DSP_tmp_task = NULL;
    __DSP_init_flag = 1;
    OSRestoreInterrupts(intr);
}