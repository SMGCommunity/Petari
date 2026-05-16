#include "Game/System/SpinDriverPathStorage.hpp"
#include "Game/System/GalaxyStatusAccessor.hpp"
#include "Game/System/ScenarioDataParser.hpp"
#include "Game/Util/HashUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/StringUtil.hpp"
#include <JSystem/JSupport/JSUMemoryInputStream.hpp>
#include <JSystem/JSupport/JSUMemoryOutputStream.hpp>

enum SpinDriverDataFlag {
    SpinDriverDataFlag_Interrupted = 0b10000000,
    SpinDriverDataFlag_Complete = 0b01000000,
    SpinDriverDataId_Mask = 0b00111111,
};

enum SpinDriverScenarioDataFlag {
    SpinDriverScenarioDataFlag_ZoneDataStart = 0b11000000,
    SpinDriverScenarioDataFlag_ZoneIdMask = 0b00111111,
    SpinDriverScenarioDataFlag_EndOfData = 0b11111111,
};

SpinDriverPathStorageOne::SpinDriverPathStorageOne() : mZoneId(-1), mSpinDriverId(-1), mDrawRange(-1.0f) {
    reset();
}

void SpinDriverPathStorageOne::reset() {
    mZoneId = -1;
    mSpinDriverId = -1;
    mDrawRange = -1.0f;
}

f32 SpinDriverPathStorageOne::registerToUse(int zoneId, int spinDriverId) {
    mZoneId = zoneId;
    mSpinDriverId = spinDriverId;
    mDrawRange = 0.0f;
    return mDrawRange;
}

void SpinDriverPathStorageOne::updateValue(f32 drawRange) {
    mDrawRange = drawRange;
}

s32 SpinDriverPathStorageOne::serialize(u8* pData) const {
    if (MR::isNearZero(mDrawRange)) {
        return 0;
    }

    pData[0] = mSpinDriverId & SpinDriverDataId_Mask | SpinDriverDataFlag_Interrupted;

    s32 writeSize;
    s32 drawRange = 256.0f * getDrawRange();
    if (drawRange <= 0) {
        writeSize = 0;
    } else {
        u8 val = drawRange >= 256 ? 0 : (u8)drawRange;

        JSUMemoryOutputStream stream(pData + 1, sizeof(u8));
        stream.writeU8(val);
        writeSize = stream.mPosition;
    }
    return writeSize + 1;
}

s32 SpinDriverPathStorageOne::deserialize(const u8* pData, int zoneId) {
    s32 readSize = 0;
    if ((*(pData + 0) & SpinDriverDataFlag_Interrupted) == 0) {
        mDrawRange = (*(pData + 0) & SpinDriverDataFlag_Complete) != 0 ? 1.0f : 0.0f;
    } else {
        u8 drawRange = 0;
        JSUMemoryInputStream stream(pData + 1, sizeof(u8));
        stream.read(&drawRange, sizeof(u8));

        mDrawRange = 0.0f;

        if (drawRange == 0) {
            mDrawRange = 1.0f;
        } else {
            mDrawRange = drawRange * getDrawRangeScale();
        }

        readSize = stream.mPosition;
    }

    mZoneId = zoneId;
    mSpinDriverId = pData[0] & SpinDriverDataId_Mask;
    return readSize + 1;
}

SpinDriverPathStorageScenario::SpinDriverPathStorageScenario() {
    mOneStorage.init(16);
    resetAllData();
}

void SpinDriverPathStorageScenario::resetAllData() {
    for (s32 idx = 0; idx < mOneStorage.size(); idx++) {
        mOneStorage[idx].reset();
    }
    mOneStorage.clear();
}

s32 SpinDriverPathStorageScenario::setup(int zoneId, int spinDriverId, f32* pDrawRange) {
    if (spinDriverId + 1 <= 1U) {
        return -1;
    }

    for (s32 idx = 0; idx < mOneStorage.size(); idx++) {
        if (((0.0f <= mOneStorage[idx].mDrawRange) != false) && zoneId == mOneStorage[idx].mZoneId &&
            spinDriverId == mOneStorage[idx].mSpinDriverId) {
            *pDrawRange = mOneStorage[idx].mDrawRange;
            return idx;
        }
    }

    SpinDriverPathStorageOne storage;
    storage.registerToUse(zoneId, spinDriverId);
    mOneStorage.push_back(storage);
    *pDrawRange = 0.0f;
    return mOneStorage.size() - 1;
}

void SpinDriverPathStorageScenario::updateValue(int spinDriverIndex, f32 drawRange) {
    mOneStorage[spinDriverIndex].updateValue(drawRange);
}

s32 SpinDriverPathStorageScenario::serialize(u8* pData, u32 dataSize) const {
    s32 size = 0;
    s32 maxZoneId = calcMaxZoneId();
    for (s32 zone = 0; zone <= maxZoneId; zone++) {
        bool newZone = true;
        for (s32 idx = 0; idx < mOneStorage.size(); idx++) {
            if (zone == mOneStorage[idx].mZoneId) {
                if (newZone) {
                    *(u8*)(pData + 2 + size) = zone & ~(SpinDriverScenarioDataFlag_ZoneDataStart) | SpinDriverScenarioDataFlag_ZoneDataStart;
                    size += 1;
                    newZone = false;
                }

                size += mOneStorage[idx].serialize(pData + 2 + size);
            }
        }
    }

    u16 blockSize = size + 3;
    *(pData + size + 2) = SpinDriverScenarioDataFlag_EndOfData;
    *(u16*)(pData + 0) = blockSize;
    return blockSize;
}

s32 SpinDriverPathStorageScenario::deserialize(const u8* pData, u32 dataSize) {
    resetAllData();

    s32 readOffset = 0;
    s32 zoneId = -1;

    for (s32 idx = 0;;) {
        if (++idx > 100) {
            return 0;
        }

        u8 val = *(pData + readOffset + 2);
        if (val != SpinDriverScenarioDataFlag_EndOfData) {
            if ((val & SpinDriverScenarioDataFlag_ZoneDataStart) == SpinDriverScenarioDataFlag_ZoneDataStart) {
                zoneId = val & SpinDriverScenarioDataFlag_ZoneIdMask;
                readOffset++;
                continue;
            }

            if (zoneId < 0) {
                return 0;
            }

            SpinDriverPathStorageOne storage;
            readOffset += storage.deserialize(pData + readOffset + 2, zoneId);
            mOneStorage.push_back(storage);
        } else {
            return readOffset + 3;
        }
    }

    return 0;
}

s32 SpinDriverPathStorageScenario::calcMaxZoneId() const {
    s32 maxId = 0;
    for (s32 idx = 0; idx < mOneStorage.size(); idx++) {
        if (maxId < mOneStorage[idx].mZoneId) {
            maxId = mOneStorage[idx].mZoneId;
        }
    }
    return maxId;
}

SpinDriverPathStorageGalaxy::SpinDriverPathStorageGalaxy(const GalaxyStatusAccessor& rAccessor)
    : mScenarioStorage(nullptr), mNumScenarios(0), mGalaxyName(nullptr) {
    s32 numScenarios = rAccessor.getScenarioNum();
    mScenarioStorage = new SpinDriverPathStorageScenario[rAccessor.getScenarioNum()];
    mNumScenarios = numScenarios;
    mGalaxyName = rAccessor.getName();
    resetAllData();
}

void SpinDriverPathStorageGalaxy::resetAllData() {
    for (s32 idx = 0; idx < mNumScenarios; idx++) {
        mScenarioStorage[idx].resetAllData();
    }
}

s32 SpinDriverPathStorageGalaxy::setup(int scenarioNo, int zoneId, int spinDriverId, f32* pDrawRange) {
    return getScenarioStorage(scenarioNo).setup(zoneId, spinDriverId, pDrawRange);
}

void SpinDriverPathStorageGalaxy::updateValue(int scenarioNo, int spinDriverIndex, f32 drawRange) {
    getScenarioStorage(scenarioNo).updateValue(spinDriverIndex, drawRange);
}

s32 SpinDriverPathStorageGalaxy::serialize(u8* pData, u32 maxBufferSize) const {
    s32 size = 0;
    for (s32 idx = 0; idx < mNumScenarios; idx++) {
        size += mScenarioStorage[idx].serialize(pData + size + 6, maxBufferSize - size - 6);
    }

    *(u16*)(pData + 0) = MR::getHashCode(mGalaxyName);
    u16 blockSize = size + 6;
    *(u16*)(pData + 2) = blockSize;
    *(u8*)(pData + 4) = mNumScenarios;
    *(u8*)(pData + 5) = 0;
    return blockSize;
}

s32 SpinDriverPathStorageGalaxy::deserialize(const u8* pData, u32 maxBufferSize) {
    if (*(u16*)(pData + 0) != (u16)MR::getHashCode(mGalaxyName)) {
        return *(u16*)(pData + 2);
    }

    if (*(u8*)(pData + 4) != mNumScenarios) {
        resetAllData();
        return *(u16*)(pData + 2);
    }

    s32 size = 0;
    for (s32 idx = 0; idx < mNumScenarios; idx++) {
        size += mScenarioStorage[idx].deserialize((u8*)(pData + size + 6), maxBufferSize - size - 6);
    }
    return *(u16*)(pData + 2);
}

SpinDriverPathStorage::SpinDriverPathStorage() {
    mGalaxyStorage.init(50);

    ScenarioDataIter iter = MR::makeBeginScenarioDataIter();
    while (!iter.isEnd()) {
        GalaxyStatusAccessor accessor = iter.makeAccessor();
        mGalaxyStorage.push_back(new SpinDriverPathStorageGalaxy(accessor));
        iter.goNext();
    }
}

void SpinDriverPathStorage::initializeData() {
    for (s32 idx = 0; idx < mGalaxyStorage.size(); idx++) {
        mGalaxyStorage[idx]->resetAllData();
    }
}

s32 SpinDriverPathStorage::setup(const char* pGalaxyName, int scenarioNo, int zoneId, int spinDriverId, f32* pDrawRange) {
    return findFromGalaxy(pGalaxyName)->getScenarioStorage(scenarioNo).setup(zoneId, spinDriverId, pDrawRange);
}

void SpinDriverPathStorage::updateValue(const char* pGalaxyName, int scenarioNo, int spinDriverIndex, f32 drawRange) {
    findFromGalaxy(pGalaxyName)->updateValue(scenarioNo, spinDriverIndex, drawRange);
}

u32 SpinDriverPathStorage::makeHeaderHashCode() const {
    return 0x12345679;
}

u32 SpinDriverPathStorage::getSignature() const {
    return 'SPN1';
}

s32 SpinDriverPathStorage::serialize(u8* pData, u32 maxBufferSize) const {
    JSUMemoryOutputStream stream(pData, maxBufferSize);
    stream.writeU8(mGalaxyStorage.size());
    for (s32 idx = 0; idx < mGalaxyStorage.size(); idx++) {
        s32 writeSize = mGalaxyStorage[idx]->serialize((u8*)stream.mBuffer + stream.mPosition, maxBufferSize - stream.mPosition);
        stream.seek(writeSize, SEEK_FROM_POSITION);
    }
    return stream.mPosition;
}

s32 SpinDriverPathStorage::deserialize(const u8* pData, u32 maxBufferSize) {
    s32 readError = 0;

    JSUMemoryInputStream stream(pData, maxBufferSize);
    u8 numEntries = stream.readU8();

    for (s32 idx = 0; idx < numEntries; idx++) {
        u8* readPtr = (u8*)stream.mBuffer + stream.mPosition;
        SpinDriverPathStorageGalaxy* storage = findFromHashCode(*(u16*)readPtr);
        if (storage != nullptr) {
            storage->deserialize((u8*)stream.mBuffer + stream.mPosition, maxBufferSize - stream.mPosition);
        } else {
            readError = 1;
        }
        stream.seek(*(u16*)(readPtr + 2), SEEK_FROM_POSITION);
    }

    return readError ? 1 : 0;
}

SpinDriverPathStorageGalaxy* SpinDriverPathStorage::findFromGalaxy(const char* pGalaxyName) {
    for (s32 idx = 0; idx < mGalaxyStorage.size(); idx++) {
        if (MR::isEqualString(mGalaxyStorage[idx]->mGalaxyName, pGalaxyName)) {
            return mGalaxyStorage[idx];
        }
    }
    return nullptr;
}

SpinDriverPathStorageGalaxy* SpinDriverPathStorage::findFromHashCode(u16 galaxyNameHash) {
    for (s32 idx = 0; idx < mGalaxyStorage.size(); idx++) {
        if ((u16)MR::getHashCode(mGalaxyStorage[idx]->mGalaxyName) == galaxyNameHash) {
            return mGalaxyStorage[idx];
        }
    }
    return nullptr;
}
