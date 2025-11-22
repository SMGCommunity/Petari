#pragma once

#include "Game/NameObj/NameObj.hpp"
#include <revolution.h>

class ChipBase;
class ChipGroup;
class ChipHolder;
class ChipCounter;

namespace MR {
    void registerChip(s32, ChipBase*, s32);

    ChipHolder* createChipHolder(s32);
    ChipHolder* getChipHolder(s32);
    void registerChipGroup(s32, ChipGroup* pChipGroup);
    s32 getGotChipCount(s32, s32);
    void noticeGetChip(s32, ChipBase*, s32);
    s32 showChipCounter(s32, s32);
    void hideChipCounter(s32, s32);
    void requestStartChipCompleteDemo(s32, s32);
    void noticeEndChipCompleteDemo(s32, s32);
    void activateChipLayout();
    void deactivateChipLayout();
};  // namespace MR

class ChipHolder : public NameObj {
public:
    enum ChipType { Chip_Blue = 0, Chip_Yellow = 1 };

    ChipHolder(const char* pName, s32 chipType);
    virtual ~ChipHolder();

    void registerChipGroup(ChipGroup* pChipGroup);
    ChipGroup* findChipGroup(s32 chipType) const;
    void init(const JMapInfoIter&);

    /* 0xC  */ ChipCounter* mChipCounter;
    /* 0x10 */ ChipGroup** mChipGroups;
    /* 0x14 */ s32 mNumChipGroups;
    /* 0x18 */ s32 mChipType;  ///< Determines whether the chip is blue (`0`) or yellow (`1`).
};
