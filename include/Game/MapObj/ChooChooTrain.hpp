#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Util/Array.hpp"

class ModelObj;

class ChooChooTrain : public LiveActor {
public:
    /// @brief Creates a new `ChooChooTrain`.
    /// @param pName A pointer to the null-terminated name of the object.
    ChooChooTrain(const char* pName);

    virtual void init(const JMapInfoIter& rIter);
    virtual void startClipped();
    virtual void endClipped();
    virtual void control();

    /* 0x8C */ MR::Vector< MR::AssignableArray< ModelObj* > > mModelArray;
    /* 0x98 */ TVec3f _98;
    /* 0xA4 */ f32 mRailSpeed;
    /* 0xA8 */ s32 mWhistleTimer;
};
