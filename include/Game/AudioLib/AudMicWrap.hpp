#pragma once

#include <JSystem/JGeometry/TVec.hpp>

namespace AudMicWrap {
    void setMicMtx(MtxPtr, s32);
    void setMicEnv();
    const TVec3f& getMicPos();
};  // namespace AudMicWrap
