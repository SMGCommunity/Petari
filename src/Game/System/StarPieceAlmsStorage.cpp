#include "Game/System/StarPieceAlmsStorage.hpp"
#include "Game/Util/HashUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MemoryUtil.hpp"

namespace {
    const static u32 sMaxValueArray[] = {32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 400, 400, 600, 1200, 1600, 1000, 800, 32767};
};  // namespace

StarPieceAlmsStorage::StarPieceAlmsStorage() : mRequirements() {
    mRequirements = new u16[16];

    for (s32 idx = 0; idx < 16; idx++) {
        mRequirements[idx] = 0;
    }
}

u16 StarPieceAlmsStorage::getValue(int idx) const {
    return mRequirements[idx];
}

u32 StarPieceAlmsStorage::getMaxValue(int idx) const {
    return ::sMaxValueArray[idx];
}

void StarPieceAlmsStorage::addValue(int idx, int value) {
    mRequirements[idx] = MR::clamp(mRequirements[idx] + value, 0l, ::sMaxValueArray[idx]);
}

u32 StarPieceAlmsStorage::makeHeaderHashCode() const {
    return MR::getHashCode("StarPieceAlmsStorage") << 5;
}

u32 StarPieceAlmsStorage::getSignature() const {
    return 'PCE1';
}

s32 StarPieceAlmsStorage::serialize(u8* pData, u32) const {
    MR::copyMemory(pData, mRequirements, 32);
    return 32;
}

s32 StarPieceAlmsStorage::deserialize(const u8* pData, u32) {
    MR::copyMemory(mRequirements, pData, 32);
    return 0;
}

void StarPieceAlmsStorage::initializeData() {
    for (s32 idx = 0; idx < 16; idx++) {
        mRequirements[idx] = 0;
    }
}
