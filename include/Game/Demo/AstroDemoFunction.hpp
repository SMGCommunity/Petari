#pragma once

#include "Game/LiveActor/LiveActor.h"
#include <revolution.h>

namespace {
    const char* cGrandStarReturnDemoTable[6] = {
        "グランドスター１帰還",
        "グランドスター２帰還",
        "グランドスター３帰還",
        "グランドスター４帰還",
        "グランドスター５帰還",
        "グランドスター６帰還"
    };
};

class AstroDemoFunction {
public:
    static s32 getOpenedAstroDomeNum();
    static const char* getGrandStarReturnDemoName(int);
    static s32 getActiveGrandStarReturnDemoIndex();
    static bool tryRegisterDemo(LiveActor *, const char *, const JMapInfoIter &);
};