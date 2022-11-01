#include "Game/System/ScenarioDataParser.h"
#include "Game/Util.h"

bool ScenarioData::getValueString(const char *pKey, s32 a2, const char **pOut) const {
    ScenarioDataIter inf = getScenarioDataIter(a2);
    bool ret = inf._0->getValue<const char *>(inf.mCur, pKey, pOut);

    if (ret) {
        if (MR::isEqualString(*pOut, "")) {
            *pOut = 0;
        }
    }

    return ret;
}

const char* ScenarioData::getZoneName(s32 zoneID) const {
    const char* zoneName = 0;
    mZoneList->getValue<const char *>(zoneID, "ZoneName", &zoneName);
    return zoneName;
}

s32 ScenarioData::getZoneNum() const {
    if (mZoneList->mData) {
        return mZoneList->mData->mNumEntries;
    }

    return 0;
}

s32 ScenarioData::getZoneId(const char *pZoneName) const {
    for (s32 i = 0; i < getZoneNum(); i++) {
        const char* zoneName = 0;
        mZoneList->getValue<const char *>(i, "ZoneName", &zoneName);

        if (MR::isEqualStringCase(zoneName, pZoneName)) {
            return i;
        }
    }

    return 0;
}

// ScenarioDataIter::isEnd()

void ScenarioDataIter::goNext() {
    if (!isEnd()) {
        mCur++;
    }
}

ScenarioDataParser::~ScenarioDataParser() {

}