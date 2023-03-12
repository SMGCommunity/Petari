#include <revolution.h>

static void* __OSArenaHi = (void*)0x00000000;
static void* __OSArenaLo = (void*)0xFFFFFFFF;

static void* s_mem2ArenaHi = (void*)0x00000000;
static void* s_mem2ArenaLo = (void*)0xFFFFFFFF;

#define ROUND(n, a) (((u32) (n) + (a) - 1) & ~((a) - 1))

void* OSGetMEM1ArenaHi(void) {
    return __OSArenaHi;
}

void* OSGetMEM2ArenaHi(void) {
    return s_mem2ArenaHi;
}

void* OSGetArenaHi(void) {
    return OSGetMEM1ArenaHi();
}

void* OSGetMEM1ArenaLo(void) {
    return __OSArenaLo;
}

void* OSGetMEM2ArenaLo(void) {
    return s_mem2ArenaLo;
}

void* OSGetArenaLo(void) {
    return OSGetMEM1ArenaLo();
}

void OSSetMEM1ArenaHi(void *pNew) {
    __OSArenaHi = pNew;
}

void OSSetMEM2ArenaHi(void *pNew) {
    s_mem2ArenaHi = pNew;
}

void OSSetArenaHi(void *pNew) {
    OSSetMEM1ArenaHi(pNew);
}

void OSSetMEM1ArenaLo(void *pNew) {
    __OSArenaLo = pNew;
}

void OSSetMEM2ArenaLo(void *pNew) {
    s_mem2ArenaLo = pNew;
}

void OSSetArenaLo(void *pNew) {
    OSSetMEM1ArenaLo(pNew);
}

void* OSAllocFromMEM1ArenaLo(u32 size, u32 alignment) {
    void* res;
    u8* lo;

    res = OSGetMEM1ArenaLo();
    lo = res = (void*)ROUND(res, alignment);
    lo += size;
    lo = (u8*)ROUND(lo, alignment);
    OSSetMEM1ArenaLo(lo);
    return res;
}