#pragma once

#include "Game/Util/Array.hpp"
#include <revolution/types.h>

class JMapInfoIter;

class AlreadyDoneInfo {
public:
    AlreadyDoneInfo();

    void clear();
    void init(const char*, s32, s32);
    bool isEqual(const AlreadyDoneInfo&) const;
    void set(bool);
    u16 mask() const;

    bool operator==(const AlreadyDoneInfo& other) const {
        return isEqual(other);
    }

    /* 0x0 */ u16 _0;
    /* 0x2 */ u16 _2;
    /* 0x4 */ u16 _4;
};

class AlreadyDoneFlagInGalaxy {
public:
    AlreadyDoneFlagInGalaxy(int);

    void clear();
    u32 setupFlag(const char*, const JMapInfoIter&, u32*);
    void updateValue(int, u32);

    /* 0x0 */ MR::AssignableArray< AlreadyDoneInfo > mDoneInfos;
    /* 0x8 */ u32 _8;
};
