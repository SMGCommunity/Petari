#include "Game/System/GameEventFlagStorage.h"
#include "Game/System/GameEventFlagTable.h"
#include "JSystem/JSupport/JSUMemoryOutputStream.h"

GameEventFlagStorage::GameEventFlagStorage() {
    mFlagBitArray = nullptr;
    mFlagBitArray = new MR::BitArray(GameEventFlagTable::getTableSize());
}

void GameEventFlagStorage::set(const GameEventFlag *pFlag, bool flag_set) {
    mFlagBitArray->set(GameEventFlagTable::getIndex(pFlag), flag_set);
}

bool GameEventFlagStorage::isOn(const GameEventFlag *pFlag) const {
    return mFlagBitArray->isOn(GameEventFlagTable::getIndex(pFlag));
}

u32 GameEventFlagStorage::getSignature() const {
    return 0x464C4731;
}

#ifdef NON_MATCHING
// some bullshit use of r30 instead of r31 for absolutely no reason???
s32 GameEventFlagStorage::serialize(u8 *pData, u32 size) const {
    JSUMemoryOutputStream stream;
    stream.setBuffer(pData, size);

    for (GameEventFlagIter iter = GameEventFlagTable::getBeginIter(); !iter.isEnd(); iter.goNext()) {
        if (!(iter.getFlag()->_5 & 0x1)) {
            GameEventFlagAccessor accessor(iter.getFlag());
            s16 hash = MR::getHashCode(accessor.getName()) & 0x7FFF;
            s16 data = hash | (mFlagBitArray->isOn(GameEventFlagTable::getIndex(iter.getFlag())) ? 0x8000 : 0);
            stream.write(&data, 2);
        }
    }

    return stream.mPosition;
}
#endif

// GameEventFlagStorage::deserialize

void GameEventFlagStorage::initializeData() {
    for (int i = 0; i < mFlagBitArray->mArraySize; i++) {
        mFlagBitArray->set(i, false);
    }
}

u32 GameEventFlagStorage::makeHeaderHashCode() const {
    return MR::getHashCode("2bytes/flag");
}