#include "Game/Util/NPCUtil.hpp"
#include "Game/LiveActor/ModelObj.hpp"
#include "Game/NPC/NPCActor.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ObjUtil.hpp"

namespace {
    static s32 sStarAppearSeStep = 103;
    static s32 sStarAppearSeStepCaretaker = 32;
    static s32 sStarAppearSeStepPenguinCoach = 95;
    static s32 sStarAppearSeStepTeresaRacer = 89;
    static s32 sStarAppearSeStepTrickRabbit = 22;
};  // namespace

namespace NrvTakeOutStar {
    NEW_NERVE(TakeOutStarNrvAnim, TakeOutStar, Anim);
    NEW_NERVE(TakeOutStarNrvDemo, TakeOutStar, Demo);
    NEW_NERVE(TakeOutStarNrvTerm, TakeOutStar, Term);
};  // namespace NrvTakeOutStar

namespace NrvFadeStarter {
    NEW_NERVE(FadeStarterNrvFade, FadeStarter, Fade);
    NEW_NERVE(FadeStarterNrvTerm, FadeStarter, Term);
};  // namespace NrvFadeStarter

namespace NrvDemoStarter {
    NEW_NERVE(DemoStarterNrvInit, DemoStarter, Init);
    NEW_NERVE(DemoStarterNrvFade, DemoStarter, Fade);
    NEW_NERVE(DemoStarterNrvWait, DemoStarter, Wait);
    NEW_NERVE(DemoStarterNrvTerm, DemoStarter, Term);
};  // namespace NrvDemoStarter

TakeOutStar::TakeOutStar(NPCActor* pActor, const char* pActionName, const char* pAnimName, const Nerve* pNerve)
    : NerveExecutor("パワースター取り出しデモ実行者"), mActor(pActor), mNerve(pNerve), mActionName(pActionName), mAnimName(pAnimName) {
    mStarModel = MR::createPowerStarDemoModel(mActor, "パワースターデモモデル", pActor->getBaseMtx());
    mStarModel->makeActorDead();

    initNerve(&NrvTakeOutStar::TakeOutStarNrvAnim::sInstance);
}

bool TakeOutStar::takeOut() {
    if (isNerve(&NrvTakeOutStar::TakeOutStarNrvTerm::sInstance)) {
        return true;
    }

    updateNerve();

    return false;
}

bool TakeOutStar::isFirstStep() {
    return isNerve(&NrvTakeOutStar::TakeOutStarNrvAnim::sInstance) && MR::isFirstStep(this);
}

bool TakeOutStar::isLastStep() {
    return isNerve(&NrvTakeOutStar::TakeOutStarNrvTerm::sInstance);
}

void TakeOutStar::exeAnim() {
    if (MR::isFirstStep(this)) {
        if (mNerve != nullptr) {
            mActor->pushNerve(mNerve);
        } else {
            mActor->tryPushNullNerve();
        }

        mStarModel->appear();
        MR::invalidateClipping(mStarModel);
        MR::requestMovementOn(mStarModel);
        MR::startBck(mStarModel, mAnimName, nullptr);
        MR::startAction(mActor, mActionName);
    }

    s32 step = sStarAppearSeStep;

    if (MR::isEqualString(mAnimName, "TakeOutStarCaretaker")) {
        step = sStarAppearSeStepCaretaker;
    } else if (MR::isEqualString(mAnimName, "TakeOutStarTeresaRacer")) {
        step = sStarAppearSeStepTeresaRacer;
    } else if (MR::isEqualString(mAnimName, "TakeOutStarPenguinCoach")) {
        step = sStarAppearSeStepPenguinCoach;
    } else if (MR::isEqualString(mAnimName, "TakeOutStarTrickRabbit")) {
        step = sStarAppearSeStepTrickRabbit;
    }

    if (MR::isGreaterStep(this, step)) {
        if (MR::isInWater(mStarModel, TVec3f(0.0f, 0.0f, 0.0f))) {
            MR::startLevelSound(mStarModel, "SE_OJ_LV_POW_STAR_EXIST_W", -1, -1, -1);
        } else {
            MR::startLevelSound(mStarModel, "SE_OJ_LV_POW_STAR_EXIST", -1, -1, -1);
        }
    }

    if (MR::isAnyAnimOneTimeAndStopped(mActor, mActionName)) {
        setNerve(&NrvTakeOutStar::TakeOutStarNrvDemo::sInstance);
    }
}

void TakeOutStar::exeDemo() {
    if (MR::isFirstStep(this)) {
        TVec3f trans;
        MR::extractMtxTrans(MR::getJointMtx(mStarModel, "PowerStar"), &trans);
        MR::appearPowerStarContinueCurrentDemo(mActor, trans);
        mStarModel->kill();
    }

    if (MR::isEndPowerStarAppearDemo(mActor)) {
        MR::validateClipping(mStarModel);
        mActor->popNerve();
        setNerve(&NrvTakeOutStar::TakeOutStarNrvTerm::sInstance);
    }
}

void TakeOutStar::exeTerm() {}

FadeStarter::FadeStarter(NPCActor* pActor, s32 a2) : NerveExecutor("フェード開始制御"), mActor(pActor), _C(nullptr), _10(a2) {
    initNerve(&NrvFadeStarter::FadeStarterNrvFade::sInstance);
}

bool FadeStarter::update() {
    if (isNerve(&NrvFadeStarter::FadeStarterNrvTerm::sInstance)) {
        return true;
    }

    updateNerve();

    return false;
}

void FadeStarter::exeFade() {
    if (MR::isFirstStep(this)) {
        if (!mActor->isEmptyNerve()) {
            _C = mActor->popNerve();
        }

        mActor->tryPushNullNerve();
        MR::closeWipeFade(_10);
    }

    if (MR::isWipeActive()) {
        return;
    }

    mActor->popNerve();

    if (_C != nullptr) {
        mActor->pushNerve(_C);
        _C = nullptr;
    }

    MR::openWipeFade(_10);
    setNerve(&NrvFadeStarter::FadeStarterNrvTerm::sInstance);
}

void FadeStarter::exeTerm() {}

DemoStarter::DemoStarter(NPCActor* pActor) : NerveExecutor("デモ開始制御"), mActor(pActor) {
    initNerve(&NrvDemoStarter::DemoStarterNrvInit::sInstance);
}

bool DemoStarter::update() {
    updateNerve();

    return isNerve(&NrvDemoStarter::DemoStarterNrvTerm::sInstance);
}

void DemoStarter::start() {
    if (isNerve(&NrvDemoStarter::DemoStarterNrvInit::sInstance)) {
        setNerve(&NrvDemoStarter::DemoStarterNrvFade::sInstance);
    }
}

void DemoStarter::exeInit() {}

void DemoStarter::exeFade() {
    if (MR::isFirstStep(this)) {
        MR::invalidateClipping(mActor);
        MR::offPlayerControl();
        MR::closeWipeFade(-1);
    }

    if (MR::isWipeActive()) {
        return;
    }

    setNerve(&NrvDemoStarter::DemoStarterNrvWait::sInstance);
}

void DemoStarter::exeWait() {
    if (MR::isLessStep(this, 30)) {
        return;
    }

    if (MR::canStartDemo()) {
        setNerve(&NrvDemoStarter::DemoStarterNrvTerm::sInstance);
    }
}

void DemoStarter::exeTerm() {}
