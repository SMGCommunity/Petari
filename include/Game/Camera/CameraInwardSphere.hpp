#pragma once

#include "Game/Camera/Camera.hpp"

class CameraInwardSphere : public Camera {
public:
    CameraInwardSphere(const char* pName = "球内部カメラ");
    /* 0x08 */ virtual ~CameraInwardSphere();

    /* 0x24 */ virtual void reset();
    /* 0x28 */ virtual CameraTargetObj* calc();
    /* 0x54 */ virtual CamTranslatorBase* createTranslator();

    void calcGravityCenter();

    void setParam(f32 minDist, f32 maxDist, f32 distBase) {
        mDistMin = minDist;
        mDistMax = maxDist;
        mDistBase = distBase;
    }

    /* 0x4C */ f32 mDistMax;
    /* 0x50 */ f32 mDistMin;
    /* 0x54 */ f32 mDistBase;
    /* 0x58 */ TVec3f mVelocity;
    /* 0x64 */ TVec3f mPos;
    /* 0x70 */ TVec3f mGravityCenter;
};
