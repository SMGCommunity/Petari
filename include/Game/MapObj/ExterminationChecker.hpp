#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class KeySwitch;

class ExterminationChecker : public LiveActor {
public:
    ExterminationChecker(const char *);

    virtual ~ExterminationChecker();
    virtual void init(const JMapInfoIter &);
    virtual void control();

    void exeWatching();
    void exeTryStartDemoAppear();
    void exeAppearStar();
    void exeAppearKeySwitch();

    LiveActorGroup* mGroup;         // _8C
    KeySwitch* mKeySwitch;          // _90
    TVec3f mKeySwitchPos;           // _94
    u8 _A0;
    u8 _A1;
};

namespace NrvExterminationChecker {
    NERVE_DECL(ExterminationCheckerNrvWatching, ExterminationChecker, ExterminationChecker::exeWatching);
    NERVE_DECL(ExterminationCheckerNrvTryStartDemoAppear, ExterminationChecker, ExterminationChecker::exeTryStartDemoAppear);
    NERVE_DECL(ExterminationCheckerNrvAppearStar, ExterminationChecker, ExterminationChecker::exeAppearStar);
    NERVE_DECL(ExterminationCheckerNrvAppearKeySwitch, ExterminationChecker, ExterminationChecker::exeAppearKeySwitch);
};

namespace MR {
    NameObj* createExterminationPowerStar(const char *);
    NameObj* createExterminationKeySwitch(const char *);
}