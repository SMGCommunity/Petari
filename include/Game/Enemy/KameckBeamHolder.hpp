#pragma once

#include <JSystem/JGeometry/TVec.hpp>
#include <revolution.h>

class KameckBeamEventListener;
class KameckBeam;

namespace MR {
    void createKameckBeamHolder();
    void createKameckFireBallHolder();
    void createKameckBeamTurtleHolder();

    KameckBeam* startFollowKameckBeam(s32, MtxPtr, f32, const TVec3f&, KameckBeamEventListener*);
};  // namespace MR