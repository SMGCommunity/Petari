#ifndef OSRESET_H
#define OSRESET_H

#include <revolution/types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef BOOL (*OSShutdownFunction)(BOOL, u32);
typedef struct OSShutdownFunctionInfo OSShutdownFunctionInfo;

struct OSShutdownFunctionInfo {
    OSShutdownFunction func;
    u32 priority;
    OSShutdownFunctionInfo* next;
    OSShutdownFunctionInfo* prev;
};

void OSReturnToMenu(void);
void OSResetSystem(int, u32, BOOL);
void OSRegisterShutdownFunction(OSShutdownFunctionInfo *);
BOOL __OSCallShutdownFunctions(BOOL, u32);
void __OSReturnToMenuForError(void);

u32 OSGetResetCode(void);

u8 __OSGetDiscState(u8);
void __OSShutdownDevices(u32);
void __OSHotResetForError(void);

#define OSIsRestart()   \
            ((OSGetResetCode() & 0x80000000) ? TRUE : FALSE)

#ifdef __cplusplus
}
#endif

#endif // OSRESET_H
