#pragma once

#include "Game/Camera/Camera.hpp"

class CameraTower : public Camera {
public:
    CameraTower(const char* pName = "塔カメラ");
    virtual ~CameraTower();

    virtual void reset();
    virtual CameraTargetObj* calc();
    virtual bool isEnableToRoundLeft() const;
    virtual bool isEnableToRoundRight() const;
    virtual CamTranslatorBase* createTranslator();

    void arrangeRoundingPose();
    void arrangeHorizontalPose();
    bool isUpsideDownByRoll() const;
    void arrangeRound();

    TVec3f mWPoint;  // 0x4C
    TVec3f mAxis;    // 0x58
    f32 mAngleB;     // 0x64
    f32 mAngleA;     // 0x68
    f32 mDist;       // 0x6C
    f32 _70;
    f32 _74;
    f32 _78;
    u8 _7C;
    u8 _7D[3];
    u32 _80;
    f32 _84;
    u8 _88;
    u8 _89;
    u8 _8A[2];
};