#pragma once

#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/PartsModel.hpp"

class PartsModel;
class AnimScaleParam;
class AnimScaleController;

class CocoSamboHead : public PartsModel {
public:
    CocoSamboHead(LiveActor*);

    virtual void init(const JMapInfoIter&);
    virtual void kill();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor*, HitSensor*);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    virtual bool receiveMsgEnemyAttack(u32, HitSensor*, HitSensor*);

    void updateFrontVecToPlayer(const TVec3f&);
    void exeHeadConnectedBody();
    void exeFall();
    void exeFallLand();
    void exeSwoon();
    void exeSwoonEnd();
    void exeBlow();

    /* 0x9C */ TVec3f mFrontVec;
};

class CocoSambo : public LiveActor {
public:
    CocoSambo(const char*);

    virtual void init(const JMapInfoIter& rIter);
    virtual void appear();
    virtual void kill();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    virtual bool receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    void initMapToolInfo(const JMapInfoIter& rIter);
    void initSensor();
    bool isPointing();
    bool tryDpdPointing(const Nerve*);
    bool tryToFallHead(const HitSensor*, const HitSensor*);
    bool tryToPress();
    bool tryStarPieceReflect();
    bool tryToBlow();
    void dirToPlayer(f32);

    void exeAppear();
    void exeWait();
    void exeFallHead();
    void exeAttack();
    void exeAttackInterval();
    void exeHide();
    void exeHideWait();
    void exeFallHeadHide();
    void exeRecoverWait();
    void exeRecover();
    void exePressY();
    void exeBlow();
    void exeDpdPointing();
    void endDpdPointing();

    inline bool isNerveDying();
    inline void startBckThisAndHead(const char*);
    inline bool isNerveDown();
    inline bool isSensorTryToFallHead(HitSensor* pSensor);

    /* 0x8C */ CocoSamboHead* mHead;
    /* 0x90 */ TVec3f mHitEffectScale;
    /* 0x9C */ TVec3f mFrontVec;
    /* 0xA8 */ TVec3f mUpVec;
    /* 0xB4 */ f32 mPlayerSearchDistance;
    /* 0xB8 */ AnimScaleParam* mAnimScaleParam;
    /* 0xBC */ AnimScaleController* mAnimScaleController;
    /* 0xC0 */ const Nerve* mDpdPointingEndNerve;
    /* 0xC4 */ LiveActor** mPointingActorArray;
};
