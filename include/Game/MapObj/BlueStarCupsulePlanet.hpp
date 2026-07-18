#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/MapObj/GCapture.hpp"

class MapPartsRailMover;
class MapPartsRotator;

namespace {
    struct BlueStarCupsulePlanetParam {
        /* 0x0 */ const char* mBlueStarName;
        /* 0x4 */ f32 mRadius;
    };
};  // namespace

class GCaptureTargetable {
public:
    GCaptureTargetable() {
    }

    virtual bool canEndHold() const;
    virtual bool isReleaseForce() const;
    virtual f32 releaseDistance() const;
    virtual f32 getPointableRange() const;
    virtual void getTargetPosition(TVec3f*);
    virtual void releasedTarget();
    virtual void decidedTarget();
};

class BlueStarCupsulePlanet : public LiveActor, public GCaptureTargetable {
public:
    BlueStarCupsulePlanet(const char*);

    virtual ~BlueStarCupsulePlanet();
    virtual void init(const JMapInfoIter&);
    virtual void startClipped();
    virtual void endClipped();
    virtual void control();
    virtual void calcAndSetBaseMtx();

    void updatePose();
    void decidedTarget();
    void releasedTarget();
    void getTargetPosition(TVec3f*);
    bool isReleaseForce() const;
    f32 getPointableRange() const;
    void emitNerveEffect();
    void exeWait();
    void exePointable();
    void exeHitPointer();
    void exeActive();
    bool isPointable() const;
    void activateMapPartsFunction();
    void deactivateMapPartsFunction();

    /* 0x90 */ MapPartsRailMover* mRailMover;
    /* 0x94 */ MapPartsRotator* mRotator;
    /* 0x98 */ const char* mName;
    /* 0x9C */ bool mIsStepUpdatable;

private:
    u8 mPad[(0xA0) - sizeof(LiveActor)];
};
