#pragma once

#include "Game/Boss/TripodBossGuardWallPart.hpp"

class ActorCameraInfo;
class CameraTargetMtx;

class TripodBossGuardWall : public LiveActor {
public:
    TripodBossGuardWall(const char*);

    virtual void init(const JMapInfoIter&);
    virtual void makeActorAppeared();
    virtual void kill();
    virtual void makeActorDead();
    virtual MtxPtr getBaseMtx() const {
        return (MtxPtr)mBaseMtx.mMtx;
    }
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    void initParts();
    void requestStart();
    void updateMatrix();
    void updateCameraTarget();

    void exeWait();
    void exeTryDemo();
    void exeDemo();
    void exeRotate();

    /* 0x08C */ CameraTargetMtx* mCameraTargetMtx;
    /* 0x090 */ TripodBossGuardWallPart mWallParts[8];
    /* 0x550 */ TPos3f mBaseMtx;
    /* 0x580 */ TVec3f _580;
    /* 0x58C */ s32 _58C;
    /* 0x590 */ ActorCameraInfo* mCameraInfo;
};
