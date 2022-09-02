#ifndef HEAPCOMMON_H
#define HEAPCOMMON_H

#ifdef __cplusplus
extern "C" {
#endif

#include <revolution/os.h>
#include <revolution/mem/list.h>

typedef struct MEMiHeapHead MEMiHeapHead;

struct MEMiHeapHead {
    u32 signature;
    MEMLink link;
    MEMList childList;
    void* heapStart;
    void* heapEnd;
    OSMutex mutex;

    union {
        u32 val;

        struct {
            u32 reserved : 24;
            u32 optFlag : 8;
        } fields;
    } attribute;
};

typedef MEMiHeapHead* MEMHeapHandle;

typedef u32 UIntPtr;

static inline UIntPtr GetUIntPtr( const void* ptr )
{
    return (UIntPtr)(ptr);
}

#ifdef __cplusplus
}
#endif

#endif // HEAPCOMMON_H