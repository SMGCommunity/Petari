#ifndef SI_H
#define SI_H

#include <revolution/os.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*SICallback)(s32, u32, OSContext *);
typedef void (*SITypeAndStatusCallback)(s32, u32);

typedef struct SIPacket {
    s32 chan;
    void* output;
    u32 outputBytes;
    void* input;
    u32 inputBytes;
    SICallback callback;
    OSTime fire;
} SIPacket;

u32 SIGetType(s32);

u32 SISetXY(u32, u32);

void SISetSamplingRate(u32);
void SIRefreshSamplingRate(void);

#ifdef __cplusplus
}
#endif

#endif // SI_H