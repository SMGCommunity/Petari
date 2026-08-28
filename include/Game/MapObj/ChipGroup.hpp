#pragma once

#include "Game/NameObj/NameObj.hpp"
#include <JSystem/JGeometry/TVec.hpp>

class ChipBase;
class StageSwitchCtrl;

class ChipGroup : public NameObj {
public:
    ChipGroup(const char*, s32);

    struct ChipEntry {
        ChipBase* mChip;  // 0x0
        bool mIsGotten;   // 0x4
    };

    /* 0x08 */ virtual ~ChipGroup();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x14 */ virtual void movement();

    void updateUIRange();
    void registerChip(ChipBase*);
    void noticeEndCompleteDemo();
    s32 getGotCount() const;
    bool isComplete() const;
    void noticeGet(ChipBase*);
    void receiveAppearRequest();

    /*inline ChipEntry** first() {
        return &mChips;
    }

    inline ChipEntry& last() {
        return mChips[4];
    }*/

    ChipEntry mChips[5];           // 0xC
    StageSwitchCtrl* mSwitchCtrl;  // 0x34
    TVec3f _38;
    s32 mGotCount;    // 0x44
    s32 mTotalCount;  // 0x48
    s32 _4C;
    s32 _50;
    u32 _54;
    u32 _58;
    f32 _5C;
    f32 _60;
    u32 _64;
    u32 _68;
    u8 _6C;
    u8 _6D;
};

class BlueChipGroup : public ChipGroup {
public:
    BlueChipGroup(const char*);

    /* 0x08 */ virtual ~BlueChipGroup();
};

class YellowChipGroup : public ChipGroup {
public:
    YellowChipGroup(const char*);

    /* 0x08 */ virtual ~YellowChipGroup();
};