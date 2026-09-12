#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class CollisionParts;

class TreasureBoxCracked : public LiveActor {
public:
    TreasureBoxCracked(const char*);

    virtual ~TreasureBoxCracked();
    virtual void init(const JMapInfoIter&);
    virtual void control();
    virtual void attackSensor(HitSensor*, HitSensor*);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    virtual bool receiveMsgEnemyAttack(u32, HitSensor*, HitSensor*);

    void exeWait();
    void exeOpen();
    void exeItemBound();
    void exeOpenWait();
    void exeAlwaysOpen();
    void initItem(const JMapInfoIter&);
    void initItemTrans();
    LiveActor* initYellowChip(const JMapInfoIter&);
    LiveActor* initBlueChip(const JMapInfoIter&);
    LiveActor* initKinokoOneUp(const JMapInfoIter&);
    bool checkItemPos();
    bool isHalfOpen() const;

    /* 0x8C */ CollisionParts* mBoxCollider;
    /* 0x90 */ TPos3f mTopMtx;
    /* 0xC0 */ s32 mBoxType;
    /* 0xC4 */ s32 mOpenCondition;
    /* 0xC8 */ LiveActor* mItem;
    /* 0xCC */ s32 mItemType;
    /* 0xD0 */ TVec3f mItemVelocity;
    /* 0xDC */ TVec3f mItemTargetPos;
    /* 0xE8 */ bool mIsItemMoving;
    /* 0xEC */ s32 mWaitForSwitchOn;
    /* 0xF0 */ s32 mPowerStarId;
};
