#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Util/Color.hpp"

class ProjmapEffectMtxSetter;

class AstroDomeSky : public LiveActor {
public:
    AstroDomeSky(const char*);

    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x18 */ virtual void draw() const;
    /* 0x4C */ virtual void calcAndSetBaseMtx();
    /* 0x74 */ virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    bool tryRotateAppearance();
    void exeHide();
    void exeAppear();
    void exeWait();
    void exeRotateAppear();
    void exeRotateDisappear();
    void exeDisappear();
    void exeReturnDemoWait();
    void exeJumpOutDemo() {};

    /* 0x8C */ f32 mBrkFrame;
    /* 0x90 */ Color8 mColor;
    /* 0x94 */ ProjmapEffectMtxSetter* mProjmap;
};
