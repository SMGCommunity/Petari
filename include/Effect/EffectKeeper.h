#ifndef EFFECTKEEPER_H
#define EFFECTKEEPER_H

#include <revolution.h>

class ResourceHolder;

class EffectKeeper
{
public:
    EffectKeeper(const char *, ResourceHolder *, s32, const char *);

    void clear();
};

#endif // EFFECTKEEPER_H