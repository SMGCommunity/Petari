#include "Game/LiveActor/LiveActorGroup.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Util/LiveActorUtil.hpp"

LiveActorGroup::LiveActorGroup(const char* pName, int numMax) : NameObjGroup(pName, numMax) {
}

void LiveActorGroup::registerActor(LiveActor* pActor) {
    registerObj(pActor);
}

LiveActor* LiveActorGroup::getActor(int index) const {
    return static_cast< LiveActor* >(getObj(index));
}

LiveActor* LiveActorGroup::getDeadActor() const {
    for (s32 i = 0; i < getObjNum(); i++) {
        if (MR::isDead(getActor(i))) {
            return getActor(i);
        }
    }

    return nullptr;
}

s32 LiveActorGroup::getLivingActorNum() const {
    s32 num = 0;

    for (s32 i = 0; i < getObjNum(); i++) {
        if (MR::isDead(getActor(i))) {
            continue;
        }

        num++;
    }

    return num;
}

void LiveActorGroup::appearAll() {
    for (s32 i = 0; i < getObjNum(); i++) {
        if (MR::isDead(getActor(i))) {
            getActor(i)->appear();
        }
    }
}

void LiveActorGroup::killAll() {
    for (s32 i = 0; i < getObjNum(); i++) {
        getActor(i)->makeActorDead();
    }
}
