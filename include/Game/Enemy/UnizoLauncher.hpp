#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/Nerve.hpp"

namespace NrvUnizoLauncher {
    NERVE(UnizoLauncherNrvWait);
    NERVE(UnizoLauncherNrvLaunch);
}  // namespace NrvUnizoLauncher

class Unizo;

class UnizoLauncher : public LiveActor {
public:
    UnizoLauncher(const char*);

    virtual ~UnizoLauncher();
    virtual void init(const JMapInfoIter&);

    void exeLaunch();

    /* 0x8C */ Unizo** mUnizos;
    /* 0x90 */ s32 mUnizoNum;
};
