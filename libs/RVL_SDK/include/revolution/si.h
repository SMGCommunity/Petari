#ifndef SI_H
#define SI_H

#include <revolution/os.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void (* SICallback )(s32, u32, OSContext *);

typedef struct SIPacket {
    s32 chan;
    void* output;
    u32 outputBytes;
    void* input;
    u32 inputBytes;
    SICallback callback;
    OSTime fire;
} SIPacket;

#ifdef __cplusplus
}
#endif

#endif // SI_H