#ifndef WENC_H
#define WENC_H

#ifdef __cplusplus
extern "C" {
#endif

#include <revolution/types.h>

typedef enum {
    WENC_FLAG_USER_INFO = (1 << 0),
} WENCFlag;

typedef struct WENCInfo {
    s32 xn;         // at 0x0
    s32 dl;         // at 0x4
    s32 qn;         // at 0x8
    s32 dn;         // at 0xC
    s32 dlh;        // at 0x10
    s32 dlq;        // at 0x14
    u8 padding[8];  // at 0x18
} WENCInfo;

s32 WENCGetEncodeData(WENCInfo*, u32, const s16*, s32, u8*);

#ifdef __cplusplus
}
#endif

#endif  // WENC_H
