#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class MeteoContainer : public LiveActor {
public:
    MeteoContainer(const char*);

    virtual ~MeteoContainer();
    virtual void init(const JMapInfoIter&);
    virtual void appear();
    virtual void kill();
    virtual void control();
    virtual void attackSensor(HitSensor*, HitSensor*);
    virtual bool receiveMsgPush(HitSensor*, HitSensor*);
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    void exeDestroy();
};

namespace NrvMeteoContainer {
    NERVE_DECL_NULL(MeteoContainerNrvWait);
    NERVE_DECL(MeteoContainerNrvDestroy, MeteoContainer, MeteoContainer::exeDestroy);
};  // namespace NrvMeteoContainer