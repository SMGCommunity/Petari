#pragma once

#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/PartsModel.hpp"

class PartsModel;
class AnimScaleParam;
class AnimScaleController;

class CocoSamboHead : public PartsModel {
public:
    CocoSamboHead(LiveActor*);

    virtual ~CocoSamboHead();
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

    TVec3f mFrontVec;  // _9C
};

class CocoSambo : public LiveActor {
public:
    CocoSambo(const char*);

    virtual ~CocoSambo();
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

    /* 0x8C */ CocoSamboHead* _8C;
    /* 0x90 */ TVec3f _90;
    /* 0x9C */ TVec3f _9C;
    /* 0xA8 */ TVec3f _A8;
    /* 0xB4 */ f32 mArg0;
    /* 0xB8 */ AnimScaleParam* _B8;
    /* 0xBC */ AnimScaleController* _BC;
    /* 0xC0 */ const Nerve* _C0;
    /* 0xC4 */ LiveActor* _C4[4];
};
