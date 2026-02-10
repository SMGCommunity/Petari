#include "Game/Enemy/BegomanSpike.hpp"

BegomanSpike::BegomanSpike(const char* pName) : BegomanBase(pName) {}
BegomanSpike::~BegomanSpike() {}

u32 BegomanSpike::getKind() const {
    return 0;
}

bool BegomanSpike::setNerveReturn() {
    return false;
}

const Nerve* BegomanSpike::getNerveWait() {
    return nullptr;
}
