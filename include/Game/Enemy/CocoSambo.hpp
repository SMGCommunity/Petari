#pragma once

#include "Game/LiveActor/PartsModel.hpp"
#include <JSystem/JGeometry.hpp>

class CocoSamboHead : public PartsModel {
public:
    CocoSamboHead(LiveActor*);

    /* 0x08 */ virtual ~CocoSamboHead();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x2C */ virtual void kill();
    /* 0x4C */ virtual void calcAndSetBaseMtx();
    /* 0x54 */ virtual void attackSensor(HitSensor*, HitSensor*);
    /* 0x5C */ virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    /* 0x60 */ virtual bool receiveMsgEnemyAttack(u32, HitSensor*, HitSensor*);

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
    /* 0x08 */ virtual ~CocoSambo();

private:
    u8 mPad[(0xC8) - sizeof(LiveActor)];
};
