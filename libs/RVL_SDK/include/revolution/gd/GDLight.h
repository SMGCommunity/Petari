#ifndef GDLIGHT_H
#define GDLIGHT_H

#include "revolution/gx/GXEnum.h"
#include "revolution/types.h"

#ifdef __cplusplus
extern "C" {
#endif

#define XF_AMBIENT0_ID 0x100a
#define XF_MATERIAL0_ID 0x100c
#define XF_COLOR0CNTRL_ID 0x100e

#define XF_COLOR0CNTRL_MATERIAL_SRC_SHIFT 0
#define XF_COLOR0CNTRL_LIGHTFUNC_SHIFT 1
#define XF_COLOR0CNTRL_LIGHT0_SHIFT 2
#define XF_COLOR0CNTRL_LIGHT1_SHIFT 3
#define XF_COLOR0CNTRL_LIGHT2_SHIFT 4
#define XF_COLOR0CNTRL_LIGHT3_SHIFT 5
#define XF_COLOR0CNTRL_AMBIENT_SRC_SHIFT 6
#define XF_COLOR0CNTRL_DIFFUSEATTEN_SHIFT 7
#define XF_COLOR0CNTRL_ATTENENABLE_SHIFT 9
#define XF_COLOR0CNTRL_ATTENSELECT_SHIFT 10
#define XF_COLOR0CNTRL_LIGHT4_SHIFT 11
#define XF_COLOR0CNTRL_LIGHT5_SHIFT 12
#define XF_COLOR0CNTRL_LIGHT6_SHIFT 13
#define XF_COLOR0CNTRL_LIGHT7_SHIFT 14
#define XF_COLOR0CNTRL_F(material_src, lightfunc, light0, light1, light2, light3, ambient_src, diffuseatten, attenenable, attenselect, light4,       \
                         light5, light6, light7)                                                                                                     \
    ((((unsigned long)(material_src)) << XF_COLOR0CNTRL_MATERIAL_SRC_SHIFT) | (((unsigned long)(lightfunc)) << XF_COLOR0CNTRL_LIGHTFUNC_SHIFT) |     \
     (((unsigned long)(light0)) << XF_COLOR0CNTRL_LIGHT0_SHIFT) | (((unsigned long)(light1)) << XF_COLOR0CNTRL_LIGHT1_SHIFT) |                       \
     (((unsigned long)(light2)) << XF_COLOR0CNTRL_LIGHT2_SHIFT) | (((unsigned long)(light3)) << XF_COLOR0CNTRL_LIGHT3_SHIFT) |                       \
     (((unsigned long)(ambient_src)) << XF_COLOR0CNTRL_AMBIENT_SRC_SHIFT) | (((unsigned long)(diffuseatten)) << XF_COLOR0CNTRL_DIFFUSEATTEN_SHIFT) | \
     (((unsigned long)(attenenable)) << XF_COLOR0CNTRL_ATTENENABLE_SHIFT) | (((unsigned long)(attenselect)) << XF_COLOR0CNTRL_ATTENSELECT_SHIFT) |   \
     (((unsigned long)(light4)) << XF_COLOR0CNTRL_LIGHT4_SHIFT) | (((unsigned long)(light5)) << XF_COLOR0CNTRL_LIGHT5_SHIFT) |                       \
     (((unsigned long)(light6)) << XF_COLOR0CNTRL_LIGHT6_SHIFT) | (((unsigned long)(light7)) << XF_COLOR0CNTRL_LIGHT7_SHIFT))

#define XF_COLOR0CNTRL_F_PS(material_src, lightfunc, light3210, ambient_src, diffuseatten, attenenable, attenselect, light7654)                      \
    ((((unsigned long)(material_src)) << XF_COLOR0CNTRL_MATERIAL_SRC_SHIFT) | (((unsigned long)(lightfunc)) << XF_COLOR0CNTRL_LIGHTFUNC_SHIFT) |     \
     (((unsigned long)(light3210)) << XF_COLOR0CNTRL_LIGHT0_SHIFT) | (((unsigned long)(ambient_src)) << XF_COLOR0CNTRL_AMBIENT_SRC_SHIFT) |          \
     (((unsigned long)(diffuseatten)) << XF_COLOR0CNTRL_DIFFUSEATTEN_SHIFT) | (((unsigned long)(attenenable)) << XF_COLOR0CNTRL_ATTENENABLE_SHIFT) | \
     (((unsigned long)(attenselect)) << XF_COLOR0CNTRL_ATTENSELECT_SHIFT) | (((unsigned long)(light7654)) << XF_COLOR0CNTRL_LIGHT4_SHIFT))

inline static u16 __GDLightID2Index(GXLightID id) {
    u16 idx;

    idx = 0x1F - __cntlzw(id);
    if (idx > 7) {
        idx = 0;
    }
    return idx;
}

static u16 __GDLightID2Offset(GXLightID id) {
    return __GDLightID2Index(id) * 16;
}

#ifdef __cplusplus
}
#endif

#endif  // GDLIGHT_H
