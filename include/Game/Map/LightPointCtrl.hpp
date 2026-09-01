#pragma once

#include "Game/Util/Color.hpp"
#include "revolution/gx/GXEnum.h"
#include <JSystem/JGeometry.hpp>
#include <revolution.h>


class LiveActor;

class PointLightInfo {
public:
    void operator=(const PointLightInfo&);

    Vec _0;
    GXColor _C;
    f32 _10;
    f32 _14;
    GXDistAttnFn _18;
};

class LightPointCtrl {
public:
    LightPointCtrl();

    void loadPointLight();
    void update();
    void requestPointLight(const LiveActor*, TVec3f, Color8, f32, s32);
    void clearPointLight(PointLightInfo*);

    bool tryBlendStart();

    void updatePointLight();

    s32 _0;
    u32 _4;
    u32 _8;
    u32 _C;
    u32 _10;
    PointLightInfo* _14;
    PointLightInfo* _18;
    PointLightInfo* _1C;
};
