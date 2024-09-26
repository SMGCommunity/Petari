#ifndef OSEXECPARAMS_H
#define OSEXECPARAMS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <revolution/types.h>

typedef struct {
    BOOL valid;
    u32 restartCode;
    u32 bootDol;
    void* regionStart;
    void* regionEnd;
    BOOL argsUseDefault;
    void* argsAddr;
} OSExecParams;

void __OSGetExecParams(OSExecParams *);

#ifdef __cplusplus
}
#endif

#endif // OSEXECPARAMS_H