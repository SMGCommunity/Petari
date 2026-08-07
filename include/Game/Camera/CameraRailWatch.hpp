#pragma once

#include "Game/Camera/Camera.hpp"

class RailRider;

class CameraRailWatch : public Camera {
public:
    CameraRailWatch(const char* pName = "レール注目カメラ");

    virtual void reset();
    virtual CameraTargetObj* calc();
    virtual CamTranslatorBase* createTranslator();

    void setParam(s32, s32, s32, f32, f32, f32);

    /* 0x4C */ RailRider* mRailRider;
    /* 0x50 */ s32 mDirection;
    /* 0x54 */ s32 mSetDirection;
    /* 0x58 */ f32 mRailCoordOffset;
    /* 0x5C */ f32 mDist;
    /* 0x60 */ f32 mAngleX;
};
