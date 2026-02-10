#include "Game/Enemy/BegomanSpring.hpp"

BegomanSpring::BegomanSpring(const char* pName) : BegomanBase(pName) {}
BegomanSpring::~BegomanSpring() {}

u32 BegomanSpring::getKind() const {
    return 0;
}

bool BegomanSpring::setNerveReturn() {
    return false;
}

const Nerve* BegomanSpring::getNerveWait() {
    return nullptr;
}
