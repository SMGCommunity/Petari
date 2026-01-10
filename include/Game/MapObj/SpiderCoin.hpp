#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class SpiderCoin : public LiveActor {
public:
    SpiderCoin(const char*);

    virtual void init(const JMapInfoIter& rIter);
    virtual void kill();
    virtual void calcAndSetBaseMtx();

    void exeWait();
    void exeTouch();
    void exeTouchAndApart();
    bool tryRub(s32, TVec2f*);

    /* 0x8C */ const TVec3f* mPointPos;
    /* 0x90 */ const TVec3f* mNeutralPos;
    /* 0x94 */ const TVec3f* mUp;
    /* 0x98 */ u32 _98;
    /* 0x9C */ u32 _9C;
    /* 0xA0 */ u32 _A0;
    /* 0xA4 */ f32 mThreadZ;
    /* 0xA8 */ TVec2f mPad0Velocity;
    /* 0xB0 */ TVec2f mPad1Velocity;
    /* 0xB8 */ PartsModel* mCocoonModel;

    /* 0xBC */ TPos3f mBaseMtx;
};
