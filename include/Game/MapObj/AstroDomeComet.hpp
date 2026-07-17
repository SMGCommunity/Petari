#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class PartsModel;

class AstroDomeComet : public LiveActor {
public:
    AstroDomeComet(const char*);

    virtual void init(const JMapInfoIter&);
    virtual void appear();
    virtual void kill();
    virtual void calcAndSetBaseMtx();
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    void initSubModel();
    void tryPointing();
    void exeSelect();
    void exeConfirm();

    /* 0x8C */ PartsModel* mBloomModel;
    /* 0x90 */ LiveActor* mMiniGalaxy;
    /* 0x94 */ LiveActor** mPointingActors;
};
