#pragma once

#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Util/JMapInfo.hpp"

class TicoRail : public LiveActor {
public:
    TicoRail(const char*);

    virtual ~TicoRail();
    virtual void init(const JMapInfoIter&);
    virtual void kill();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor*, HitSensor*);
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    void exeWait();
    void exeLookAround();
    void exeMoveSign();
    void exeMove();
    void exeStop();
    void exeTalkStart();
    void exeTalk();
    void exeTalkCancel();
    void exeGoodBye();
    bool isGreaterEqualStepAndRandom(s32) const;
    bool isSameRailActor(const LiveActor*) const;

    TVec3f _8C;
    LiveActor* _98;
    LodCtrl* mLodCtrl;  // 0x9C
};
