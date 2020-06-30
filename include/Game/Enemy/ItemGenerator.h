#pragma once

#include <revolution.h>

class LiveActor;

class ItemGenerator
{
public:
    ItemGenerator();

    void setTypeNone();
    void setTypeCoin(s32);
    void setTypeStarPeace(s32);
    void generate(const LiveActor *);
    bool isUseFarSE() const;

    s32 mItemCount; // _0
    u8 mType; // _4
};