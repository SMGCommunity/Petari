#pragma once

#include "Game/Gravity/GravityCreator.hpp"
#include "Game/LiveActor/LiveActor.hpp"

class GlobalGravityObj : public LiveActor {
public:
    GlobalGravityObj(const char* pName);

    /* 0x08 */ virtual ~GlobalGravityObj() {
    }
    /* 0x0C */ virtual void init(const JMapInfoIter& rIter);
    /* 0x14 */ virtual void movement();
    /* 0x28 */ virtual void makeActorAppeared();
    /* 0x30 */ virtual void makeActorDead();

    void updateSwitch();
    PlanetGravity* getGravity();

    GravityCreator* mGravityCreator;  // 0x8C
};

namespace MR {
    NameObj* createGlobalCubeGravityObj(const char* pName);
    NameObj* createGlobalConeGravityObj(const char* pName);
    NameObj* createGlobalDiskGravityObj(const char* pName);
    NameObj* createGlobalDiskTorusGravityObj(const char* pName);
    NameObj* createGlobalPlaneGravityObj(const char* pName);
    NameObj* createGlobalPlaneInBoxGravityObj(const char* pName);
    NameObj* createGlobalPlaneInCylinderGravityObj(const char* pName);
    NameObj* createGlobalPointGravityObj(const char* pName);
    NameObj* createGlobalSegmentGravityObj(const char* pName);
    NameObj* createGlobalWireGravityObj(const char* pName);
};  // namespace MR
