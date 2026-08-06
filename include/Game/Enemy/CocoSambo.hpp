#pragma once

#include "Game/LiveActor/PartsModel.hpp"
#include <JSystem/JGeometry.hpp>

class CocoSamboHead : public PartsModel {
public:
    CocoSamboHead(LiveActor*);

    virtual ~CocoSamboHead();
    virtual void init(const JMapInfoIter&);
    virtual void kill();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor*, HitSensor*);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    virtual bool receiveMsgEnemyAttack(u32, HitSensor*, HitSensor*);

    void updateFrontVecToPlayer(const TVec3f&);
    void exeHeadConnectedBody();
    void exeFall();
    void exeFallLand();
    void exeSwoon();
    void exeSwoonEnd();
    void exeBlow();

    TVec3f mFrontVec;  // _9C
};

class CocoSambo : public LiveActor {
public:
    CocoSambo(const char*);
    virtual ~CocoSambo();

private:
    u8 mPad[(0xC8) - sizeof(LiveActor)];
};
