#pragma once
#include <revolution/types.h>

class GalaxyMoveArgument;
class LuigiMailDirector;

class FindingLuigiEventScheduler {
public:
    FindingLuigiEventScheduler();

    void initAfterResourceLoaded();
    void setStateHiding();
    void getHidingGalaxyNameAndStarId(const char **, s32 *) const;
    bool isActive() const;
    bool isDisappear() const;
    bool isHiding() const;
    bool isEnd() const;
    void sendMail();
    void writeSendSize();
    void clearLostAndFoundCount();
    void syncWithGameEventFlag();
    int calcPowerStarIndexLuigiHas() const;
    bool isState(unsigned long) const;
    void setStateReturnAstroGalaxy(int);
    void update(const GalaxyMoveArgument&);
    void updateOnStageResult(const char *, s32);

    /* 0x00 */ const char* mLuigiLostStageName;
    /* 0x04 */ s32 mLuigiLostStarID;
    /* 0x08 */ LuigiMailDirector* mLuigiMailDirector;
    /* 0x0C */ u32 _C;
    /* 0x10 */ u32 _10;
    /* 0x14 */ u32 _14;
};
