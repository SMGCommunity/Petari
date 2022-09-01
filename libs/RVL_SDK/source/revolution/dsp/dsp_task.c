#include <revolution/dsp/dsp_task.h>
#include <revolution/dsp/dsp_debug.h>

#define WAIT_FOR_MAIL  \
    while (DSPCheckMailTDSP()) { }

DSPTaskInfo* __DSP_curr_task;
DSPTaskInfo* __DSP_first_task;
DSPTaskInfo* __DSP_last_task;
DSPTaskInfo* __DSP_tmp_task;

void __DSP_exec_task(DSPTaskInfo *pCurrent, DSPTaskInfo *pNext) {
    if (pCurrent != NULL) {
        DSPSendMailToDSP((u32)pCurrent->dram_mem_addr);
        WAIT_FOR_MAIL
        DSPSendMailToDSP(pCurrent->dram_len);
        WAIT_FOR_MAIL
        DSPSendMailToDSP(pCurrent->dram_addr);
        WAIT_FOR_MAIL
    }
    else {
        DSPSendMailToDSP(0);
        WAIT_FOR_MAIL
        DSPSendMailToDSP(0);
        WAIT_FOR_MAIL
        DSPSendMailToDSP(0);
        WAIT_FOR_MAIL
    }

    DSPSendMailToDSP((u32)pNext->iram_mmem_addr);
    WAIT_FOR_MAIL
    DSPSendMailToDSP(pNext->iram_length);
    WAIT_FOR_MAIL
    DSPSendMailToDSP(pNext->iram_addr);
    WAIT_FOR_MAIL

    if (pNext->state == 0) {
        DSPSendMailToDSP((u32)pNext->dsp_vector);
        WAIT_FOR_MAIL
        DSPSendMailToDSP(0);
        WAIT_FOR_MAIL
        DSPSendMailToDSP(0);
        WAIT_FOR_MAIL
        DSPSendMailToDSP(0);
        WAIT_FOR_MAIL
    }
    else {
        DSPSendMailToDSP((u32)pNext->dsp_res_vector);
        WAIT_FOR_MAIL
        DSPSendMailToDSP((u32)pNext->dram_mem_addr);
        WAIT_FOR_MAIL
        DSPSendMailToDSP(pNext->dram_len);
        WAIT_FOR_MAIL
        DSPSendMailToDSP(pNext->dram_addr);
        WAIT_FOR_MAIL
    }
}

void __DSP_boot_task(DSPTaskInfo *pTask) {
    // volatile because it's stored in stack but never used
    volatile u32 mail;

    while (!DSPCheckMailFromDSP()) { }
    mail = DSPRealMailFromDSP();

    DSPSendMailToDSP(0x80F3A001);
    WAIT_FOR_MAIL
    DSPSendMailToDSP((u32)(pTask->iram_mmem_addr));
    WAIT_FOR_MAIL
    DSPSendMailToDSP(0x80F3C002);
    WAIT_FOR_MAIL
    DSPSendMailToDSP((u32)(pTask->iram_addr & 0xFFFF));
    WAIT_FOR_MAIL
    DSPSendMailToDSP(0x80F3A002);
    WAIT_FOR_MAIL
    DSPSendMailToDSP(pTask->iram_length);
    WAIT_FOR_MAIL
    DSPSendMailToDSP(0x80F3B002);
    WAIT_FOR_MAIL
    DSPSendMailToDSP(0);
    WAIT_FOR_MAIL
    DSPSendMailToDSP(0x80F3D001);
    WAIT_FOR_MAIL
    DSPSendMailToDSP((u32)(0xFFFF & pTask->dsp_vector));
    WAIT_FOR_MAIL

    __DSP_debug_printf("DSP is booting task: 0x%08X\n", pTask);
    __DSP_debug_printf("__DSP_boot_task()  : IRAM MMEM ADDR: 0x%08X\n", (u32)(pTask->iram_mmem_addr));
    __DSP_debug_printf("__DSP_boot_task()  : IRAM DSP ADDR : 0x%08X\n", pTask->iram_addr);
    __DSP_debug_printf("__DSP_boot_task()  : IRAM LENGTH   : 0x%08X\n", pTask->iram_length);
    __DSP_debug_printf("__DSP_boot_task()  : DRAM MMEM ADDR: 0x%08X\n", pTask->dram_len);
    __DSP_debug_printf("__DSP_boot_task()  : Start Vector  : 0x%08X\n", (u32)(pTask->dsp_vector));
}

void __DSP_remove_task(DSPTaskInfo *pTask) {
    pTask->flags = 0;
    pTask->state = 3;

    if (__DSP_first_task == pTask) {
        if (pTask->next != NULL) {
            __DSP_first_task = pTask->next;
            pTask->next->prev = NULL;
        }
        else {
            __DSP_first_task = __DSP_last_task = __DSP_curr_task = NULL;
        }
    }
    else if (__DSP_last_task == pTask) {
        __DSP_last_task = pTask->prev;
        pTask->prev->next = NULL;
        __DSP_curr_task = __DSP_first_task;
    }
    else {
        __DSP_curr_task = pTask->next;
        pTask->prev->next = pTask->next;
        pTask->next->prev = pTask->prev;
    }
}