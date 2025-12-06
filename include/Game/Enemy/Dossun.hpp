#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Util/EventUtil.hpp"

class Dossun : public LiveActor {
public:
    Dossun(const char*);

    virtual void init(const JMapInfoIter&);
    virtual void appear();

    void initMapToolInfo(const JMapInfoIter&);
    void initShadow();
    void calcParameters();
    s32 getUpperFrame() const;
    void exeReady();
    void exeUpper();
    void exeFallSign();
    void exeFalling();
    void exeOnGround();
    void exeRising();

    TVec3f _8C;
    TVec3f _98;
    s32 mFallingTime;    // 0xA4
    s32 mHoldTime;       // 0xA8
    f32 mMovementDist;   // 0xAC
    s32 mRisenWaitTime;  // 0xB0
    s32 mShadowType;     // 0xB4
    bool mHasShadow;     // 0xB8
};
