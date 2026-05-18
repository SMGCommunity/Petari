#include "Game/Enemy/CannonShellBase.hpp"

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Util/LiveActorUtil.hpp"

CannonShellHolder::CannonShellHolder(int num) {
    mShells.mArr = nullptr;
    mShells.mMaxSize = 0;
    mShellCount = 0;
    mShells.init(num);
}

void CannonShellHolder::registerCannonShell(CannonShellBase* pShell) {
    mShells[mShellCount++] = pShell;
}

// What the hell is even going on here...
//CannonShellBase* CannonShellHolder::getValidShell() const {
    //bool (*isdead)(const LiveActor*) = MR::isDead;
    //LiveActor** begin = (LiveActor**)mShells.begin();
    //LiveActor** end = (LiveActor**)mShells.end();
    //s32 i;

    //while () {
    //
    //}
    //return mShells[i];
//}

// 100% a loop of some kind that calls kill on all undead actors
//void CannonShellHolder::killActiveShells() const {
//    for (s32 i = 0; i < mShellCount; i++) {
//        if (MR::isDead((const LiveActor*)mShells[i]))
//            ((LiveActor*)mShells[i])->kill();
//    }
//}