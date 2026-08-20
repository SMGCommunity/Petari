#pragma once

#include "Game/Util/NPCUtil.hpp"

class NameObjArchiveListCollector;
class Rosetta;

class RosettaDemoHeavensDoor1 : public NerveExecutor {
public:
    RosettaDemoHeavensDoor1(Rosetta*, const JMapInfoIter&);

    static void makeArchiveList(NameObjArchiveListCollector*, const JMapInfoIter&);

    template < typename T >
    void changeNerve();

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
    RosettaDemoHeavensDoor2(Rosetta*, const JMapInfoIter&);

    static void makeArchiveList(NameObjArchiveListCollector*, const JMapInfoIter&);

    template < typename T >
    void changeNerve();

    void exeWait();
    void exeDemo();

    /* 0x08 */ DemoStarter mDemoStarter;
    /* 0x14 */ Rosetta* mHost;
};
