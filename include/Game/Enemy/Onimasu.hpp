#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include <JSystem/JGeometry.hpp>

class Onimasu : public LiveActor {
public:
    Onimasu(const char*);
    virtual ~Onimasu();

    TPos3f _8C;
    f32 _BC;
    f32 _C0;
    f32 _C4;
    TQuat4f _C8;
    TQuat4f _D8;
    TQuat4f _E8;
    TVec3f _F8;
};

namespace OnimasuFunction {
    bool getPolygonOnRailPoint(TVec3f*, TVec3f*, const LiveActor*, int);
};
