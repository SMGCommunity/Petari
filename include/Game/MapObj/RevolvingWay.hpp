#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "JSystem/JGeometry/TQuat.hpp"
#include "JSystem/JGeometry/TVec.hpp"

class RevolvingWay : public LiveActor {
public:
    RevolvingWay(const char*);

    virtual ~RevolvingWay();
    virtual void init(const JMapInfoIter&);
    virtual void control();
    virtual void calcAndSetBaseMtx();

    void exeWait();
    void addAccelMoment();

    TQuat4f _8C;
    TVec3f _9C;
    f32 _A8;
};
