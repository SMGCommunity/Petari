#pragma once

#include "NameObj/NameObj.h"
#include "LiveActor/LiveActorGroup.h"

class LiveActorGroupArray : public NameObj
{
public:
    LiveActorGroupArray(const char *);

    virtual void init(JMapInfoIter const &);

    LiveActorGroup* getLiveActorGroup(LiveActor const *) const;
};