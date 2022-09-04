#pragma once

#include "Game/LiveActor/LiveActor.h"
#include "Game/MapObj/MapObjActor.h"
#include "Game/MapObj/MapObjActorInitInfo.h"
#include "Game/NameObj/NameObjArchiveListCollector.h"

class UFOKinoko : public MapObjActor {
public:
    UFOKinoko(const char *);
    ~UFOKinoko();

    virtual void init(const JMapInfoIter &);
    virtual void control();
    virtual void initCaseUseSwitchB(const MapObjActorInitInfo &);
    virtual void initCaseNoUseSwitchB(const MapObjActorInitInfo &);

    static void makeArchiveList(NameObjArchiveListCollector *, const JMapInfoIter &);

    void startMove();

    void exeMove();
    void exeStop();
};

namespace NrvUFOKinoko {
    NERVE(HostTypeWait)
    NERVE(HostTypeMove)
    NERVE(HostTypeStop)
};
