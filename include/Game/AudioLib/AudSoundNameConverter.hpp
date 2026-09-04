#pragma once

#include "Game/Util/SingletonHolder.hpp"
#include <JSystem/JAudio2/JAISound.hpp>

class JAUSoundNameTable;

class AudSoundNameData {
public:
    AudSoundNameData();

    /* 0x0 */ const char* mName;
    /* 0x4 */ JAISoundID mID;
    /* 0x8 */ u32 mHash;
};

class AudSoundNameConverter : public AudSingletonHolder< AudSoundNameConverter > {
public:
    AudSoundNameConverter();

    JAISoundID getSoundID(const char*) const;
    JAISoundID getSoundID(const char*, u32) const;
    void init() NO_INLINE;
    void initDataTable(const JAUSoundNameTable*);
    void addGroupInfo(const JAUSoundNameTable*, u8, u8);
    void addSectionInfo(const JAUSoundNameTable*, u8);
    u32 getSoundNameTableNumItem_inSection(const JAUSoundNameTable*, u8);

    inline s32 getSeSoundCategory(const char*) const;
    inline s32 getOtherSoundCategory(const char*) const;

    /* 0x00 */ s32 mNumItems;
    /* 0x04 */ s32 mNumNameDatas;
    /* 0x08 */ u32* mGroupItemOffsets;
    /* 0x0C */ s32 mNumItemsSection0;
    /* 0x10 */ s32 mNumItemsSection1;
    /* 0x14 */ s32 mNumItemsSection2;
    /* 0x18 */ AudSoundNameData* mSoundNameData;
};
