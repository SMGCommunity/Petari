#ifndef OSALLOC_H
#define OSALLOC_H

#include <revolution/types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef int OSHeapHandle;
extern volatile OSHeapHandle __OSCurrHeap;

void* OSInitAlloc(void *, void *, int);
OSHeapHandle OSCreateHeap(void *, void *);

OSHeapHandle OSSetCurrentHeap(OSHeapHandle);

void OSFreeToHeap(OSHeapHandle, void *);

#ifdef __cplusplus
}
#endif

#endif // OSALLOC_H