#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class ActorJointCtrl;
class KoopaParts;
class KoopaSensorCtrl;
class KoopaSequencer;

class Koopa : public LiveActor {
public:
    Koopa(const char* pName, KoopaSequencer* pSequencer);

    /* 0x08 */ virtual ~Koopa();
    /* 0x0C */ virtual void init(const JMapInfoIter& rIter);
    /* 0x2C */ virtual void kill();
    /* 0x48 */ virtual void control();
    /* 0x4C */ virtual void calcAndSetBaseMtx();
    /* 0x50 */ virtual void updateHitSensor(HitSensor* pSensor);
    /* 0x54 */ virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);
    /* 0x5C */ virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    /* 0x60 */ virtual bool receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    /* 0x74 */ virtual bool receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    /* 0x8C */ TVec3f mFront;
    /* 0x98 */ KoopaSequencer* mSequencer;
    /* 0x9C */ KoopaSensorCtrl* mSensorCtrl;
    /* 0xA0 */ KoopaParts* mParts;
    /* 0xA4 */ ActorJointCtrl* mJointCtrl;
};
