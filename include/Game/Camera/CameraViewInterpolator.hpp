#pragma once

#include "JSystem/JGeometry.hpp"

class CameraTargetObj;

class CameraViewInterpolator {
public:
    CameraViewInterpolator();

    void updateCameraMtx(MtxPtr, const TVec3f&, const CameraTargetObj*, f32);
    void setInterpolation(u32);
    bool isInterpolating() const;
    bool isInterpolatingNearlyEnd() const;
    void lookAtCenter();
    void calcCollision(const TPos3f&);
    void calcBinder(TVec3f*, const TVec3f&, const TVec3f&);
    void reduceOscillation();
    void updateCalcState(const CameraTargetObj*);
    void interpolateCameraSwitching(MtxPtr, const TVec3f&, f32);
    void checkNearlyEnd(MtxPtr);
    void translateByRepulsion();

    u32 _0;
    u32 _4;
    bool _8;
    bool _9;
    bool _A;
    u8 _B;
    TMtx34f _C;
    TMtx34f _3C;
    f32 _6C;
    f32 _70;
    f32 _74;
    f32 _78;
    bool _7C;
    bool _7D;
    u8 _7E[2];
    f32 _80;
    f32 _84;
    bool _88;
    bool _89;
    bool _8A;
    u8 _8B;
    u32 _8C;
    u32 _90;
    u32 _94;
    f32 _98;
    f32 _9C;
    f32 _A0;
    f32 _A4;
    f32 _A8;
    f32 _AC;
    bool _B0;
    u8 _B1[3];
    f32 _B4;
    f32 _B8;
    f32 _BC;
};