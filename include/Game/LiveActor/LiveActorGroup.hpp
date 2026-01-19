#pragma once

#include "Game/NameObj/NameObjGroup.hpp"

class LiveActor;

class LiveActorGroup : public NameObjGroup {
public:
    LiveActorGroup(const char*, int);

    virtual ~LiveActorGroup() {}

    void registerActor(LiveActor*);
    LiveActor* getActor(int) const;
    LiveActor* getDeadActor() const;
    s32 getLivingActorNum() const;
    void appearAll();
    void killAll();
};

template < typename T >
class DeriveActorGroup : public LiveActorGroup {
public:
    inline DeriveActorGroup(const char* pName, int maxCount) : LiveActorGroup(pName, maxCount) {}

    T* getDeadMember() const NO_INLINE {
        if (getDeadActor()) {
            return reinterpret_cast< T* >(getDeadActor());
        }

        return nullptr;
    }

    ~DeriveActorGroup() NO_INLINE {}
};
