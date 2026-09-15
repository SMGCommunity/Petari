#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/Nerve.hpp"

class Unizo;

class UnizoLauncher : public LiveActor {
public:
    UnizoLauncher(const char*);

    virtual ~UnizoLauncher();
    virtual void init(const JMapInfoIter&);

    void exeWait();
    void exeLaunch();

    /* 0x8C */ Unizo** mUnizos;
    /* 0x90 */ s32 mUnizoNum;
};
