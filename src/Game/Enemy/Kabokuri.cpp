#include "Game/Enemy/Kabokuri.hpp"
#include "Game/LiveActor/Nerve.hpp"

namespace NrvKabokuri {
    NEW_NERVE(KabokuriNrvWait, Kabokuri, Wait);
    NEW_NERVE(KabokuriNrvWalk, Kabokuri, Walk);
    NEW_NERVE(KabokuriNrvDropFire, Kabokuri, DropFire);
    NEW_NERVE(KabokuriNrvAttacksuccess, Kabokuri, AttackSuccess);
    NEW_NERVE(KabokuriNrvTrampled, Kabokuri, Trampled);
    NEW_NERVE(KabokuriNrvStarPieceHitted, Kabokuri, StarPieceHitted);
    NEW_NERVE(KabokuriNrvBindStarPointer, Kabokuri, BindStarPointer);
    NEW_NERVE(KabokuriNrvHitAttacked, Kabokuri, HitAttacked);
    NEW_NERVE(KabokuriNrvHipDropped, Kabokuri, HipDropped);
    NEW_NERVE(KabokuriNrvBreak, Kabokuri, Break);
};  // namespace NrvKabokuri

Kabokuri::Kabokuri(const char* pName) : LiveActor(pName) {
}
Kabokuri::~Kabokuri() {
}
