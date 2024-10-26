#include "Game/System/GameSystemStationedArchiveLoader.hpp"
#include "Game/Util.hpp"

bool ConditionIfIsNotPlayer::isExecute(const MR::StationedFileInfo *pInfo) const {
    bool result = false;

    if (pInfo->mLoadType != 2 && pInfo->mLoadType != 3) {
        result = true;
    }

    return result;
}

ConditionUsePlayerHeap::ConditionUsePlayerHeap() {
    mNapaHeap = nullptr;
    mGDDRHeap = nullptr;
    mIsDataMario = 1;
}

bool ConditionUsePlayerHeap::isExecute(const MR::StationedFileInfo *pInfo) const {
    s32 type = 3;

    if (mIsDataMario != 0) {
        type = 2;
    }

    return pInfo->mLoadType == type;
}

JKRExpHeap* ConditionUsePlayerHeap::getProperHeap(const MR::StationedFileInfo *pInfo) const {
    switch (pInfo->mHeapType) {
        case 0:
            return mNapaHeap;
            break;
        case 1:
            return mGDDRHeap;
            break;
        case 2:
            return nullptr;
            break;
    }

    return nullptr;
}

PlayerHeapHolder::PlayerHeapHolder() {
    mCondition = nullptr;
    mNapaHeap = nullptr;
    mGDDRHeap = nullptr;
    mIsDataMario = true;
    ConditionUsePlayerHeap* condition = new ConditionUsePlayerHeap();
    mNapaHeap = PlayerHeapHolder::createHeap(0x500000, (JKRHeap*)MR::getStationedHeapNapa());
    JKRExpHeap* gddr = PlayerHeapHolder::createHeap(0x500000, (JKRHeap*)MR::getStationedHeapGDDR3());
    mGDDRHeap = gddr;
    condition->mNapaHeap = mNapaHeap;
    condition->mGDDRHeap = gddr;
    mCondition = condition;
    condition->mIsDataMario = mIsDataMario;
}

void PlayerHeapHolder::adjust() {
    JKRExpHeap* napa = mNapaHeap;
    if (!napa->isEmpty()) {
        napa->alloc(0x10000, nullptr);
    }

    JKRExpHeap* gddr = mGDDRHeap;
    if (!gddr->isEmpty()) {
        gddr->alloc(0x10000, nullptr);
    }

    MR::adjustHeapSize(mNapaHeap, nullptr);
    MR::adjustHeapSize(mGDDRHeap, nullptr);
}

void PlayerHeapHolder::dispose() {
    MR::removeResourceAndFileHolderIfIsEqualHeap(mNapaHeap);
    MR::removeResourceAndFileHolderIfIsEqualHeap(mGDDRHeap);

    if (mNapaHeap->mSize != 0) {
        mNapaHeap->freeAll();
    }

    if (mGDDRHeap->mSize != 0) {
        mGDDRHeap->freeAll();
    }
}

void PlayerHeapHolder::setIsDataMario(bool data) {
    mIsDataMario = data;
    mCondition->mIsDataMario = data;
}

JKRExpHeap* PlayerHeapHolder::createHeap(u32 size, JKRHeap* pParent) {
    if (pParent->getMaxAllocatableSize(0x10) < size) {
        size = -1;
    }

    return JKRExpHeap::create(size, pParent, true);
}