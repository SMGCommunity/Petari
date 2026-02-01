#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Map/CollisionParts.hpp"
#include "Game/Util/BaseMatrixFollowTargetHolder.hpp"
#include "JSystem/JGeometry/TVec.hpp"

class ActorJointCtrl;
class BossStinkBugActionSequencer;
class BossStinkBugBombHolder;
class GroundChecker;
class PartsModel;

class BossStinkBug : public LiveActor {
public:
    BossStinkBug(const char*);

    virtual void init(const JMapInfoIter&);
    virtual void calcAnim();
    virtual void makeActorAppeared();
    virtual void kill();
    virtual void makeActorDead();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    void start();
    void initCamera(const JMapInfoIter&);
    void initSensor();
    void initCollision();
    void disposeBomb();
    void reuestMovementOnParts();
    LiveActor* getWingModel();
    LiveActor* getBombLauncher();
    void calcUpVec(TVec3f*) const;
    void updateAction();
    void updatePose();
    void setPose(MtxPtr);
    void updateCamera();
    bool isValidFollowId(s32) const;
    void validateCollisionGround();
    void validateCollisionFly();
    void validateCollisionHitOnly();
    void invalidateCollision();
    void onBindLeg();
    void offBindLeg();
    bool isLegInWater() const;
    void updateJointControl();
    void appearStarPiece(s32);
    void startEventCamera(const char*);
    void endEventCamera(const char*);
    bool isSensorBody(const HitSensor*) const;
    bool throwBomb(f32, f32);

    CollisionParts** _8C;
    GroundChecker** _90;
    BossStinkBugBombHolder* _94;
    ActorJointCtrl* _98;
    TPos3f _9C;
    ActorCameraInfo* _CC;
    BossStinkBugActionSequencer* _D0;
    PartsModel* _D4;
    PartsModel* _D8;
    TQuat4f _DC;
    TVec3f _EC;
    TVec3f _F8;
    s32 _104;
    f32 _108;
    f32 _10C;
    bool _110;
    bool _111;
};

class BossStinkBugFollowValidater : public BaseMatrixFollowValidater {
public:
    BossStinkBugFollowValidater(BossStinkBug* pStinkBug) { mStinkBug = pStinkBug; }

    virtual bool isValid(s32 id) const { return mStinkBug->isValidFollowId(id); }

    BossStinkBug* mStinkBug;
};
