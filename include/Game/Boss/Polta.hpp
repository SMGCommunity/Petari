#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/LiveActorGroup.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "JSystem/JGeometry/TVec.hpp"

class PoltaSensorCtrl;
class PoltaArm;
class PoltaActionSequencer;
class PoltaRockHolder;
class PoltaGroundRockHolder;
class BombTeresa;

class BombTeresaHolder : public DeriveActorGroup< BombTeresa > {
public:
    BombTeresaHolder();

    void disperseAll();
};

class Polta : public LiveActor {
public:
    Polta(const char*);

    virtual void init(const JMapInfoIter& rIter);
    virtual void calcAnim();
    virtual void makeActorAppeared();
    virtual void kill();
    virtual void makeActorDead();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    void initArm();
    void initPartsModel();
    void initSensor();
    void initBombTeresaHolder();
    void initCamera(const JMapInfoIter&);
    void appearBreakModel(const TVec3f&, const char*);
    void appearBreakModelFirst(const TVec3f&);
    void appearBreakModelMiddle(const TVec3f&);
    void appearBreakModelLast(const TVec3f&);
    void appearStarPiece(s32);
    bool isEndBreakModel() const;
    void killBreakModel();
    void rotateToPlayer();
    f32 calcToPlayerRotateSpeed() const;
    void updateAction() NO_INLINE;
    void updatePose(f32, f32);
    void start();
    void setStartPose();

    /* 0x8C */ PoltaSensorCtrl* mSensorCtrl;
    /* 0x90 */ PoltaArm* mLeftArm;
    /* 0x94 */ PoltaArm* mRightArm;
    /* 0x94 */ PoltaActionSequencer* mSequencer;
    /* 0x9C */ BombTeresaHolder* mBombTeresaHolder;
    /* 0xA0 */ PoltaRockHolder* mRockHolder;
    /* 0xA4 */ PoltaGroundRockHolder* mGroundRockHolder;
    /* 0xA8 */ ActorCameraInfo* mCameraInfo;
    /* 0xAC */ ModelObj* mBreakModel;
    /* 0xB0 */ ModelObj* mFormationModel;
    TQuat4f _B4;
    TVec3f _C4;
    TQuat4f _D0;
    TVec3f _E0;
    f32 _EC;
};
