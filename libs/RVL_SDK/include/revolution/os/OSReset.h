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

#ifdef __cplusplus
}
#endif

#endif // OSRESET_H