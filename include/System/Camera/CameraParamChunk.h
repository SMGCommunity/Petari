#ifndef CAMERAPARAMCHUNK_H
#define CAMERAPARAMCHUNK_H

#include "types.h"
#include "JGeometry/TVec3.h"

class CameraParamChunk
{
public:
    u8 _0[0x2C];
    u16 mFlags; // _2C
    u8 _2E[0x54-0x2E];
    f32 mVPanAxisX; // _54
    f32 mVPanAxisY; // _58
    f32 mVPanAxisZ; // _5C

    s32 getZoneID() const;
    bool isOnNoReset() const;
    bool isOnUseFovy() const;
    bool isLOfsErpOff() const;
    bool isAntiBlurOff() const;
    bool isCollisionOff() const;
    bool isSubjectiveCameraOff() const;
    void getVPanAxis(JGeometry::TVec3<f32> *) const;

    void setUseFovy(bool);
    void setLOFsErpOff(bool);
    void setCollisionOff(bool);

    void arrangeCamTypeName(u32, const char**);
};

#endif // CAMERAPARAMCHUNK_H