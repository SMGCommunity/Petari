#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Ride/PlantLeaf.hpp"
#include "Game/Util/JMapInfo.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class PlantStalk;
class PlantLeaf;

class Plant : public LiveActor {
public:
    Plant(const char*);

    virtual void init(const JMapInfoIter&);
    virtual void draw() const;
    virtual void appear();
    virtual void updateHitSensor(HitSensor* pSensor);
    virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    virtual void initAfterPlacement();

    void exeWaitFar();
    void exeSeedWait();
    void exeWaitDemoWaitGrowUp();
    void exeDemoWaitGrowUp();
    void exeGrowUp();
    void exeHangWaitGrowUp();
    void exeHangUpGrowUp();
    void exeGrowthStop();  /// ookay
    void exeGrowthWait();
    void exeGrowthWaitInvalid();
    void exeHangStart();
    void exeHangUp();
    void exeHangDown();

    void initLeaf();
    void calcAnim();
    void startClipped();
    void endClipped();

    void updateTopMtx();
    bool updateGrowUp();
    bool updateHangUp(f32);
    void updateBindLeaf();

    void startGrowUp();

    bool tryHangUp();
    bool tryReachGoal();
    bool tryAccelHangUp();

    /* 0x8C */ PartsModel* mSeedPartsModel;
    /* 0x90 */ TPos3f mSeedMtx;
    /* 0xC0 */ PlantStalk* mStalk;
    /* 0xC4 */ PartsModel* mTopPartsModel;
    /* 0xC8 */ TPos3f mTopMtx;

    /* 0xF8 */ s32 mNumLeaves;
    /* 0xF8 */ PlantLeaf** mLeaves;
    /* 0x100 */ J3DShapeDraw* mShapeDraw;
    /* 0x104 */ LiveActor* mRider;
    /* 0x108 */ f32 mRailCoord;
    /* 0x10C */ TVec3f mUp;
    /* 0x118 */ TVec3f mFront;

    /* 0x124 */ f32 mRideVelocity;
    /// @note this system is not actually used in the final game,
    // as the delay is set to 0 in Plant::tryAccelHangUp.
    /* 0x128 */ s32 mAccelTimer;

    /* 0x12C */ f32 mLaunchSpeed;
    /* 0x130 */ f32 mLaunchNormal;
    /* 0x134 */ TVec3f mClippingCenter;

    /* 0x140 */ ActorCameraInfo* mCameraInfo;
    /* 0x144 */ bool mPlayAppearDemo;
    /* 0x145 */ bool mGrabbedTop;
};

namespace MR {
    // should this be in MathUtil?
    void clampMax(f32*, f32);
}  // namespace MR
