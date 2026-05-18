#include "Game/Enemy/CannonShellBase.hpp"

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Util/LiveActorUtil.hpp"

#include <algorithm>

CannonShellHolder::CannonShellHolder(int num) {
    mShells.init(num);
}

void CannonShellHolder::registerCannonShell(CannonShellBase* pShell) {
    mShells.push_back(pShell);
}

// What the hell is even going on here...
CannonShellBase* CannonShellHolder::getValidShell() const {

}

// Thanks shibbo
void CannonShellHolder::killActiveShells() const {
    for (CannonShellBase* const* it = mShells.begin(); it != mShells.end(); ++it) {
        if (!MR::isDead(*it))
            (*it)->kill();
    }
}