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

    virtual ~ChipGroup();
    virtual void init(const JMapInfoIter&);
    virtual void movement();

    void updateUIRange();
    void registerChip(ChipBase*);
    void noticeEndCompleteDemo();
    s32 getGotCount() const;
    bool isComplete() const;
    void noticeGet(ChipBase*);
    void receiveAppearRequest();

    /* 0xC */ ChipEntry mChips[5];
    /* 0x34 */ StageSwitchCtrl* mSwitchCtrl;
    /* 0x38 */ TVec3f mUIPos;
    /* 0x44 */ s32 mGotCount;
    /* 0x48 */ s32 mTotalCount;
    /* 0x4C */ s32 mStageSwitchArg;
    /* 0x50 */ s32 mChipType;
    /* 0x54 */ s32 mFlashFrame;
    /* 0x58 */ s32 mCounterTimer;
    /* 0x5C */ f32 mUIRange;
    /* 0x60 */ f32 mUIRangeFixed;
    /* 0x64 */ u32 mIsDeactivated;
    /* 0x68 */ s32 mAlreadyDoneFlag;
    /* 0x6C */ u8 mIsListeningAppear;
    /* 0x6D */ u8 mIsCounterVisible;
};

class BlueChipGroup : public ChipGroup {
public:
    BlueChipGroup(const char*);

    virtual ~BlueChipGroup();
};

class YellowChipGroup : public ChipGroup {
public:
    YellowChipGroup(const char*);

    virtual ~YellowChipGroup();
};