#pragma once

#include "Actor/NameObj/NameObjGroup.h"
#include "Actor/LiveActor/LiveActor.h"
#include "MR/actor/LiveActorUtil.h"

class LiveActorGroup : public NameObjGroup
{
public:
    LiveActorGroup(const char *, s32);

    virtual ~LiveActorGroup();

    void registerActor(LiveActor *);
    LiveActor* getActor(s32) const;
    LiveActor* getDeadActor() const;
    s32 getLivingActorNum() const;
    void appearAll();
    void killAll();
};