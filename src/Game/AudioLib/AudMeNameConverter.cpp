#include "Game/AudioLib/AudMeNameConverter.hpp"
#include "Game/AudioLib/AudWrap.hpp"
#include "Game/RhythmLib/AudRhythmMeSystem.hpp"
#include "Game/RhythmLib/AudRhythmWrap.hpp"
#include <JSystem/JGadget/hashcode.hpp>
#include <cstring>


AudMeNameConverter::AudMeNameConverter() : mNumEntries(), mEntries() {
    init();
}

u32 AudMeNameConverter::getMeID(const char* pName) const {
    u32 hash = JGadget::getHashCode(pName);
    for (s32 i = 0; i < mNumEntries; i++) {
        const AudMeNameData& data = mEntries[i];
        if (hash == data.mHash && strcmp(pName, data.mName) == 0) {
            return data.mID;
        }
    }
    return -1;
}

void AudMeNameConverter::init() {
    AudMeMgr* meMgr = AudWrap::getRhythmMeSystem()->mMeMgr;
    mNumEntries = meMgr->mParams.mNumEntries;
    mEntries = new AudMeNameData[mNumEntries];

    for (s32 i = 0; i < mNumEntries; i++) {
        mEntries[i].mName = meMgr->mParams.mNames[i];
        mEntries[i].mID = i;
        mEntries[i].mHash = JGadget::getHashCode(mEntries[i].mName);
    }
}
