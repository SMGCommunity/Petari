#pragma once

#include <revolution.h>

class ResourceHolder;

class EffectKeeper
{
public:
    EffectKeeper(const char *, ResourceHolder *, s32, const char *);

    void clear();
};