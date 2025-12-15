#include "Game/Util/StringUtil.hpp"

namespace {
    struct PlacedHiddenStarInfo {
        /* 0x00 */ const char* mStageName;
        /* 0x04 */ s32 mHiddenScenarioNo;
        /* 0x08 */ s32 mPlacedScenarioNo;
    };

    const PlacedHiddenStarInfo cPlacedHiddenStarInfoTable[] = {
        { "HeavenlyBeachGalaxy", 6, 3 },
    };
};  // namespace

namespace MR {
    s32 getPlacedHiddenStarScenarioNoFromTable(const char* pStageName, s32 scenarioNo) {
        for (s32 i = 0; i < sizeof(cPlacedHiddenStarInfoTable) / sizeof(*cPlacedHiddenStarInfoTable); i++) {
            if (!MR::isEqualString(pStageName, cPlacedHiddenStarInfoTable[i].mStageName)) {
                continue;
            }

            if (scenarioNo != cPlacedHiddenStarInfoTable[i].mHiddenScenarioNo) {
                continue;
            }

            return cPlacedHiddenStarInfoTable[i].mPlacedScenarioNo;
        }

        return -1;
    }
};  // namespace MR
