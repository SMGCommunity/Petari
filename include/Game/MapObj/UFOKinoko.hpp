#pragma once

#include "Game/MapObj/MapObjActor.hpp"

class NameObjArchiveListCollector;

class UFOKinoko : public MapObjActor {
public:
    UFOKinoko(const char*);

    /* 0x08 */ virtual ~UFOKinoko();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x48 */ virtual void control();
    /* 0x84 */ virtual void initCaseUseSwitchB(const MapObjActorInitInfo&);
    /* 0x88 */ virtual void initCaseNoUseSwitchB(const MapObjActorInitInfo&);

    static void makeArchiveList(NameObjArchiveListCollector*, const JMapInfoIter&);

    void startMove();

    void exeWait();
    void exeMove();
    void exeStop();
};
