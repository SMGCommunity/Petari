#pragma once

#include <revolution.h>
#include "JSystem/JGeometry/TVec.h"

class JMapInfo;

namespace {
    void loadLightDiffuse(_GXColor color, const TVec3f &rPos, _GXLightID lightID) {
        GXLightObj lightObj;

        GXInitLightPos(&lightObj, rPos.x, rPos.y, rPos.z);
        GXInitLightAttn(&lightObj, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);

        _GXColor new_color(color);
        GXInitLightColor(&lightObj, new_color);
        GXLoadLightObjImm(&lightObj, lightID);
    }
};

class LightFunction {
public:
    static const char* getDefaultAreaLightName();

    static s32 createZoneDataParser(const char *, JMapInfo **);
};
