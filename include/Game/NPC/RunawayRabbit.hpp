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

    RunawayRabbit(const char*, RunawayRabbitCollect*);

    virtual void init(const JMapInfoIter&);
    virtual void initAfterPlacement();
    virtual void appear();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor*, HitSensor*);
    virtual bool receiveMsgPush(HitSensor*, HitSensor*);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    virtual bool receiveMsgEnemyAttack(u32, HitSensor*, HitSensor*);
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    void initSensor();
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
    void setMsgCtrl(TalkMessageCtrl*);
    bool isCaught() const NO_INLINE;
    bool isCaughtable() const;
    bool isRunnaway() const;
    bool isChasing() const;
    bool isEnableBlow() const NO_INLINE;
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
