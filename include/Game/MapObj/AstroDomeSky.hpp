#pragma once

#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/MaterialCtrl.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "revolution/gx/GXStruct.h"
#include "revolution/types.h"

class AstroDomeSky : public LiveActor {
public:
    AstroDomeSky(const char*);
    ~AstroDomeSky();
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
