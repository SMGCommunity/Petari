#pragma once

#include <revolution.h>

class LiveActor;

#define TYPE_NONE 0;
#define TYPE_COIN 1;
#define TYPE_STARPIECE 2;

class ItemGenerator {
public:
    ItemGenerator();

    void setTypeNone();
    void setTypeCoin(s32);
    void setTypeStarPeace(s32);
    void generate(const LiveActor*);
    bool isUseFarSE() const;

    s32 mNumType;  // 0x00
    u8 mType;      // 0x04
};
