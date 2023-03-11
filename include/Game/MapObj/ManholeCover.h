#pragma once

#include "Game/MapObj/MapObjActor.h"

class ManholeCover : public MapObjActor {
public:

    inline ManholeCover(const char *pName) : MapObjActor(pName) {

    }

    virtual ~ManholeCover();
    virtual void init(const JMapInfoIter &);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor *, HitSensor *);
    virtual bool receiveOtherMsg(u32, HitSensor *, HitSensor *);

    void exeOpen();
    void exeRattle();
};

namespace NrvManholeCover {
    NERVE_DECL_NULL(HostTypeWait);
    NERVE_DECL(HostTypeOpen, ManholeCover, ManholeCover::exeOpen);
    NERVE_DECL(HostTypeRattle, ManholeCover, ManholeCover::exeRattle);
};