#pragma once

#include "JSystem/JGeometry.hpp"

class NameObj;

// Gravity vector calculation flags
#define GRAVITY_TYPE_NORMAL 1
#define GRAVITY_TYPE_SHADOW 2
#define GRAVITY_TYPE_MAGNET 4
#define GRAVITY_TYPE_MARIO_LAUNCHER 8

// Gravity power types
#define GRAVITY_POWER_LIGHT 0
#define GRAVITY_POWER_NORMAL 1
#define GRAVITY_POWER_HEAVY 2

class PlanetGravity {
public:
    PlanetGravity();

    virtual void updateMtx(const TPos3f& rMtx) {}

    virtual bool calcOwnGravityVector(TVec3f* pDest, f32* pScalar, const TVec3f& rPosition) const { return false; }

    bool calcGravity(TVec3f* pDest, const TVec3f& rPosition) const;
    bool calcGravityFromMassPosition(TVec3f* pDirection, f32* pScalar, const TVec3f& rPosition, const TVec3f& rMassPosition) const;

    bool isInRangeSquare(f32 radius) const;
    bool isInRangeDistance(f32 radius) const;

    void setPriority(s32 priority);
    void updateIdentityMtx();

    f32 mRange;           // 0x4
    f32 mDistant;         // 0x8
    s32 mPriority;        // 0xC
    s32 mGravityId;       // 0x10, read from the stage files but never used
    const void* mHost;    // 0x14, host object pointer, can be any pointer
    u32 mGravityType;     // 0x18, gravity type bits
    s32 mGravityPower;    // 0x1C, gravity power type
    bool mActivated;      // 0x20, corresponding GlobalGravityObj has switch activated?
    bool mIsInverse;      // 0x21, invert gravity vector?
    bool mValidFollower;  // 0x22
    bool mIsRegistered;   // 0x23, is registered by PlanetGravityManager?
    bool mAppeared;       // 0x24, corresponding GlobalGravityObj has appeared?
};
