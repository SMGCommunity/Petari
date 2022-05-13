#pragma once

#include "Game/NameObj/NameObjGroup.h"

class LiveActor;

class LiveActorGroup : public NameObjGroup {
public:
    LiveActorGroup(const char *, int);

    virtual ~LiveActorGroup() {
        
    }

    void registerActor(LiveActor *);
    LiveActor* getActor(int) const;
    LiveActor* getDeadActor() const;
    s32 getLivingActorNum() const;
    void appearAll();
    void killAll();
};

template<typename T>
class DeriveActorGroup : public LiveActorGroup {
public:
    inline DeriveActorGroup(const char *pName, int maxCount) : LiveActorGroup(pName, maxCount) {

    }

    ~DeriveActorGroup() {

    }
};