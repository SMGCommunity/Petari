#pragma once

class JMapInfoIter;
class LiveActor;

namespace MR {
    class FunctorBase;
};

namespace AstroDemoFunction {
    int getOpenedAstroDomeNum();
    const char* getGrandStarReturnDemoName(int);
    int getActiveGrandStarReturnDemoIndex();
    void tryRegisterDemo(LiveActor*, const char*, const JMapInfoIter&);
    void tryRegisterAstroDemoAll(LiveActor*, const JMapInfoIter&);
    void tryRegisterGrandStarReturn(LiveActor*, const JMapInfoIter&);
    void tryRegisterGrandStarReturnWithFunction(LiveActor*, const JMapInfoIter&, const MR::FunctorBase&);
    void tryRegisterGrandStarReturnAndSimpleCast(LiveActor*, const JMapInfoIter&);
    void tryRegisterGrandStarReturnWithFunctionAndSimpleCast(LiveActor*, const JMapInfoIter&, const MR::FunctorBase&);
    bool tryRegisterSimpleCastIfAstroGalaxy(LiveActor*);
    void tryRegisterDemoForTico(LiveActor*, const JMapInfoIter&);
    void tryRegisterDemoForLuigiAndKinopio(LiveActor*, const JMapInfoIter&);
};  // namespace AstroDemoFunction
