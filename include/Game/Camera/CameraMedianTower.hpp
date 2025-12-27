#pragma once

#include "Game/Camera/Camera.hpp"

class CameraMedianTower : public Camera {
public:
    CameraMedianTower(const char* pName = "中点塔カメラ");
    virtual ~CameraMedianTower();

    virtual void reset();
    virtual CameraTargetObj* calc();
    virtual CamTranslatorBase* createTranslator();

    void calcFovyPos(TVec3f*, const TVec3f&, const TVec3f&, const TVec3f&);
    void limitAngularVelocity(TVec3f*, TVec3f*, const TVec3f&, const TVec3f&, const TVec3f&);
    void calcDistance(TVec3f*, const TVec3f&);
    void updateReset(const TVec3f&, const TVec3f&, const TVec3f&);

    const char* mString;  // 0x4C
    TVec3f mWPoint;       // 0x50
    TVec3f mAxis;         // 0x5C
    f32 mAngleA;          // 0x68
    f32 mUpX;             // 0x6C
    f32 mUpY;             // 0x70
    f32 mUpZ;             // 0x74
    f32 _78;
    f32 _7C;
    f32 _80;
    u8 _84;
    u8 _85[3];
    u32 _88;
};