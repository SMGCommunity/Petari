#pragma once
#include <revolution/types.h>

class GalaxyMoveArgument;
class LuigiMailDirector;

class FindingLuigiEventScheduler {
public:
    FindingLuigiEventScheduler();

    void initAfterResourceLoaded();
    void setStateHiding();
    void getHidingGalaxyNameAndStarId(const char**, s32*) const;
    bool isActive() const;
    bool isDisappear() const;
    bool isHiding() const;
    bool isEnd() const;
    void sendMail();
    void writeSendSize();
    void clearLostAndFoundCount();
    void syncWithGameEventFlag();
    s32 calcPowerStarIndexLuigiHas() const;
    bool isState(u32) const;
    void setStateReturnAstroGalaxy(int);
    void update(const GalaxyMoveArgument&);
    void updateOnStageResult(const char*, s32);

    /* 0x00 */ const char* mLuigiLostStageName;
    /* 0x04 */ s32 mLuigiLostStarID;
    /* 0x08 */ LuigiMailDirector* mLuigiMailDirector;
};
