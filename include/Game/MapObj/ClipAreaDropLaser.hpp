#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/JMapInfo.hpp"
#include <JSystem/JGeometry/TVec.hpp>
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

    /* 0x08C */ TVec3f mPoints[0x40];
    /* 0x38C */ f32 mNumPointsToDraw;
    /* 0x390 */ int mPointIndexToSkipDraw;
    /* 0x394 */ int mDrawCount;
    /* 0x398 */ f32 mSpeed;
};
