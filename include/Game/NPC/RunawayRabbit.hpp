#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class FootPrint;
class RunawayRabbitCollect;
class SpotMarkLight;
class TalkMessageCtrl;
class WalkerStateBlowDamage;
class WalkerStateRunaway;

class RunawayRabbit : public LiveActor {
public:
    RunawayRabbit(const char*, RunawayRabbitCollect*);

    virtual void init(const JMapInfoIter& rIter);
    virtual void initAfterPlacement();
    virtual void appear();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgPush(HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    void initSensor();
    void updatePose();
    void updateBindActorMatrix();
    void activate();
    void startRunnaway();
    void incrementRunawayLevel();
    void setLastMessage();
    void setMessage();
    void setNotCaughtable();
    void startJumpSound();
    void setMsgCtrl(TalkMessageCtrl*);
    bool isCaught() const;
    bool isCaughtable() const;
    bool isRunnaway() const;
    bool isChasing() const;
    bool isEnableBlow() const;
    bool isValidFollow(s32) const;

    void exeNoActive();
    void exeHide();
    void exeAppear();
    void exeRunaway();
    void exeTryCaughtDemo();
    void exeCaught();
    void exeCaughtTalk();
    void exeCaughtEnd();
    void exeStop();
    void exeBlowDamage();

    /* 0x8C */ WalkerStateRunaway* mStateRunaway;
    /* 0x90 */ WalkerStateBlowDamage* mStateBlowDamage;
    /* 0x94 */ RunawayRabbitCollect* mCollect;
    /* 0x98 */ FootPrint* mFootPrint;
    /* 0x9C */ SpotMarkLight* mSpotMarkLight;
    /* 0xA0 */ TalkMessageCtrl* mMsgCtrl;
    /* 0xA4 */ TQuat4f _A4;
    /* 0xB4 */ TVec3f _B4;
    /* 0xC0 */ TQuat4f _C0;
    /* 0xD0 */ TVec3f _D0;
    /* 0xDC */ u8 _DC[4];
    /* 0xE0 */ s32 mObjArg0;
    /* 0xE4 */ s32 mRunawayLevel;
    /* 0xE8 */ s32 mObjArg1;
    /* 0xEC */ s32 _EC;
    /* 0xF0 */ s32 mNotCaughtableTimer;
    /* 0xF4 */ bool _F4;
    /* 0xF5 */ bool _F5;
    /* 0xF8 */ f32 mObjArg3;
};
