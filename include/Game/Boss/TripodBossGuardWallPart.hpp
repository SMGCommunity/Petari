#pragma once

#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/LiveActor.hpp"

class TripodBossGuardWallPart : public LiveActor {
public:
    TripodBossGuardWallPart(const char* pName = "三脚ボスコア防壁部品");

    virtual void init(const JMapInfoIter&);
    virtual void makeActorAppeared();
    virtual void kill();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual bool receiveMsgEnemyAttack(u32, HitSensor*, HitSensor*);

    void requestStartDemo();
    bool requestBreak();
    bool isEndDemo() const;
    void exeBreak();
    void exeRepair();
    inline void exeActive();
    inline void exeDemo();
    inline void exeNonActive();
    void setHostMatrix(const TPos3f*);
    void setPlacementAngle(f32);
    void setStartTiming(s32);

    const TPos3f* mHostMtx;  // 0x8C
    f32 mPlacementAngle;     // 0x90
    s32 mStartTiming;        // 0x94
};
