#include "Game/System/GameEventFlagStorage.hpp"
#include "Game/System/GameEventFlagTable.hpp"
#include <JSystem/JSupport/JSUMemoryInputStream.hpp>
#include <JSystem/JSupport/JSUMemoryOutputStream.hpp>

GameEventFlagStorage::GameEventFlagStorage() : mFlagBitArray(nullptr) {
    mFlagBitArray = new MR::BitArray(GameEventFlagTable::getTableSize());
}

void GameEventFlagStorage::set(const GameEventFlag* pFlag, bool isOn) {
    mFlagBitArray->set(GameEventFlagTable::getIndex(pFlag), isOn);
}

bool GameEventFlagStorage::isOn(const GameEventFlag* pFlag) const {
    return mFlagBitArray->isOn(GameEventFlagTable::getIndex(pFlag));
}

u32 GameEventFlagStorage::getSignature() const {
    return 'FLG1';
}

s32 GameEventFlagStorage::serialize(u8* pData, u32 size) const {
    JSUMemoryOutputStream stream(pData, size);

    for (GameEventFlagIter iter = GameEventFlagTable::getBeginIter(); !iter.isEnd(); iter.goNext()) {
        if ((iter.getFlag()->mSaveFlag & 0x1) != 0) {
            continue;
        }

        GameEventFlagAccessor accessor(iter.getFlag());
        u16 hash = MR::getHashCode(accessor.getName()) & 0x7FFF;
        u16 data = hash | (isOn(iter.getFlag()) ? 0x8000 : 0);
        stream.write(&data, sizeof(data));
    }

    return stream.mPosition;
}

s32 GameEventFlagStorage::deserialize(const u8* pData, u32 size) {
    s32 deserializationFailed = false;
    JSUMemoryInputStream stream(pData, size);

    for (s32 i = 0; i < (int)size / (int)sizeof(u16); i++) {
        u16 data;
        stream.read(&data, sizeof(data));

        bool isOn = ((data >> 15) & 0x1) != 0;
        int index = GameEventFlagTable::getIndexFromHashCode(data & 0x7FFF);

        if (index >= 0) {
            mFlagBitArray->set(index, isOn);
        } else {
            deserializationFailed = true;
        }
    }

    return deserializationFailed ? true : false;
}

void GameEventFlagStorage::initializeData() {
    for (int i = 0; i < mFlagBitArray->size(); i++) {
        mFlagBitArray->set(i, false);
    }
}

u32 GameEventFlagStorage::makeHeaderHashCode() const {
    return MR::getHashCode("2bytes/flag");
}
