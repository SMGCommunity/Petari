#pragma once

#include "Game/NameObj/NameObj.hpp"

class ChipBase;
class ChipCounter;
class ChipGroup;
class ChipHolder;

class ChipHolder : public NameObj {
public:
    ChipHolder(const char* pName, s32 chipType);

    virtual ~ChipHolder();
    virtual void init(const JMapInfoIter&);

    void registerChipGroup(ChipGroup*);
    ChipGroup* findChipGroup(s32) const;

    /* 0x0C */ ChipCounter* mChipCounter;
    /* 0x10 */ ChipGroup** mChipGroups;
    /* 0x14 */ s32 mNumChipGroups;
    /* 0x18 */ s32 mChipType;  ///< Determines whether the chip is blue (`0`) or yellow (`1`).
};

namespace MR {
    void createChipHolder(s32);
    ChipHolder* getChipHolder(s32);
    void registerChipGroup(s32, ChipGroup* pChipGroup);
    void registerChip(s32, ChipBase*, s32);
    void noticeGetChip(s32, ChipBase*, s32);
    s32 showChipCounter(s32, s32);
    void hideChipCounter(s32, s32);
    void requestStartChipCompleteDemo(s32, s32);
    void noticeEndChipCompleteDemo(s32, s32);
    s32 getGotChipCount(s32, s32);
    void activateChipLayout();
    void deactivateChipLayout();
};  // namespace MR
