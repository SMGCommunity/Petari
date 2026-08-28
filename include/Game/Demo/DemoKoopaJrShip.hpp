#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class KoopaJr;

class DemoKoopaJrShip : public LiveActor {
public:
    /// @brief Creates a new `DemoKoopaJrShip`.
    /// @param pName A pointer to the null-terminated name of the object.
    DemoKoopaJrShip(const char* pName);

    /* 0x0C */ virtual void init(const JMapInfoIter& rIter);
    /* 0x24 */ virtual void appear();
    /* 0x48 */ virtual void control();

    void exeAppear();
    void exeTalk();
    void exeFlyAway();
    void initAnimID(const JMapInfoIter& rIter);
    void createKoopaJrObj(const JMapInfoIter& rIter);
    bool tryDemoEnd() NO_INLINE;
    void startEntryAnim();
    void startLeaveAnim();

    /* 0x8C */ KoopaJr* mKoopaJrObj;
    /* 0x90 */ s32 mAnimCameraIndex;
};
