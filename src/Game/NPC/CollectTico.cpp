#include "Game/NPC/CollectTico.hpp"
#include "Game/NPC/StrayTico.hpp"
#include "Game/Util/Functor.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "JSystem/JMath/JMath.hpp"

namespace NrvCollectTico {
    NEW_NERVE(CollectTicoNrvWait, CollectTico, Wait);
    NEW_NERVE(CollectTicoNrvTryStartDemo, CollectTico, TryStartDemo);
    NEW_NERVE(CollectTicoNrvCompleteDemo, CollectTico, CompleteDemo);
    NEW_NERVE(CollectTicoNrvFlash, CollectTico, Flash);
    NEW_NERVE(CollectTicoNrvAppearPowerStar, CollectTico, AppearPowerStar);
};  // namespace NrvCollectTico

CollectTico::CollectTico(const char* pName) : LiveActor(pName) {
    mStrayTicos = nullptr;
    mTicoNum = 0;
    _A0 = 0;
}

void CollectTico::init(const JMapInfoIter& rIter) {
    MR::connectToSceneNpcMovement(this);
    mTicoNum = MR::getChildObjNum(rIter);
    mStrayTicos = new StrayTico*[mTicoNum];
    for (s32 i = 0; i < mTicoNum; i++) {
        mStrayTicos[i] = new StrayTico("はぐれチコ", this);
        MR::initChildObj(mStrayTicos[i], rIter, i);
    }

    initEffectKeeper(0, "CollectTico", false);
    initSound(2, false);
    initNerve(&NrvCollectTico::CollectTicoNrvWait::sInstance);
    if (MR::tryRegisterDemoCast(this, rIter)) {
        MR::registerDemoActionFunctor(this, MR::Functor_Inline(this, &CollectTico::startAppearPowerStar), "集めチコスター出現");
        _A0 = 1;
    }

    MR::useStageSwitchWriteA(this, rIter);
    MR::declarePowerStar(this);
    MR::invalidateClipping(this);
    makeActorAppeared();
}

void CollectTico::exeWait() {
    bool needsDemo = true;
    for (s32 i = 0; i < mTicoNum; i++) {
        if (!mStrayTicos[i]->isRescued()) {
            needsDemo = false;
            break;
        }
    }

    if (needsDemo && !MR::isPlayerDead()) {
        MR::requestStartDemo(this, "チコ集めコンプリート", &NrvCollectTico::CollectTicoNrvCompleteDemo::sInstance,
                             &NrvCollectTico::CollectTicoNrvTryStartDemo::sInstance);
    }
}

void CollectTico::exeCompleteDemo() {
    if (MR::isFirstStep(this)) {
        mPosition.set< f32 >(*MR::getPlayerPos());
        MR::calcGravity(this);

        TVec3f v13;
        JMathInlineVEC::PSVECNegate(&mGravity, &v13);
        JMAVECScaleAdd(&v13, &mPosition, &_94, 300.0f);
        mPosition.set< f32 >(_94);

        for (s32 i = 0; i < mTicoNum; i++) {
            MR::requestMovementOn(mStrayTicos[i]);
            mStrayTicos[i]->requestCompleteDemo(_94, mGravity, 360.0f * ((f32)i / mTicoNum));
        }
    }

    MR::startLevelSound(this, "SE_SM_LV_STRAYTICO_COMPLETE", -1, -1, -1);
    bool needFlash = true;
    for (s32 i = 0; i < mTicoNum; i++) {
        if (!mStrayTicos[i]->isCompleteDemoEnd()) {
            needFlash = false;
            break;
        }
    }

    if (needFlash) {
        setNerve(&NrvCollectTico::CollectTicoNrvFlash::sInstance);
    }
}

void CollectTico::exeFlash() {
    if (MR::isFirstStep(this)) {
        MR::emitEffect(this, "CollectTicoLight");
        MR::startSound(this, "SE_SM_STRAYTICO_FUSION", -1, -1);
    }

    if (MR::isStep(this, 10)) {
        for (s32 i = 0; i < mTicoNum; i++) {
            mStrayTicos[i]->kill();
        }
    }

    if (MR::isStep(this, 60)) {
        if (!_A0) {
            MR::requestAppearPowerStar(this, _94);
            setNerve(&NrvCollectTico::CollectTicoNrvAppearPowerStar::sInstance);
        }

        MR::endDemo(this, "チコ集めコンプリート");
        if (MR::isValidSwitchA(this)) {
            MR::onSwitchA(this);
        }
    }
}

void CollectTico::exeAppearPowerStar() {
    if (MR::isFirstStep(this)) {
        for (s32 i = 0; i < mTicoNum; i++) {
            mStrayTicos[i]->noticeEnd();
        }

        kill();
    }
}

s32 CollectTico::calcNoRescuedCount() const {
    s32 count = 0;
    for (s32 i = 0; i < mTicoNum; i++) {
        if (!mStrayTicos[i]->isRescued()) {
            count++;
        }
    }

    return count;
}

void CollectTico::startAppearPowerStar() {
    MR::requestAppearPowerStar(this, _94);
    setNerve(&NrvCollectTico::CollectTicoNrvAppearPowerStar::sInstance);
}

void CollectTico::exeTryStartDemo() {}

CollectTico::~CollectTico() {}
