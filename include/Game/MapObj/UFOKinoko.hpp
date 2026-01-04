#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/MapObj/MapObjActor.hpp"
#include "Game/MapObj/MapObjActorInitInfo.hpp"
#include "Game/NameObj/NameObjArchiveListCollector.hpp"

class UFOKinoko : public MapObjActor {
public:
    UFOKinoko(const char*);
    ~UFOKinoko();

    virtual void init(const JMapInfoIter&);
    virtual void control();
    virtual void initCaseUseSwitchB(const MapObjActorInitInfo&);
    virtual void initCaseNoUseSwitchB(const MapObjActorInitInfo&);

    static void makeArchiveList(NameObjArchiveListCollector*, const JMapInfoIter&);

    void startMove();

    void exeWait();
    void exeMove();
    void exeStop();
};
