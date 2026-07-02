#include "Game/Enemy/CannonShellBase.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include <algorithm>

CannonShellHolder::CannonShellHolder(int num) {
    mShells.init(num);
}

void CannonShellHolder::registerCannonShell(CannonShellBase* pShell) {
    mShells.push_back(pShell);
}

// Annoying instruction ordering and register swaps
CannonShellBase* CannonShellHolder::getValidShell() const {
    bool (*func)(const LiveActor*) = MR::isDead;
    CannonShellBase* const* end = mShells.end();
    CannonShellBase* const* begin = mShells.begin();
    CannonShellBase* const* found = std::find_if(begin, end, func);

    if (found != mShells.end()) {
        return *found;
    }

    return nullptr;
}

// Thanks shibbo
void CannonShellHolder::killActiveShells() const {
    for (CannonShellBase* const* it = mShells.begin(); it != mShells.end(); it++) {
        if (!MR::isDead(*it)) {
            (*it)->kill();
        }
    }
}
