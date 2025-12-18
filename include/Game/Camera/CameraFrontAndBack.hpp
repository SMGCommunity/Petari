#pragma once

#include "Game/Camera/Camera.hpp"
#include "JSystem/JGeometry/TQuat.hpp"

class CameraFrontAndBack : public Camera {
public:
    CameraFrontAndBack(const char* pName = "表裏カメラ");
    virtual ~CameraFrontAndBack();

    virtual void reset();
    virtual CameraTargetObj* calc();
    virtual CamTranslatorBase* createTranslator();

    void calcBaseQuat(TQuat4f*, const TQuat4f&);
    void calcDipQuat(TQuat4f*, const TQuat4f&, const TQuat4f&);

    TVec3f mWPoint;  // 0x4C
    TVec3f mAxis;    // 0x58
    f32 mAngleA;     // 0x64
    f32 mAngleB;     // 0x68
    f32 mDist;       // 0x6C
};