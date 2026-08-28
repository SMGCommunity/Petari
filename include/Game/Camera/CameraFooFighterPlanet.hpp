#pragma once

#include "Game/Camera/Camera.hpp"

class CameraFooFighterPlanet : public Camera {
public:
    CameraFooFighterPlanet(const char* pName = "フーファイタープラネットカメラ");
    /* 0x08 */ virtual ~CameraFooFighterPlanet();

    /* 0x24 */ virtual void reset();
    /* 0x28 */ virtual CameraTargetObj* calc();
    /* 0x54 */ virtual CamTranslatorBase* createTranslator();

    void goRoundBehind(TVec3f&, TVec3f&, TVec3f&);

    void setParam(f32 distMin, f32 distMax, f32 pitchMax) {
        mDistMin = distMin;
        mDistMax = distMax;
        mPitchMax = pitchMax;
    }

    /* 0x4C */ f32 mDistMin;
    /* 0x50 */ f32 mDistMax;
    /* 0x54 */ f32 mPitchMax;
};
