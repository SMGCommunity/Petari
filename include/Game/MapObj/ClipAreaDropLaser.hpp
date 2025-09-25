#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/NameObj/NameObj.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "JSystem/JGeometry/TVec.hpp"
#include <cstddef>

class ClipAreaDropLaser : public LiveActor {
public: 
    ClipAreaDropLaser(const char* pName);
    virtual ~ClipAreaDropLaser();
    virtual void init(const JMapInfoIter& rIter);
    virtual void control();
    virtual void draw() const;
    void exeWait();
    void exeMove();
    void incrementDrawCount();

    TVec3f _8C[0x40];
    f32 _38C;
    int _390;
    int _394;
    f32 _398;
};

namespace NrvClipAreaDropLaser {
    NERVE_DECL_EXE(ClipAreaDropLaserNrvWait, ClipAreaDropLaser, Wait);
    NERVE_DECL_EXE(ClipAreaDropLaserNrvMove, ClipAreaDropLaser, Move);
};
