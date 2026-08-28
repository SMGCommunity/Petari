#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class Polta;
class WalkerStateBindStarPointer;

class PoltaRock : public LiveActor {
public:
    PoltaRock(const char*);

    /* 0x08 */ virtual ~PoltaRock();
    /* 0x0C */ virtual void init(const JMapInfoIter& rIter);
    /* 0x2C */ virtual void kill();
    /* 0x48 */ virtual void control();
    /* 0x4C */ virtual void calcAndSetBaseMtx();
    /* 0x54 */ virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);
    /* 0x5C */ virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    /* 0x60 */ virtual bool receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    void start(Polta*, const TVec3f&, const TVec3f&);
    void setColorWhite();
    void setColorBlack();
    void setColorYellow();
    void exeFloat();
    void exeSign();
    void exeMove();
    void generateKill();
    bool isEnableAttack() const NO_INLINE;
    bool isEnableExplosion() const;

    inline Polta* getOwner() {
        return mOwner;
    }  // required to match instruction order in generateKill

    /* 0x8C */ Polta* mOwner;
    TQuat4f _90;
    TVec3f _A0;
    TVec3f _AC;
    /* 0xB8 */ s32 mType;
};
