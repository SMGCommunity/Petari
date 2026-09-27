#include "Game/Enemy/CannonShellBase.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include <algorithm>

CannonShellHolder::CannonShellHolder(int num) {
    mShells.init(num);
}

void CannonShellHolder::registerCannonShell(CannonShellBase* pShell) {
    mShells.push_back(pShell);
}

CannonShellBase* CannonShellHolder::getValidShell() const {
    CannonShellBase* const* found = std::find_if(mShells.begin(), mShells.end(), std::ptr_fun(&MR::isDead));

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
