#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class Dossun : public LiveActor {
public:
    Dossun(const char*);

    virtual void init(const JMapInfoIter&) override;
    virtual void appear() override;

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

    /* 0x8C */ TVec3f _8C;
    /* 0x98 */ TVec3f _98;
    /* 0xA4 */ s32 mFallingTime;
    /* 0xA8 */ s32 mHoldTime;
    /* 0xAC */ f32 mMovementDist;
    /* 0xB0 */ s32 mRisenWaitTime;
    /* 0xB4 */ s32 mShadowType;
    /* 0xB8 */ bool mHasShadow;
};
