#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/ModelObj.hpp"
#include "Game/Util.hpp"

class ChooChooTrain : public LiveActor {
public:
    ChooChooTrain(const char*);

    virtual void init(const JMapInfoIter&);
    virtual void startClipped();
    virtual void endClipped();
    virtual void control();

    MR::Vector< MR::AssignableArray< ModelObj* > > mModelArray;  // 0x8C
    TVec3f _98;
    f32 mRailCoord;     // 0xA4
    s32 mWhistleTimer;  // 0xA8
};