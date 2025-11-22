#pragma once

#include "Game/Gravity/ConeGravity.hpp"
#include "Game/Gravity/CubeGravity.hpp"
#include "Game/Gravity/DiskGravity.hpp"
#include "Game/Gravity/DiskTorusGravity.hpp"
#include "Game/Gravity/ParallelGravity.hpp"
#include "Game/Gravity/PointGravity.hpp"
#include "Game/Gravity/SegmentGravity.hpp"
#include "Game/Gravity/WireGravity.hpp"
#include "Game/LiveActor/RailRider.hpp"
#include "Game/Util/JMapInfo.hpp"

class GravityCreator {
public:
    GravityCreator();

    virtual PlanetGravity* getGravity() = 0;
    virtual PlanetGravity* createInstance() = 0;
    virtual void settingFromSRT(const TVec3f& rTrans, const TVec3f& rRotate, const TVec3f& rScale) {}
    virtual void settingFromJMapArgs(s32 arg0, s32 arg1, s32 arg2) {}
    virtual void settingFromJMapOtherParam(const JMapInfoIter& rIter) {}

    PlanetGravity* createFromJMap(const JMapInfoIter& rIter);
};

class CubeGravityCreator : public GravityCreator {
public:
    inline CubeGravityCreator() : GravityCreator(), mGravityInstance(nullptr) {}

    virtual PlanetGravity* getGravity();
    virtual PlanetGravity* createInstance();
    virtual void settingFromSRT(const TVec3f& rTrans, const TVec3f& rRotate, const TVec3f& rScale);
    virtual void settingFromJMapArgs(s32 arg0, s32 arg1, s32 arg2);

    CubeGravity* mGravityInstance;  // 0x4
};

class DiskGravityCreator : public GravityCreator {
public:
    inline DiskGravityCreator() : GravityCreator(), mGravityInstance(nullptr) {}

    virtual PlanetGravity* getGravity();
    virtual PlanetGravity* createInstance();
    virtual void settingFromSRT(const TVec3f& rTrans, const TVec3f& rRotate, const TVec3f& rScale);
    virtual void settingFromJMapArgs(s32 arg0, s32 arg1, s32 arg2);

    DiskGravity* mGravityInstance;  // 0x4
};

class DiskTorusGravityCreator : public GravityCreator {
public:
    inline DiskTorusGravityCreator() : GravityCreator(), mGravityInstance(nullptr) {}

    virtual PlanetGravity* getGravity();
    virtual PlanetGravity* createInstance();
    virtual void settingFromSRT(const TVec3f& rTrans, const TVec3f& rRotate, const TVec3f& rScale);
    virtual void settingFromJMapArgs(s32 arg0, s32 arg1, s32 arg2);

    DiskTorusGravity* mGravityInstance;  // 0x4
};

class ConeGravityCreator : public GravityCreator {
public:
    inline ConeGravityCreator() : GravityCreator(), mGravityInstance(nullptr) {}

    virtual PlanetGravity* getGravity();
    virtual PlanetGravity* createInstance();
    virtual void settingFromSRT(const TVec3f& rTrans, const TVec3f& rRotate, const TVec3f& rScale);
    virtual void settingFromJMapArgs(s32 arg0, s32 arg1, s32 arg2);

    ConeGravity* mGravityInstance;  // 0x4
};

class PlaneGravityCreator : public GravityCreator {
public:
    inline PlaneGravityCreator() : GravityCreator(), mGravityInstance(nullptr) {}

    virtual PlanetGravity* getGravity();
    virtual PlanetGravity* createInstance();
    virtual void settingFromSRT(const TVec3f& rTrans, const TVec3f& rRotate, const TVec3f& rScale);

    ParallelGravity* mGravityInstance;  // 0x4
};

class PlaneInBoxGravityCreator : public GravityCreator {
public:
    inline PlaneInBoxGravityCreator() : GravityCreator(), mGravityInstance(nullptr) {}

    virtual PlanetGravity* getGravity();
    virtual PlanetGravity* createInstance();
    virtual void settingFromSRT(const TVec3f& rTrans, const TVec3f& rRotate, const TVec3f& rScale);
    virtual void settingFromJMapArgs(s32 arg0, s32 arg1, s32 arg2);

    ParallelGravity* mGravityInstance;  // 0x4
};

class PlaneInCylinderGravityCreator : public GravityCreator {
public:
    inline PlaneInCylinderGravityCreator() : GravityCreator(), mGravityInstance(nullptr) {}

    virtual PlanetGravity* getGravity();
    virtual PlanetGravity* createInstance();
    virtual void settingFromSRT(const TVec3f& rTrans, const TVec3f& rRotate, const TVec3f& rScale);
    virtual void settingFromJMapArgs(s32 arg0, s32 arg1, s32 arg2);

    ParallelGravity* mGravityInstance;  // 0x4
};

class PointGravityCreator : public GravityCreator {
public:
    inline PointGravityCreator() : GravityCreator(), mGravityInstance(nullptr) {}

    virtual PlanetGravity* getGravity();
    virtual PlanetGravity* createInstance();
    virtual void settingFromSRT(const TVec3f& rTrans, const TVec3f& rRotate, const TVec3f& rScale);

    PointGravity* mGravityInstance;  // 0x4
};

class SegmentGravityCreator : public GravityCreator {
public:
    inline SegmentGravityCreator() : GravityCreator(), mGravityInstance(nullptr) {}

    virtual PlanetGravity* getGravity();
    virtual PlanetGravity* createInstance();
    virtual void settingFromSRT(const TVec3f& rTrans, const TVec3f& rRotate, const TVec3f& rScale);
    virtual void settingFromJMapArgs(s32 arg0, s32 arg1, s32 arg2);

    SegmentGravity* mGravityInstance;  // 0x4
};

class WireGravityCreator : public GravityCreator {
public:
    inline WireGravityCreator() : GravityCreator(), mRailRider(nullptr), mGravityInstance(nullptr) {}

    virtual PlanetGravity* getGravity();
    virtual PlanetGravity* createInstance();
    virtual void settingFromJMapOtherParam(const JMapInfoIter& rIter);

    RailRider* mRailRider;          // 0x4
    WireGravity* mGravityInstance;  // 0x8
};
