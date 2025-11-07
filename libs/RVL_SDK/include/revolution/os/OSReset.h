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

void OSRegisterShutdownFunction(OSShutdownFunctionInfo *);

void OSRebootSystem(void);
void OSShutdownSystem(void);
void OSRestart(u32);
void OSReturnToMenu(void);

u32 OSGetResetCode(void);

void OSResetSystem(int, u32, BOOL);

BOOL __OSCallShutdownFunctions(BOOL, u32);
void __OSReturnToMenuForError(void);

u8 __OSGetDiscState(u8);
void __OSShutdownDevices(u32);
void __OSHotResetForError(void);

#define OSIsRestart()   \
            ((OSGetResetCode() & 0x80000000) ? TRUE : FALSE)

#ifdef __cplusplus
}
#endif

#endif // OSRESET_H
