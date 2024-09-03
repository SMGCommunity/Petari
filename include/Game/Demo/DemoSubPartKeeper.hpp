#pragma once

#include <revolution/types.h>

class DemoSubPartKeeper {
public:
    bool isDemoPartActive(const char *);
    s32 getDemoPartStep(const char *);
    s32 getDemoPartTotalStep(const char *);
};
