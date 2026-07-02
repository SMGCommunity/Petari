#pragma once

#include "Game/MapObj/MapObjActor.hpp"

class NameObjArchiveListCollector;

class UFOKinoko : public MapObjActor {
public:
    UFOKinoko(const char*);

    virtual ~UFOKinoko();
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
