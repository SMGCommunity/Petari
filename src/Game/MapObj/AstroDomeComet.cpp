#include "Game/MapObj/AstroDomeComet.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Map/SphereSelector.hpp"
#include "Game/MapObj/AstroDome.hpp"
#include "Game/MapObj/MiniatureGalaxyHolder.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ObjUtil.hpp"

namespace {
    static const char* cCometBrkName[5] = {"Red", "Blue", "White", "Yellow", "Purple"};
};

namespace NrvAstroDomeComet {
    NEW_NERVE(AstroDomeCometNrvSelect, AstroDomeComet, Select);
    NEW_NERVE(AstroDomeCometNrvConfirm, AstroDomeComet, Confirm);
};  // namespace NrvAstroDomeComet

AstroDomeComet::AstroDomeComet(const char* pName) : LiveActor(pName) {
    mBloomModel = nullptr;
    mMiniGalaxy = nullptr;
    mPointingActors = nullptr;
}

void AstroDomeComet::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("AstroDomeComet", nullptr, false);
    initSubModel();
    MR::connectToSceneIndirectMapObj(this);
    mPointingActors = new LiveActor*[6];

    for (s32 i = 0; i < 6; i++) {
        LiveActor* pointingActor = new LiveActor("ポインティング用アクター");
        mPointingActors[i] = pointingActor;
        mPointingActors[i]->initWithoutIter();
        MR::invalidateClipping(mPointingActors[i]);
        TVec3f offs;
        offs.x = 0.0f;
        offs.z = 0.0f;
        offs.y = 2000.0f + (1500.0f * i);
        MR::initStarPointerTargetAtMtx(mPointingActors[i], 1000.0f, MR::getJointMtx(this, "CometIcon"), offs);
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
        const char* cometName = cCometBrkName[MiniatureGalaxyFunction::getCometNameId()];
        MR::startBck(this, "AstroDomeComet", nullptr);
        MR::startBtk(this, "AstroDomeComet");
        MR::startBrk(this, cometName);
        mBloomModel->appear();
        MR::startBck(mBloomModel, "AstroDomeCometBloom", nullptr);
        MR::startBrk(mBloomModel, cometName);

        for (s32 i = 0; i < 6; i++) {
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

    for (s32 i = 0; i < 6; i++) {
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

bool AstroDomeComet::receiveOtherMsg(u32 msg, HitSensor*, HitSensor*) {
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
    if (!SphereSelectorFunction::isHandleHolding()) {
        for (s32 i = 0; i < 6; i++) {
            if (MR::isStarPointerPointing1PWithoutCheckZ(mPointingActors[i], nullptr, true, false)) {
                SphereSelectorFunction::registerPointingTarget(mMiniGalaxy, (HandlePointingPriority)0);
                return;
            }
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

AstroDomeComet::~AstroDomeComet() {}
