#pragma once

#include <JSystem/JGeometry/TVec.hpp>
#include <revolution/gx.h>

namespace MR {
    void ddSetVtxFormat(u32);
    void ddChangeTev();
    void ddSendVtxData(const TVec3f&, const TVec3f&, const TVec2f&);
    void ddSendVtxData(const TVec3f&, const TVec2f&);
    void ddLightingOn(u16);
    void ddLightingOff();
};  // namespace MR
