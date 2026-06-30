#include "Game/AudioLib/CSSoundNameConverter.hpp"
#include "Game/Speaker/SpkSystem.hpp"
#include "Game/Speaker/SpkTable.hpp"
#include "JSystem/JAudio2/JASGlobal.hpp"
#include <JSystem/JGadget/hashcode.hpp>
#include <cstring>

CSSoundNameConverter::CSSoundNameConverter() : mNumSounds(0), mSoundHashes(nullptr) {
    init();
}

u32 CSSoundNameConverter::getSoundID(const char* pSoundName) const {
    u32 hash = JGadget::getHashCode(pSoundName);

    for (int idx = 0; idx < mNumSounds; idx++) {
        CSSoundNameHashData* hashData = &mSoundHashes[idx];
        if (hash == hashData->mHash && strcmp(pSoundName, hashData->mName) == 0) {
            return hashData->mSoundID;
        }
    }
    return -1;
}

void CSSoundNameConverter::init() {
    SpkTable* table = &JASGlobalInstance< SpkSystem >::getInstance()->mData->mTable;
    initDataTable(table);
    makeDataTable(table);
}

void CSSoundNameConverter::initDataTable(const SpkTable* pTable) {
    mNumSounds = pTable->mResourceCount;
    mSoundHashes = new CSSoundNameHashData[mNumSounds];
    for (int idx = 0; idx < mNumSounds; idx++) {
        mSoundHashes[idx].mName = nullptr;
        mSoundHashes[idx].mSoundID = 0;
        mSoundHashes[idx].mHash = 0;
    }
}

void CSSoundNameConverter::makeDataTable(const SpkTable* pTable) {
    for (int idx = 0; idx < pTable->mResourceCount; idx++) {
        CSSoundNameHashData* data = &mSoundHashes[idx];
        data->mSoundID = idx;
        data->mName = pTable->mNames[idx];
        data->mHash = JGadget::getHashCode(data->mName);
    }
}
