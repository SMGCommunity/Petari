#include "revolution/ipc/ipcclt.h"
#include "revolution/ipc/ipcProfile.h"
#include "revolution/ipc/memory.h"

extern u32 IPCReadReg(u32);

void IPCInit(void);

extern void* IPCGetBufferHi(void);
extern void* IPCGetBufferLo(void);
extern void IPCSetBufferLo(void *);