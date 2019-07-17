#ifndef ACTORSTATEBASEINTERFACE_H
#define ACTORSTATEBASEINTERFACE_H

#include "Actor/Nerve/NerveExecutor.h"

class ActorStateBaseInterface : public NerveExecutor
{
public:
    void control();
    void kill();
    void appear();
    void init();
    bool update();

    u8 _8;
};

#endif // ACTORSTATEBASEINTERFACE_H