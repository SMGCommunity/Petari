#pragma once

#include <revolution.h>
#include <JSystem/JGeometry/TVec.hpp>

class KameckBeamEventListener;
class KameckBeam;

namespace MR {
    KameckBeam* startFollowKameckBeam(s32, MtxPtr, f32, const TVec3f &, KameckBeamEventListener *);
};