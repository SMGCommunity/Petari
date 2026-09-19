#pragma once

#include <revolution/types.h>

struct GameEventFlag {
    enum Type {
        Type_None = 0x00,
        Type_StarNum = 0x01,
        Type_GalaxyOpenStar = 0x02,
        Type_SpecialStar = 0x03,
        Type_EventFlag = 0x04,
        Type_StoryEvent = 0x05,
        Type_Galaxy = 0x06,
        Type_Comet = 0x07,
        Type_StarPiece = 0x08,
        Type_EventValueIsZero = 0x09,
        Type_CompleteMarioAndLuigi = 0x0A,
        Type_StoryEventSync = 0x0B,
    };

    /* 0x00 */ const char* mName;
    /* 0x04 */ u8 mType;
    /* 0x05 */ u8 mSaveFlag;

    union {
        /* 0x06 */ u8 mCondition1;
        /* 0x06 */ u8 mStarID;
        /* 0x06 */ u8 mStarNum;
        /* 0x06 */ u8 mStarPieceIndex;
    };

    union {
        /* 0x07 */ u8 mCondition2;
        /* 0x07 */ u8 mNeedStarPieceNum;
    };

    /* 0x08 */ u32 _8; // Unused

    union {
        /* 0x0C */ const char* mCondition3;
        /* 0x0C */ const char* mGalaxyName;
        /* 0x0C */ const char* mEventFlagName;
        /* 0x0C */ const char* mEventValueName;
        /* 0x0C */ const char* mStoryEventName;
        /* 0x0C */ const char* mRequirement1;
    };

    union {
        /* 0x10 */ const char* mCondition4;
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
