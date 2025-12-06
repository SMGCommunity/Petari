#pragma once

#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/PartsModel.hpp"

class AstroDomeComet : public LiveActor {
public:
    AstroDomeComet(const char*);

    virtual ~AstroDomeComet();
    virtual void init(const JMapInfoIter&);
    virtual void appear();
    virtual void kill();
    virtual void calcAndSetBaseMtx();
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    void initSubModel();
    void tryPointing();
    void exeSelect();
    void exeConfirm();

    PartsModel* mBloomModel;      // 0x8C
    LiveActor* mMiniGalaxy;       // 0x90
    LiveActor** mPointingActors;  // 0x94
};
