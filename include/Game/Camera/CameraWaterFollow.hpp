#pragma once

#include "Game/Camera/Camera.hpp"

class CameraWaterFollow : public Camera {
public:
    CameraWaterFollow(const char* pName = "水中フォロー");

    /* 0x24 */ virtual void reset();
    /* 0x28 */ virtual CameraTargetObj* calc();
    /* 0x50 */ virtual bool isEnableToReset() const {
        return true;
    }
    /* 0x54 */ virtual CamTranslatorBase* createTranslator();

    void setParam(f32 distMin, f32 distMax, f32 blendRateMin) {
        mDistMin = distMin;
        mDistMax = distMax;
        mSideBlendRateMin = blendRateMin;
    }

    /* 0x4C */ f32 mDistMin;
    /* 0x50 */ f32 mDistMax;
    /* 0x54 */ f32 mSideBlendRateMin;
    /* 0x58 */ f32 mSideBlendRate;
    /* 0x5C */ s32 mCollideCount;
    /* 0x60 */ TVec3f mLastMoveDir;
    /* 0x6C */ bool mIsRounding;
    /* 0x70 */ s32 mRoundingFrame;
    /* 0x74 */ TVec3f mSide;
};
