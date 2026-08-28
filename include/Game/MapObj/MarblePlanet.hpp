#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class MarblePlanetElectron;
class MarblePlanetElectronShadow;
class ModelObj;

class MarblePlanet : public LiveActor {
public:
    MarblePlanet(const char*);

    /* 0x08 */ virtual ~MarblePlanet();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x2C */ virtual void kill();
    /* 0x40 */ virtual void startClipped();
    /* 0x44 */ virtual void endClipped();
    /* 0x60 */ virtual bool receiveMsgEnemyAttack(u32, HitSensor*, HitSensor*);

    void exeWait();
    void exeScaleUpCore();
    void exeBreakCore();
    void initCoreAndElectron();

    ModelObj* mCorePlanetModel;               // 0x8C
    MarblePlanetElectron** mPlanetElectrons;  // 0x90
    CollisionParts* mWatermelonCollision;     // 0x94
    s32 mNumElectrons;                        // 0x98
    s32 mRemainingElectrons;                  // 0x9C
};

class MarblePlanetElectron : public LiveActor {
public:
    MarblePlanetElectron(LiveActor*, const TVec3f&, const TVec3f&, const char*);

    /* 0x08 */ virtual ~MarblePlanetElectron();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x48 */ virtual void control();
    /* 0x54 */ virtual void attackSensor(HitSensor*, HitSensor*);
    /* 0x58 */ virtual bool receiveMsgPush(HitSensor*, HitSensor*);
    /* 0x5C */ virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);

    void exeMove();
    void exeAttack();
    void crashElectron(HitSensor*);

    MarblePlanet* mParentPlanet;                  // 0x8C
    MarblePlanetElectronShadow* mElectronShadow;  // 0x90
    TVec3f _94;
};

class MarblePlanetElectronShadow : public LiveActor {
public:
    MarblePlanetElectronShadow(LiveActor*, const TVec3f&, const char*);

    /* 0x08 */ virtual ~MarblePlanetElectronShadow();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x4C */ virtual void calcAndSetBaseMtx();

    MarblePlanetElectron* mParentElectron;  // 0x8C
    const TVec3f* _90;
};
