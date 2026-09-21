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
    enum HideLocation {
        /* -0x1 */ HideLocation_None = -1,
        /* 0x0 */ HideLocation_Hole,
        /* 0x1 */ HideLocation_Leaves
    };

    RunawayRabbit(const char* pName, RunawayRabbitCollect* pCollect);

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
    void initShadow();
    void initFootPrint();
    void updatePose();
    void updateBindActorMatrix();
    void activate();
    void startRunnaway();
    void incrementRunawayLevel();
    void setLastMessage();
    void setMessage();
    void setNotCaughtable();
    void startJumpSound();
    void setMsgCtrl(TalkMessageCtrl* pMsgCtrl);
    bool isCaught() const NO_INLINE;
    bool isCaughtable() const;
    bool isRunnaway() const;
    bool isChasing() const;
    bool isEnableBlow() const NO_INLINE;
    bool isValidFollow(s32 id) const;
    s32 getGroupId() const {
        return mGroupId;
    }

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
    /* 0xA4 */ TQuat4f mPoseQuat;
    /* 0xB4 */ TVec3f mFrontVec;
    /* 0xC0 */ TQuat4f mPlayerPoseQuat;
    /* 0xD0 */ TVec3f mPlayerBindPos;
    /* 0xDC */ u8 _DC[4];
    /* 0xE0 */ s32 mGroupId;
    /* 0xE4 */ s32 mRunawayLevel;
    /* 0xE8 */ s32 mHideLocation;
    /* 0xEC */ s32 mPressureTimer;
    /* 0xF0 */ s32 mNotCaughtableTimer;
    /* 0xF4 */ bool mIsActive;
    /* 0xF5 */ bool mIsLinked;
    /* 0xF8 */ f32 mNearSoundRadius;
};
