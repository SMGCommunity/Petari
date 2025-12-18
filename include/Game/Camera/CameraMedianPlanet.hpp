#pragma once

#include "Game/Camera/Camera.hpp"
#include "JSystem/JGeometry.hpp"

class CameraMedianPlanet : public Camera {
public:
    CameraMedianPlanet(const char* pName = "中点注目プラネットカメラ");
    virtual ~CameraMedianPlanet();

    virtual void reset();
    virtual CameraTargetObj* calc();
    virtual CamTranslatorBase* createTranslator();

    void getPrevFront(TVec3f*);
    void constrainDipAngle(TVec3f*);
    void makeTargetMtx(TPos3f*);
    void peepMtx(TPos3f*);
    void rotate33(TPos3f*, const TVec3f&, f32);
    void constrainLength(TVec3f*);
    void getMaxRate();
    void getRegisterUpVec(TVec3f*);
    void calcWatchPos(TVec3f*);
    void calcUpVec(TVec3f*, const TVec3f&, const TVec3f&);

    const char* mString;  // 0x4C
    f32 mAxisX;           // 0x50
    f32 mAxisY;           // 0x54
    f32 mAngleA;          // 0x58
    f32 mAngleB;          // 0x5C
    f32 mDist;            // 0x60
    f32 mAxisZ;           // 0x64
    f32 mWPointX;         // 0x68
};