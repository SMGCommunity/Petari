#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class FootPrint;
class RabbitStateCaught;
class RabbitStateWaitStart;
class SpotMarkLight;
class TalkMessageCtrl;
class TrickRabbitSnowListener;
class WalkerStateBlowDamage;
class WalkerStateRunaway;

class TrickRabbitSnow : public LiveActor {
    friend class TrickRabbitSnowCollect;

public:
    /// @brief Creates a new `TrickRabbitSnow`.
    /// @param pName A pointer to the null-terminated name of the object.
    TrickRabbitSnow(const char* pName);

    virtual void init(const JMapInfoIter& rIter);
    virtual void initAfterPlacement();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgPush(HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    void setListener(TrickRabbitSnowListener*);
    void initSensor();
    void initState();
    void clearFootPrint();
    bool receiveMsgBlowDamage(HitSensor*, HitSensor*);
    bool requestCaught();
    bool requestStartHideSnow();
    void setFinishPosition();
    void setNotCaughtable();
    bool isGiveUp() const;
    bool isHideSnow() const;
    bool isEnableCaught() const;
    bool isEnableBlowDamage() const NO_INLINE;
    void startJumpSound();

    void exeWaitSnow();
    void exeStartHideSnow();
    void exeVanish();
    void exeHideSnow();
    void exeAppearFromSnow();
    void exeRunaway();
    void exeBlowDamage();
    void exeCaught();
    void exeGiveUp();

private:
    /* 0x8C */ TQuat4f mFinishQuat;
    /* 0x9C */ TVec3f mFinishPos;
    /* 0xA8 */ TQuat4f mRotateQuat;
    /* 0xB8 */ TVec3f mFrontVec;
    /* 0xC4 */ RabbitStateWaitStart* mStateWaitStart;
    /* 0xC8 */ WalkerStateRunaway* mStateRunaway;
    /* 0xCC */ WalkerStateBlowDamage* mStateBlowDamage;
    /* 0xD0 */ RabbitStateCaught* mStateCaught;
    /* 0xD4 */ TalkMessageCtrl* mMsgCtrl;
    /* 0xD8 */ FootPrint* mFootPrint;
    /* 0xDC */ SpotMarkLight* mSpotMarkLight;
    /* 0xE0 */ TrickRabbitSnowListener* mListener;
    /* 0xE4 */ s32 mNotCaughtableTimer;
    /* 0xE8 */ bool mIsValidAppearStarPiece;
};

class TrickRabbitSnowListener {
public:
    /// @brief Creates a new `TrickRabbitSnowListener`.
    TrickRabbitSnowListener();

    virtual void noticeStart(TrickRabbitSnow*) {
    }

    virtual void noticeAppear(TrickRabbitSnow*) {
    }

    virtual void noticeCaught(TrickRabbitSnow*) {
    }

    virtual void noticeGiveUp(TrickRabbitSnow*) {
    }
};
