#pragma once

#include <revolution/types.h>

class AlreadyDoneFlagInGalaxy;
class JMapIdInfo;
class JMapInfoIter;

class GameDataTemporaryInGalaxy {
public:
    /// @brief Creates a new `GameDataTemporaryInGalaxy`.
    GameDataTemporaryInGalaxy();

    void resetPlayerRestartIdInfo();
    void setPlayerRestartIdInfo(const JMapIdInfo&);
    void resetRaceBestTime();
    bool isStageCleared() const;
    void resetStageResultPowerStarParam();
    void resetStageResultStarPieceParam();
    void resetStageResultCoinParam();
    void addStarPiece(int);
    void setLast1upStarPieceNum(int);
    s32 getStarPieceNum() const;
    void resetStageResultParam();
    void clearAlreadyDoneFlag();
    s32 setupAlreadyDoneFlag(const char*, const JMapInfoIter&, u32*);
    void updateAlreadyDoneFlag(int, u32);
    void receiveStageResultParam(const char*, s32, int, int);

    /* 0x00 */ JMapIdInfo* mPlayerRestartIdInfo;
    /* 0x04 */ bool _4;
    /* 0x08 */ u32 _8;
    /* 0x0C */ s16 mPowerStarId;
    /* 0x0E */ char mStageName[48];
    /* 0x40 */ int mStarPieceNum;
    /* 0x44 */ int mLast1upStarPieceNum;
    /* 0x48 */ int mCoinNum;
    /* 0x4C */ u32 _4C;
    /* 0x50 */ u32 _50;
    /* 0x54 */ AlreadyDoneFlagInGalaxy* mAlreadyDoneFlag;
};
