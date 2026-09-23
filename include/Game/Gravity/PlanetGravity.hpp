#pragma once

#include <JSystem/JGeometry.hpp>

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

    virtual void updateMtx(const TPos3f& rMtx);

    virtual bool calcOwnGravityVector(TVec3f* pDest, f32* pScalar, const TVec3f& rPosition) const {
        return false;
    }

    bool calcGravity(TVec3f* pDest, const TVec3f& rPosition) const;
    bool calcGravityFromMassPosition(TVec3f* pDirection, f32* pScalar, const TVec3f& rPosition, const TVec3f& rMassPosition) const;

    bool isInRangeSquare(f32 radius) const;
    bool isInRangeDistance(f32 radius) const;

    f32 getDistant() const {
        return mDistant;
    }

    void setPriority(s32 priority);
    void updateIdentityMtx();

    /* 0x4 */ f32 mRange;
    /* 0x8 */ f32 mDistant;
    /* 0xC */ s32 mPriority;
    /* 0x10 */ s32 mGravityId;     // read from the stage files but never used
    /* 0x14 */ const void* mHost;  // host object pointer, can be any pointer
    /* 0x18 */ u32 mGravityType;   // gravity type bits
    /* 0x1C */ s32 mGravityPower;  // gravity power type
    /* 0x20 */ bool mActivated;    // corresponding GlobalGravityObj has switch activated?
    /* 0x21 */ bool mIsInverse;    // invert gravity vector?
    /* 0x22 */ bool mValidFollower;
    /* 0x23 */ bool mIsRegistered;  // is registered by PlanetGravityManager?
    /* 0x24 */ bool mAppeared;      // corresponding GlobalGravityObj has appeared?
};
