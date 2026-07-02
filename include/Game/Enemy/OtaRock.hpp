#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class AnimScaleController;
class CocoNutBall;
class FireBall;
class FixedPosition;
class NameObjArchiveListCollector;

class OtaRock : public LiveActor {
public:
    /// @brief Creates a new `OtaRock`.
    /// @param pName A pointer to the null-terminated name of the object.
    OtaRock(const char* pName);

    virtual ~OtaRock();
    virtual void init(const JMapInfoIter&);
    virtual void appear();
    virtual void kill();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    static void makeArchiveList(NameObjArchiveListCollector*, const JMapInfoIter&);

    void initModel();
    void initSensor();
    void updateBaseMtx();
    CocoNutBall* getDisappearedCocoNut();
    FireBall* getDisappearedFireBall();
    s32 getDisappearedCocoNutNum() const;
    s32 getDisappearedFireBallNum() const;
    void throwCocoNut();
    void throwFireBall();
    bool tryToHide();

    void exeWait();
    void exeThrowCocoNut();
    void exeThrowFireBall();
    void exeDown();
    void exeHide();
    void exeHideWait();
    void exeShow();

private:
    /* 0x8C */ bool _8C;
    /* 0x90 */ CocoNutBall** mCocoNutArray;
    /* 0x94 */ FireBall** mFireBallArray;
    /* 0x98 */ FixedPosition* mFixedPosition;
    /* 0x9C */ u32 _9C;
    /* 0xA0 */ u32 _A0;
    /* 0xA4 */ TPos3f _A4;
    /* 0xD4 */ TVec3f _D4;
    /* 0xE0 */ AnimScaleController* mAnimScaleController;
};
