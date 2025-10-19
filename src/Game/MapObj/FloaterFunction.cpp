#include "Game/MapObj/FloaterFunction.hpp"
#include "Game/Util/StringUtil.hpp"

namespace { 
    struct SeName {
        /* 0x0 */ const char* mObjName;
        /* 0x4 */ const char* mSeGroundOn;
        /* 0x8 */ const char* mSeMoveDown;
        /* 0xC */ const char* mSeMoveUp;
    };

    static const SeName sSeNameTable[] = {
        {
            "LavaHomeFloaterA",
            "SE_OJ_LAVA_FLOATER_ON",
            "SE_OJ_LV_LAVA_FLOATER_DOWN",
            "SE_OJ_LV_LAVA_FLOATER_UP"
        },
        {
            "LavaHomeFloaterB",
            "SE_OJ_LAVA_FLOATER_ON_METAL",
            "SE_OJ_LV_LAVA_FLOATER_DOWN",
            "SE_OJ_LV_LAVA_FLOATER_UP"
        },
        {
            "LavaFloatingStepA",
            "SE_OJ_LAVA_FLOATER_ON_METAL",
            "SE_OJ_LV_LAVA_FLOATER_DOWN",
            "SE_OJ_LV_LAVA_FLOATER_UP"
        },
        {
            "LavaFloatingStepB",
            "SE_OJ_LAVA_FLOATER_ON_METAL",
            "SE_OJ_LV_LAVA_FLOATER_DOWN",
            "SE_OJ_LV_LAVA_FLOATER_UP"
        },
        {
            "LavaFloatingStepC",
            "SE_OJ_LAVA_FLOATER_ON_METAL",
            "SE_OJ_LV_LAVA_FLOATER_DOWN",
            "SE_OJ_LV_LAVA_FLOATER_UP"
        },
        {
            "LavaFloatingStepD",
            "SE_OJ_LAVA_FLOATER_ON_METAL",
            "SE_OJ_LV_LAVA_FLOATER_DOWN",
            "SE_OJ_LV_LAVA_FLOATER_UP"
        },
        {
            "IceVolcanoFloaterA",
            "SE_OJ_LAVA_FLOATER_ON",
            "SE_OJ_LV_LAVA_FLOATER_DOWN",
            "SE_OJ_LV_LAVA_FLOATER_UP"
        },
        {
            "IceVolcanoFloaterB",
            "SE_OJ_LAVA_FLOATER_ON",
            "SE_OJ_LV_LAVA_FLOATER_DOWN",
            "SE_OJ_LV_LAVA_FLOATER_UP"
        },
        {
            "FloaterOtakingFloaterA",
            "SE_OJ_LAVA_FLOATER_ON_METAL",
            "SE_OJ_LV_LAVA_FLOATER_DOWN",
            "SE_OJ_LV_LAVA_FLOATER_UP"
        },
        {
            "OceanPierFloaterA",
            nullptr,
            nullptr,
            nullptr
        },
        {
            "OceanHexagonFloater",
            nullptr,
            nullptr,
            nullptr
        },
        {
            "PhantomShipBoxFloaterA",
            nullptr,
            nullptr,
            nullptr
        },
        {
            "PhantomShipBoxFloaterB",
            nullptr,
            nullptr,
            nullptr
        },
        {
            "PhantomShipBoxFloaterC",
            nullptr,
            nullptr,
            nullptr
        },
        {
            "IceMountainFloaterA",
            nullptr,
            "SE_OJ_LV_ICE_M_FLOATER_DOWN",
            "SE_OJ_LV_ICE_M_FLOATER_UP"
        },
    };
};

namespace {
    const SeName* getSeNameTable(const char* pObjName) {
        u32 size = sizeof(sSeNameTable) / sizeof(*sSeNameTable);

        for (u32 i = 0; i < size; i++) {
            if (MR::isEqualString(pObjName, sSeNameTable[i].mObjName)) {
                return &sSeNameTable[i];
            }
        }

        return nullptr;
    }
};

namespace FloaterFunction {
    const char* getSeGroundOn(const char* pObjName) {
        return getSeNameTable(pObjName)->mSeGroundOn;
    }

    const char* getSeMoveDown(const char* pObjName) {
        return getSeNameTable(pObjName)->mSeMoveDown;
    }

    const char* getSeMoveUp(const char* pObjName) {
        return getSeNameTable(pObjName)->mSeMoveUp;
    }

    bool isExistImpactOnRide(const char* pObjName) {
        return MR::isEqualString("LavaHomeFloaterA", pObjName)
            || MR::isEqualString("LavaHomeFloaterB", pObjName)
            || MR::isEqualString("LavaFloatingStepA", pObjName)
            || MR::isEqualString("LavaFloatingStepB", pObjName)
            || MR::isEqualString("LavaFloatingStepC", pObjName)
            || MR::isEqualString("LavaFloatingStepD", pObjName);
    }

    bool isExistSpeedUpLanding(const char* pObjName) {
        return MR::isEqualString("FloaterOtakingFloaterA", pObjName);
    }
};
