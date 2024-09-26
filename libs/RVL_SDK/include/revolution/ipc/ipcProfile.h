#ifndef IPCPROFILE_H
#define IPCPROFILE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <revolution.h>

void IPCiProfInit(void);
void IPCiProfQueueReq(void *, s32);
void IPCiProfReply(void *, s32);
void IPCiProfAck(void);

#ifdef __cplusplus
}
#endif

#endif // IPCPROFILE_H