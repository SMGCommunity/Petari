#pragma once

#include <revolution/types.h>

class AreaObj;
class JMapInfoIter;
class LiveActor;

class SleepControlFunc {
public:
    static void addSleepControl(AreaObj*, const JMapInfoIter&);
    static void addSleepControl(LiveActor*, const JMapInfoIter&);

    static void initSyncSleepController();
};
