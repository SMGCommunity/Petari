#ifndef GALAXYCOMETSCHEDULER_H
#define GALAXYCOMETSCHEDULER_H

#include "types.h"
#include "System/Galaxy/GalaxyCometState.h"

struct GalaxyCometTimePaper
{
    const char* mName; // _0
    s32 mCometCategory; // _4
};

struct GalaxyCometSerializeInfo
{
    u16 _0;
    u16 _2;
};

class GalaxyCometTimeTable
{
public:
    inline GalaxyCometTimeTable() { }
    GalaxyCometTimeTable(const GalaxyCometTimePaper *);

    void update();
    void checkIsReady();
    void serializeStateToGameData(GalaxyCometSerializeInfo *);
    void deserializeStateFromGameData(const GalaxyCometSerializeInfo *);
    bool isLand() const;
    bool isIncluded(const char *) const;
    s32 getCometCategory() const;
    void updateTimePaperPos();
    bool findSuitableTimePaperPos(s32 *, s32);
    s32 calcTimePaperLength();

    GalaxyCometState* mCometState; // _0
    GalaxyCometTimePaper* mPapers; // _4
    u32 _8;
    u8 _C;
    u8 _D;
};

class GalaxyCometScheduler
{
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
    const char* getEncounterCometName(const char *) const;
    GalaxyCometTimeTable* findFromGalaxy(const char *) const;
    bool isHideAll() const;
    s32 getStateValueIncluded(const char *);

    GalaxyCometTimeTable** mCometTables; // _0
    s32 mNumTables; // _4
    bool mIsActive; // _8
};

#endif // GALAXYCOMETSCHEDULER_H