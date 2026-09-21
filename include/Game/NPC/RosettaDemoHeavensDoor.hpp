#pragma once

#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/NPCUtil.hpp"

class NameObjArchiveListCollector;
class PartsModel;
class Rosetta;

class RosettaDemoHeavensDoor1 : public NerveExecutor {
public:
    RosettaDemoHeavensDoor1(Rosetta* pHost, const JMapInfoIter& rIter);

    static void makeArchiveList(NameObjArchiveListCollector* pCollector, const JMapInfoIter& rIter);

    template < typename T >
    void changeNerve() {
        setNerve(GET_NERVE_GLOBAL(T));
    }

    void preDemo();
    void pstDemo();
    void fadeOut();
    void fadeIn();

    void exeWait();
    void exeFade();
    void exeDemo();

    /* 0x08 */ Rosetta* mHost;
    /* 0x0C */ PartsModel* mLightDomeModel;
    /* 0x10 */ PartsModel* mLightHaloModel;
};

class RosettaDemoHeavensDoor2 : public NerveExecutor {
public:
    RosettaDemoHeavensDoor2(Rosetta* pHost, const JMapInfoIter& rIter);

    static void makeArchiveList(NameObjArchiveListCollector* pCollector, const JMapInfoIter& rIter);

    template < typename T >
    void changeNerve() {
        setNerve(GET_NERVE_GLOBAL(T));
    }

    void exeWait();
    void exeDemo() {
    }

    /* 0x08 */ DemoStarter mDemoStarter;
    /* 0x14 */ Rosetta* mHost;
};
