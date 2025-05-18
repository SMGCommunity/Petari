#include "Game/Demo/DemoCameraKeeper.hpp"
#include "Game/Demo/DemoCameraFunction.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/StringUtil.hpp"

namespace DemoCameraFunction {
    bool isCameraTargetMario(const DemoCameraInfo* pInfo) {
        if (pInfo->_4 == NULL) {
            return NULL;
        }

        return MR::isEqualString(pInfo->_4, MR::getPlayerDemoActor()->mName);
    }

    bool setStringNullIfEmpty(const char** ppStr) {
        if (*ppStr == NULL) {
            return false;
        }

        if (*ppStr[0] == '\0') {
            *ppStr = NULL;

            return true;
        }

        return false;
    }

    void makeAnimCameraName(char* pName, u32 size, const DemoCameraInfo* pInfo) {
        MR::removeExtensionString(pName, size, pInfo->_C);
    }
};
