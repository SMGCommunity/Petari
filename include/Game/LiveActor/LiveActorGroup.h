#pragma once

#include "Game/NameObj/NameObjGroup.h"

class LiveActor;

class LiveActorGroup : public NameObjGroup {
public:
    LiveActorGroup(const char *, int);

    void registerActor(LiveActor *);
    LiveActor* getActor(int) const;
};