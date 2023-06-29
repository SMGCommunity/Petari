#pragma once

#include "private/iostypes.h"

IOSError iosFree(IOSHeapId, void *);
void* iosAlloc(IOSHeapId, u32);
void* iosAllocAligned(IOSHeapId, u32, u32);

IOSHeapId iosCreateHeap(void *, u32);