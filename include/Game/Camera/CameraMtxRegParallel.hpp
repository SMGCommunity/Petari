#pragma once

#include "Game/Camera/Camera.hpp"

class CameraMtxRegParallel : public Camera {
public:
    CameraMtxRegParallel(const char* pName = "マトリクスレジスタ並行カメラ");
    virtual ~CameraMtxRegParallel();

    virtual void reset();
    virtual CameraTargetObj* calc();
    virtual CamTranslatorBase* createTranslator();

    void setParam(const char* str, f32 f1, const TVec2f& v2, const TVec3f& vec) {
        mString = str;
        mAngleB = v2.x;
        mAngleA = v2.y;
        mDist = f1;
        mWPoint = vec;
    }

    void calcIdealPose();

    const char* mString;  // 0x4C
    f32 mAngleB;          // 0x50
    f32 mAngleA;          // 0x54
    f32 mDist;            // 0x58
    TVec3f mWPoint;       // 0x5C
};
