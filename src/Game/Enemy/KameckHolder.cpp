#include "Game/Enemy/KameckHolder.hpp"
#include "Game/Enemy/Kameck.hpp"
#include "Game/Util.hpp"

KameckHolder::KameckHolder(s32 max) : DeriveActorGroup< Kameck >("カメック管理", max) {
}

void KameckHolder::startDemoAppear() {
    for (s32 i = 0; i < getObjNum(); i++) {
        Kameck* pActor = getMember(i);
        if (MR::isDead(pActor)) {
            pActor->startDemoAppear();
            MR::requestMovementOn(pActor);
        }
    }
}

void KameckHolder::endDemoAppear() {
    for (s32 i = 0; i < getObjNum(); i++) {
        getMember(i)->endDemoAppear();
    }
}

void KameckHolder::deadForceAll() {
    for (s32 i = 0; i < getObjNum(); i++) {
        Kameck* pActor = getMember(i);
        if (!MR::isDead(pActor)) {
            pActor->makeActorDeadForce();
        }
    }
}
