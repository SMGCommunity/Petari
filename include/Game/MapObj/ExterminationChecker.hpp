#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class KeySwitch;

typedef LiveActor* (*CreationFunc)(const char*);

struct ExterminationEntry {
    const char* mChildName;
    CreationFunc mCreationFunc;
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

    LiveActorGroup* mGroup;  // 0x8C
    KeySwitch* mKeySwitch;   // 0x90
    TVec3f mKeySwitchPos;    // 0x94
    u8 _A0;
    u8 _A1;
};

namespace MR {
    NameObj* createExterminationPowerStar(const char*);
    NameObj* createExterminationKeySwitch(const char*);
};  // namespace MR
