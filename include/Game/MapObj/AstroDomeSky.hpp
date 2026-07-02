#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class ProjmapEffectMtxSetter;

class AstroDomeSky : public LiveActor {
public:
    AstroDomeSky(const char*);

    virtual ~AstroDomeSky();
    virtual void init(const JMapInfoIter&);
    virtual void draw() const;
    virtual void calcAndSetBaseMtx();
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    bool tryRotateAppearence();
    void exeHide();
    void exeAppear();
    void exeWait();
    void exeRotateAppear();
    void exeRotateDisappear();
    void exeDisappear();
    void exeReturnDemoWait();
    void exeJumpOutDemo() {};

    f32 _8C;
    GXColor _90;
    ProjmapEffectMtxSetter* _94;
};
