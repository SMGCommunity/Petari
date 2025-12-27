#include "Game/System/ScenarioDataParser.hpp"
#include "Game/SingletonHolder.hpp"
#include "Game/System/GalaxyNameSortTable.hpp"
#include "Game/System/GalaxyStatusAccessor.hpp"
#include "Game/System/GameSystem.hpp"
#include "Game/System/GameSystemSceneController.hpp"
#include "Game/Util/FileUtil.hpp"
#include "Game/Util/SceneUtil.hpp"
#include "Game/Util/StringUtil.hpp"
#include <JSystem/JKernel/JKRMemArchive.hpp>
#include <algorithm>

namespace {
    struct GalaxyNameSortLt {
        bool operator()(ScenarioData** ppLhs, ScenarioData** ppRhs) {
            return GalaxyNameSortTable::getGalaxySortIndex((*ppLhs)->mGalaxyName) < GalaxyNameSortTable::getGalaxySortIndex((*ppRhs)->mGalaxyName);
        }
    };

    const ScenarioData* getCurrentScenarioData() {
        return ScenarioDataFunction::getScenarioDataParser()->getScenarioData(MR::getCurrentStageName());
    }
};  // namespace

ScenarioData::ScenarioData(const char* pFilePath) : mScenarioData(nullptr), mGalaxyName(nullptr), mZoneList(nullptr) {
    char fileName[64];
    MR::removeExtensionString(fileName, sizeof(fileName), MR::getBasename(pFilePath));

    char* pGalaxyName = strstr(fileName, "Scenario");

    if (pGalaxyName != nullptr) {
        pGalaxyName[0] = '\0';
    }

    int galaxyNameSize = strlen(fileName) + 1;

    mGalaxyName = new char[galaxyNameSize];
    MR::copyString(mGalaxyName, fileName, galaxyNameSize);

    JKRMemArchive* pArchive = static_cast< JKRMemArchive* >(MR::receiveArchive(pFilePath));

    mScenarioData = new JMapInfo();
    mScenarioData->attach(pArchive->getResource("/ScenarioData.bcsv"));

    mZoneList = new JMapInfo();
    mZoneList->attach(pArchive->getResource("/ZoneList.bcsv"));
}

// ScenarioData::getScenarioNum
// ScenarioData::getPowerStarNum

bool ScenarioData::getValueString(const char* pKey, s32 a2, const char** ppOut) const {
    ScenarioDataIter iter = getScenarioDataIter(a2);
    bool isExist = reinterpret_cast< const JMapInfo* >(iter.mParser)->getValue< const char* >(iter.mCur, pKey, ppOut);

    if (isExist) {
        if (MR::isEqualString(*ppOut, "")) {
            *ppOut = nullptr;
        }
    }

    return isExist;
}

const char* ScenarioData::getZoneName(s32 zoneId) const {
    const char* pZoneName = nullptr;
    mZoneList->getValue< const char* >(zoneId, "ZoneName", &pZoneName);

    return pZoneName;
}

// ScenarioData::getScenarioDataIter

bool ScenarioData::getValueU32(const char* pKey, s32 a2, u32* pOut) const {
    ScenarioDataIter iter = getScenarioDataIter(a2);
    s32 index = reinterpret_cast< const JMapInfo* >(iter.mParser)->searchItemInfo(pKey);

    if (index < 0) {
        return nullptr;
    }

    return reinterpret_cast< const JMapInfo* >(iter.mParser)->getValueFast(a2, index, pOut);
}

// ScenarioData::getValueBool

s32 ScenarioData::getZoneNum() const {
    if (mZoneList->mData != nullptr) {
        return mZoneList->mData->mNumEntries;
    }

    return 0;
}

s32 ScenarioData::getZoneId(const char* pZoneName) const {
    for (s32 i = 0; i < getZoneNum(); i++) {
        const char* pName = nullptr;
        mZoneList->getValue< const char* >(i, "ZoneName", &pName);

        if (MR::isEqualStringCase(pName, pZoneName)) {
            return i;
        }
    }

    return 0;
}

ScenarioDataIter::ScenarioDataIter(const ScenarioDataParser* pParser, int param2) : mParser(pParser), mCur(param2) {}

bool ScenarioDataIter::isEnd() const {
    return mParser->mScenarioData.size() <= mCur;
}

void ScenarioDataIter::goNext() {
    if (!isEnd()) {
        mCur++;
    }
}

GalaxyStatusAccessor ScenarioDataIter::makeAccessor() const {
    return GalaxyStatusAccessor(mParser->getScenarioData(mCur));
}

ScenarioDataParser::ScenarioDataParser(const char* pName) : NameObj(pName), mScenarioData() {
    DVDDir dir;
    DVDDirEntry dirent;
    DVDOpenDir("/StageData", &dir);

    while (DVDReadDir(&dir, &dirent)) {
        if (!dirent.isDir) {
            continue;
        }

        char fileName[256];
        MR::makeScenarioArchiveFileName(fileName, sizeof(fileName), dirent.name);

        if (!MR::isFileExist(fileName, false)) {
            continue;
        }

        ScenarioData* pData = new ScenarioData(fileName);

        mScenarioData.push_back(pData);
    }

    DVDCloseDir(&dir);

    std::sort(mScenarioData.begin(), mScenarioData.end(), GalaxyNameSortLt());
}

const ScenarioData* ScenarioDataParser::getScenarioData(const char* pGalaxyName) const {
    for (s32 i = 0; i < mScenarioData.size(); i++) {
        const ScenarioData* pScenarioData = mScenarioData[i];

        if (MR::isEqualStringCase(pScenarioData->mGalaxyName, pGalaxyName)) {
            return pScenarioData;
        }
    }

    return nullptr;
}

const ScenarioData* ScenarioDataParser::getScenarioData(s32 index) const {
    return mScenarioData[index];
}

GalaxyStatusAccessor ScenarioDataParser::makeAccessor(const char* pGalaxyName) const {
    return GalaxyStatusAccessor(getScenarioData(pGalaxyName));
}

namespace ScenarioDataFunction {
    ScenarioDataParser* getScenarioDataParser() {
        return SingletonHolder< GameSystem >::get()->mSceneController->mScenarioParser;
    }

    u32 getCurrentCommonLayers(const char* pParam1) {
        getCurrentScenarioData();

        return 1;
    }

    u32 getCurrentScenarioLayers(const char* pParam1, s32 param2) {
        u32 layerNum;

        if (getCurrentScenarioData()->getValueU32(pParam1, param2, &layerNum)) {
            return layerNum * 2;
        }

        return 0;
    }
};  // namespace ScenarioDataFunction

namespace MR {
    ScenarioDataIter makeBeginScenarioDataIter() {
        return ScenarioDataIter(ScenarioDataFunction::getScenarioDataParser(), 0);
    }
};  // namespace MR
