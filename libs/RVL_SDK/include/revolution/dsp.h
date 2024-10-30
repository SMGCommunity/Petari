#ifndef DSP_H
#define DSP_H

#ifdef __cplusplus
extern "C" {
#endif

#include <revolution/types.h>
#include <revolution/os.h>

typedef void (*DSPCallback)(void *);

typedef struct DSPTaskStruct {
    u32 state;
    u32 priority;
    u32 flags;
    u16* iram_mmem_addr;
    u32 iram_length;
    u32 iram_addr;
    u16* dram_mem_addr;
    u32 dram_len;
    u32 dram_addr;
    u16 dsp_vector;
    u16 dsp_res_vector;
    DSPCallback init_cb;
    DSPCallback res_cb;
    DSPCallback done_cb;
    DSPCallback req_cb;
    struct DSPTaskStruct* next;
    struct DSPTaskStruct* prev;
    OSTime context;
    OSTime task;
} DSPTaskInfo;

u32 DSPCheckMailToDSP(void);
u32 DSPCheckMailFromDSP(void);
u32 DSPReadMailFromDSP(void);
void DSPSendMailToDSP(u32);
void DSPAssertInt(void);
void DSPInit(void);

#ifdef __cplusplus
}
#endif

#endif // DSP_H
