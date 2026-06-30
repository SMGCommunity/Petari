#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class FireBubble : public LiveActor {
public:
    FireBubble(const char* = "バブル");
    virtual void init(const JMapInfoIter& rIter);
    virtual void appear();
    virtual void kill();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    void appear(const TVec3f&, const TVec3f&, const TVec3f&);
    void initMapToolInfo(const JMapInfoIter& rIter);
    void updateChaseFrontVec(f32);
    void updateChaseFrontVecAndVelocity(f32);
    void updateGravity(f32);
    void calcReflectVelocity(TVec3f*) const;
    bool tryChaseEnd();
    bool tryReflect();
    void killOrWaitToApppear();

    void exeAppear();
    void exeWait();
    void exeChase();
    void exeReflect();
    void exeReflectLand();
    void exeReflectWait();
    void exeDown();
    void exeWaitToAppear();

    bool isMovingDown();  // fabricated

    /* 0x8C */ bool mIsValidInfo;
    /* 0x90 */ TVec3f _90;
    /* 0x9C */ TVec3f _9C;
    /* 0xA8 */ TVec3f _A8;
    /* 0xB4 */ TVec3f _B4;
    /* 0xC0 */ s32 mActFrame;
    /* 0xC4 */ s32 mChaseCounter;
};
