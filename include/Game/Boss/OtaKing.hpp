#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"

class ActorCameraInfo;
class AnimScaleController;
class CocoNutBall;
class FireBall;
class FireBubble;
class FixedPosition;
class NameObjArchiveListCollector;
class OtaKingMagma;
class OtaKingLongFoot;
class PartsModel;

class OtaKing : public LiveActor {
public:
    OtaKing(const char*);

    virtual void init(const JMapInfoIter& rIter);
    virtual void movement();
    virtual void makeActorAppeared();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    void startAppearDemo();
    static void makeArchiveList(NameObjArchiveListCollector*, const JMapInfoIter&);
    void initMapToolInfo(const JMapInfoIter& rIter);
    void initModel(const JMapInfoIter& rIter);
    void initSensor();
    void dirToPlayer();
    CocoNutBall* getDisappearedCocoNut();
    FireBall* getDisappearedFireBall();
    FireBubble* getDisappearedBubble();
    s32 getDisappearedCocoNutNum() const;
    s32 getDisappearedFireBallNum() const;
    s32 getDisappearedFireBubbleNum() const;
    bool isValidThrowCocoNut() const;
    bool isValidThrowFireBall() const;
    bool isValidBubbleAttack() const;
    void damage();
    void throwCocoNut();
    void throwFireBall();
    void startBckWaitIfNotPlaying();
    void startBckWithFrontFoot(const char*);
    bool isNerveValidNearAttack() const;
    void appearBubble();
    void startDemo();
    void appearStarPiece();
    void initLongFoot(const JMapInfoIter& rIter);
    bool tryThrowCocoNutOrFireBallIfWait(s32);

    void exeWaitOnSwitch();
    void exeAppearDemo();
    void exeWait();
    void exeThrowCocoNutWait();
    void exeThrowCocoNut();
    void exeThrowFireBallWait();
    void exeThrowFireBall();
    void exeDamage();
    void exePowerUp();
    void exeDown();
    void exeDownDemo();
    void exeAppearStar();
    void exeDead();
    void exeHitBackStart();
    void exeHitBack();
    void exeNearAttack();
    void exeBubbleAttack();
    void exeWaitStartDemo();

    bool isSensorBodyOrFace(HitSensor*);
    bool isDamageNerve();
    bool isOneHP() const;

    void invalidateClippingAndStartDemo(const char* demoStr, const Nerve* pNerve1, const Nerve* pNerve2) {
        MR::invalidateClipping(this);
        MR::requestStartDemo(this, demoStr, pNerve1, pNerve2);
    }

    void validateClippingAndEndDemo(const char* demoStr) {
        MR::validateClipping(this);
        MR::endDemo(this, demoStr);
    }

    /* 0x08C */ PartsModel* mFeet[2];
    /* 0x094 */ OtaKingLongFoot* mLongFeet[4];
    /* 0x0A4 */ OtaKingMagma* mMagma;
    /* 0x0A8 */ TPos3f mLongFootMtx;
    /* 0x0D8 */ CocoNutBall* mCocoNutBallArray;
    /* 0x0DC */ FireBall* mFireBallArray;
    /* 0x0E0 */ FireBubble* mFireBubbleArray;
    /* 0x0E4 */ FixedPosition* mThrowPos;
    /* 0x0E8 */ s32 mHits;
    /* 0x0EC */ s32 _EC;
    /* 0x0F0 */ f32 mTurnSpeed;
    /* 0x0F4 */ AudAnmSoundObject* mSoundObj;
    /* 0x0F8 */ TVec3f mSoundObjPos;
    /* 0x104 */ FixedPosition* mBubbleAppearPos;
    /* 0x108 */ CocoNutBall* mDownCocoNut;
    /* 0x10C */ ActorCameraInfo* mAnimCamera;
    /* 0x110 */ char mBckLBuffer[0x20];
    /* 0x130 */ char mBckRBuffer[0x20];
    /* 0x150 */ bool mIsLv2;
    /* 0x154 */ AnimScaleController* mScaleController;
};
