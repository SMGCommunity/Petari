#ifndef OSRESET_H
#define OSRESET_H

#include <revolution.h>

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

void OSResetSystem(int, u32, BOOL);
void OSRegisterShutdownFunction(OSShutdownFunctionInfo *);

u32 OSGetResetCode(void);

#define OSIsRestart()   \
            ((OSGetResetCode() & 0x80000000) ? TRUE : FALSE)

#ifdef __cplusplus
}
#endif

#endif // OSRESET_H