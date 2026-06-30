#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/Util/Array.hpp"

class AreaObj;
class LiveActor;
class SleepController;
class SwitchEventListener;

class SleepControllerHolder : public NameObj {
public:
    /// @brief Creates a new `SleepControllerHolder`.
    SleepControllerHolder();

    virtual void movement();

    void add(NameObj*, const JMapInfoIter&, SwitchEventListener*);
    void initSync();

private:
    /* 0x0C */ MR::Vector< MR::FixedArray< SleepController*, 1024 > > mSleepControl;
};

namespace SleepControlFunc {
    void initSyncSleepController();
    void addSleepControl(AreaObj*, const JMapInfoIter&);
    void addSleepControl(LiveActor*, const JMapInfoIter&);
};  // namespace SleepControlFunc
