#pragma once

#include <JSystem/JGeometry/TVec.hpp>
#include <revolution/types.h>

class CameraPoseParam {
public:
    CameraPoseParam();

    void copyFrom(const CameraPoseParam&);

    /* 0x00 */ TVec3f mWatchUpVec;
    /* 0x0C */ TVec3f mWatchPos;
    /* 0x18 */ TVec3f mUpVec;
    /* 0x24 */ TVec3f mPos;
    /* 0x30 */ f32 mFovy;
    /* 0x34 */ TVec3f mGlobalOffset;
    /* 0x40 */ TVec3f mLocalOffset;
    /* 0x4C */ f32 mFrontOffset;
    /* 0x50 */ f32 mUpperOffset;
    /* 0x54 */ f32 mRoll;
};
