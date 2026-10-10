#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class KeySwitch;
class LiveActorGroup;

typedef LiveActor* (*CreationFunc)(const char*);

struct ExterminationEntry {
    /* 0x0 */ const char* mChildName;
    /* 0x4 */ CreationFunc mCreationFunc;
};

class ExterminationChecker : public LiveActor {
public:
    ExterminationChecker(const char*);

    virtual ~ExterminationChecker();
    virtual void init(const JMapInfoIter&);
    virtual void control();

    void exeWatching();
    void exeTryStartDemoAppear();
    void exeAppearStar();
    void exeAppearKeySwitch();

    /* 0x8C */ LiveActorGroup* mGroup;
    /* 0x90 */ KeySwitch* mKeySwitch;
    /* 0x94 */ TVec3f mKeySwitchPos;
    /* 0xA0 */ u8 _A0;
    /* 0xA1 */ u8 _A1;
};

namespace MR {
    NameObj* createExterminationPowerStar(const char*);
    NameObj* createExterminationKeySwitch(const char*);
};  // namespace MR
