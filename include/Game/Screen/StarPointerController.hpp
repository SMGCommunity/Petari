#pragma once

#include <JSystem/JGeometry/TVec.hpp>

class DpdInfo {
public:
    DpdInfo();

    /* 0x00 */ TVec2f mPos;
    /* 0x08 */ bool mInScreen;
    /* 0x09 */ bool _9;
    /* 0x0C */ u32 _C;
    /* 0x10 */ f32 mViewDistZ;
};

class StarPointerController {
public:
    StarPointerController();

    bool isOutScreenLong() const;
    void initAndSetPort(s32);
    void movement(const f32*, const f32*);
    void storeDataFromCallback(const f32*, const f32*);
    void storePastPointingData();
    void calcPastPointingPosOnScreen(TVec2f*, s32) const;
    void updateDpdInfo();
    void updateAdditionalInfo();
    f32 calcViewDistanceZ(const TVec3f&, MtxPtr);

    void drawDebug3D() const;

    /* 0x00 */ s32 mPadChannel;
    /* 0x04 */ DpdInfo mPastInfo;
    /* 0x18 */ DpdInfo mInfo;
    /* 0x2C */ TVec2f mScreenPos;
    /* 0x34 */ s32 mOutScreenTime;
    /* 0x38 */ TVec2f* mPrevPastPointingPos;
    /* 0x3C */ s32 mPrevPastCount;
    /* 0x40 */ TVec2f* mPastPointingPos;
    /* 0x44 */ s32 mPastCount;
    /* 0x48 */ f32 mScreenDist;
    /* 0x4C */ f32 mScreenSpeed;
    /* 0x50 */ TVec2f mScreenVel;
    /* 0x58 */ TVec3f mWorldVel;
    /* 0x64 */ TVec3f mWorldPos;
    /* 0x70 */ bool mNotInScreen;
};
