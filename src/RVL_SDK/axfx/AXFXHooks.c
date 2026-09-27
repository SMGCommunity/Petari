#include <cstdio>
#include "revolution/axfx.h"
#include "revolution/os.h"
#include "revolution/os/OSAlloc.h"

static void* __AXFXAllocFunction(size_t size);
static void __AXFXFreeFunction(void* block);

AXFXAllocHook __AXFXAlloc = __AXFXAllocFunction;
AXFXFreeHook __AXFXFree = __AXFXFreeFunction;

static void* __AXFXAllocFunction(size_t size) {
    return OSAllocFromHeap(__OSCurrHeap, size);
}

static void __AXFXFreeFunction(void* block) {
    OSFreeToHeap(__OSCurrHeap, block);
}

// These SDK entry points are retained in the retail binary for the product.sel export symbol table.
#pragma push
#pragma force_active on
void AXFXSetHooks(AXFXAllocHook alloc, AXFXFreeHook free) {
    __AXFXAlloc = alloc;
    __AXFXFree = free;
}

void AXFXGetHooks(AXFXAllocHook* alloc, AXFXFreeHook* free) {
    *alloc = __AXFXAlloc;
    *free = __AXFXFree;
}
#pragma pop
