#ifndef WENC_H
#define WENC_H

#ifdef __cplusplus
extern "C" {
#endif

#include <revolution/types.h>

typedef struct {
    u8 data[32];
} WENCInfo;

s32 WENCGetEncodeData(WENCInfo *, u32, const s16 *, s32, u8 *);

#ifdef __cplusplus
}
#endif

#endif // WENC_H