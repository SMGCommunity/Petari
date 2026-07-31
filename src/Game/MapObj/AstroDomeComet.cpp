#include "Game/MapObj/AstroDomeComet.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/LiveActor/PartsModel.hpp"
#include "Game/Map/SphereSelector.hpp"
#include "Game/MapObj/MiniatureGalaxy.hpp"
#include "Game/MapObj/MiniatureGalaxyHolder.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/JointUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/StarPointerUtil.hpp"

namespace {
    const char* const cCometBrkName[] = {"Red", "Blue", "White", "Yellow", "Purple"};
    const s32 cPointingActorNum = 6;
    const f32 cPointingRadius = 1000.0f;
    const f32 cPointingPosY = 2000.0f;
    const f32 cPointingOffsetY = 1500.0f;
};  // namespace

namespace NrvAstroDomeComet {
    NEW_NERVE(AstroDomeCometNrvSelect, AstroDomeComet, Select);
    NEW_NERVE(AstroDomeCometNrvConfirm, AstroDomeComet, Confirm);
};  // namespace NrvAstroDomeComet

AstroDomeComet::AstroDomeComet(const char* pName) : LiveActor(pName), mBloomModel(), mMiniGalaxy(), mPointingActors() {
}

void AstroDomeComet::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("AstroDomeComet", nullptr, false);
    initSubModel();
    MR::connectToSceneIndirectMapObj(this);
    mPointingActors = new LiveActor*[::cPointingActorNum];

    for (s32 i = 0; i < ::cPointingActorNum; i++) {
        mPointingActors[i] = new LiveActor("ポインティング用アクター");
        mPointingActors[i]->initWithoutIter();
        MR::invalidateClipping(mPointingActors[i]);
        MR::initStarPointerTargetAtMtx(mPointingActors[i], ::cPointingRadius, MR::getJointMtx(this, "CometIcon"),
                                       TVec3f(0.0f, ::cPointingPosY + ::cPointingOffsetY * i, 0.0f));
        mPointingActors[i]->makeActorDead();
    }

    MR::invalidateClipping(this);
    initNerve(&NrvAstroDomeComet::AstroDomeCometNrvSelect::sInstance);
    MR::tryRegisterDemoCast(this, rIter);
    SphereSelectorFunction::registerTarget(this);
    makeActorDead();
}

void AstroDomeComet::appear() {
    MiniatureGalaxyFunction::updateCometStatus();
    mMiniGalaxy = MiniatureGalaxyFunction::getCometLandMiniatureGalaxy();

    if (mMiniGalaxy != nullptr) {
        LiveActor::appear();
        const char* cometName = ::cCometBrkName[MiniatureGalaxyFunction::getCometNameId()];
        MR::startBck(this, "AstroDomeComet", nullptr);
        MR::startBtk(this, "AstroDomeComet");
        MR::startBrk(this, cometName);
        mBloomModel->appear();
        MR::startBck(mBloomModel, "AstroDomeCometBloom", nullptr);
        MR::startBrk(mBloomModel, cometName);

        for (s32 i = 0; i < ::cPointingActorNum; i++) {
            mPointingActors[i]->appear();
        }
    }
}

void AstroDomeComet::kill() {
    if (!MR::isDead(this)) {
        LiveActor::kill();
    }

    if (!MR::isDead(mBloomModel)) {
        mBloomModel->kill();
    }

    for (s32 i = 0; i < ::cPointingActorNum; i++) {
        if (!MR::isDead(mPointingActors[i])) {
            mPointingActors[i]->kill();
        }
    }
}

void AstroDomeComet::calcAndSetBaseMtx() {
    if (mMiniGalaxy != nullptr) {
        mPosition.set(mMiniGalaxy->mPosition);
    }

    LiveActor::calcAndSetBaseMtx();
}

bool AstroDomeComet::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (SphereSelectorFunction::trySyncAppearMsgSelectStart(this, msg)) {
        return true;
    }

    if (SphereSelectorFunction::isMsgConfirmStart(msg)) {
        if (!MR::isDead(this)) {
            setNerve(&NrvAstroDomeComet::AstroDomeCometNrvConfirm::sInstance);
            return true;
        }
    } else if (SphereSelectorFunction::isMsgConfirmCancel(msg)) {
        setNerve(&NrvAstroDomeComet::AstroDomeCometNrvSelect::sInstance);
        return true;
    }

    return false;
}

void AstroDomeComet::initSubModel() {
    mBloomModel = MR::createBloomModel(this, getBaseMtx());
    mBloomModel->kill();
}

void AstroDomeComet::tryPointing() {
    if (SphereSelectorFunction::isHandleHolding()) {
        return;
    }

    for (s32 i = 0; i < ::cPointingActorNum; i++) {
        if (MR::isStarPointerPointing1PWithoutCheckZ(mPointingActors[i], nullptr, true, false)) {
            SphereSelectorFunction::registerPointingTarget(mMiniGalaxy, (HandlePointingPriority)0);
            break;
        }
    }
}

void AstroDomeComet::exeSelect() {
    if (MR::isFirstStep(this)) {
        MR::showModelIfHidden(this);
        MR::showModelIfHidden(mBloomModel);
    }

    tryPointing();
}

void AstroDomeComet::exeConfirm() {
    if (MR::isFirstStep(this)) {
        if (mMiniGalaxy != SphereSelectorFunction::getSelectedTarget()) {
            MR::hideModelIfShown(this);
            MR::hideModelIfShown(mBloomModel);
        }
    }
}
