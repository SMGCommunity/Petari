#pragma once

#include "Game/Util/Color.hpp"
#include "revolution/gx/GXEnum.h"
#include <JSystem/JGeometry.hpp>
#include <revolution.h>

class LiveActor;

struct PointLightInfo {
    /* 0x00 */ Vec mPos;
    /* 0x0C */ GXColor mColor;
    /* 0x10 */ f32 mRefDistance;
    /* 0x14 */ f32 mRefBrightness;
    /* 0x18 */ GXDistAttnFn mDistAttnFn;
};

class LightPointCtrl {
public:
    LightPointCtrl();

    void loadPointLight();
    void update();
    void requestPointLight(const LiveActor* pActor, TVec3f pos, Color8 color, f32 intensity, s32 duration);
    void clearPointLight(PointLightInfo* pInfo);
    void blendPointLight(PointLightInfo* pDst, const PointLightInfo& rStart, const PointLightInfo& rEnd, f32 t);

    bool tryBlendStart();
    bool isUpdateCandidateActor(const LiveActor* pActor) const;

    void updatePointLight();

    s32 getStep() const {
        return mStep;
    }

    /* 0x00 */ s32 mStep;
    /* 0x04 */ s32 mBlendTime;
    /* 0x08 */ const LiveActor* mCurrentActor;
    /* 0x0C */ const LiveActor* mPreviousActor;
    /* 0x10 */ const LiveActor* mCandidateActor;
    /* 0x14 */ PointLightInfo* mCurrentInfo;
    /* 0x18 */ PointLightInfo* mTargetInfo;
    /* 0x1C */ PointLightInfo* mPreviousInfo;
};
