#pragma once

#include <revolution.h>

struct GameEventFlag {
    const char* mName;          // 0x0
    u8 mType;                   // 0x4
    u8 _5;
    u8 mStarID;                 // 0x6
    u8 mStarPieceNum;           // 0x7
    u8 _8;
    u8 _9;
    u8 _A;
    u8 _B;

    // 0xC
    union {
        const char* mGalaxyName;
        const char* mEventValueName;
    };

    u32 mRequirement;           // 0x10
};

class GameEventFlagAccessor {
public:
    GameEventFlagAccessor(const GameEventFlag *);

    const char* getName() const;
    bool isTypeGalaxyOpenStar() const;
    bool isTypeSpecialStar() const;
    bool isTypeStarPiece() const;
    bool isTypeEventValueIsZero() const;
    const char* getGalaxyName() const;
    s32 getStarId() const;
    u8 getStarPieceIndex() const;
    const char* getGalaxyNameWithStarPiece() const;
    u8 getNeedStarPieceNum() const;
    const char* getEventValueName() const;
    u32 getRequirement() const;

    const GameEventFlag* mFlag;     // 0x0
};