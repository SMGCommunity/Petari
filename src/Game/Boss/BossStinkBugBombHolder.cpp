#include "Game/Boss/BossStinkBugBombHolder.hpp"
#include "Game/Boss/BossStinkBugBomb.hpp"
#include "Game/Util/LiveActorUtil.hpp"

namespace {
    static const s32 sMaxBossStinkBugBomb = 16;
};  // namespace

BossStinkBugBombHolder::BossStinkBugBombHolder(LiveActor* pHost)
    : DeriveActorGroup< BossStinkBugBomb >("ボスカメムシ爆弾管理", ::sMaxBossStinkBugBomb) {
    BossStinkBugBomb* pBomb;

    for (s32 i = 0; i < ::sMaxBossStinkBugBomb; i++) {
        pBomb = new BossStinkBugBomb("ボスカメムシ爆弾");
        pBomb->initWithoutIter();

        MR::setBinderExceptActor(pBomb, pHost);
        registerActor(pBomb);
    }
}
