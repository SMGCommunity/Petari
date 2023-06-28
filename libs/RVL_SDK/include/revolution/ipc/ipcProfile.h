#ifndef IPCPROFILE_H
#define IPCPROFILE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <revolution.h>

void IPCiProfQueueReq(void *, s32);
void IPCiProfReply(void *, s32);

#ifdef __cplusplus
}
#endif

#endif // IPCPROFILE_H