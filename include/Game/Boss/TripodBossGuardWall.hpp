#pragma once

#include "Game/Boss/TripodBossGuardWallPart.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/LiveActor.hpp"

class TripodBossGuardWall : public LiveActor {
public:
    TripodBossGuardWall(const char*);

    virtual void init(const JMapInfoIter&);
    virtual void makeActorAppeared();
    virtual void kill();
    virtual void makeActorDead();
    virtual MtxPtr getBaseMtx() const;
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    void initParts();
    void requestStart();
    void exeRotate();
    void updateMatrix();
    void updateCameraTarget();
    void exeDemo();
    inline void exeTryDemo();
    void exeWait() {}

    CameraTargetMtx* mCameraTargetMtx;      // 0x8C
    TripodBossGuardWallPart mWallParts[8];  // 0x90
    TPos3f mBaseMtx;                        // 0x550
    TVec3f _580;
    s32 _58C;
    ActorCameraInfo* mCameraInfo;  // 0x590
};
