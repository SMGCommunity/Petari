#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Map/StageSwitch.hpp"

class GroupSwitchWatcher : public LiveActor {
public:
    GroupSwitchWatcher(const char*);

    virtual ~GroupSwitchWatcher();
    virtual void init(const JMapInfoIter&);
    virtual void control();

    StageSwitchCtrl* mSwitchCtrl;  // 0x8C
    s32 _90;
    s32 _94;
    s32 _98;
};
