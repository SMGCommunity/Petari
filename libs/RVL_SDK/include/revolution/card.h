#ifndef RVL_SDK_CARD_H
#define RVL_SDK_CARD_H
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef struct CARDFileInfo {
    s32 chan;    // at 0x0
    s32 fileNo;  // at 0x4
    s32 offset;  // at 0x8
    s32 length;  // at 0xC
    u16 iBlock;  // at 0x10
    u16 padding; // at 0x12
} CARDFileInfo;

#ifdef __cplusplus
}
#endif
#endif