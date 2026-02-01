#include "Game/Boss/PoltaRockHolder.hpp"
#include "Game/Boss/PoltaRock.hpp"

PoltaRockHolder::PoltaRockHolder() : DeriveActorGroup<PoltaRock>("ポルタの岩管理", 16) {
    for (int i = 0; i < 16; i++) {
        PoltaRock* curRock = new PoltaRock("ポルタ岩");
        curRock->initWithoutIter();
        registerActor(curRock);
    }
}

PoltaRockHolder::~PoltaRockHolder() {}
