#pragma once

#include "Game/Enemy/Onimasu.hpp"
#include "Game/Util/Array.hpp"

class OnimasuJump : public Onimasu {
public:
    /// @brief Creates a new `OnimasuJump`.
    /// @param pName A pointer to the null-terminated name of the object.
    OnimasuJump(const char* pName);

    virtual void initFromRailPoint();
    virtual void incrementNextPoint();
    virtual void collectRailPointInfo();
    virtual s32 getNextPointNo() const;
    virtual s32 getLastPointNo() const;
    virtual const TVec3f& getLastPointNormal() const;
    virtual const TVec3f& getNextPointNormal() const;
    virtual void startMoveInner();
    virtual void updatePoseInner();

    void calcJumpVelocity();

private:
    /* 0x104 */ s32 mCurNormal;
    /* 0x108 */ MR::AssignableArray< TVec3f > mNormals;
};
