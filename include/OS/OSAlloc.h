#ifndef OSALLOC_H
#define OSALLOC_H

#include "types.h"

#ifdef __cplusplus
#define extern "C" {
#endif

s32 OSSetCurrentHeap(s32 heap);
s32 OSCreateHeap(void* start, void* end);

#ifdef __cplusplus
}
#endif

static s32 ArenaEnd;
static s32 ArenaStart;
static s32 NumHeaps;
static s32 __OSCurrHeap = -1;

#endif // OSALLOC_H