#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class DashRing : public LiveActor {
public:
    DashRing(const char* pName) : LiveActor(pName) {
    }

    virtual ~DashRing();
    virtual void init(const JMapInfoIter& rIter) {
        initCommon(rIter, "DashRing");
    }
    virtual void draw() const;
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor*, HitSensor*);

    void initCommon(const JMapInfoIter&, const char*);
    void calcAxis();
    void calcSubAxis();

    /* 0x8C */ TVec3f mAxis;
    /* 0x98 */ TVec3f mSubAxis;
    /* 0xA4 */ u16 mActiveTime;
    /* 0xA6 */ u16 mRevivalTime;
    /* 0xA8 */ s32 mBoostTime;
    /* 0xAC */ s32 mMaxDuration;
    /* 0xB0 */ f32 mSpeedScale;
    /* 0xB4 */ bool mDoLoop;
    /* 0xB5 */ bool mDoTurn;
    /* 0xB8 */ TVec3f mInitialPosition;
};
