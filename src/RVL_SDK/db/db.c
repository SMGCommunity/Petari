#include <revolution/db.h>
#include <revolution/os.h>

DBInterface* __DBInterface = NULL;
BOOL DBVerbose;

void __DBExceptionDestination(void);

void DBInit(void) {
    __DBInterface = (DBInterface*)OSPhysicalToCached(0x40);
    __DBInterface->exceptionDestination = (void*)OSCachedToPhysical(__DBExceptionDestination);
    DBVerbose = 1;
}

void __DBExceptionDestinationAux(void) {
    u32* context_addr = (void*)0xC0;
    OSContext* context = (OSContext*)(0x80000000 + *context_addr);
    OSReport("DBExceptionDestination\n");
    OSDumpContext(context);
    PPCHalt();
}

asm void __DBExceptionDestination(void) {
    nofralloc
    mfmsr r3
    ori r3, r3, 0x30
    mtmsr r3
    b __DBExceptionDestinationAux
}

BOOL __DBIsExceptionMarked(__OSException ex) {
    return __DBInterface->mask & (1 << ex);
}

void DBPrintf(char *, ...) {
    va_list list;
}