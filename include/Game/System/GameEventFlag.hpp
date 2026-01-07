#pragma once

#include <revolution/types.h>

struct GameEventFlag {
    enum Type {
        Type_0 = 0x00,
        Type_1 = 0x01,
        Type_GalaxyOpenStar = 0x02,
        Type_SpecialStar = 0x03,
        Type_4 = 0x04,
        Type_5 = 0x05,
        Type_Galaxy = 0x06,
        Type_Comet = 0x07,
        Type_StarPiece = 0x08,
        Type_EventValueIsZero = 0x09,
        Type_10 = 0x0A,
        Type_11 = 0x0B,
    };

    /* 0x00 */ const char* mName;
    /* 0x04 */ u8 mType;
    /* 0x05 */ u8 mSaveFlag;

    union {
        /* 0x06 */ u8 mStarID;
        /* 0x06 */ u8 mStarNum;
        /* 0x06 */ u8 mStarPieceIndex;
    };

    /* 0x07 */ u8 mNeedStarPieceNum;
    /* 0x08 */ u32 _8;

    union {
        /* 0x0C */ const char* mGalaxyName;
        /* 0x0C */ const char* mEventValueName;
        /* 0x0C */ const char* mRequirement1;
    };

    union {
        /* 0x10 */ const char* mRequirement;
        /* 0x10 */ const char* mRequirement2;
    };
};

class GameEventFlagAccessor {
public:
    GameEventFlagAccessor(const GameEventFlag*);

    const char* getName() const;
    bool isTypeGalaxyOpenStar() const;
    bool isTypeSpecialStar() const;
    bool isTypeStarPiece() const;
    bool isTypeEventValueIsZero() const;
    const char* getGalaxyName() const;
    s32 getStarId() const;
    s32 getStarPieceIndex() const;
    const char* getGalaxyNameWithStarPiece() const;
    s32 getNeedStarPieceNum() const;
    const char* getEventValueName() const;
    const char* getRequirement() const;

    /* 0x00 */ const GameEventFlag* mFlag;
};
