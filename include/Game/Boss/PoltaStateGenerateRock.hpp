#pragma once

#include "Game/LiveActor/ActorStateBase.hpp"

class Polta;

class PoltaStateGenerateRock : public ActorStateBase< Polta > {
public:
    PoltaStateGenerateRock(Polta*);

    virtual ~PoltaStateGenerateRock();
    virtual void appear();

    void exeSign();
    void exeGenerate();
    void exeEnd();

    /* 0x10 */ s32 mPatternIndex;
    /* 0x14 */ s32 mIndexIntoPattern;
    /* 0x18 */ s32 mMaxIndexIntoPattern;
    /* 0x1C */ s32 mEndDelayStep;
};
