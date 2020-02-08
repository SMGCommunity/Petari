#ifndef CAMERAPARAMCHUNK_H
#define CAMERAPARAMCHUNK_H

#include <revolution.h>
#include "JGeometry/TVec3.h"
#include "System/Camera/DotCamParams.h"
#include "System/Camera/CameraHolder.h"

class CameraParamChunk
{
public:

    s32 getZoneID() const;
    bool isOnNoReset() const;
    bool isOnUseFovy() const;
    bool isLOfsErpOff() const;
    bool isAntiBlurOff() const;
    bool isCollisionOff() const;
    bool isSubjectiveCameraOff() const;
    void getVPanAxis(JGeometry::TVec3<f32> *) const;

    void load(DotCamReader *, CameraHolder *);

    void setUseFovy(bool);
    void setLOFsErpOff(bool);
    void setCollisionOff(bool);

    void arrangeCamTypeName(u32, const char**);

    u8 _0[0xC];
    JGeometry::TVec3<f32> mWOffset; // _C
    f32 mLOffset; // _18
    f32 mLOffsetV; // _1C
    f32 mRoll; // _20
    f32 mFovy; // _24
    s32 mCamInt; // _28
    u16 mFlags; // _2C
    u8 _2E[0x54-0x2E];
    f32 mVPanAxisX; // _54
    f32 mVPanAxisY; // _58
    f32 mVPanAxisZ; // _5C
};

#endif // CAMERAPARAMCHUNK_H