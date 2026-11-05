#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class Polta;
class WalkerStateBindStarPointer;

class PoltaRock : public LiveActor {
public:
    PoltaRock(const char*);

    virtual ~PoltaRock();
    virtual void init(const JMapInfoIter& rIter);
    virtual void kill();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    void start(Polta* pHost, const TVec3f& rPosition, const TVec3f& rOffset);
    void setColorWhite();
    void setColorBlack();
    void setColorYellow();
    void exeFloat();
    void exeSign();
    void exeMove();
    void generateKill();
    bool isEnableAttack() const NO_INLINE;
    bool isEnableExplosion() const;

    inline Polta* getHost() {
        return mHost;
    }  // required to match instruction order in generateKill

    /* 0x8C */ Polta* mHost;
    /* 0x90 */ TQuat4f mQuat;
    /* 0xA0 */ TVec3f mTargetPosition;
    /* 0xAC */ TVec3f mOffset;
    /* 0xB8 */ s32 mType;
};
