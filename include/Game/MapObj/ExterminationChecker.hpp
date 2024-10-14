#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class KeySwitch;

typedef LiveActor* (*CreationFunc)(const char*);

struct ExterminationEntry {
    const char* mChildName;
    CreationFunc mCreationFunc;
};

namespace {
    template<typename T>
    LiveActor* createLiveActor(const char *pName) {
        return new T(pName);
    }

    // we will define the creation funcs later
    static const ExterminationEntry sCreateTable[3]= {
        { "ChildKuribo", nullptr },
        { "ChildSkeletalFishBaby", nullptr },
        { "ChildMeramera", nullptr }
    };
};

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

    inline CreationFunc findEntry(const char *pName) {
        for (s32 j = 0; j < 3; j++) {
            if (sCreateTable[j].mChildName && MR::isEqualStringCase(sCreateTable[j].mChildName, pName)) {
                return sCreateTable[j].mCreationFunc;
            }
        }

        return nullptr;
    }

    LiveActorGroup* mGroup;         // 0x8C
    KeySwitch* mKeySwitch;          // 0x90
    TVec3f mKeySwitchPos;           // 0x94
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
};