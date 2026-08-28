#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class MiniatureGalaxy;
class PartsModel;

class AstroDomeComet : public LiveActor {
public:
    AstroDomeComet(const char*);

    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x24 */ virtual void appear();
    /* 0x2C */ virtual void kill();
    /* 0x4C */ virtual void calcAndSetBaseMtx();
    /* 0x74 */ virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    void initSubModel();
    void tryPointing();
    void exeSelect();
    void exeConfirm();

    /* 0x8C */ PartsModel* mBloomModel;
    /* 0x90 */ MiniatureGalaxy* mMiniGalaxy;
    /* 0x94 */ LiveActor** mPointingActors;
};
