#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include <JSystem/JGeometry.hpp>

class PartsModel;

class LavaProminenceTriple : public LiveActor {
public:
    LavaProminenceTriple(const char*);

    virtual void init(const JMapInfoIter&);
    virtual void appear();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual void updateHitSensor(HitSensor*);
    virtual void attackSensor(HitSensor*, HitSensor*);

    void updateEffectClipping();
    void updateEffectClippingIndividual(bool*, const char*, const char*);

    void calcAngle(f32*);

    /* 0x08C */ PartsModel* mBloomModel;
    /* 0x090 */ TPos3f mBloomMtx;
    /* 0x0C0 */ TQuat4f mBaseRot;
    /* 0x0D0 */ TQuat4f mCurrRot;
    /* 0x0E0 */ TVec3f mSide;
    /* 0x0EC */ TVec3f mFront;
    /* 0x0F8 */ f32 mRotateVel;
    /* 0x0FC */ bool mIsAppearDrop1;
    /* 0x0FD */ bool mIsAppearDrop2;
    /* 0x0FE */ bool mIsAppearDrop3;
    /* 0x0FF */ bool mIsEnvironment;
    /* 0x100 */ s32 mDefaultAppearTime;
    /* 0x104 */ s32 mAppearTime;
};
