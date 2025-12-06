#include "Game/System/GameEventFlagStorage.hpp"
#include "Game/System/GameEventFlagTable.hpp"
#include "JSystem/JSupport/JSUMemoryInputStream.hpp"
#include "JSystem/JSupport/JSUMemoryOutputStream.hpp"

GameEventFlagStorage::GameEventFlagStorage() {
    mFlagBitArray = nullptr;
    mFlagBitArray = new MR::BitArray(GameEventFlagTable::getTableSize());
}

void GameEventFlagStorage::set(const GameEventFlag* pFlag, bool flag_set) {
    mFlagBitArray->set(GameEventFlagTable::getIndex(pFlag), flag_set);
}

bool GameEventFlagStorage::isOn(const GameEventFlag* pFlag) const {
    return mFlagBitArray->isOn(GameEventFlagTable::getIndex(pFlag));
}

u32 GameEventFlagStorage::getSignature() const {
    return 0x464C4731;
}

// some bullshit use of r30 instead of r31 for absolutely no reason???
s32 GameEventFlagStorage::serialize(u8* pData, u32 size) const {
    JSUMemoryOutputStream stream(pData, size);

    for (GameEventFlagIter iter = GameEventFlagTable::getBeginIter(); !iter.isEnd(); iter.goNext()) {
        if (!(iter.getFlag()->saveFlag & 0x1)) {
            GameEventFlagAccessor accessor(iter.getFlag());
            s16 shortHash = MR::getHashCode(accessor.getName()) & 0x7FFF;
            s16 data = shortHash | (mFlagBitArray->isOn(GameEventFlagTable::getIndex(iter.getFlag())) ? 0x8000 : 0);
            stream.write(&data, 2);
        }
    }
    return stream.mPosition;
}

s32 GameEventFlagStorage::deserialize(const u8* pData, u32 size) {
    s32 deserializationFailed = false;
    JSUMemoryInputStream stream(pData, size);

    for (int i = 0; i < (int)size / 2; i++) {
        u16 data;

        stream.read(&data, 2);

        bool extractedFlag = (data >> 15) & 0x1;
        int index = GameEventFlagTable::getIndexFromHashCode(data & 0x7FFF);

        if (index >= 0) {
            mFlagBitArray->set(index, extractedFlag);
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
