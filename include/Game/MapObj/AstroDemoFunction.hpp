#pragma once

#include <revolution/types.h>

class JMapInfoIter;
class LiveActor;

namespace MR {
    class FunctorBase;
};  // namespace MR

class AstroDemoFunction {
public:
    static s32 getOpenedAstroDomeNum();
    static const char* getGrandStarReturnDemoName(int);

    static bool tryRegisterDemo(LiveActor*, const char*, const JMapInfoIter&);
    static bool tryRegisterGrandStarReturnAndSimpleCast(LiveActor*, const JMapInfoIter&);
    static bool tryRegisterGrandStarReturnWithFunctionAndSimpleCast(LiveActor*, const JMapInfoIter&, const MR::FunctorBase&);
};
