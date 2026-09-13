#pragma once

#include "Game/Enemy/Onimasu.hpp"
#include "Game/Util/Array.hpp"

class OnimasuPivot : public Onimasu {
public:
    /// @brief Creates a new `OnimasuPivot`.
    /// @param pName A pointer to the null-terminated name of the object.
    OnimasuPivot(const char* pName);

    virtual void initFromRailPoint();
    virtual void incrementNextPoint();
    virtual void collectRailPointInfo();
    virtual s32 getNextPointNo() const;
    virtual s32 getLastPointNo() const;
    virtual const TVec3f& getLastPointNormal() const;
    virtual const TVec3f& getNextPointNormal() const;
    virtual void startMoveInner();
    virtual void updatePoseInner();

    const TVec3f getLastPointPos() const;
    const TVec3f getPivotPointPos() const;
    const TVec3f getNextPointPos() const;

    /* 0x104 */ s32 mCurNormal;
    /* 0x108 */ MR::AssignableArray< TVec3f > mNormals;
    /* 0x110 */ TQuat4f _110;
    /* 0x120 */ TQuat4f _120;
};
