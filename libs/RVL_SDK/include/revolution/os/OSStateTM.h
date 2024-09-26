#ifndef OSSTATETM_H
#define OSSTATETM_H

#include <revolution/types.h>

#ifdef __cplusplus
extern "C" {
#endif

s32 __OSSetIdleLEDMode(u32);
s32 __OSUnRegisterStateEvent(void);

#ifdef __cplusplus
}
#endif

#endif // OSSTATETM_H