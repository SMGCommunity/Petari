#pragma once

#include "Game/Boss/TripodBossGuardWallPart.hpp"

class ActorCameraInfo;
class CameraTargetMtx;

class TripodBossGuardWall : public LiveActor {
public:
    TripodBossGuardWall(const char*);

    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x28 */ virtual void makeActorAppeared();
    /* 0x2C */ virtual void kill();
    /* 0x30 */ virtual void makeActorDead();
    /* 0x38 */ virtual MtxPtr getBaseMtx() const {
        return (MtxPtr)mBaseMtx.mMtx;
    }
    /* 0x48 */ virtual void control();
    /* 0x4C */ virtual void calcAndSetBaseMtx();
    /* 0x74 */ virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

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
