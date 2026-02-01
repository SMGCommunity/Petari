#include "Game/Boss/BossStinkBugBombHolder.hpp"
#include "Game/Boss/BossStinkBugBomb.hpp"
#include "Game/Util/LiveActorUtil.hpp"

BossStinkBugBombHolder::BossStinkBugBombHolder(LiveActor* pActor) : DeriveActorGroup<BossStinkBugBomb>("ボスカメムシ爆弾管理", 16) {

    for(int i = 0; i < 16; i++) {
        BossStinkBugBomb* newBomb = new BossStinkBugBomb("ボスカメムシ爆弾");
        newBomb->initWithoutIter();
        MR::setBinderExceptActor(newBomb, pActor);
        registerActor(newBomb);
    }
}
