#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class ActorCameraInfo;
class DummyDisplayModel;
class ModelObj;

class BreakableCage : public LiveActor {
public:
    enum CageType { CAGE_INVALID = 0, CAGE_NORMAL = 1, CAGE_LARGE = 2, CAGE_FIX = 3, CAGE_TRASH = 4 };

    BreakableCage(const char* pName);

    virtual ~BreakableCage();
    virtual void init(const JMapInfoIter& rIter);
    virtual void appear();
    virtual void kill();
    virtual void calcAndSetBaseMtx();
    virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    const ActorCameraInfo* getCamInfo() const {
        return mCameraInfo;
    }

    void initMapToolInfo(const JMapInfoIter& rIter);
    void initModel(const char* pName, const JMapInfoIter& rIter);
    void initBaseMtxForCage();
    bool isTypeCage() const;
    bool isAppearPowerStar() const;
    bool tryBreak();

    void exeWait();
    void exeWaitStartDemoBreak();
    void exeBreak();

    /* 0x8C */ TPos3f mMtx;
    /* 0xBC */ ModelObj* mBreakModel;
    /* 0xC0 */ CageType mCageType;
    /* 0xC4 */ f32 mRotationSpeed;
    /* 0xC8 */ DummyDisplayModel* mItemModel;
    /* 0xCC */ bool mIgnoreGravity;
    /* 0xCD */ bool mDelayDeadActivate;
    /* 0xD0 */ ActorCameraInfo* mCameraInfo;
};
