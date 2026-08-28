#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class TripodBossGuardWallPart : public LiveActor {
public:
    TripodBossGuardWallPart(const char* pName = "三脚ボスコア防壁部品");

    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x28 */ virtual void makeActorAppeared();
    /* 0x2C */ virtual void kill();
    /* 0x48 */ virtual void control();
    /* 0x4C */ virtual void calcAndSetBaseMtx();
    /* 0x60 */ virtual bool receiveMsgEnemyAttack(u32, HitSensor*, HitSensor*);

    void requestStartDemo();
    bool requestBreak();
    bool isEndDemo() const;
    void setHostMatrix(const TPos3f*);
    void setPlacementAngle(f32);
    void setStartTiming(s32);

    void exeNonActive();
    void exeDemo();
    void exeActive();
    void exeBreak();
    void exeRepair();

    /* 0x8C */ const TPos3f* mHostMtx;
    /* 0x90 */ f32 mPlacementAngle;
    /* 0x94 */ s32 mStartTiming;
};
