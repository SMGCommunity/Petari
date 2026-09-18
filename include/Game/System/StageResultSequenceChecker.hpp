#pragma once

#include <revolution/types.h>

class GameDataHolder;

class StageResultSequenceCheckList {
    friend class StageResultSequenceChecker;

public:
    void init();

public:
    /* 0x00 */ s32 mTicoGalaxyNum;
    /* 0x04 */ s32 mPictureBookChapterCanRead;
    /* 0x08 */ s32 mGalaxyNumCanOpen;
    /* 0x0C */ s32 mPowerStarNum;
    /* 0x10 */ s32 mGreenStarNum;
    /* 0x14 */ bool mIsCompleteGalaxy;
};

class StageResultSequenceChecker {
public:
    /// @brief Creates a new `StageResultSequenceChecker`.
    StageResultSequenceChecker();

    void check();
    void checkIfComplete(const char*);
    bool isJustAppearTicoGalaxy() const;
    bool isAddPictureBook() const;
    bool isJustOpenLibraryRoom() const;
    bool isJustGetGreenStarFirst() const;
    bool isJustCompleteGalaxy() const;
    s32 getPrevPowerStarNum() const;
    s32 getAfterPowerStarNum() const;
    static void fillCheckList(StageResultSequenceCheckList*, const GameDataHolder*);
    static void fillCheckListIfComplete(StageResultSequenceCheckList*, const GameDataHolder*, const char*);
    bool isJustOpenGalaxyWithoutChallengeGalaxy() const;

public:
    /* 0x00 */ StageResultSequenceCheckList mCheckListPrev;
    /* 0x18 */ StageResultSequenceCheckList mCheckListAfter;
};
