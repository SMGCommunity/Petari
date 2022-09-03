#include <revolution/os.h>

/* this inline was require for reg matches */
static inline void Init(void) {
    void* arenaLo;
    void* arenaHi;

    OSReport("GCN_Mem_Alloc.c : InitDefaultHeap. No Heap Available\n");
    OSReport("Metrowerks CW runtime library initializing default heap\n");
    arenaLo = OSGetArenaLo();
    arenaHi = OSGetArenaHi();
    arenaLo = OSInitAlloc(arenaLo, arenaHi, 1);
    OSSetArenaLo(arenaLo);

    arenaLo = (void*)OSRoundUp32B(arenaLo);
    arenaHi = (void*)OSRoundDown32B(arenaHi);
    OSSetCurrentHeap(OSCreateHeap(arenaLo, arenaHi));
    OSSetArenaLo(arenaLo = arenaHi);
}

void __sys__free(void *ptr) {
    if (__OSCurrHeap == -1) {
        Init();
    }

    OSFreeToHeap(__OSCurrHeap, ptr);
}