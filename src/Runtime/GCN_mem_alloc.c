extern void OSReport(const char *, ...);
extern void* OSGetArenaLo();
extern void* OSGetArenaHi();
extern void* OSInitAlloc(void *, void *, int);
extern volatile int __OSCurrHeap;

#define OSRoundUp32B(x) (((unsigned int)(x) + 32 - 1) & ~(32 - 1))
#define OSRoundDown32B(x) (((unsigned int)(x)) & ~(32 - 1))

static void InitDefaultHeap()
{
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

// unused
void __sys_alloc()
{
}

__declspec(weak) extern void __sys_free(void* ptr)
{
	if (__OSCurrHeap == -1) {
		InitDefaultHeap();
	}
	OSFreeToHeap(__OSCurrHeap, ptr);
}
