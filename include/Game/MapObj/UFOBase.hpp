#pragma once

#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Map/CollisionParts.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "JSystem/JGeometry/TVec.hpp"
#include "revolution/types.h"

class UFOBase : public LiveActor {
public:
    UFOBase(const char*);

    virtual void init(const JMapInfoIter&);
    virtual void kill();
    virtual void makeActorDead();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual void initSensorType();

    void initSubModel(const JMapInfoIter&, const char*) NO_INLINE;
    void exeWaitForPlayerOn();
    void exeMove();
    void exeWait();
    void exeBreak();

    CollisionParts* mCollisionParts;  // _84
    LodCtrl* mLODCtrl;                // _90
    ModelObj* mModel;                 // _94
    MapPartsRailMover* mRailMover;    // _98
    TVec3f _9C;
    f32 _A8;
    s32 _AC;
    bool _B0;
    const char* _B4;
};

class UFOSolid : public UFOBase {
public:
    UFOSolid(const char*);
};

class UFOBreakable : public UFOBase {
public:
    UFOBreakable(const char*);

    virtual void initSensorType();
    virtual bool receiveMsgEnemyAttack(u32, HitSensor*, HitSensor*);
};
