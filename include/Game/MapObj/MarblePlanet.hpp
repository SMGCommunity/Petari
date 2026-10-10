#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class MarblePlanetElectron;
class MarblePlanetElectronShadow;
class ModelObj;

class MarblePlanet : public LiveActor {
public:
    MarblePlanet(const char*);

    virtual ~MarblePlanet();
    virtual void init(const JMapInfoIter&);
    virtual void kill();
    virtual void startClipped();
    virtual void endClipped();
    virtual bool receiveMsgEnemyAttack(u32, HitSensor*, HitSensor*);

    void exeWait();
    void exeScaleUpCore();
    void exeBreakCore();
    void initCoreAndElectron();

    /* 0x8C */ ModelObj* mCorePlanetModel;
    /* 0x90 */ MarblePlanetElectron** mPlanetElectrons;
    /* 0x94 */ CollisionParts* mWatermelonCollision;
    /* 0x98 */ s32 mNumElectrons;
    /* 0x9C */ s32 mRemainingElectrons;
};

class MarblePlanetElectron : public LiveActor {
public:
    MarblePlanetElectron(LiveActor*, const TVec3f&, const TVec3f&, const char*);

    virtual ~MarblePlanetElectron();
    virtual void init(const JMapInfoIter&);
    virtual void control();
    virtual void attackSensor(HitSensor*, HitSensor*);
    virtual bool receiveMsgPush(HitSensor*, HitSensor*);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);

    void exeMove();
    void exeAttack();
    void crashElectron(HitSensor*);

    /* 0x8C */ MarblePlanet* mParentPlanet;
    /* 0x90 */ MarblePlanetElectronShadow* mElectronShadow;
    /* 0x94 */ TVec3f _94;
};

class MarblePlanetElectronShadow : public LiveActor {
public:
    MarblePlanetElectronShadow(LiveActor*, const TVec3f&, const char*);

    virtual ~MarblePlanetElectronShadow();
    virtual void init(const JMapInfoIter&);
    virtual void calcAndSetBaseMtx();

    /* 0x8C */ MarblePlanetElectron* mParentElectron;
    /* 0x90 */ const TVec3f* _90;
};
