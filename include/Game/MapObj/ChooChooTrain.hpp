#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/ModelObj.hpp"
#include "Game/Util/Array.hpp"

class ChooChooTrain : public LiveActor {
public:
    ChooChooTrain(const char*);

    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x40 */ virtual void startClipped();
    /* 0x44 */ virtual void endClipped();
    /* 0x48 */ virtual void control();

    MR::Vector< MR::AssignableArray< ModelObj* > > mModelArray;  // 0x8C
    TVec3f _98;
    f32 mRailCoord;     // 0xA4
    s32 mWhistleTimer;  // 0xA8
};