#pragma once

#include "Game/Camera/Camera.hpp"

class CameraCharmedTripodBoss : public Camera {
public:
    CameraCharmedTripodBoss(const char* pName = "三脚ボスジョイント注視カメラ");

    /* 0x24 */ virtual void reset();
    /* 0x28 */ virtual CameraTargetObj* calc();
    /* 0x54 */ virtual CamTranslatorBase* createTranslator();

    void setParam(s32, TVec3f, const TVec3f&, const TVec2f&);

    /* 0x4C */ s32 mJointId;
    /* 0x50 */ TVec3f mUp;
    /* 0x5C */ TVec3f mWPoint;
    /* 0x68 */ f32 mAngleX;
    /* 0x6C */ f32 mAngleY;
};
