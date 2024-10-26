#include "Game/Demo/DemoParamCommonDataTable.hpp"
#include "Game/Util/StringUtil.hpp"

namespace {
    struct Param {
        const char *mParamName;         // 0x0
        float mCameraShakeIntensity;    // 0x4
        float mCameraShakeSpeed;        // 0x8
        const char *pRumbleName;        // 0xC
    };

    const Param sDataTable[3] = {
        { "SeaBottomBigDoorA", 0.1f, 2.2f, "中" },
        { "SeaBottomBigDoorB", 0.1f, 2.2f, "中" },
        { "SeaBottomBigDoorC", 0.1f, 2.2f, "中" },
    };

    const Param *getParam(const char *pParamName) {
        for (u32 i = 0; i < 3; i++) {
            if (MR::isEqualString(pParamName, sDataTable[i].mParamName)) {
                return &sDataTable[i];
            }
        }
        return &sDataTable[0];
    }
}

namespace MR {
    namespace Demo {
        float getCameraShakeIntensity(const char *pParamName) {
            return getParam(pParamName)->mCameraShakeIntensity;
        }
        float getCameraShakeSpeed(const char *pParamName) {
            return getParam(pParamName)->mCameraShakeSpeed;
        }
        const char* getPadRumble(const char *pParamName) {
            return getParam(pParamName)->pRumbleName;
        }
        bool isExistCameraShaking(const char *pParamName) {
            return getParam(pParamName)->mCameraShakeIntensity > 0.0f;
        }
        bool isExistPadRumble(const char *pParamName) {
            return getParam(pParamName)->pRumbleName != nullptr;
        }
        const char* getStartEffect(const char *pParamName) {
            return "Open";
        }
    }
}
