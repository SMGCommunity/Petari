#pragma once

#include "Game/Enemy/Onimasu.hpp"
#include "Game/Util/Array.hpp"

class OnimasuPivot : public Onimasu {
public:
    OnimasuPivot(const char*);
    virtual ~OnimasuPivot();

    virtual void initFromRailPoint();
    virtual void incrementNextPoint();
    virtual void collectRailPointInfo();
    virtual s32 getNextPointNo() const;
    virtual s32 getLastPointNo() const;
    const TVec3f& getLastPointNormal() const;
    const TVec3f& getNextPointNormal() const;
    virtual void startMoveInner();
    virtual void updatePoseInner();

    /* 0x104 */ s32 mCurNormal;
    MR::AssignableArray< TVec3f > mNormals;
    TQuat4f _110;
    TQuat4f _120;
};
