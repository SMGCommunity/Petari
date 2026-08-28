#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class CollisionParts;
class LodCtrl;
class MapPartsRailMover;
class ModelObj;

class UFOBase : public LiveActor {
public:
    UFOBase(const char*);

    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x2C */ virtual void kill();
    /* 0x30 */ virtual void makeActorDead();
    /* 0x48 */ virtual void control();
    /* 0x4C */ virtual void calcAndSetBaseMtx();
    /* 0x78 */ virtual void initSensorType();

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

    /* 0x78 */ virtual void initSensorType();
    /* 0x60 */ virtual bool receiveMsgEnemyAttack(u32, HitSensor*, HitSensor*);
};
