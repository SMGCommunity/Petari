#pragma once

#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "revolution/mtx.h"

class TripodBossGuardWallPart : public LiveActor {
public:
    TripodBossGuardWallPart(const char *pName = "三脚ボスコア防壁部品");

    virtual ~TripodBossGuardWallPart();
    virtual void init(const JMapInfoIter &);
    virtual void makeActorAppeared();
    virtual void kill();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual bool receiveMsgEnemyAttack(u32, HitSensor *, HitSensor *);

    void requestStartDemo();
    void requestBreak();
    bool isEndDemo() const;
    void exeBreak();
    void exeRepair();
    void setHostMatrix(const TPos3f *);
    void setPlacementAngle(f32);
    void setStartTiminig(s32);

    const TPos3f* mHostMtx;         // 0x8C
    f32 mPlacementAngle;            // 0x90
    s32 mStartTime;                 // 0x94
};

class TripodBossGuardWall :public LiveActor {
public:
    TripodBossGuardWall(const char *);

    virtual ~TripodBossGuardWall();
    virtual void init(const JMapInfoIter &);
    virtual void makeActorAppeared();
    virtual void kill();
    virtual void makeActorDead();
    virtual MtxPtr getBaseMtx() const;
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual bool receiveOtherMsg(u32, HitSensor *, HitSensor *);

    void initParts();
    void requestStart();
    void exeRotate();
    void updateMatrix();
    void updateCameraTarget();
    void exeDemo();
    inline void exeTryDemo();
    void exeWait() { }

    CameraTargetMtx* mCameraTargetMtx;          // 0x8C
    TripodBossGuardWallPart mWallParts[8];      // 0x90
    TPos3f mBaseMtx;                            // 0x550
    TVec3f _580;
    s32 _58C;
    ActorCameraInfo* mCameraInfo;               // 0x590
};
