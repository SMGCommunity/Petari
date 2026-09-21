#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class SpringWaterFloaterSpot : public LiveActor {
public:
    SpringWaterFloaterSpot(const char*);

    virtual void init(const JMapInfoIter&);
    virtual void control();
    virtual void calcAndSetBaseMtx();

    void exeWait();
    void exeFloatSignal();
    void exeFloatUp();
    void exeFloatEnd();

    void startFloat();
    bool boundFloater();
    void floatFloater();
    bool isModelLongSide();
    bool isNeedsForBound();

    /* 0x8C */ f32 _8C;
    /* 0x90 */ f32 _90;
    /* 0x94 */ u32 _94;
    /* 0x98 */ u32 _98;
    /* 0x9C */ u32 _9C;
    /* 0xA0 */ f32 mHeight;
    /* 0xA4 */ f32 _A4;
    /* 0xA8 */ s32 mBoundStep;
    /* 0xAC */ TVec3f _AC;
    /* 0xB8 */ TVec3f mInitialRotation;
    /* 0xC4 */ f32 mBoundSize;
    /* 0xC8 */ bool _C8;
    /* 0xC9 */ bool mNeedsBound;
    /* 0xCA */ bool _CA;
    /* 0xCC */ TPos3f _CC;
    /* 0xFC */ s32 mFloatTimer;
};
