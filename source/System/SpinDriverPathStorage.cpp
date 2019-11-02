#include "System/SpinDriverPathStorage.h"

SpinDriverPathStorageOne::SpinDriverPathStorageOne()
    : mZoneID(-1), _4(-1), _8(-1.0f)
{
    reset();
}

void SpinDriverPathStorageOne::reset()
{
    mZoneID = -1;
    _4 = -1;
    _8 = -1.0f;
}

void SpinDriverPathStorageOne::registerToUse(s32 zoneID, s32 unk2)
{
    mZoneID = zoneID;
    _4 = unk2;
    _8 = 0.0f;
}

// SpinDriverPathStorageOne::serialize()
// SpinDriverPathStorageOne::deserialize()

SpinDriverPathStorageScenario::SpinDriverPathStorageScenario()
    : mPaths(0), mPathCount(0), _8(0)
{
    mPaths = new SpinDriverPathStorageOne[0x10];
    mPathCount = 0x10;
    resetAllData();
}

void SpinDriverPathStorageScenario::resetAllData()
{
    s32 curIdx = 0;
    
    while(curIdx < _8)
    {
        mPaths[curIdx].reset();
        curIdx++;
    }

    _8 = 0;
}

// SpinDriverPathStorageScenario::setup()
// SpinDriverPathStorageScenario::serialize()
// SpinDriverPathStorageScenario::deserialize()
// SpinDriverPathStorageScenario::calcMaxZoneId() const