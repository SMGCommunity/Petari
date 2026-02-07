#include "Game/Boss/PoltaGroundRockHolder.hpp"
#include "Game/Boss/PoltaGroundRock.hpp"

PoltaGroundRockHolder::PoltaGroundRockHolder() : DeriveActorGroup<PoltaGroundRock>("ポルタ地面岩管理", 16) {
    for (int i = 0; i < 16; i++) {
        PoltaGroundRock* curGroundRock = new PoltaGroundRock("ポルタ地面岩");
        curGroundRock->initWithoutIter();
        registerActor(curGroundRock);
    }
}

void PoltaGroundRockHolder::breakAll() {
    int objectCount = getObjectCount();
    for (int i = 0; i < objectCount; i++) {
        PoltaGroundRock* curGroundRock = (PoltaGroundRock*)getActor(i);
        curGroundRock->requestBreak();
    }
}

PoltaGroundRockHolder::~PoltaGroundRockHolder() {}
