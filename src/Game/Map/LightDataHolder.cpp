#include "Game/Map/LightDataHolder.hpp"
#include "Game/Util.hpp"

namespace {
    static const char* sDefaultAreaLightName = "デフォルト";
};

LightDataHolder::LightDataHolder() {
    mLightCount = 0;
    mLights = 0;
}

#ifdef NON_MATCHING
// regalloc issues
AreaLightInfo* LightDataHolder::findAreaLight(const char *pName) const {
    for (s32 i = 0; i < mLightCount; i++) {
        AreaLightInfo* inf = &mLights[i];

        if (MR::isEqualString(pName, inf->mAreaLightName)) {
            return inf;
        }
    }

    return mLights;
}
#endif

const char* LightDataHolder::getDefaultAreaLightName() const {
    return sDefaultAreaLightName;
}

s32 LightDataHolder::getDefaultStepInterpolate() const {
    return 0x1E;
}
