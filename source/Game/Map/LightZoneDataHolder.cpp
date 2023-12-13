#include "Game/AreaObj/LightArea.hpp"
#include "Game/Map/LightZoneDataHolder.hpp"
#include "Game/Map/LightFunction.hpp"
#include "Game/Util.hpp"

ZoneLightID::ZoneLightID() {
    _0 = -1;
    mLightID = -1;
}

void ZoneLightID::clear() {
    _0 = -1;
    mLightID = -1;
}

bool ZoneLightID::isTargetArea(const LightArea *pLightArea) const {
    if (_0 == pLightArea->mPlacedZoneID && mLightID == pLightArea->mObjArg0) {
        return false;
    }

    return true;
}

bool ZoneLightID::isOutOfArea() const {
    return mLightID < 0;
}

LightZoneInfo::LightZoneInfo() {
    mAreaCount = 0;
    mAreaInfo = 0;
}

const char* LightZoneInfo::getAreaLightNameInZoneData(s32 idx) const {
    for (int i = 0; i < mAreaCount; i++) {
        if (mAreaInfo[i].mID == idx) {
            return mAreaInfo[i].mAreaLightName;
        }
    }

    return LightFunction::getDefaultAreaLightName();
}

LightZoneDataHolder::LightZoneDataHolder() {
    mCount = 0;
    mZoneInfo = 0;
}

void LightZoneDataHolder::initZoneData() {
    mCount = MR::getZoneNum();
    mZoneInfo = new LightZoneInfo[mCount];

    for (s32 i = 0; i < mCount; i++) {
        mZoneInfo[i].init(i);
    }
}

const char* LightZoneDataHolder::getAreaLightNameInZoneData(const ZoneLightID &zoneID) const {
    s32 val = zoneID._0;

    if (val < 0) {
        val = 0;
    }

    return mZoneInfo[val].getAreaLightNameInZoneData(zoneID.mLightID);
}

// this will load the first entry, but they will all return the same thing anyways
const char* LightZoneDataHolder::getDefaultStageAreaLightName() const {
    return mZoneInfo->getAreaLightNameInZoneData(-1);
}

void LightZoneInfo::init(s32 zoneID) {
    const char* zoneName = MR::getZoneNameFromZoneId(zoneID);
    JMapInfo* parser = 0;
    mAreaCount = LightFunction::createZoneDataParser(zoneName, &parser);
    mAreaInfo = new AreaInfo[mAreaCount];

    for (int i = 0; i < mAreaCount; i++) {
        AreaInfo* inf = &mAreaInfo[i];

        MR::getCsvDataS32(&inf->mID, parser, "LightID", i);
        MR::getCsvDataStr(&inf->mAreaLightName, parser, "AreaLightName", i);
    }
}