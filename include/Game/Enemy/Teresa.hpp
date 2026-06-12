#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Util/ActorMovementUtil.hpp"

class KeySwitch;
class PartsModel;
class Triangle;

class Teresa : public LiveActor {
public:
    Teresa(const char*);

    virtual ~Teresa();
    virtual void init(const JMapInfoIter&);
    virtual void appear();
    virtual void makeActorAppeared();
    virtual void kill();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor*, HitSensor*);
    virtual bool receiveMsgPush(HitSensor*, HitSensor*);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    void initDummyModel(const JMapInfoIter&);
    void initFromJMapParam(const JMapInfoIter&);
    void initSensor();
    void initBind();
    bool filterBind(const Triangle*);
    bool requestAttack(HitSensor*, HitSensor*);
    bool requestDrift();
    bool requestLoveHit(HitSensor*, HitSensor*);
    bool requestSearchLightDead();
    void setStartNerve();
    bool tryAppearFromWallEnd();
    bool tryAppearFromGroundEnd();
    bool tryRailTurn();
    bool tryRailTurnEnd();
    bool tryWalk();
    bool tryWalkEnd();
    bool tryChase();
    bool tryCheseEnd();
    bool tryShay();
    bool tryShayEnd();
    bool tryLoveFind();
    bool tryLoveEnd();
    bool tryLoveFindEnd();
    bool tryLoveChaseEnd();
    bool tryLoveHitEnd();
    bool tryAggressiveEnd();
    bool tryAttackSuccessEnd();
    bool tryDriftEnd();
    bool tryHideWater();
    bool tryHideWall();
    bool tryHideWallEnd();
    bool tryAscension();
    void endAppear();
    void exeAppearFromWall();
    void exeAppearFromGround();
    void exeWait();
    void exeWalk();
    void exeRailWalk();
    void exeRailTurn();
    void exeLoveFind();
    void exeLoveChase();
    void exeLoveHit();
    void exeLoveEnd();
    void exeChase();
    void exeShay();
    void exeAggressive();
    void exeAttackSuccess();
    void exeDrift();
    void exeHideWall();
    void exeHideWater();
    void exeAscension();
    void exeStop();
    void updateNormalVelocity();
    void addDriftVelocity();
    void updateDriftAnimScale();
    void updateDriftTransparency();
    void updateNormalTransparency();
    void setTransparency(f32);
    bool canAttack() const;
    bool canDrift() const;
    bool canSearchLightDead() const;
    bool isEnableStarPieceStop() const;
    bool isCheckWater() const;
    bool isShay() const;
    void endDrift();
    void endAppearFromGround();
    void endAppearFromWall();

    inline bool isNotNear() const {
        bool isNearPlayer = MR::isNearPlayer(this, 2000.0f) == false;
        return isNearPlayer;
    }

    PartsModel* mDisplayModel;  // 0x8C
    KeySwitch* mKeySwitch;      // 0x90
    TQuat4f _94;
    TVec3f mWallNormal;  // _A4
    TVec3f mWallHitPos;  // _B0
    TVec3f _BC;
    TVec3f _C8;
    TVec3f _D4;
    TVec3f _E0;
    f32 _EC;
    f32 _F0;
    f32 _F4;
    s32 mAppearanceType;  // _F8
    u8 _FC;
    u8 _FD;
    u8 _FE;
};
