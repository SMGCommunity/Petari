#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class StageSwitchCtrl;

class GroupSwitchWatcher : public LiveActor {
public:
    GroupSwitchWatcher(const char*);

    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x48 */ virtual void control();

    /* 0x8C */ StageSwitchCtrl* mSwitchCtrl;
    /* 0x90 */ s32 mNumSwitches;
    /* 0x94 */ s32 mLogicType;
    /* 0x98 */ s32 mActionType;
};
