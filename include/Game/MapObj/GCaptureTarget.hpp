#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/MapObj/BlueStarCupsulePlanet.hpp"

class MapPartsRailMover;

class GCaptureTarget : public LiveActor, public GCaptureTargetable {
public:
    GCaptureTarget(const char*);

    virtual void init(const JMapInfoIter&);
    virtual void initAfterPlacement();
    virtual void appear();
    virtual void makeActorAppeared();
    virtual void makeActorDead();
    virtual void startClipped();
    virtual void endClipped();
    virtual void control();

    void decidedTarget();
    void releasedTarget();
    void emitNerveEffect();
    bool isReleaseForce() const;
    void getTargetPosition(TVec3f*);
    f32 releaseDistance() const;
    f32 getPointableRange() const;

    void exeTryDemoAppear();
    void exeAppear();
    void exeWait();
    void exePointable();
    void exeHitPointer();
    void exeActive();

    /* 0x90 */ LiveActor* _90;
    /* 0x94 */ f32 _94;
    /* 0x98 */ MapPartsRailMover* mRailMover;
    /* 0x9C */ f32 mReleaseDistance;
    /* 0xA0 */ f32 mPointableRange;
    /* 0xA4 */ bool mFarAwayColor;
};
