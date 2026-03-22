#pragma once

#include "Game/Enemy/AnimScaleController.hpp"
#include "Game/Enemy/FireBall.hpp"
#include "Game/Enemy/CocoNutBall.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Util/FixedPosition.hpp"

#include <JSystem/JGeometry/TMatrix.hpp>

class NameObjArchiveListCollector;
class JMapInfoIter;

class OtaRock : public LiveActor {
public:
    OtaRock(const char*);
    virtual ~OtaRock();

    virtual void init(const JMapInfoIter&);
    virtual void appear();
    virtual void kill();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    void exeWait();
    void exeThrowCocoNut();
    void exeThrowFireBall();
    void exeDown();
    void exeHide();
    void exeHideWait();
    void exeShow();

    void updateBaseMtx();
    void initModel();
    void initSensor();
    bool tryToHide();
    void throwCocoNut();
    void throwFireBall();

    CocoNutBall* getDisappearedCocoNut();
    FireBall* getDisappearedFireBall();

    s32 getDisappearedCocoNutNum() const;
    s32 getDisappearedFireBallNum() const;

    static void makeArchiveList(NameObjArchiveListCollector*, const JMapInfoIter&);

    bool _8C;
    /* 0x90 */ CocoNutBall** mCocoNutArray;
    /* 0x94 */ FireBall** mFireBallArray;
    /* 0x98 */ FixedPosition* mFixedPosition;
    /* 0x9c */ u32 _9C;
    /* 0xa0 */ u32 _A0;
    /* 0xa4 */ TMtx34f _A4;
    f32 _D4;
    f32 _D8;
    f32 _DC;
    /* 0xe0 */ AnimScaleController* mAnimScaleController;

private:
    u8 mPad[(0xE4) - sizeof(LiveActor)];
};
