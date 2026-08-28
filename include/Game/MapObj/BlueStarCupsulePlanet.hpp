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

    /* 0x08 */ virtual void decidedTarget();
    /* 0x0C */ virtual void releasedTarget();
    /* 0x10 */ virtual void getTargetPosition(TVec3f*);
    /* 0x14 */ virtual bool canEndHold() const;
    /* 0x18 */ virtual bool isReleaseForce() const;
    virtual f32 releaseDistance() const;
    virtual f32 getPointableRange() const;
};

class BlueStarCupsulePlanet : public LiveActor, public GCaptureTargetable {
public:
    BlueStarCupsulePlanet(const char*);

    /* 0x1C */ virtual ~BlueStarCupsulePlanet();
    /* 0x20 */ virtual void init(const JMapInfoIter&);
    /* 0x24 */ virtual void startClipped();
    /* 0x28 */ virtual void endClipped();
    /* 0x2C */ virtual void control();
    /* 0x30 */ virtual void calcAndSetBaseMtx();

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
};
