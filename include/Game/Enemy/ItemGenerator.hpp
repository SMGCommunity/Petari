#pragma once

#include <revolution.h>

class LiveActor;

class ItemGenerator {
public:
    ItemGenerator();

    enum Type {
        None = 0,
        Coin = 1,
        StarPiece = 2
    };

    void setTypeNone();
    void setTypeCoin(s32);
    void setTypeStarPeace(s32);
    void generate(const LiveActor *);
    bool isUseFarSE() const;

    s32 mNumType;       // 0x00
    Type mType;         // 0x04
};
