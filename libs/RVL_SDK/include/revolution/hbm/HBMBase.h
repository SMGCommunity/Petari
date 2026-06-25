#ifndef HBMBASE_H
#define HBMBASE_H

#include <revolution/kpad.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct HBMKPadData {
    /* 0x0 */ KPADStatus* kpad;
    /* 0x4 */ Vec2 pos;
    /* 0xC */ u32 use_devtype;
} HBMKPadData;


#ifdef __cplusplus
}
#endif

#endif  // HBMBASE_H
