#pragma once

#include <revolution/types.h>

#define COMET_CATEGORY_NORMAL   0
#define COMET_CATEGORY_COIN_100 1

class GalaxyCometState;

struct GalaxyCometSerializeInfo {
    u16 mStateIndexPacked;  // 0x0
    u16 mPastSecond;        // 0x2
};

struct GalaxyCometTimePaper {
    const char *mGalaxyName;    // 0x0
    s32 mCategory;              // 0x4
};

class GalaxyCometTimeTable {
public:
    GalaxyCometTimeTable(const GalaxyCometTimePaper *);
    void update();
    void checkIsReady();
    void serializeStateToGameData(GalaxyCometSerializeInfo *);
    void deserializeStateFromGameData(const GalaxyCometSerializeInfo *);
    bool isLand() const;
    bool isIncluded(const char *) const;
    s32 getCometCategory() const;
    void updateTimePaperPos();
    bool findSuitableTimePaperPos(long *, long) const;
    s32 calcTimePaperLength() const;
    void advance();

    GalaxyCometState *mState;               // 0x0
    const GalaxyCometTimePaper *mTimePaper; // 0x4
    s32 mTimePaperPos;                      // 0x8
    bool mIsReady;                          // 0xC
    bool mIsHide;                           // 0xD
};

class GalaxyCometScheduler {
public:
    GalaxyCometScheduler();
    void update();
    void activate();
    void deactivate();
    void forceToNextState();
    void syncWithFlags();
    void updateStateToGameData();
    void restoreStateFromGameData();
    void setScheduleFirstContact();
    void setScheduleMostForwardCometAppear();
    bool isCometLand(const char *) const;
    s32 getEncounterCometPowerStarId(const char *) const;
    const char *getEncounterCometName(const char *) const;
    GalaxyCometTimeTable *findFromGalaxy(const char *) const;
    bool isHideAll() const;
    s32 getStateValueIncluded(const char *);

    GalaxyCometTimeTable **mTimeTables; // 0x0
    int mCount;                         // 0x4
    bool mEnabled;                      // 0x8
};
