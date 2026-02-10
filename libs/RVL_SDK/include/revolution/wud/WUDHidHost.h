#ifndef RVL_SDK_WUD_HID_HOST_H
#define RVL_SDK_WUD_HID_HOST_H
#include <revolution/bte.h>
#include <revolution/types.h>


#ifdef __cplusplus
extern "C" {
#endif

void WUDHidHostCallback(tBTA_HH_EVT event, tBTA_HH* pData);

#ifdef __cplusplus
}
#endif
#endif
