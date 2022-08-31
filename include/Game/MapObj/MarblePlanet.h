#pragma once

#include "Game/LiveActor/LiveActor.h"

class MarblePlanetElectron;
class MarblePlanetElectronShadow;

class MarblePlanet : public LiveActor {
public:
    MarblePlanet(const char *);

    virtual ~MarblePlanet();
    virtual void init(const JMapInfoIter &);
    virtual void kill();
    virtual void startClipped();
    virtual void endClipped();
    virtual bool receiveMsgEnemyAttack(u32, HitSensor *, HitSensor *);

    void exeScaleUpCore();
    void exeBreakCore();
    void initCoreAndElectron();

    ModelObj* mCorePlanetModel;                 // _8C
    MarblePlanetElectron** mPlanetElectrons;    // _90
    CollisionParts* mWatermelonCollision;       // _94
    s32 mNumElectrons;                          // _98
    s32 mRemainingElectrons;                    // _9C
};

class MarblePlanetElectron : public LiveActor {
public:
    MarblePlanetElectron(LiveActor *, const TVec3f &, const TVec3f &, const char *);

    virtual ~MarblePlanetElectron();
    virtual void init(const JMapInfoIter &);
    virtual void control();
    virtual void attackSensor(HitSensor *, HitSensor *);
    virtual bool receiveMsgPush(HitSensor *, HitSensor *);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor *, HitSensor *);

    void exeMove();
    void exeAttack();
    void crashElectron(HitSensor *);

    MarblePlanet* mParentPlanet;                    // _8C
    MarblePlanetElectronShadow* mElectronShadow;    // _90
    TVec3f _94;
};

class MarblePlanetElectronShadow : public LiveActor {
public:
    MarblePlanetElectronShadow(LiveActor *, const TVec3f &, const char *);

    virtual ~MarblePlanetElectronShadow();
    virtual void init(const JMapInfoIter &);
    virtual void calcAndSetBaseMtx();

    MarblePlanetElectron* mParentElectron;  // _8C
    const TVec3f* _90;
};

namespace NrvMarblePlanet {
    NERVE(MarblePlanetNrvWait);
    NERVE(MarblePlanetNrvScaleUpCore);
    NERVE(MarblePlanetNrvBreakCore);
};

namespace NrvMarblePlanetElectron {
    NERVE(MarblePlanetElectronNrvMove);
    NERVE(MarblePlanetElectronNrvAttack);
};
