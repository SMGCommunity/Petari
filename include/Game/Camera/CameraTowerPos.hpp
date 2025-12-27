#pragma once

#include "Game/Camera/CameraTowerBase.hpp"

class CameraTowerPos : public CameraTowerBase {
public:
    CameraTowerPos(const char* pName = "塔カメラ（サブターゲット付き）");
    virtual ~CameraTowerPos();

    virtual void reset();
    virtual CameraTargetObj* calc();
    virtual bool isEnableToReset() const;
    virtual CamTranslatorBase* createTranslator();

    void calcIdealPose(bool);
    void arrangeHorizontalPose(const TVec3f&, TVec3f&);
    void arrangeWatchPos(TVec3f&, const TVec3f&);

    TVec3f mWPoint;  // 0x6C
    TVec3f mAxis;    // 0x78
    f32 mAngleA;     // 0x84
    f32 mAngleB;     // 0x88
    f32 mUpX;        // 0x8C
    f32 mUpY;        // 0x90
    f32 _94;
    f32 _98;
    f32 _9C;
};