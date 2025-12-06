#include "Game/Util/PostureHolder.hpp"
#include "Game/LiveActor/LiveActor.hpp"

PostureHolder::PostureHolder(const LiveActor* pActor) : mScale(pActor->mScale), mRotation(pActor->mRotation), mPosition(pActor->mPosition) {}

void PostureHolder::store(const LiveActor* pActor) {
    mScale.set(pActor->mScale);
    mRotation.set(pActor->mRotation);
    mPosition.set(pActor->mPosition);
}

void PostureHolder::restore(LiveActor* pActor) {
    pActor->mScale.set(mScale);
    pActor->mRotation.set(mRotation);
    pActor->mPosition.set(mPosition);
}
