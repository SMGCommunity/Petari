#ifndef ACTORSTATEBASE_H
#define ACTORSTATEBASE_H

#include "Actor/Nerve/NerveExecutor.h"

class ActorStateBaseInterface : public NerveExecutor
{
public:
    virtual void init();
    virtual void appear();
    virtual void kill();
    virtual bool update();
    virtual void control();

    u8 _8;
};

#endif // ACTORSTATEBASE_H