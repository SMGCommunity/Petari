#pragma once

#include "Game/Camera/Camera.hpp"
#include "JSystem/JGeometry/TMatrix.hpp"

class CameraDPD : public Camera {
public:
    CameraDPD(const char* pName = "DPD操作カメラ");
    virtual ~CameraDPD();

    virtual void reset();
    virtual CameraTargetObj* calc();
    virtual CamTranslatorBase* createTranslator();

    f32 _4C;
    f32 _50;
    f32 _54;
    f32 _58;
    f32 _5C;
    TMtx34f _60;
    bool _90;
    u8 _91[3];
    u32 _94;
    f32 mDist;  // 0x98
    f32 _9C;
    f32 mAngleA;   // 0xA0
    f32 mAngleB;   // 0xA4
    f32 mWPointZ;  // 0xA8
    f32 mWPointX;  // 0xAC
    f32 mWPointY;  // 0xB0
    bool _B4;
    u8 _B5[3];
    f32 mUpX;  // 0xB8
};