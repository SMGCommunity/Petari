#include "Game/LiveActor/LiveActorGroup.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Util.hpp"

LiveActorGroup::LiveActorGroup(const char *pName, int count) : NameObjGroup(pName, count) {

}

void LiveActorGroup::registerActor(LiveActor *pActor) {
    registerObj(pActor);
}

LiveActor* LiveActorGroup::getActor(int idx) const {
    return static_cast<LiveActor*>(mObjects[idx]);
}

LiveActor* LiveActorGroup::getDeadActor() const {
    for (s32 i = 0; i < mObjectCount; i++) {
        LiveActor* actor = static_cast<LiveActor*>(mObjects[i]);
        if (MR::isDead(actor)) {
            return static_cast<LiveActor*>(mObjects[i]);
        }
    }

    return 0;
}

s32 LiveActorGroup::getLivingActorNum() const {
    s32 num = 0;
    for (s32 i = 0; i < mObjectCount; i++) {
        LiveActor* actor = static_cast<LiveActor*>(mObjects[i]);

        if (!MR::isDead(actor)) {
            num++;
        }
    }

    return num;
}

void LiveActorGroup::appearAll() {
    for (s32 i = 0; i < mObjectCount; i++) {
        if (!MR::isDead(static_cast<LiveActor*>(mObjects[i]))) {
            static_cast<LiveActor*>(mObjects[i])->appear();
        }
    }
}

void LiveActorGroup::killAll() {
    for (s32 i = 0; i < mObjectCount; i++) {
        static_cast<LiveActor*>(mObjects[i])->kill();
    }
}