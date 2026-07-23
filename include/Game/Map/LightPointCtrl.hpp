#pragma once

#include "Game/Util/Color.hpp"
#include <JSystem/JGeometry/TVec.hpp>
#include <revolution.h>

class LiveActor;

class PointLightInfo {
public:
    void operator=(const PointLightInfo&);

    u32 _0;
    u32 _4;
    u32 _8;
    u8 _C;
    u8 _D;
    u8 _E;
    u8 _F;
    f32 _10;
    f32 _14;
    u32 _18;
};

class LightPointCtrl {
public:
    LightPointCtrl();

    void loadPointLight();
    void update();
    void clearPointLight(PointLightInfo*);
    void requestPointLight(const LiveActor*, TVec3f, Color8, f32, s32);

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
