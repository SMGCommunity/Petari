#pragma once

#include "Game/Util.hpp"

class AreaObj;

class SleepControlFunc {
public:
    static void addSleepControl(AreaObj *, const JMapInfoIter&);

    static void initSyncSleepController();
};
