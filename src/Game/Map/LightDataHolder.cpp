#include "Game/Map/LightDataHolder.hpp"
#include "Game/Map/LightFunction.hpp"
#include "Game/Util.hpp"

namespace {
    static const char* sDefaultAreaLightName = "デフォルト";

    static LightInfoCoin sDefaultLightSetCoin = {{{0xFF, 0xFF, 0, 0}, {0.0f, 0.0f, 0.0f}, true}, {0, 0, 0, 0, 65.0f}};
};  // namespace

LightDataHolder::LightDataHolder() {
    mLightCount = 0;
    mLights = 0;
}

void LightDataHolder::initLightData() {
    JMapInfo* data = NULL;
    mLightCount = LightFunction::createLightDataParser(&data);
    mLights = new AreaLightInfo[mLightCount];

    for (s32 i = 0; i < mLightCount; i++) {
        LightFunction::getAreaLightLightData(data, i, &mLights[i]);
    }

    _8 = sDefaultLightSetCoin;
}

AreaLightInfo* LightDataHolder::findAreaLight(const char* pName) const {
    for (s32 i = 0; i < mLightCount; i++) {
        AreaLightInfo* inf = getLightInfo(i);

        if (MR::isEqualString(pName, inf->mAreaLightName)) {
            return inf;
        }
    }

    return mLights;
}

const char* LightDataHolder::getDefaultAreaLightName() const {
    return ::sDefaultAreaLightName;
}

s32 LightDataHolder::getDefaultStepInterpolate() const {
    return 0x1E;
}
