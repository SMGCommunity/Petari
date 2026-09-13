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

    ChipEntry mChips[5];           // 0xC
    StageSwitchCtrl* mSwitchCtrl;  // 0x34
    TVec3f mUIPos;
    s32 mGotCount;    // 0x44
    s32 mTotalCount;  // 0x48
    s32 mStageSwitchArg;
    s32 mChipType;
    s32 mFlashFrame;
    s32 mCounterTimer;
    f32 mUIRange;
    f32 mUIRangeFixed;
    u32 mIsDeactivated;
    s32 mAlreadyDoneFlag;
    u8 mIsListeningAppear;
    u8 mIsCounterVisible;
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